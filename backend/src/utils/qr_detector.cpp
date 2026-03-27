#include <utils/qr_detector.hpp>

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <set>

namespace btrgb {

namespace {

struct DetectionCandidate {
    std::string label;
    cv::Mat image;
    double pointScale = 1.0;
};

cv::Mat to_grayscale(const cv::Mat& image) {
    if (image.channels() == 3) {
        cv::Mat gray;
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
        return gray;
    }

    if (image.channels() == 4) {
        cv::Mat gray;
        cv::cvtColor(image, gray, cv::COLOR_BGRA2GRAY);
        return gray;
    }

    return image.clone();
}

cv::Mat apply_clahe(const cv::Mat& input) {
    cv::Mat normalized;
    cv::normalize(input, normalized, 0, 255, cv::NORM_MINMAX);

    auto clahe = cv::createCLAHE();
    clahe->setClipLimit(3.0);
    clahe->setTilesGridSize(cv::Size(8, 8));

    cv::Mat enhanced;
    clahe->apply(normalized, enhanced);
    return enhanced;
}

cv::Mat adaptive_binarize(const cv::Mat& input) {
    cv::Mat blurred;
    cv::GaussianBlur(input, blurred, cv::Size(3, 3), 0.0);

    cv::Mat thresholded;
    cv::adaptiveThreshold(blurred, thresholded, 255,
                          cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY,
                          31, 5);
    return thresholded;
}

cv::Mat morphological_cleanup(const cv::Mat& binary) {
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::Mat cleaned;
    cv::morphologyEx(binary, cleaned, cv::MORPH_CLOSE, kernel);
    cv::morphologyEx(cleaned, cleaned, cv::MORPH_OPEN, kernel);
    return cleaned;
}

cv::Mat unsharp_mask(const cv::Mat& input, double sigma, double strength) {
    cv::Mat blurred;
    cv::GaussianBlur(input, blurred, cv::Size(0, 0), sigma);
    cv::Mat sharpened;
    cv::addWeighted(input, 1.0 + strength, blurred, -strength, 0, sharpened);
    return sharpened;
}

double channel_contrast_score(const cv::Mat& channel) {
    cv::Scalar mean;
    cv::Scalar stddev;
    cv::meanStdDev(channel, mean, stddev);
    return stddev[0];
}

void scale_points(std::vector<cv::Point>& points, double factor) {
    if (factor == 1.0) {
        return;
    }

    for (auto& point : points) {
        point.x = static_cast<int>(std::round(point.x * factor));
        point.y = static_cast<int>(std::round(point.y * factor));
    }
}

void add_candidate(std::vector<DetectionCandidate>& candidates,
                   const std::string& label,
                   const cv::Mat& image,
                   double pointScale = 1.0) {
    if (!image.empty()) {
        candidates.push_back({label, image, pointScale});
    }
}

std::vector<DetectionCandidate> build_detection_candidates(const cv::Mat& image) {
    std::vector<DetectionCandidate> candidates;
    add_candidate(candidates, "original", image);

    const cv::Mat grayscale = to_grayscale(image);
    const cv::Mat enhancedGray = apply_clahe(grayscale);
    add_candidate(candidates, "grayscale", grayscale);
    add_candidate(candidates, "grayscale-clahe", enhancedGray);

    const cv::Mat binarizedGray = adaptive_binarize(enhancedGray);
    add_candidate(candidates, "grayscale-clahe-threshold", binarizedGray);
    add_candidate(candidates, "grayscale-clahe-threshold-morph",
                  morphological_cleanup(binarizedGray));

    // Inverted (negative) grayscale — helps when QR is light-on-dark.
    cv::Mat invertedGray;
    cv::bitwise_not(enhancedGray, invertedGray);
    add_candidate(candidates, "grayscale-clahe-inverted", invertedGray);
    add_candidate(candidates, "grayscale-clahe-inverted-threshold",
                  adaptive_binarize(invertedGray));

    // Unsharp mask for low-contrast images.
    const cv::Mat sharpened = unsharp_mask(grayscale, 3.0, 1.5);
    const cv::Mat sharpenedClahe = apply_clahe(sharpened);
    add_candidate(candidates, "grayscale-unsharp-clahe", sharpenedClahe);
    add_candidate(candidates, "grayscale-unsharp-clahe-threshold",
                  adaptive_binarize(sharpenedClahe));

    if (image.channels() == 3 || image.channels() == 4) {
        cv::Mat bgrImage;
        if (image.channels() == 4) {
            cv::cvtColor(image, bgrImage, cv::COLOR_BGRA2BGR);
        } else {
            bgrImage = image;
        }

        // --- BGR channel isolation (all 3 channels, ranked by contrast) ---
        std::vector<cv::Mat> channels;
        cv::split(bgrImage, channels);

        std::vector<std::pair<double, int>> rankedChannels;
        for (int i = 0; i < static_cast<int>(channels.size()); ++i) {
            rankedChannels.push_back({channel_contrast_score(channels[i]), i});
        }

        std::sort(rankedChannels.begin(), rankedChannels.end(),
                  [](const auto& lhs, const auto& rhs) {
                      return lhs.first > rhs.first;
                  });

        const std::array<const char*, 3> channelNames = {"blue", "green", "red"};
        const int channelCount = static_cast<int>(rankedChannels.size());
        for (int i = 0; i < channelCount; ++i) {
            const int channelIndex = rankedChannels[i].second;
            const cv::Mat& channel = channels[channelIndex];
            const cv::Mat enhancedChannel = apply_clahe(channel);
            const std::string channelPrefix =
                std::string(channelNames[channelIndex]) + "-channel";

            add_candidate(candidates, channelPrefix, channel);
            add_candidate(candidates, channelPrefix + "-clahe", enhancedChannel);

            const cv::Mat binarizedChannel = adaptive_binarize(enhancedChannel);
            add_candidate(candidates, channelPrefix + "-clahe-threshold",
                          binarizedChannel);
            add_candidate(candidates, channelPrefix + "-clahe-threshold-morph",
                          morphological_cleanup(binarizedChannel));
        }

        // --- LAB color space: L channel isolates luminance from chrominance ---
        cv::Mat labImage;
        cv::cvtColor(bgrImage, labImage, cv::COLOR_BGR2Lab);
        std::vector<cv::Mat> labChannels;
        cv::split(labImage, labChannels);

        const cv::Mat& lChannel = labChannels[0];
        const cv::Mat lClahe = apply_clahe(lChannel);
        add_candidate(candidates, "lab-L", lChannel);
        add_candidate(candidates, "lab-L-clahe", lClahe);

        const cv::Mat lBinarized = adaptive_binarize(lClahe);
        add_candidate(candidates, "lab-L-clahe-threshold", lBinarized);
        add_candidate(candidates, "lab-L-clahe-threshold-morph",
                      morphological_cleanup(lBinarized));

        // --- HSV color space: V channel for brightness ---
        cv::Mat hsvImage;
        cv::cvtColor(bgrImage, hsvImage, cv::COLOR_BGR2HSV);
        std::vector<cv::Mat> hsvChannels;
        cv::split(hsvImage, hsvChannels);

        const cv::Mat& vChannel = hsvChannels[2];
        const cv::Mat vClahe = apply_clahe(vChannel);
        add_candidate(candidates, "hsv-V", vChannel);
        add_candidate(candidates, "hsv-V-clahe", vClahe);

        const cv::Mat vBinarized = adaptive_binarize(vClahe);
        add_candidate(candidates, "hsv-V-clahe-threshold", vBinarized);
        add_candidate(candidates, "hsv-V-clahe-threshold-morph",
                      morphological_cleanup(vBinarized));
    }

    if (image.cols > 4000 || image.rows > 4000) {
        cv::Mat halfGray;
        cv::resize(grayscale, halfGray, cv::Size(), 0.5, 0.5, cv::INTER_AREA);
        add_candidate(candidates, "grayscale-half", halfGray, 2.0);
        add_candidate(candidates, "grayscale-half-clahe",
                      apply_clahe(halfGray), 2.0);

        cv::Mat quarterGray;
        cv::resize(grayscale, quarterGray, cv::Size(), 0.25, 0.25,
                   cv::INTER_AREA);
        add_candidate(candidates, "grayscale-quarter", quarterGray, 4.0);
        add_candidate(candidates, "grayscale-quarter-clahe",
                      apply_clahe(quarterGray), 4.0);
    }

    return candidates;
}

} // namespace

QRDetector::QRDetector() : detector_() {
    // QRCodeDetector is ready to use after construction
}

cv::Mat QRDetector::preprocessImage(const cv::Mat& image) {
    return apply_clahe(to_grayscale(image));
}

QRDetectionResult QRDetector::detectFromMatRegion(const cv::Mat& image,
                                                  const QRScanRegion& region,
                                                  double marginPercent) {
    QRDetectionResult result;
    result.found = false;

    if (image.empty()) {
        result.error = "Empty image provided";
        return result;
    }

    auto clampInt = [](int value, int minVal, int maxVal) {
        return std::max(minVal, std::min(value, maxVal));
    };

    const int imgW = image.cols;
    const int imgH = image.rows;

    int left = static_cast<int>(std::round(region.left));
    int right = static_cast<int>(std::round(region.right));
    int top = static_cast<int>(std::round(region.top));
    int bottom = static_cast<int>(std::round(region.bottom));

    if (left > right) std::swap(left, right);
    if (top > bottom) std::swap(top, bottom);

    const int baseW = std::max(1, right - left);
    const int baseH = std::max(1, bottom - top);
    const double marginScale = std::max(0.0, marginPercent) / 100.0;
    const int expandX = static_cast<int>(std::round(baseW * marginScale));
    const int expandY = static_cast<int>(std::round(baseH * marginScale));

    left = clampInt(left - expandX, 0, imgW - 1);
    right = clampInt(right + expandX, 0, imgW);
    top = clampInt(top - expandY, 0, imgH - 1);
    bottom = clampInt(bottom + expandY, 0, imgH);

    if (right <= left || bottom <= top) {
        result.error = "Invalid scan region";
        return result;
    }

    cv::Rect roi(left, top, right - left, bottom - top);
    cv::Mat cropped = image(roi);
    result = detectFromMat(cropped);

    // Tiny QR codes can decode better when the cropped region is upscaled.
    if (!result.found && (cropped.cols < 1200 || cropped.rows < 1200)) {
        cv::Mat upscaled2x;
        cv::resize(cropped, upscaled2x, cv::Size(), 2.0, 2.0, cv::INTER_CUBIC);
        auto upscaledResult = detectFromMat(upscaled2x);
        if (upscaledResult.found) {
            result = upscaledResult;
            scale_points(result.boundingBox, 0.5);
        } else {
            cv::Mat upscaled4x;
            cv::resize(cropped, upscaled4x, cv::Size(), 4.0, 4.0,
                       cv::INTER_CUBIC);
            auto upscaled4xResult = detectFromMat(upscaled4x);
            if (upscaled4xResult.found) {
                result = upscaled4xResult;
                scale_points(result.boundingBox, 0.25);
            }
        }
    }

    if (result.found && !result.boundingBox.empty()) {
        for (auto& point : result.boundingBox) {
            point.x += left;
            point.y += top;
        }
    }

    return result;
}

QRDetectionResult QRDetector::detectFromFile(const std::string& imagePath) {
    QRDetectionResult result;
    result.found = false;

    std::cout << "[QRDetector] Loading image: " << imagePath << std::endl;

    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
    if (image.empty()) {
        result.error = "Failed to load image: " + imagePath;
        std::cerr << "[QRDetector] " << result.error << std::endl;
        return result;
    }

    return detectFromMat(image);
}

QRDetectionResult QRDetector::detectFromMat(const cv::Mat& image) {
    QRDetectionResult result;
    result.found = false;

    if (image.empty()) {
        result.error = "Empty image provided";
        return result;
    }

    try {
        for (const auto& candidate : build_detection_candidates(image)) {
            std::vector<cv::Point> points;
            std::string decoded = detector_.detectAndDecode(candidate.image, points);

            if (!decoded.empty()) {
                scale_points(points, candidate.pointScale);
                result.found = true;
                result.decodedText = decoded;
                result.boundingBox = points;
                std::cout << "[QRDetector] Found QR code via "
                          << candidate.label << ": " << decoded << std::endl;
                return result;
            }
        }

        result.error = "No QR code found in image";
        std::cout << "[QRDetector] No QR code found" << std::endl;

    } catch (const cv::Exception& e) {
        result.error = std::string("OpenCV error: ") + e.what();
        std::cerr << "[QRDetector] " << result.error << std::endl;
    }

    return result;
}

std::vector<QRDetectionResult> QRDetector::detectAllFromMat(const cv::Mat& image) {
    std::vector<QRDetectionResult> results;

    if (image.empty()) {
        return results;
    }

    try {
        std::set<std::string> seenDecodedText;
        for (const auto& candidate : build_detection_candidates(image)) {
            std::vector<std::string> decoded;
            std::vector<cv::Point> points;

            bool success =
                detector_.detectAndDecodeMulti(candidate.image, decoded, points);

            if (!success || decoded.empty()) {
                continue;
            }

            for (size_t i = 0; i < decoded.size(); i++) {
                if (decoded[i].empty() || !seenDecodedText.insert(decoded[i]).second) {
                    continue;
                }

                QRDetectionResult result;
                result.found = true;
                result.decodedText = decoded[i];

                if (points.size() >= (i + 1) * 4) {
                    for (size_t j = 0; j < 4; j++) {
                        result.boundingBox.push_back(points[i * 4 + j]);
                    }
                    scale_points(result.boundingBox, candidate.pointScale);
                }

                results.push_back(result);
                std::cout << "[QRDetector] Found QR code " << results.size()
                          << " via " << candidate.label << ": "
                          << decoded[i] << std::endl;
            }
        }

    } catch (const cv::Exception& e) {
        std::cerr << "[QRDetector] OpenCV error in multi-detect: " << e.what()
                  << std::endl;
    }

    return results;
}

bool QRDetector::isOpenQualiaUrl(const std::string& text) {
    if (text.find("https://") != 0) {
        return false;
    }

    bool hasManufacturer = text.find("Manufacturer=") != std::string::npos;
    bool hasTargetType = text.find("TargetType=") != std::string::npos;
    bool hasTargetID = text.find("TargetID=") != std::string::npos;

    return hasManufacturer && hasTargetType && hasTargetID;
}

std::optional<std::string> QRDetector::findOpenQualiaUrl(const std::string& imagePath) {
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cerr << "[QRDetector] Failed to load image: " << imagePath << std::endl;
        return std::nullopt;
    }

    auto result = detectFromMat(image);
    if (result.found && isOpenQualiaUrl(result.decodedText)) {
        return result.decodedText;
    }

    auto allResults = detectAllFromMat(image);
    for (const auto& res : allResults) {
        if (res.found && isOpenQualiaUrl(res.decodedText)) {
            return res.decodedText;
        }
    }

    if (result.found) {
        std::cout << "[QRDetector] Found QR but not OpenQualia format: "
                  << result.decodedText << std::endl;
        return result.decodedText;
    }

    return std::nullopt;
}

} // namespace btrgb

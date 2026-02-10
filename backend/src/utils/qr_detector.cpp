#include <utils/qr_detector.hpp>
#include <iostream>
#include <algorithm>
#include <cmath>

namespace btrgb {

QRDetector::QRDetector() : detector_() {
    // QRCodeDetector is ready to use after construction
}

cv::Mat QRDetector::preprocessImage(const cv::Mat& image) {
    cv::Mat processed;
    
    // Convert to grayscale if color
    if (image.channels() == 3 || image.channels() == 4) {
        cv::cvtColor(image, processed, cv::COLOR_BGR2GRAY);
    } else {
        processed = image.clone();
    }
    
    // Normalize to improve contrast
    cv::normalize(processed, processed, 0, 255, cv::NORM_MINMAX);
    
    return processed;
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
            for (auto& point : result.boundingBox) {
                point.x = static_cast<int>(std::round(point.x / 2.0));
                point.y = static_cast<int>(std::round(point.y / 2.0));
            }
        } else {
            cv::Mat upscaled4x;
            cv::resize(cropped, upscaled4x, cv::Size(), 4.0, 4.0, cv::INTER_CUBIC);
            auto upscaled4xResult = detectFromMat(upscaled4x);
            if (upscaled4xResult.found) {
                result = upscaled4xResult;
                for (auto& point : result.boundingBox) {
                    point.x = static_cast<int>(std::round(point.x / 4.0));
                    point.y = static_cast<int>(std::round(point.y / 4.0));
                }
            }
        }
    }

    // Re-project detected bounding box to original image coordinates.
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
        // Try detection on original image first
        std::vector<cv::Point> points;
        std::string decoded = detector_.detectAndDecode(image, points);
        
        if (!decoded.empty()) {
            result.found = true;
            result.decodedText = decoded;
            result.boundingBox = points;
            std::cout << "[QRDetector] Found QR code: " << decoded << std::endl;
            return result;
        }
        
        // If not found, try with preprocessed image
        cv::Mat processed = preprocessImage(image);
        decoded = detector_.detectAndDecode(processed, points);
        
        if (!decoded.empty()) {
            result.found = true;
            result.decodedText = decoded;
            result.boundingBox = points;
            std::cout << "[QRDetector] Found QR code (preprocessed): " << decoded << std::endl;
            return result;
        }
        
        // Try with different scales if image is very large
        if (image.cols > 4000 || image.rows > 4000) {
            std::cout << "[QRDetector] Trying scaled detection for large image" << std::endl;
            
            // Try half scale
            cv::Mat scaled;
            cv::resize(image, scaled, cv::Size(), 0.5, 0.5, cv::INTER_AREA);
            decoded = detector_.detectAndDecode(scaled, points);
            
            if (!decoded.empty()) {
                result.found = true;
                result.decodedText = decoded;
                // Scale points back
                for (auto& pt : points) {
                    pt.x *= 2;
                    pt.y *= 2;
                }
                result.boundingBox = points;
                std::cout << "[QRDetector] Found QR code (scaled): " << decoded << std::endl;
                return result;
            }
            
            // Try quarter scale
            cv::resize(image, scaled, cv::Size(), 0.25, 0.25, cv::INTER_AREA);
            decoded = detector_.detectAndDecode(scaled, points);
            
            if (!decoded.empty()) {
                result.found = true;
                result.decodedText = decoded;
                for (auto& pt : points) {
                    pt.x *= 4;
                    pt.y *= 4;
                }
                result.boundingBox = points;
                std::cout << "[QRDetector] Found QR code (quarter scale): " << decoded << std::endl;
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
        std::vector<std::string> decoded;
        std::vector<cv::Point> points;
        
        // detectAndDecodeMulti returns all QR codes
        bool success = detector_.detectAndDecodeMulti(image, decoded, points);
        
        if (success && !decoded.empty()) {
            // Points are returned as groups of 4 for each QR code
            for (size_t i = 0; i < decoded.size(); i++) {
                QRDetectionResult result;
                result.found = true;
                result.decodedText = decoded[i];
                
                // Extract bounding box for this QR code (4 points per QR)
                if (points.size() >= (i + 1) * 4) {
                    for (size_t j = 0; j < 4; j++) {
                        result.boundingBox.push_back(points[i * 4 + j]);
                    }
                }
                
                results.push_back(result);
                std::cout << "[QRDetector] Found QR code " << (i + 1) << ": " 
                          << decoded[i] << std::endl;
            }
        }
        
    } catch (const cv::Exception& e) {
        std::cerr << "[QRDetector] OpenCV error in multi-detect: " << e.what() << std::endl;
    }
    
    return results;
}

bool QRDetector::isOpenQualiaUrl(const std::string& text) {
    // Check for required OpenQualia URL components
    if (text.find("http") != 0) {
        return false;
    }
    
    bool hasManufacturer = text.find("Manufacturer=") != std::string::npos;
    bool hasTargetType = text.find("TargetType=") != std::string::npos;
    bool hasTargetID = text.find("TargetID=") != std::string::npos;
    
    // Also check for common OpenQualia domains
    bool hasKnownDomain = 
        text.find("digitaltransitions.com") != std::string::npos ||
        text.find("openqualia") != std::string::npos ||
        text.find("targets.") != std::string::npos;
    
    return (hasManufacturer && hasTargetType && hasTargetID) || hasKnownDomain;
}

std::optional<std::string> QRDetector::findOpenQualiaUrl(const std::string& imagePath) {
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cerr << "[QRDetector] Failed to load image: " << imagePath << std::endl;
        return std::nullopt;
    }
    
    // Try single detection first
    auto result = detectFromMat(image);
    if (result.found && isOpenQualiaUrl(result.decodedText)) {
        return result.decodedText;
    }
    
    // If single detection didn't find OpenQualia URL, try multi-detection
    auto allResults = detectAllFromMat(image);
    for (const auto& res : allResults) {
        if (res.found && isOpenQualiaUrl(res.decodedText)) {
            return res.decodedText;
        }
    }
    
    // Return whatever we found, even if not a known OpenQualia format
    if (result.found) {
        std::cout << "[QRDetector] Found QR but not OpenQualia format: " 
                  << result.decodedText << std::endl;
        return result.decodedText;
    }
    
    return std::nullopt;
}

} // namespace btrgb

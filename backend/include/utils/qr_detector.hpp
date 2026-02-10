#ifndef QR_DETECTOR_HPP
#define QR_DETECTOR_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <string>
#include <vector>
#include <optional>

namespace btrgb {

/**
 * @brief Result of QR code detection
 */
struct QRDetectionResult {
    bool found;
    std::string decodedText;
    std::vector<cv::Point> boundingBox;
    std::string error;
};

/**
 * @brief Region definition for focused QR scanning.
 *
 * Pixel coordinates are expected in the source image space.
 */
struct QRScanRegion {
    double top;
    double left;
    double bottom;
    double right;
};

/**
 * @brief QR code detector using OpenCV's QRCodeDetector
 * 
 * This class provides methods to detect and decode QR codes in images,
 * specifically designed to extract OpenQualia URLs from color target images.
 */
class QRDetector {
public:
    QRDetector();
    ~QRDetector() = default;

    /**
     * @brief Detect and decode QR code from an image file
     * 
     * @param imagePath Path to the image file
     * @return QRDetectionResult containing the decoded text if found
     */
    QRDetectionResult detectFromFile(const std::string& imagePath);

    /**
     * @brief Detect and decode QR code from a cv::Mat image
     * 
     * @param image The OpenCV Mat image
     * @return QRDetectionResult containing the decoded text if found
     */
    QRDetectionResult detectFromMat(const cv::Mat& image);

    /**
     * @brief Detect and decode QR code from a region of interest.
     *
     * Useful for target-driven scans where QR labels are expected near a known
     * target area. The region can optionally be expanded by marginPercent.
     *
     * @param image Source image
     * @param region Region of interest in pixel coordinates
     * @param marginPercent Margin expansion around region (e.g. 10.0 = 10%)
     * @return QRDetectionResult containing decoded text if found
     */
    QRDetectionResult detectFromMatRegion(const cv::Mat& image,
                                          const QRScanRegion& region,
                                          double marginPercent = 10.0);

    /**
     * @brief Detect and decode all QR codes in an image
     * 
     * Some targets may have multiple QR codes. This method returns all of them.
     * 
     * @param image The OpenCV Mat image
     * @return Vector of QRDetectionResult for each QR code found
     */
    std::vector<QRDetectionResult> detectAllFromMat(const cv::Mat& image);

    /**
     * @brief Check if a decoded string is a valid OpenQualia URL
     * 
     * @param text The decoded QR text
     * @return true if it's a valid OpenQualia URL
     */
    static bool isOpenQualiaUrl(const std::string& text);

    /**
     * @brief Find OpenQualia URL from an image
     * 
     * Convenience method that detects QR codes and returns the first
     * one that matches OpenQualia URL format.
     * 
     * @param imagePath Path to the image file
     * @return Optional string containing the OpenQualia URL if found
     */
    std::optional<std::string> findOpenQualiaUrl(const std::string& imagePath);

private:
    cv::QRCodeDetector detector_;
    
    /**
     * @brief Preprocess image to improve QR detection
     * 
     * Applies various image processing techniques to enhance QR visibility.
     */
    cv::Mat preprocessImage(const cv::Mat& image);
};

} // namespace btrgb

#endif // QR_DETECTOR_HPP

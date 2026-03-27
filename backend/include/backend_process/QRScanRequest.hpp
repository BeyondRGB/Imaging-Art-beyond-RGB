#ifndef QRSCANREQUEST_HPP
#define QRSCANREQUEST_HPP

#include <backend_process/backend_process.hpp>
#include <utils/qr_detector.hpp>
#include <string>

/**
 * @brief Backend process for scanning QR codes from images
 * 
 * Request format:
 * {
 *     "RequestType": "ScanQRCode",
 *     "RequestData": {
 *         "imagePath": "/path/to/target/image.tif"
 *     }
 * }
 * 
 * Response format:
 * {
 *     "ResponseType": "QRCodeScanned",
 *     "ResponseData": {
 *         "found": true,
 *         "url": "https://...",
 *         "isOpenQualia": true,
 *         "error": null
 *     }
 * }
 */
class QRScanRequest : public BackendProcess {
public:
    QRScanRequest(std::string name) : BackendProcess(name) {}
    ~QRScanRequest() = default;

    /**
     * @brief Execute the QR scan request
     */
    void run() override;

private:
    /**
     * @brief Send success response with QR data
     */
    void sendSuccessResponse(const std::string& url, bool isOpenQualia);

    /**
     * @brief Send error/not found response
     */
    void sendErrorResponse(const std::string& error);

    btrgb::QRDetector detector_;
};

#endif // QRSCANREQUEST_HPP

#ifndef OPENQUALIAREQUEST_HPP
#define OPENQUALIAREQUEST_HPP

#include <backend_process/backend_process.hpp>
#include <reference_data/oqm_parser.hpp>
#include <utils/qr_detector.hpp>
#include <string>
#include <memory>

/**
 * @brief Backend process for handling OpenQualia requests
 * 
 * This process handles:
 * - Fetching OQM files from OpenQualia URLs
 * - Loading local OQM files
 * - Parsing OQM/CGATS format
 * - Converting to RefData-compatible CSV
 * - Scanning QR codes from target images
 * 
 * Request format for FetchOpenQualia:
 * {
 *     "RequestType": "FetchOpenQualia",
 *     "RequestData": {
 *         "url": "https://...",     // OpenQualia URL (optional)
 *         "filePath": "/path/..."   // Local file path (optional)
 *     }
 * }
 * 
 * Request format for ScanQRCode:
 * {
 *     "RequestType": "ScanQRCode",
 *     "RequestData": {
 *         "imagePath": "/path/to/target/image.tif"
 *     }
 * }
 * 
 * Response format for OpenQualiaLoaded:
 * {
 *     "ResponseType": "OpenQualiaLoaded",
 *     "ResponseData": {
 *         "success": true,
 *         "targetName": "DT NGT2",
 *         "serialNumber": "...",
 *         "measurementDate": "2024-01-15",
 *         "patchCount": 140,
 *         "csvPath": "/tmp/openqualia_xxx.csv",
 *         "error": null
 *     }
 * }
 * 
 * Response format for QRCodeScanned:
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
class OpenQualiaRequest : public BackendProcess {
public:
    OpenQualiaRequest(std::string name) : BackendProcess(name) {}
    ~OpenQualiaRequest() = default;

    /**
     * @brief Execute the OpenQualia request
     */
    void run() override;

private:
    /**
     * @brief Fetch OQM content from URL
     */
    std::string fetchFromUrl(const std::string& url);

    /**
     * @brief Load OQM content from local file
     */
    std::string loadFromFile(const std::string& filePath);

    /**
     * @brief Process OQM content and send response
     */
    void processOQMContent(const std::string& content, const std::string& source);

    /**
     * @brief Send success response with metadata
     */
    void sendSuccessResponse(const btrgb::OQMMetadata& metadata, 
                             const std::string& csvPath);

    /**
     * @brief Send error response
     */
    void sendErrorResponse(const std::string& error);

    /**
     * @brief Generate a unique temp file path for the CSV
     */
    std::string generateTempCsvPath() const;
};

#endif // OPENQUALIAREQUEST_HPP

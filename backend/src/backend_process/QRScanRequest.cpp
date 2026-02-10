#include <backend_process/QRScanRequest.hpp>
#include <iostream>

void QRScanRequest::run() {
    std::cout << "[QRScan] Processing request" << std::endl;
    
    try {
        std::string imagePath = "";
        
        try {
            imagePath = this->process_data_m->get_string("imagePath");
        } catch (...) {
            sendErrorResponse("No image path provided");
            return;
        }
        
        if (imagePath.empty()) {
            sendErrorResponse("Empty image path");
            return;
        }
        
        std::cout << "[QRScan] Scanning image: " << imagePath << std::endl;

        cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
        if (image.empty()) {
            sendErrorResponse("Failed to load image");
            return;
        }

        bool hasRegion = false;
        btrgb::QRScanRegion region{};
        double marginPercent = 10.0;

        try {
            Json regionJson = this->process_data_m->get_obj("targetRegion");
            hasRegion = true;

            double resolution = 1.0;
            try {
                resolution = regionJson.get_number("resolution");
            } catch (...) {
                // Optional; defaults to 1.0
            }

            const double top = regionJson.get_number("top");
            const double left = regionJson.get_number("left");
            const double bottom = regionJson.get_number("bottom");
            const double right = regionJson.get_number("right");

            // If coordinates look normalized (0..1), convert to pixel coordinates.
            // Use explicit resolution when provided; otherwise derive from image dims.
            const bool normalizedCoords =
                top >= 0.0 && top <= 1.0 &&
                left >= 0.0 && left <= 1.0 &&
                bottom >= 0.0 && bottom <= 1.0 &&
                right >= 0.0 && right <= 1.0;

            if (normalizedCoords) {
                const double scaleY = resolution > 1.0 ? resolution : static_cast<double>(image.rows);
                const double scaleX = resolution > 1.0 ? resolution : static_cast<double>(image.cols);
                region.top = top * scaleY;
                region.left = left * scaleX;
                region.bottom = bottom * scaleY;
                region.right = right * scaleX;
            } else {
                region.top = top;
                region.left = left;
                region.bottom = bottom;
                region.right = right;
            }
        } catch (...) {
            hasRegion = false;
        }

        try {
            marginPercent = this->process_data_m->get_number("marginPercent");
        } catch (...) {
            marginPercent = 10.0;
        }

        btrgb::QRDetectionResult scanResult;
        if (hasRegion) {
            scanResult = detector_.detectFromMatRegion(image, region, marginPercent);
        } else {
            scanResult = detector_.detectFromMat(image);
        }

        if (scanResult.found) {
            bool isOpenQualia = btrgb::QRDetector::isOpenQualiaUrl(scanResult.decodedText);
            sendSuccessResponse(scanResult.decodedText, isOpenQualia);
        } else {
            sendErrorResponse(scanResult.error.empty() ? "No QR code found in image" : scanResult.error);
        }
        
    } catch (const std::exception& e) {
        std::cerr << "[QRScan] Exception: " << e.what() << std::endl;
        sendErrorResponse(std::string("Exception: ") + e.what());
    }
}

void QRScanRequest::sendSuccessResponse(const std::string& url, bool isOpenQualia) {
    jsoncons::json response;
    response["ResponseType"] = "QRCodeScanned";
    response["RequestID"] = this->coms_obj_m->get_id();
    
    jsoncons::json data;
    data["found"] = true;
    data["url"] = url;
    data["isOpenQualia"] = isOpenQualia;
    data["error"] = jsoncons::json::null();
    
    response["ResponseData"] = data;
    
    std::string responseStr = response.as<std::string>();
    std::cout << "[QRScan] Sending success response: " << url << std::endl;
    this->coms_obj_m->send_raw(responseStr);
}

void QRScanRequest::sendErrorResponse(const std::string& error) {
    jsoncons::json response;
    response["ResponseType"] = "QRCodeScanned";
    response["RequestID"] = this->coms_obj_m->get_id();
    
    jsoncons::json data;
    data["found"] = false;
    data["url"] = "";
    data["isOpenQualia"] = false;
    data["error"] = error;
    
    response["ResponseData"] = data;
    
    std::string responseStr = response.as<std::string>();
    std::cerr << "[QRScan] Sending error response: " << error << std::endl;
    this->coms_obj_m->send_raw(responseStr);
}

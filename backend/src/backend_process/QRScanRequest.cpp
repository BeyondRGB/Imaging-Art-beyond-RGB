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
        
        // Try to find an OpenQualia URL
        auto result = detector_.findOpenQualiaUrl(imagePath);
        
        if (result.has_value()) {
            bool isOpenQualia = btrgb::QRDetector::isOpenQualiaUrl(result.value());
            sendSuccessResponse(result.value(), isOpenQualia);
        } else {
            sendErrorResponse("No QR code found in image");
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
    this->coms_obj_m->send_msg(responseStr);
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
    this->coms_obj_m->send_msg(responseStr);
}

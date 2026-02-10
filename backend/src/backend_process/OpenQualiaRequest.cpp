#include <backend_process/OpenQualiaRequest.hpp>
#include <utils/http_client.hpp>
#include <cpptrace/cpptrace.hpp>

#include <fstream>
#include <sstream>
#include <filesystem>
#include <chrono>
#include <random>

void OpenQualiaRequest::run() {
    std::cout << "[OpenQualia] Processing request" << std::endl;
    
    try {
        // Get request parameters
        std::string url = "";
        std::string filePath = "";
        
        try {
            url = this->process_data_m->get_string("url");
        } catch (...) {
            // url not provided
        }
        
        try {
            filePath = this->process_data_m->get_string("filePath");
        } catch (...) {
            // filePath not provided
        }
        
        std::string content;
        std::string source;
        
        if (!url.empty()) {
            // Fetch from URL
            std::cout << "[OpenQualia] Fetching from URL: " << url << std::endl;
            content = fetchFromUrl(url);
            source = "url";
        } else if (!filePath.empty()) {
            // Load from file
            std::cout << "[OpenQualia] Loading from file: " << filePath << std::endl;
            content = loadFromFile(filePath);
            source = "file";
        } else {
            sendErrorResponse("No URL or file path provided");
            return;
        }
        
        if (content.empty()) {
            // Error already sent by fetch/load function
            return;
        }
        
        processOQMContent(content, source);
        
    } catch (const std::exception& e) {
        std::cerr << "[OpenQualia] Exception: " << e.what() << std::endl;
        sendErrorResponse(std::string("Exception: ") + e.what());
    }
}

std::string OpenQualiaRequest::fetchFromUrl(const std::string& url) {
    // Validate URL
    if (!btrgb::HttpClient::isValidOpenQualiaUrl(url)) {
        sendErrorResponse("Invalid OpenQualia URL. Must be HTTPS and contain Manufacturer, TargetType, and TargetID parameters.");
        return "";
    }
    
    // Fetch the file
    auto response = btrgb::HttpClient::fetchOpenQualia(url);
    
    if (!response.success) {
        sendErrorResponse("Failed to fetch from URL: " + response.error);
        return "";
    }
    
    return response.body;
}

std::string OpenQualiaRequest::loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        sendErrorResponse("Failed to open file: " + filePath);
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void OpenQualiaRequest::processOQMContent(const std::string& content, 
                                          const std::string& source) {
    // Validate format
    if (!btrgb::OQMParser::isValidFormat(content)) {
        sendErrorResponse("Invalid OQM/CGATS format. File must start with 'OQM' or 'CGATS.17'");
        return;
    }
    
    // Parse the content
    btrgb::OQMParser parser;
    if (!parser.parse(content)) {
        sendErrorResponse("Failed to parse OQM file: " + parser.getError());
        return;
    }
    
    // Generate temp CSV path
    std::string csvPath = generateTempCsvPath();
    
    // Write CSV for RefData consumption
    if (!parser.writeToCSV(csvPath)) {
        sendErrorResponse("Failed to write temporary CSV file");
        return;
    }
    
    // Send success response
    sendSuccessResponse(parser.getMetadata(), csvPath);
}

void OpenQualiaRequest::sendSuccessResponse(const btrgb::OQMMetadata& metadata,
                                            const std::string& csvPath) {
    jsoncons::json response;
    response["ResponseType"] = "OpenQualiaLoaded";
    response["RequestID"] = this->coms_obj_m->get_id();
    
    jsoncons::json data;
    data["success"] = true;
    data["targetName"] = metadata.targetName;
    data["serialNumber"] = metadata.serialNumber;
    data["measurementDate"] = metadata.measurementDate;
    data["patchCount"] = metadata.patchCount;
    data["spectralBands"] = metadata.spectralBands;
    data["csvPath"] = csvPath;
    data["error"] = jsoncons::json::null();
    
    response["ResponseData"] = data;
    
    std::string responseStr = response.as<std::string>();
    std::cout << "[OpenQualia] Sending success response" << std::endl;
    this->coms_obj_m->send_raw(responseStr);
}

void OpenQualiaRequest::sendErrorResponse(const std::string& error) {
    jsoncons::json response;
    response["ResponseType"] = "OpenQualiaLoaded";
    response["RequestID"] = this->coms_obj_m->get_id();
    
    jsoncons::json data;
    data["success"] = false;
    data["targetName"] = "";
    data["serialNumber"] = "";
    data["measurementDate"] = "";
    data["patchCount"] = 0;
    data["csvPath"] = "";
    data["error"] = error;
    
    response["ResponseData"] = data;
    
    std::string responseStr = response.as<std::string>();
    std::cerr << "[OpenQualia] Sending error response: " << error << std::endl;
    this->coms_obj_m->send_raw(responseStr);
}

std::string OpenQualiaRequest::generateTempCsvPath() const {
    // Get temp directory
    std::filesystem::path tempDir = std::filesystem::temp_directory_path();
    
    // Generate unique filename
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()).count();
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1000, 9999);
    
    std::string filename = "openqualia_" + std::to_string(timestamp) + 
                          "_" + std::to_string(dis(gen)) + ".csv";
    
    return (tempDir / filename).string();
}

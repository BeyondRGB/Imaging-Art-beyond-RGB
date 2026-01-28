#include <utils/http_client.hpp>

// Enable OpenSSL support for HTTPS
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

#include <regex>
#include <iostream>

namespace btrgb {

std::optional<HttpClient::UrlComponents> HttpClient::parseUrl(const std::string& url) {
    // Simple URL parser for https://host:port/path format
    std::regex urlRegex(R"(^(https?)://([^/:]+)(?::(\d+))?(/.*)?$)");
    std::smatch matches;
    
    if (!std::regex_match(url, matches, urlRegex)) {
        return std::nullopt;
    }
    
    UrlComponents components;
    components.scheme = matches[1].str();
    components.host = matches[2].str();
    
    if (matches[3].matched) {
        components.port = std::stoi(matches[3].str());
    } else {
        components.port = (components.scheme == "https") ? 443 : 80;
    }
    
    components.path = matches[4].matched ? matches[4].str() : "/";
    
    return components;
}

HttpResponse HttpClient::fetch(const std::string& url, int timeoutSeconds) {
    HttpResponse response;
    response.success = false;
    response.statusCode = 0;
    
    auto components = parseUrl(url);
    if (!components) {
        response.error = "Invalid URL format";
        return response;
    }
    
    // Only allow HTTPS for security
    if (components->scheme != "https") {
        response.error = "Only HTTPS URLs are supported";
        return response;
    }
    
    try {
        // Create HTTPS client
        httplib::SSLClient client(components->host, components->port);
        client.set_connection_timeout(timeoutSeconds, 0);
        client.set_read_timeout(timeoutSeconds, 0);
        client.set_write_timeout(timeoutSeconds, 0);
        
        // Enable certificate verification
        client.enable_server_certificate_verification(true);
        
        std::cout << "[HTTP] Fetching: " << url << std::endl;
        
        // Make GET request
        auto result = client.Get(components->path.c_str());
        
        if (!result) {
            auto err = result.error();
            response.error = "Request failed: " + httplib::to_string(err);
            std::cerr << "[HTTP] " << response.error << std::endl;
            return response;
        }
        
        response.statusCode = result->status;
        response.body = result->body;
        
        if (result->status >= 200 && result->status < 300) {
            response.success = true;
            std::cout << "[HTTP] Success: " << result->status << ", " 
                      << result->body.size() << " bytes" << std::endl;
        } else {
            response.error = "HTTP " + std::to_string(result->status);
            std::cerr << "[HTTP] " << response.error << std::endl;
        }
        
    } catch (const std::exception& e) {
        response.error = std::string("Exception: ") + e.what();
        std::cerr << "[HTTP] " << response.error << std::endl;
    }
    
    return response;
}

HttpResponse HttpClient::fetchOpenQualia(const std::string& url) {
    std::string fetchUrl = url;
    
    // Append AccessMode=ActiveMeasurement if not present
    if (url.find("AccessMode=") == std::string::npos) {
        if (url.find('?') != std::string::npos) {
            fetchUrl += "&AccessMode=ActiveMeasurement";
        } else {
            fetchUrl += "?AccessMode=ActiveMeasurement";
        }
    }
    
    return fetch(fetchUrl);
}

bool HttpClient::isValidOpenQualiaUrl(const std::string& url) {
    // Must be HTTPS
    if (url.find("https://") != 0) {
        return false;
    }
    
    // Must contain required parameters
    bool hasManufacturer = url.find("Manufacturer=") != std::string::npos;
    bool hasTargetType = url.find("TargetType=") != std::string::npos;
    bool hasTargetID = url.find("TargetID=") != std::string::npos;
    
    return hasManufacturer && hasTargetType && hasTargetID;
}

} // namespace btrgb

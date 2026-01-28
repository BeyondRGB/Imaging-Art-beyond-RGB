#ifndef HTTP_CLIENT_HPP
#define HTTP_CLIENT_HPP

#include <string>
#include <optional>

namespace btrgb {

/**
 * @brief Result of an HTTP request
 */
struct HttpResponse {
    bool success;
    int statusCode;
    std::string body;
    std::string error;
};

/**
 * @brief Simple HTTP client for fetching OpenQualia measurement files
 * 
 * Uses cpp-httplib for HTTPS requests with OpenSSL support.
 */
class HttpClient {
public:
    /**
     * @brief Fetch content from a URL
     * 
     * @param url The URL to fetch (must be HTTPS)
     * @param timeoutSeconds Request timeout in seconds (default 30)
     * @return HttpResponse containing the result
     */
    static HttpResponse fetch(const std::string& url, int timeoutSeconds = 30);

    /**
     * @brief Fetch OpenQualia measurement file
     * 
     * Automatically appends &AccessMode=ActiveMeasurement to the URL
     * if not already present.
     * 
     * @param url The OpenQualia URL
     * @return HttpResponse containing the OQM/CGATS content
     */
    static HttpResponse fetchOpenQualia(const std::string& url);

    /**
     * @brief Validate that a URL is a valid OpenQualia URL
     * 
     * Checks for required parameters: Manufacturer, TargetType, TargetID
     * 
     * @param url The URL to validate
     * @return true if valid, false otherwise
     */
    static bool isValidOpenQualiaUrl(const std::string& url);

private:
    /**
     * @brief Parse URL into host, port, and path components
     */
    struct UrlComponents {
        std::string scheme;
        std::string host;
        int port;
        std::string path;
    };
    
    static std::optional<UrlComponents> parseUrl(const std::string& url);
};

} // namespace btrgb

#endif // HTTP_CLIENT_HPP

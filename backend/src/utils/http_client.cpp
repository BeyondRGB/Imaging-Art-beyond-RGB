#include <utils/http_client.hpp>

#include <httplib.h>

#include <regex>
#include <array>
#include <iostream>

namespace btrgb {

namespace { // File-local HTTP parsing and download-resolution helpers.

void replace_all(std::string& text,
                 const std::string& from,
                 const std::string& to) {
    size_t startPos = 0;
    while ((startPos = text.find(from, startPos)) != std::string::npos) {
        text.replace(startPos, from.size(), to);
        startPos += to.size();
    }
}

std::string decodeHtmlEntities(std::string value) {
    replace_all(value, "&amp;", "&");
    replace_all(value, "&quot;", "\"");
    replace_all(value, "&#39;", "'");
    replace_all(value, "&lt;", "<");
    replace_all(value, "&gt;", ">");
    return value;
}

bool startsWith(const std::string& value, const std::string& prefix) {
    return value.rfind(prefix, 0) == 0;
}

std::optional<std::string> resolveUrl(const std::string& baseUrl,
                                      std::string candidateUrl) {
    if (candidateUrl.empty()) {
        return std::nullopt;
    }

    candidateUrl = decodeHtmlEntities(candidateUrl);

    if (startsWith(candidateUrl, "https://")) {
        return candidateUrl;
    }

    // Matches scheme://host[:port][/path] so relative links can be resolved
    // against the original download page.
    std::regex urlRegex(R"(^(https?)://([^/:]+)(?::(\d+))?(/.*)?$)");
    std::smatch matches;
    if (!std::regex_match(baseUrl, matches, urlRegex)) {
        return std::nullopt;
    }

    const std::string scheme = matches[1].str();
    const std::string host = matches[2].str();

    if (startsWith(candidateUrl, "//")) {
        return scheme + ":" + candidateUrl;
    }

    if (startsWith(candidateUrl, "/")) {
        return scheme + "://" + host + candidateUrl;
    }

    return std::nullopt;
}

} // namespace

std::optional<HttpClient::UrlComponents> HttpClient::parseUrl(const std::string& url) {
    // Matches scheme://host[:port][/path] and leaves policy decisions to fetch().
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
    
    std::optional<UrlComponents> components = parseUrl(url);
    if (!components) {
        response.error = "Invalid URL format";
        return response;
    }
    
    // URL parsing is generic, but remote OpenQualia fetches are restricted to
    // HTTPS so measurement retrieval stays on encrypted transport.
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
        client.set_follow_location(true);
        
        // Enable certificate verification
        client.enable_server_certificate_verification(true);
        
        std::cout << "[HTTP] Fetching: " << url << std::endl;
        
        // Make GET request
        httplib::Result result = client.Get(components->path.c_str());

        if (!result) {
            httplib::Error err = result.error();
            response.error = "Request failed: " + httplib::to_string(err);
            std::cerr << "[HTTP] " << response.error << std::endl;
            return response;
        }
        
        response.statusCode = result->status;
        response.body = result->body;
        response.contentType = result->get_header_value("Content-Type");
        
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
    std::string fetchUrl = setOrReplaceQueryParam(url, "AccessMode",
                                                  "ActiveMeasurement");
    fetchUrl = normalizeDropboxDownloadUrl(fetchUrl);

    HttpResponse response = fetch(fetchUrl);
    if (!response.success) {
        return response;
    }

    if (!responseLooksLikeHtml(response)) {
        return response;
    }

    std::optional<std::string> downloadUrl =
        extractDownloadUrlFromHtml(fetchUrl, response.body);
    if (!downloadUrl) {
        response.success = false;
        response.error = "Received HTML page instead of measurement data";
        return response;
    }

    std::cout << "[HTTP] Resolved download page to: " << *downloadUrl
              << std::endl;
    return fetch(normalizeDropboxDownloadUrl(*downloadUrl));
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

std::string HttpClient::setOrReplaceQueryParam(const std::string& url,
                                               const std::string& key,
                                               const std::string& value) {
    std::string updatedUrl = url;
    const std::regex queryRegex("([?&])" + key + "=[^&]*");
    std::smatch match;

    if (std::regex_search(updatedUrl, match, queryRegex)) {
        updatedUrl.replace(match.position(0), match.length(0),
                           match[1].str() + key + "=" + value);
    } else if (updatedUrl.find('?') != std::string::npos) {
        updatedUrl += "&" + key + "=" + value;
    } else {
        updatedUrl += "?" + key + "=" + value;
    }

    return updatedUrl;
}

std::string HttpClient::normalizeDropboxDownloadUrl(const std::string& url) {
    std::optional<UrlComponents> components = parseUrl(url);
    if (!components) {
        return url;
    }

    if (components->host.find("dropbox.com") == std::string::npos) {
        return url;
    }

    std::string normalizedUrl = setOrReplaceQueryParam(url, "raw", "1");
    normalizedUrl = setOrReplaceQueryParam(normalizedUrl, "dl", "1");
    return normalizedUrl;
}

bool HttpClient::responseLooksLikeHtml(const HttpResponse& response) {
    if (response.contentType.find("text/html") != std::string::npos) {
        return true;
    }

    const std::string::size_type firstNonWhitespace =
        response.body.find_first_not_of(" \t\r\n");
    if (firstNonWhitespace == std::string::npos) {
        return false;
    }

    return response.body.compare(firstNonWhitespace, 5, "<html") == 0 ||
           response.body.compare(firstNonWhitespace, 9, "<!DOCTYPE") == 0;
}

std::optional<std::string> HttpClient::extractDownloadUrlFromHtml(
    const std::string& baseUrl,
    const std::string& body) {
    // Keep these compiled once because they are used on every HTML download-page
    // fallback and the patterns are fixed.
    static const std::array<std::regex, 4> patterns = {
        // Common anchor targets for raw/download links.
        std::regex(R"(href\s*=\s*["']([^"']*(?:raw=1|dl=1|download|\.oqm)[^"']*)["'])",
                   std::regex::icase),
        // Meta refresh redirects that point at the measurement file.
        std::regex(R"(content\s*=\s*["'][^"']*url=([^"']+)["'])",
                   std::regex::icase),
        // Script-driven redirects used by some file hosting pages.
        std::regex(R"(window\.location(?:\.href)?\s*=\s*["']([^"']+)["'])",
                   std::regex::icase),
        // Direct absolute URLs embedded in page content.
        std::regex(R"(https://[^"'\s>]+(?:raw=1|dl=1|download|\.oqm)[^"'\s<]*)",
                   std::regex::icase),
    };

    for (const std::regex& pattern : patterns) {
        std::smatch match;
        if (!std::regex_search(body, match, pattern)) {
            continue;
        }

        const std::string candidate =
            match.size() > 1 ? match[1].str() : match[0].str();
        std::optional<std::string> resolvedUrl = resolveUrl(baseUrl, candidate);
        if (resolvedUrl) {
            return resolvedUrl;
        }
    }

    return std::nullopt;
}

} // namespace btrgb

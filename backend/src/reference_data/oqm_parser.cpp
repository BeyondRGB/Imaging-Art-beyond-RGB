#include <reference_data/oqm_parser.hpp>

#include <sstream>
#include <fstream>
#include <algorithm>
#include <regex>
#include <iostream>
#include <cctype>

namespace btrgb {

bool OQMParser::isValidFormat(const std::string& content) {
    // Check if content starts with OQM or CGATS identifier
    if (content.empty()) return false;
    
    // First non-whitespace should be "OQM" or "CGATS"
    size_t start = content.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return false;
    
    std::string firstLine = content.substr(start, 10);
    return (firstLine.find("OQM") == 0 || 
            firstLine.find("CGATS") == 0);
}

std::string OQMParser::stripQuotes(const std::string& value) {
    std::string result = value;
    if (result.size() >= 2) {
        if ((result.front() == '"' && result.back() == '"') ||
            (result.front() == '\'' && result.back() == '\'')) {
            result = result.substr(1, result.size() - 2);
        }
    }
    return result;
}

void OQMParser::parseKeyword(const std::string& line) {
    // Skip comments and empty lines
    if (line.empty() || line[0] == '#') return;
    
    // Find first whitespace to split keyword from value
    size_t spacePos = line.find_first_of(" \t");
    if (spacePos == std::string::npos) return;
    
    std::string keyword = line.substr(0, spacePos);
    std::string value = line.substr(spacePos + 1);
    
    // Trim whitespace from value
    size_t valueStart = value.find_first_not_of(" \t");
    if (valueStart != std::string::npos) {
        value = value.substr(valueStart);
    }
    value = stripQuotes(value);
    
    // Map keywords to metadata
    if (keyword == "DESCRIPTOR") {
        metadata_.targetName = value;
    } else if (keyword == "SERIAL") {
        metadata_.serialNumber = value;
    } else if (keyword == "CREATED") {
        metadata_.measurementDate = value;
    } else if (keyword == "ORIGINATOR") {
        metadata_.originator = value;
    } else if (keyword == "TARGET_INSTRUMENT") {
        metadata_.instrument = value;
    } else if (keyword == "NUMBER_OF_SETS") {
        metadata_.patchCount = std::stoi(value);
    } else if (keyword == "SPECTRAL_BANDS") {
        metadata_.spectralBands = std::stoi(value);
    } else if (keyword == "SPECTRAL_START_NM") {
        metadata_.spectralStartNm = static_cast<int>(std::stod(value));
    } else if (keyword == "SPECTRAL_END_NM") {
        metadata_.spectralEndNm = static_cast<int>(std::stod(value));
    }
}

void OQMParser::parseDataFormat(const std::vector<std::string>& formatLines) {
    spectralIndices_.clear();
    wavelengths_.clear();
    sampleNameIndex_ = -1;
    
    int index = 0;
    for (const auto& field : formatLines) {
        if (field == "SAMPLE_NAME" || field == "SAMPLE_ID") {
            sampleNameIndex_ = index;
        } else if (field.find("SPEC_") == 0 || field.find("nm") == 0 || 
                   field.find("SPECTRAL_NM") == 0) {
            // Extract wavelength from field name
            std::regex numRegex(R"(\d+)");
            std::smatch match;
            if (std::regex_search(field, match, numRegex)) {
                int wavelength = std::stoi(match[0]);
                spectralIndices_.push_back(index);
                wavelengths_.push_back(wavelength);
            }
        }
        index++;
    }
    
    std::cout << "[OQM] Found " << spectralIndices_.size() << " spectral columns" << std::endl;
}

std::pair<int, int> OQMParser::patchNameToRowCol(const std::string& name) const {
    // Parse patch names like "A1", "A-1", "B2", etc.
    // Letter = column, Number = row
    std::regex patchRegex(R"(([A-Za-z]+)-?(\d+))");
    std::smatch match;
    
    if (std::regex_match(name, match, patchRegex)) {
        std::string colLetter = match[1].str();
        int rowNum = std::stoi(match[2].str());
        
        // Convert letter to column index (A=0, B=1, etc.)
        int colNum = 0;
        for (char c : colLetter) {
            colNum = colNum * 26 + (std::toupper(c) - 'A');
        }
        
        return {rowNum - 1, colNum}; // 0-indexed
    }
    
    // If not letter-number format, try sequential number
    try {
        int seqNum = std::stoi(name);
        // For sequential, we'll figure out grid later
        return {seqNum - 1, 0};
    } catch (...) {
        return {-1, -1};
    }
}

void OQMParser::parseDataLine(const std::string& line) {
    if (line.empty()) return;
    
    std::istringstream iss(line);
    std::vector<std::string> tokens;
    std::string token;
    
    // Split by whitespace or tabs
    while (iss >> token) {
        tokens.push_back(token);
    }
    
    if (tokens.empty() || sampleNameIndex_ < 0 || 
        sampleNameIndex_ >= static_cast<int>(tokens.size())) {
        return;
    }
    
    std::string patchName = tokens[sampleNameIndex_];
    std::vector<double> spectralValues;
    
    for (int idx : spectralIndices_) {
        if (idx < static_cast<int>(tokens.size())) {
            try {
                double val = std::stod(tokens[idx]);
                // OQM uses percentage (0-100), RefData expects fraction (0-1)
                // Check if values seem to be percentage
                spectralValues.push_back(val / 100.0);
            } catch (...) {
                spectralValues.push_back(0.0);
            }
        }
    }
    
    if (!spectralValues.empty()) {
        patchData_[patchName] = spectralValues;
        
        // Update grid dimensions
        auto [row, col] = patchNameToRowCol(patchName);
        if (row >= 0 && col >= 0) {
            rowCount_ = std::max(rowCount_, row + 1);
            colCount_ = std::max(colCount_, col + 1);
        }
    }
}

bool OQMParser::parse(const std::string& content) {
    error_.clear();
    patchData_.clear();
    rowCount_ = 0;
    colCount_ = 0;
    metadata_ = OQMMetadata{};
    
    if (!isValidFormat(content)) {
        error_ = "Invalid OQM/CGATS format: missing header identifier";
        return false;
    }
    
    std::istringstream stream(content);
    std::string line;
    bool inDataFormat = false;
    bool inData = false;
    std::vector<std::string> formatFields;
    
    while (std::getline(stream, line)) {
        // Trim carriage return if present
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') continue;
        
        // Check for section markers
        if (line.find("BEGIN_DATA_FORMAT") != std::string::npos) {
            inDataFormat = true;
            continue;
        }
        if (line.find("END_DATA_FORMAT") != std::string::npos) {
            inDataFormat = false;
            parseDataFormat(formatFields);
            continue;
        }
        if (line.find("BEGIN_DATA") != std::string::npos) {
            inData = true;
            continue;
        }
        if (line.find("END_DATA") != std::string::npos) {
            inData = false;
            continue;
        }
        
        // Process line based on current section
        if (inDataFormat) {
            // Data format lines contain field names
            std::istringstream iss(line);
            std::string field;
            while (iss >> field) {
                formatFields.push_back(field);
            }
        } else if (inData) {
            parseDataLine(line);
        } else {
            // Parse keywords in header section
            parseKeyword(line);
        }
    }
    
    if (patchData_.empty()) {
        error_ = "No patch data found in file";
        return false;
    }
    
    std::cout << "[OQM] Parsed " << patchData_.size() << " patches, "
              << "grid " << rowCount_ << "x" << colCount_ << std::endl;
    
    return true;
}

std::string OQMParser::toRefDataCSV() const {
    if (patchData_.empty() || wavelengths_.empty()) {
        return "";
    }
    
    std::ostringstream csv;
    
    // Build header: Wavelength,A:1,A:2,...,B:1,B:2,...
    csv << "Wavelength";
    
    // Generate column names in order
    std::vector<std::string> orderedPatches;
    for (int col = 0; col < colCount_; col++) {
        for (int row = 0; row < rowCount_; row++) {
            char colLetter = 'A' + col;
            std::string patchName = std::string(1, colLetter) + ":" + std::to_string(row + 1);
            csv << "," << patchName;
            
            // Find matching patch in data (try different formats)
            std::string key1 = std::string(1, colLetter) + std::to_string(row + 1);
            std::string key2 = std::string(1, colLetter) + "-" + std::to_string(row + 1);
            
            if (patchData_.count(key1)) {
                orderedPatches.push_back(key1);
            } else if (patchData_.count(key2)) {
                orderedPatches.push_back(key2);
            } else {
                orderedPatches.push_back(""); // Missing patch
            }
        }
    }
    csv << "\n";
    
    // Output data rows (one per wavelength)
    for (size_t w = 0; w < wavelengths_.size(); w++) {
        csv << wavelengths_[w];
        
        for (const auto& patchKey : orderedPatches) {
            csv << ",";
            if (!patchKey.empty() && patchData_.count(patchKey)) {
                const auto& values = patchData_.at(patchKey);
                if (w < values.size()) {
                    csv << values[w];
                } else {
                    csv << "0";
                }
            } else {
                csv << "0";
            }
        }
        csv << "\n";
    }
    
    return csv.str();
}

bool OQMParser::writeToCSV(const std::string& outputPath) const {
    std::string csvContent = toRefDataCSV();
    if (csvContent.empty()) {
        return false;
    }
    
    std::ofstream file(outputPath);
    if (!file.is_open()) {
        return false;
    }
    
    file << csvContent;
    file.close();
    
    std::cout << "[OQM] Wrote CSV to: " << outputPath << std::endl;
    return true;
}

} // namespace btrgb

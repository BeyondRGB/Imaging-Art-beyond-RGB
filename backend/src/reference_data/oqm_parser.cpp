#include <reference_data/oqm_parser.hpp>

#include <sstream>
#include <fstream>
#include <algorithm>
#include <regex>
#include <iostream>
#include <cctype>
#include <limits>

namespace btrgb {

namespace { // File-local OQM parsing helpers.

// Converts a zero-based column index into spreadsheet labels such as A, Z, AA.
// The decrement after division is what makes Z roll over to AA correctly, and
// the do/while ensures index 0 still produces "A".
std::string column_label_from_index(int index) {
    std::string label;
    int value = index;
    do {
        label.insert(label.begin(), static_cast<char>('A' + (value % 26)));
        value = (value / 26) - 1;
    } while (value >= 0);
    return label;
}

bool is_letter_number_patch_name(const std::string& value) {
    // Matches patch labels like A1, A-1, or AA12.
    static const std::regex patchRegex(R"(^[A-Za-z]+-?\d+$)");
    return std::regex_match(value, patchRegex);
}

double normalize_reflectance_value(double value) {
    if (value > 1.5) {
        return value / 100.0;
    }
    return value;
}

int map_observer_angle_to_standard_observer(int observerAngle) {
    if (observerAngle == 10) {
        return 1964;
    }
    if (observerAngle == 2) {
        return 1931;
    }
    return 1931;
}

void parse_measurement_source(const std::string& value, OQMMetadata& metadata) {
    // Pull embedded tokens like "Illumination=D50 ObserverAngle=2" from the
    // MEASUREMENT_SOURCE field when dedicated keywords are not present.
    static const std::regex illumRegex(R"((?:^|\s)Illumination=([^\s]+))");
    static const std::regex observerRegex(R"((?:^|\s)ObserverAngle=([0-9]+))");
    std::smatch match;

    if (metadata.illuminant.empty() && std::regex_search(value, match, illumRegex)) {
        metadata.illuminant = match[1].str();
    }

    if (metadata.observerAngle == 0 &&
        std::regex_search(value, match, observerRegex)) {
        metadata.observerAngle = std::stoi(match[1].str());
        metadata.standardObserver =
            map_observer_angle_to_standard_observer(metadata.observerAngle);
    }
}

std::string build_csv_patch_name(const std::string& originalName,
                                 int rowIndex,
                                 int colIndex) {
    // Matches labels like A1 or A-1 so the CSV header can normalize them to A:1.
    static const std::regex patchRegex(R"(^([A-Za-z]+)-?(\d+)$)");
    std::smatch match;
    if (std::regex_match(originalName, match, patchRegex)) {
        std::string columnLabel = match[1].str();
        std::transform(columnLabel.begin(), columnLabel.end(), columnLabel.begin(),
                       [](unsigned char c) {
                           return static_cast<char>(std::toupper(c));
                       });
        return columnLabel + ":" + match[2].str();
    }

    return column_label_from_index(colIndex) + ":" + std::to_string(rowIndex + 1);
}

std::string get_token_or_empty(const std::vector<std::string>& tokens, int index) {
    if (index >= 0 && index < static_cast<int>(tokens.size())) {
        return tokens[index];
    }

    return "";
}

bool try_parse_int(const std::string& value, int& parsedValue) {
    try {
        parsedValue = std::stoi(value);
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

bool try_parse_double(const std::string& value, double& parsedValue) {
    try {
        parsedValue = std::stod(value);
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

bool try_parse_luminance(const std::vector<std::string>& tokens,
                         int index,
                         double& luminance) {
    if (index < 0 || index >= static_cast<int>(tokens.size())) {
        return false;
    }

    return try_parse_double(tokens[index], luminance);
}

} // namespace

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
    
    int parsedInt = 0;
    double parsedDouble = 0.0;

    if (keyword == "DESCRIPTOR") {
        metadata_.targetName = value;
    } else if (keyword == "SERIAL") {
        metadata_.serialNumber = value;
    } else if (keyword == "CREATED") {
        metadata_.measurementDate = value;
    } else if (keyword == "CALIBRATION_DATE") {
        metadata_.calibrationDate = value;
    } else if (keyword == "ORIGINATOR") {
        metadata_.originator = value;
    } else if (keyword == "TARGET_INSTRUMENT") {
        metadata_.instrument = value;
    } else if (keyword == "MEASUREMENT_SOURCE") {
        parse_measurement_source(value, metadata_);
    } else if (keyword == "ILLUMINANT") {
        metadata_.illuminant = value;
    } else if (keyword == "OBSERVER") {
        if (try_parse_int(value, parsedInt)) {
            metadata_.observerAngle = parsedInt;
            metadata_.standardObserver =
                mapObserverAngleToStandardObserver(metadata_.observerAngle);
        }
    } else if (keyword == "NUMBER_OF_SETS") {
        if (try_parse_int(value, parsedInt)) {
            metadata_.patchCount = parsedInt;
        }
    } else if (keyword == "SPECTRAL_BANDS") {
        if (try_parse_int(value, parsedInt)) {
            metadata_.spectralBands = parsedInt;
        }
    } else if (keyword == "SPECTRAL_START_NM") {
        if (try_parse_double(value, parsedDouble)) {
            metadata_.spectralStartNm = static_cast<int>(parsedDouble);
        }
    } else if (keyword == "SPECTRAL_END_NM") {
        if (try_parse_double(value, parsedDouble)) {
            metadata_.spectralEndNm = static_cast<int>(parsedDouble);
        }
    }
}

void OQMParser::parseDataFormat(const std::vector<std::string>& formatLines) {
    spectralIndices_.clear();
    wavelengths_.clear();
    sampleNameIndex_ = -1;
    sampleIdIndex_ = -1;
    labLIndex_ = -1;
    xyzYIndex_ = -1;
    
    int index = 0;
    for (const std::string& field : formatLines) {
        if (field == "SAMPLE_NAME") {
            sampleNameIndex_ = index;
        } else if (field == "SAMPLE_ID") {
            sampleIdIndex_ = index;
        } else if (field == "LAB_L") {
            labLIndex_ = index;
        } else if (field == "XYZ_Y") {
            xyzYIndex_ = index;
        } else if (field.find("SPEC_") == 0 || field.find("nm") == 0 ||
                   field.find("SPECTRAL_NM") == 0) {
            std::regex numRegex(R"(\d+)");
            std::smatch match;
            int wavelength = 0;
            if (std::regex_search(field, match, numRegex) &&
                try_parse_int(match[0].str(), wavelength)) {
                spectralIndices_.push_back(index);
                wavelengths_.push_back(wavelength);
            }
        }

        index++;
    }
    
    std::cout << "[OQM] Found " << spectralIndices_.size() << " spectral columns" << std::endl;
}

std::pair<int, int> OQMParser::patchNameToRowCol(const std::string& name) const {
    // Matches patch labels like A1, A-1, or AA12 where letters encode the
    // column and digits encode the row.
    std::regex patchRegex(R"(([A-Za-z]+)-?(\d+))");
    std::smatch match;
    
    if (std::regex_match(name, match, patchRegex)) {
        std::string colLetter = match[1].str();
        int rowNum = 0;
        if (!try_parse_int(match[2].str(), rowNum)) {
            return {-1, -1};
        }
        
        // Convert letter to column index (A=0, B=1, ..., AA=26).
        int colNum = 0;
        for (char c : colLetter) {
            colNum = colNum * 26 + (std::toupper(c) - 'A' + 1);
        }
        
        return {rowNum, colNum - 1};
    }
    
    // If not letter-number format, try sequential number
    int seqNum = 0;
    if (try_parse_int(name, seqNum)) {
        return {seqNum - 1, 0};
    }
    return {-1, -1};
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
    
    std::string sampleName = get_token_or_empty(tokens, sampleNameIndex_);
    std::string sampleId = get_token_or_empty(tokens, sampleIdIndex_);
    std::string patchName = "";

    if (is_letter_number_patch_name(sampleName)) {
        patchName = sampleName;
    } else if (is_letter_number_patch_name(sampleId)) {
        patchName = sampleId;
    } else if (!sampleName.empty()) {
        patchName = sampleName;
    } else {
        patchName = sampleId;
    }

    if (tokens.empty() || patchName.empty()) {
        return;
    }

    std::vector<double> spectralValues;
    
    for (int idx : spectralIndices_) {
        if (idx < static_cast<int>(tokens.size())) {
            double val = 0.0;
            if (try_parse_double(tokens[idx], val)) {
                spectralValues.push_back(normalize_reflectance_value(val));
            } else {
                spectralValues.push_back(0.0);
            }
        }
    }
    
    if (!spectralValues.empty()) {
        patchData_[patchName] = spectralValues;

        double luminance = 0.0;
        if (try_parse_luminance(tokens, xyzYIndex_, luminance) ||
            try_parse_luminance(tokens, labLIndex_, luminance)) {
            patchLuminance_[patchName] = luminance;
        }
        
        // Update grid dimensions
        const std::pair<int, int> rawPosition = patchNameToRowCol(patchName);
        const int rawRow = rawPosition.first;
        const int col = rawPosition.second;
        if (rawRow >= 0 && col >= 0) {
            patchGridRawPosition_[patchName] = {rawRow, col};
        }
    }
}

void OQMParser::finalizeLayout() {
    patchGridPosition_.clear();
    patchNameByPosition_.clear();
    rowCount_ = 0;
    colCount_ = 0;
    minRowLabel_ = std::numeric_limits<int>::max();

    for (const auto& [patchName, position] : patchGridRawPosition_) {
        minRowLabel_ = std::min(minRowLabel_, position.first);
    }

    if (minRowLabel_ == std::numeric_limits<int>::max()) {
        minRowLabel_ = 0;
    }

    if (patchGridRawPosition_.empty()) {
        int sequentialRow = 0;
        for (const auto& [patchName, _] : patchData_) {
            patchGridPosition_[patchName] = {sequentialRow, 0};
            patchNameByPosition_[{sequentialRow, 0}] = patchName;
            rowCount_ = sequentialRow + 1;
            colCount_ = 1;
            sequentialRow++;
        }
    }

    for (const auto& [patchName, rawPosition] : patchGridRawPosition_) {
        int normalizedRow = rawPosition.first - minRowLabel_;
        int normalizedCol = rawPosition.second;

        if (normalizedRow < 0 || normalizedCol < 0) {
            continue;
        }

        patchGridPosition_[patchName] = {normalizedRow, normalizedCol};
        patchNameByPosition_[{normalizedRow, normalizedCol}] = patchName;
        rowCount_ = std::max(rowCount_, normalizedRow + 1);
        colCount_ = std::max(colCount_, normalizedCol + 1);
    }

    if (metadata_.patchCount == 0) {
        metadata_.patchCount = static_cast<int>(patchData_.size());
    }
    if (metadata_.spectralBands == 0) {
        metadata_.spectralBands = static_cast<int>(wavelengths_.size());
    }
    metadata_.rowCount = rowCount_;
    metadata_.colCount = colCount_;

    if (metadata_.standardObserver == 0) {
        metadata_.standardObserver =
            mapObserverAngleToStandardObserver(metadata_.observerAngle);
    }

    double brightestValue = -std::numeric_limits<double>::infinity();
    std::string brightestPatch;
    for (const auto& [patchName, luminance] : patchLuminance_) {
        auto position = patchGridPosition_.find(patchName);
        if (position == patchGridPosition_.end()) {
            continue;
        }
        if (luminance > brightestValue) {
            brightestValue = luminance;
            brightestPatch = patchName;
        }
    }

    if (!brightestPatch.empty()) {
        const auto [row, col] = patchGridPosition_.at(brightestPatch);
        metadata_.suggestedWhitePatchRow = row + 1;
        metadata_.suggestedWhitePatchCol = col + 1;
        metadata_.suggestedWhitePatchName = brightestPatch;
    }
}

bool OQMParser::parse(const std::string& content) {
    error_.clear();
    patchData_.clear();
    patchGridPosition_.clear();
    patchGridRawPosition_.clear();
    patchNameByPosition_.clear();
    patchLuminance_.clear();
    rowCount_ = 0;
    colCount_ = 0;
    minRowLabel_ = 1;
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

    finalizeLayout();
    
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
            const auto position = std::make_pair(row, col);
            const auto patchNameIt = patchNameByPosition_.find(position);
            const std::string patchName =
                patchNameIt != patchNameByPosition_.end() ? patchNameIt->second : "";
            csv << "," << build_csv_patch_name(patchName, row, col);
            orderedPatches.push_back(patchName);
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

int OQMParser::mapObserverAngleToStandardObserver(int observerAngle) {
    return map_observer_angle_to_standard_observer(observerAngle);
}

} // namespace btrgb

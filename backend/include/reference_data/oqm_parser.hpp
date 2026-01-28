#ifndef OQM_PARSER_HPP
#define OQM_PARSER_HPP

#include <string>
#include <vector>
#include <map>
#include <optional>
#include <memory>

namespace btrgb {

/**
 * @brief Metadata extracted from an OQM/CGATS file
 */
struct OQMMetadata {
    std::string targetName;      // DESCRIPTOR field
    std::string serialNumber;    // SERIAL field
    std::string measurementDate; // CREATED field
    std::string originator;      // ORIGINATOR field (optional)
    std::string instrument;      // TARGET_INSTRUMENT field (optional)
    int patchCount;              // NUMBER_OF_SETS
    int spectralBands;           // SPECTRAL_BANDS
    int spectralStartNm;         // SPECTRAL_START_NM
    int spectralEndNm;           // SPECTRAL_END_NM
};

/**
 * @brief Parser for OQM/CGATS measurement files
 * 
 * The OpenQualia Measurement (OQM) format is a specific flavor of the CGATS standard
 * used for color target spectral measurements. This parser extracts the spectral data
 * and converts it to a format compatible with the existing RefData CSV loader.
 * 
 * OQM Format Overview:
 * - File starts with "OQM" or "CGATS.17"
 * - Keywords: DESCRIPTOR, SERIAL, CREATED, SPECTRAL_BANDS, etc.
 * - Data section between BEGIN_DATA and END_DATA
 * - Spectral values named SPEC_380, SPEC_390, ..., SPEC_730
 */
class OQMParser {
public:
    OQMParser() = default;
    ~OQMParser() = default;

    /**
     * @brief Parse OQM/CGATS content from a string
     * 
     * @param content The raw OQM/CGATS file content
     * @return true if parsing succeeded, false otherwise
     */
    bool parse(const std::string& content);

    /**
     * @brief Get the metadata extracted from the file
     */
    const OQMMetadata& getMetadata() const { return metadata_; }

    /**
     * @brief Check if the content appears to be valid OQM/CGATS format
     */
    static bool isValidFormat(const std::string& content);

    /**
     * @brief Convert parsed data to CSV format compatible with RefData
     * 
     * The output CSV format matches what RefData expects:
     * - First row: Wavelength,A:1,A:2,...,B:1,B:2,...
     * - Subsequent rows: wavelength_value,patch_values...
     * 
     * @return CSV content string
     */
    std::string toRefDataCSV() const;

    /**
     * @brief Write parsed data to a temporary CSV file
     * 
     * @param outputPath Path to write the CSV file
     * @return true if write succeeded
     */
    bool writeToCSV(const std::string& outputPath) const;

    /**
     * @brief Get the last error message
     */
    const std::string& getError() const { return error_; }

private:
    /**
     * @brief Parse a keyword line (e.g., "DESCRIPTOR \"DT NGT2\"")
     */
    void parseKeyword(const std::string& line);

    /**
     * @brief Parse the data format section to determine column order
     */
    void parseDataFormat(const std::vector<std::string>& formatLines);

    /**
     * @brief Parse a data line into patch values
     */
    void parseDataLine(const std::string& line);

    /**
     * @brief Convert patch name (e.g., "A1" or "A-1") to row/col
     */
    std::pair<int, int> patchNameToRowCol(const std::string& name) const;

    /**
     * @brief Strip quotes from a string value
     */
    static std::string stripQuotes(const std::string& value);

    OQMMetadata metadata_;
    std::string error_;

    // Column indices in data section
    int sampleNameIndex_ = -1;
    std::vector<int> spectralIndices_; // Index of SPEC_XXX columns
    std::vector<int> wavelengths_;     // Corresponding wavelengths

    // Parsed patch data: patch_name -> [spectral_values]
    std::map<std::string, std::vector<double>> patchData_;

    // Grid dimensions (determined from patch names)
    int rowCount_ = 0;
    int colCount_ = 0;
};

/**
 * @brief Exception for OQM parsing errors
 */
class OQMParseError : public std::exception {
public:
    OQMParseError(const std::string& msg) : message_(msg) {}
    const char* what() const noexcept override { return message_.c_str(); }
private:
    std::string message_;
};

} // namespace btrgb

#endif // OQM_PARSER_HPP

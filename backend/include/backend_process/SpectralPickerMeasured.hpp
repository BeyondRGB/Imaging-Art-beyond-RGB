#ifndef BTRGB_SPECTRALPICKERMEASURED_HPP
#define BTRGB_SPECTRALPICKERMEASURED_HPP

#include <fstream>
#include <jsoncons/json.hpp>

#include <image_util/Image.hpp>
#include <server/communication_obj.hpp>
#include <backend_process/backend_process.hpp>

class SpectralPickerMeasured : public BackendProcess {

public:
    SpectralPickerMeasured(std::string name) : BackendProcess(name) {};
    ~SpectralPickerMeasured();
    void run() override;

private:
    static std::vector<std::string> split(const std::string& s, char delimiter);

    // Method to parse the CSV file and fill the provided vectors with measured and reference values for a given coordinate
    static void parseCSV(const std::string& filename, const std::string& coordinate, std::vector<float>& measured, std::vector<float>& reference);

};


#endif

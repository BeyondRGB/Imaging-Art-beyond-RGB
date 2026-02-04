#include "backend_process/SpectralPickerMeasured.hpp"

SpectralPickerMeasured::~SpectralPickerMeasured() {}

std::vector<std::string> SpectralPickerMeasured::split(const std::string &s,
                                                       char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void SpectralPickerMeasured::parseCSV(const std::string &filename,
                                      const std::string &coordinate,
                                      std::vector<float> &measured,
                                      std::vector<float> &reference) {
    std::ifstream file(filename);
    std::string line;
    bool isMeasuring = false; // False for R_camera, True for R_reference
    size_t columnIndex = 0;

    if (!file.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        return;
    }

    while (getline(file, line)) {
        // Check for sections and skip headers
        if (line.find("R_camera") != std::string::npos) {
            isMeasuring = false;
            getline(file, line); // Skip the header row
            auto headers = split(line, ',');
            // Find the index our coorindate aligns with
            for (size_t i = 1; i < headers.size(); ++i) {
                if (headers[i] == coordinate) {
                    columnIndex = i;
                    break;
                }
            }
            continue;
        } else if (line.find("R_reference") != std::string::npos) {
            isMeasuring = true;
            continue;
        } else if (line.find("RMSE") != std::string::npos) {
            break; // Stop parsing if we reach RMSE line
        }

        auto values = split(line, ',');
        if (!isMeasuring) {
            if (columnIndex > 0 && columnIndex < values.size()) {
                try {
                    std::cout << values[columnIndex] << std::endl;
                    float value = std::stof(values[columnIndex]);
                    measured.push_back(value);
                } catch (const std::invalid_argument &e) {
                    std::cerr
                        << "Invalid argument for stof: " << values[columnIndex]
                        << std::endl;
                } catch (const std::out_of_range &e) {
                    std::cerr
                        << "Out of range for stof: " << values[columnIndex]
                        << std::endl;
                }
            }
        } else {
            if (columnIndex > 0 && columnIndex < values.size()) {
                try {
                    float value = std::stof(values[columnIndex]);
                    reference.push_back(value);
                } catch (const std::invalid_argument &e) {
                    std::cerr
                        << "Invalid argument for stof: " << values[columnIndex]
                        << std::endl;
                } catch (const std::out_of_range &e) {
                    std::cerr
                        << "Out of range for stof: " << values[columnIndex]
                        << std::endl;
                }
            }
        }
    }

    file.close();
}

void SpectralPickerMeasured::run() {
    try {
        std::string prj_filename = this->process_data_m->get_string("name");
        std::string x_rel =
            this->process_data_m->get_obj("coordinates").get_string("x");
        double y_rel =
            this->process_data_m->get_obj("coordinates").get_number("y");
        std::string coordinate =
            x_rel + ":" + std::to_string(static_cast<int>(y_rel) - 1);

        std::cout << "Project filename: " << prj_filename << std::endl;
        std::cout << "Coordinates: " << coordinate << std::endl;

        std::ifstream prj_file(prj_filename);
        if (!prj_file.is_open()) {
            throw std::runtime_error("Failed to open project file: " + prj_filename);
        }

        auto json = jsoncons::json::parse(prj_file);
        std::string local_file =
            json["OutPutFiles"]["R_camera"].as<std::string>();
        std::string filename =
            prj_filename.substr(0, prj_filename.find_last_of("/\\") + 1) +
            local_file;

        std::vector<float> measured;
        std::vector<float> reference;
        parseCSV(filename, coordinate, measured, reference);

        // Vectors are of equal size and contain data for processing
        cv::Mat camera_mat(measured.size(), 1, CV_32FC1, measured.data());
        cv::Mat reference_mat(reference.size(), 1, CV_32FC1, reference.data());

        this->coms_obj_m->send_spectrum_measured(camera_mat.ptr<float>(),
                                                 reference_mat.ptr<float>(),
                                                 camera_mat.rows);
        std::cout << "Finished processing." << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        this->coms_obj_m->send_error("Invalid SpectralPicker JSON. Restart BeyondRGB to View the image.",
            this->get_process_name(),
            cpptrace::generate_trace());
    }
}

#include <reference_data/illuminants.hpp>
#include <reference_data/ref_data_defines.hpp>

Illuminants::Illuminants(IlluminantType type) {
    this->type = type;
    this->init();
}

Illuminants::~Illuminants() {
    if (nullptr != this->illuminants)
        delete this->illuminants;
}

void Illuminants::init() {
    std::string fpath = ILLUMINANTS_FILE_PATH;
    if (!this->open_file(fpath))
        throw std::runtime_error("[illuminants.cpp] Failed to open file: " +
                                 fpath);
    this->illuminants = new RefDataArray(ILLUMINATNTS_SIZE);
    this->get_next_line(); // ignore header
    for (int i = 0; i < ILLUMINATNTS_SIZE; i++) {
        std::string line = this->get_next_line();
        // Ignor wavelength
        this->get_next<double>(line);
        double value_A = this->get_next<double>(line);
        double value_D50 = this->get_next<double>(line);
        double value_D65 = this->get_next<double>(line);
        // Only store what we need based on set type
        switch (this->type) {
        case IlluminantType::A:
            this->illuminants->append(value_A);
            break;
        case IlluminantType::D65:
            this->illuminants->append(value_D65);
            break;
        default:
            this->illuminants->append(value_D50);
        }
    }
    this->close_file();
}

double Illuminants::value_by_index(int index) {
    return this->illuminants->get_by_index(index);
}

double Illuminants::value_by_wavelen(int wavelen) {
    return this->value_by_index(WAVELEN_TO_INDEX(wavelen));
}

#include <reference_data/ref_data_defines.hpp>
#include <reference_data/standard_observer.hpp>
#include <utils/resource_paths.hpp>

StandardObserver::StandardObserver(ObserverType type) {
    if (type == ObserverType::SO_1931)
        this->init(
            btrgb::paths::build_ref_data_file_path(STANDARD_OBSERVER_1931_FILE_NAME));
    if (type == ObserverType::SO_1964)
        this->init(
            btrgb::paths::build_ref_data_file_path(STANDARD_OBSERVER_1964_FILE_NAME));
}

StandardObserver::~StandardObserver() {
    if (nullptr != this->x) {
        delete this->x;
    }
    if (nullptr != this->y)
        delete this->y;
    if (nullptr != this->z)
        delete this->z;
}

void StandardObserver::init(std::string file) {
    if (!this->open_file(file))
        throw std::runtime_error(
            "[standarad_observer.cpp] Failed to open file: " + file);
    this->x = new RefDataArray(STANDARD_OBSERVER_SIZE);
    this->y = new RefDataArray(STANDARD_OBSERVER_SIZE);
    this->z = new RefDataArray(STANDARD_OBSERVER_SIZE);
    // Ignore header
    this->get_next_line();
    for (int i = 0; i < STANDARD_OBSERVER_SIZE; i++) {
        std::string line = this->get_next_line();
        // Ignor wavelength
        this->get_next<double>(line);
        this->x->append(this->get_next<double>(line));
        this->y->append(this->get_next<double>(line));
        this->z->append(this->get_next<double>(line));
    }
    this->close_file();
}

double StandardObserver::value_by_index(ValueType type, int index) {
    switch (type) {
    case X:
        return this->x->get_by_index(index);
    case Y:
        return this->y->get_by_index(index);
    case Z:
        return this->z->get_by_index(index);
    default:
        return this->y->get_by_index(index);
    }
}

double StandardObserver::value_by_wavelen(ValueType type, int wavelen) {
    return this->value_by_index(type, WAVELEN_TO_INDEX(wavelen));
}

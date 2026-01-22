#ifndef REF_DATA_ARRAY_H
#define REF_DATA_ARRAY_H

#include "ref_data_defines.hpp"
#include <stdexcept>

/**
 * Class to hold an array of Reference data.
 * This is just a wrapper class to hold and access the data
 */
class RefDataArray {

  public:
    RefDataArray(int size);
    ~RefDataArray();

    /**
     * Get data value by index
     * This will throw an std::out_of_range exeption if
     * Index is out of bounds
     * @param index: the index to get value from
     * @return: value as double
     */
    double get_by_index(int index);

    /**
     * Get data value by wavelength
     * This will throw an std::out_of_range exeption if
     * the wavelen translates to an Index that is out of bounds
     * @param wavelength: the wavelength to get value from
     * @return: value as double
     */
    double get_by_wavelen(int wavelen);

    /**
     * Append value to data array
     * This will throw an std::exception if array is full
     * @param value: numeric value to append
     */
    void append(double value);

  private:
    double *data = nullptr;
    int size = 0;
    int curr_index = 0;
    bool is_valid_index(int index);
};
#endif // !REF_DATA_ARRAY_H

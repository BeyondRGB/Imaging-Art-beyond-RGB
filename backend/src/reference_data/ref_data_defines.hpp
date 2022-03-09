#ifndef REF_DATA_DEFINE
#define REF_DATA_DEFINE
#include "server/globals_siglton.hpp"

#define MAX_WAVE_LEN 730 // max wave length value
#define MIN_WAVE_LEN 380 // min wave length value
#define SAMPLING_INCREMENT 10 // wave length increments

#define WAVELEN_TO_INDEX(wavlen) (wavlen - MIN_WAVE_LEN) / SAMPLING_INCREMENT
#define INDEX_TO_WAVELEN(index) (index * SAMPLING_INCREMENT) + MIN_WAVE_LEN 

#define REFLECTANCE_SIZE 1 + (MAX_WAVE_LEN - MIN_WAVE_LEN) / SAMPLING_INCREMENT
#define ILLUMINATNTS_SIZE REFLECTANCE_SIZE
#define STANDARD_OBSERVER_SIZE REFLECTANCE_SIZE

#define BASE_PATH GlobalsSinglton::get_instance()->app_root()+ // relative to .exe
#define REF_DATA_PATH BASE_PATH "/res/ref_data/"
#define ILLUMINANTS_FILE_PATH REF_DATA_PATH "Illuminants.csv"
#define STANDARD_OBSERVER_1931_PATH REF_DATA_PATH "Standard_Observer_1931.csv"
#define STANDARD_OBSERVER_1964_PATH REF_DATA_PATH "Standard_Observer_1964.csv"


#endif // !REF_DATA_DEFINE
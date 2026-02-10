#ifndef REF_DATA_DEFINE
#define REF_DATA_DEFINE

#define MAX_WAVE_LEN 730      // max wave length value
#define MIN_WAVE_LEN 380      // min wave length value
#define SAMPLING_INCREMENT 10 // wave length increments

#define WAVELEN_TO_INDEX(wavlen) (wavlen - MIN_WAVE_LEN) / SAMPLING_INCREMENT
#define INDEX_TO_WAVELEN(index) (index * SAMPLING_INCREMENT) + MIN_WAVE_LEN

#define REFLECTANCE_SIZE 1 + (MAX_WAVE_LEN - MIN_WAVE_LEN) / SAMPLING_INCREMENT
#define ILLUMINATNTS_SIZE REFLECTANCE_SIZE
#define STANDARD_OBSERVER_SIZE REFLECTANCE_SIZE

#define ILLUMINANTS_FILE_NAME "Illuminants.csv"
#define STANDARD_OBSERVER_1931_FILE_NAME "Standard_Observer_1931.csv"
#define STANDARD_OBSERVER_1964_FILE_NAME "Standard_Observer_1964.csv"

#endif // !REF_DATA_DEFINE
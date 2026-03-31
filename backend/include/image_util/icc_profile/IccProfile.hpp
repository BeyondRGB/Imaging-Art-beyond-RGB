#ifndef CREATEICCPROFILE_H
#define CREATEICCPROFILE_H

#include <IccProfLib/IccProfile.h>

namespace btrgb::icc {

#define VALIDATE_COLOR_SPACE(space) if (space > ProfileColorSpace::cs_Linear_Normalized_XYZ || space < 0) { return nullptr; }

enum ProfileColorSpace {
    cs_Adobe_RGB_1998 = 0,
    cs_ProPhoto,
    cs_sRGB,
    cs_Wide_Gamut_RGB,
    cs_Linear_Normalized_XYZ
};

class IccProfile {
public:
    // max_size is the maximum size in bytes that the created profile can be,
    // and should be large enough to hold the profile data created by
    // createHybridProfile for the given input parameters
    IccProfile(size_t max_size = 1000000);

    // destructor will free the memory allocated for the profile, so the caller
    // should not free the memory returned by getProfileMem
    ~IccProfile();

    // dataMatrix is expected to be in row major order with num_in columns and
    // num_out rows, and should not include the base channels if
    // ignore_base_channels is true
    bool createHybridProfile(ProfileColorSpace space,
                             const float *dataMatrix,
                             int numInputChannels,
                             int numOutputChannels,
                             bool ignore_base_channels,
                             const float *inv_matrix = nullptr);

    void write(const std::string &filename) const;

    /**
     * @brief Retrieves raw icc profile memory so it can be saved into TIFF images.
     * You are required to run @fn createHybridProfile before this will return anything.
     *
     * @return The raw icc profile memory.
     */
    unsigned char *getProfileMem();

    /**
     * @brief Retrieves size of the raw icc profile memory.
     * You are required to run @fn createHybridProfile before this will return anything.
     *
     * @return The size of the profile's memory.
     */
    [[nodiscard]] size_t getProfileSize() const;

    /**
     * Get access to the created profile object, which will be owned by this
     * class and should not be deleted by the caller. This is provided for
     * advanced use cases where the caller may want to access the profile object.
     *
     * @return The internal CIccProfile object, owned by this class.
     */
    [[nodiscard]] CIccProfile *getHybridProfile() const { return hybrid_icc; }

protected:
    /**
     * Helper functions to create the base RGB profile and the MPE profile to be embedded in the hybrid profile
     *
     * @param space The color space that the iccProfile should use.
     * @return An RGB CIccProfile for the specified color space.
     */
    static CIccProfile *createRgbProfile(ProfileColorSpace space);

    // if inverseMatrix is provided then it must be the inverse of dataMatrix and
    // include entries for all channels (including ignored channels if dataMatrix
    // doesn't include them. If inverseMatrix is not provided then no BToD3 tag
    // will be created
    /**
     *
     * @param baseSpace
     * @param dataMatrix
     * @param numInputChannels
     * @param numOutputChannels
     * @param ignore_base_channels
     * @param inverseMatrix
     * @return
     */
    static CIccProfile *createSpecProfile(ProfileColorSpace baseSpace,
                                          const float *dataMatrix,
                                          int numInputChannels,
                                          int numOutputChannels,
                                          bool ignore_base_channels,
                                          const float *inverseMatrix = nullptr);

    /**
     *
     * @param icc_profile
     * @param filename
     */
    static void write(CIccProfile *icc_profile, const std::string &filename);

    // MARK: Data Members
    size_t max_profile_size;
    CIccProfile *hybrid_icc;
    unsigned char *profile_mem;
    size_t profile_size;
};
}; // namespace btrgb

#endif
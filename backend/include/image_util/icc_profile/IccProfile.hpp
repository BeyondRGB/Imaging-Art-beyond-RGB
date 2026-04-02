#ifndef CREATEICCPROFILE_H
#define CREATEICCPROFILE_H

#include "image_util/ColorProfiles.hpp"

#include <IccProfLib/IccProfile.h>

namespace btrgb::icc {

// An enum that validates if a given ProfileColorSpace enum is valid.
#define VALIDATE_COLOR_SPACE(space) if (space > ColorSpace::none || space < 0) { return nullptr; }

class IccProfile {
public:
    /**
     * Sets up an IccProfile, but does not create one. For creation call @fn createHybridProfile.
     * @param max_size The maximum size in bytes that the created profile can be.
     */
    explicit IccProfile(size_t max_size = 1000000);

    /**
     * Destructor will free the memory allocated for the profile, so the caller should not free the memory returned by get_profile_mem.
     */
    ~IccProfile();

    /**
     * @brief Create a hybrid rgb & spectral Icc Profile.
     *
     * @param space The color space to generate an IccProfile for.
     * @param data_matrix data in row major order with @param num_input_channels columns and @param num_output_channels rows. Should not include base channels if @param ignore_ignore_base_channels is set.
     * @param num_input_channels The number of input channels for the given data.
     * @param num_output_channels The number of output channels for the given data.
     * @param ignore_ignore_base_channels Should we ignore the base channels? If so, dataMatrix should not include base channels.
     * @param inverse_matrix If inverseMatrix is provided, then it must be the inverse of dataMatrix and include entries for all channels (including ignored channels if dataMatrix doesn't include them).
     * @return If creation of the profile was successful, true. The created profile can be accessed using @fn getHybridProfile.
     */
    bool create_hybrid_profile(ColorSpace space,
                             const float *data_matrix,
                             int num_input_channels,
                             int num_output_channels,
                             bool ignore_ignore_base_channels,
                             const float *inverse_matrix = nullptr);

    /**
     * Writes the binary IccProfile to the given file path.
     *
     * @param path the full system path to the file to write to.
     */
    void write(const std::string &path) const;

    /**
     * @brief Retrieves raw icc profile memory so it can be saved into TIFF images.
     * You are required to run @fn createHybridProfile before this will return anything.
     *
     * @return The raw icc profile memory.
     */
    unsigned char *get_profile_mem();

    /**
     * @brief Retrieves size of the raw icc profile memory.
     * You are required to run @fn createHybridProfile before this will return anything.
     *
     * @return The size of the profile's memory.
     */
    [[nodiscard]] size_t get_profile_size() const;

    /**
     * Get access to the created profile object, which will be owned by this
     * class and should not be deleted by the caller. This is provided for
     * advanced use cases where the caller may want to access the profile object.
     *
     * @return The internal CIccProfile object, owned by this class.
     */
    [[nodiscard]] CIccProfile *get_hybrid_profile() const { return hybrid_icc_profile; }

protected:
    /**
     * Helper functions to create the base RGB profile and the MPE profile to be embedded in the hybrid profile
     *
     * @param space The color space that the iccProfile should use.
     * @return An RGB CIccProfile for the specified color space.
     */
    static CIccProfile *create_rgb_profile(ColorSpace space);

    /**
     * @brief Create a spectral icc profile for the given space & data.
     *
     * @note If inverseMatrix is not provided, then no BToD3 tag will be created
     *
     * @param base_space The color space to generate an spectral profile for.
     * @param data_matrix data in row major order with @param num_input_channels columns and @param num_output_channels rows. Should not include base channels if @param ignore_base_channels is set.
     * @param num_input_channels The number of input channels for the given data.
     * @param num_output_channels The number of output channels for the given data.
     * @param ignore_base_channels Should we ignore the base channels? If so, dataMatrix should not include base channels.
     * @param inverse_matrix If inverseMatrix is provided, then it must be the inverse of dataMatrix and include entries for all channels (including ignored channels if dataMatrix doesn't include them).
     * @return A spectral CIccProfile.
     */
    static CIccProfile *create_spec_profile(ColorSpace base_space,
                                          const float *data_matrix,
                                          int num_input_channels,
                                          int num_output_channels,
                                          bool ignore_base_channels,
                                          const float *inverse_matrix = nullptr);


    // MARK: Data Members
    /// The maximum size that the icc profile can be.
    size_t max_profile_size;
    /// A reference to the created hybrid icc profile.
    CIccProfile *hybrid_icc_profile;
    /// The raw memory of the profile, populated after calling get_profile_mem.
    unsigned char *profile_memory;
    /// The size of the profile, populated after calling get_profile_mem
    size_t profile_size;
};
}; // namespace btrgb

#endif
#ifndef CREATEICCPROFILE_H
#define CREATEICCPROFILE_H

#include "IccProfile.h"

//namespace btrgb {

  enum ProfileColorSpace { cs_Adobe_RGB_1998, cs_ProPhoto, cs_sRGB, cs_Wide_Gamut_RGB, cs_Linear_Wide_Gamut_RGB};

  class CreateIccProfile
  {
    //max_size is the maximum size in bytes that the created profile can be, and should be large enough to hold the profile data created by createHybridProfile for the given input parameters
    CreateIccProfile(size_t max_size=1000000);

    //destructor will free the memory allocated for the profile, so the caller should not free the memory returned by getProfileMem
    ~CreateIccProfile();

    //matrix is expected to be in row major order with num_in columns and num_out rows, and should not include the base channels if ignore_base_channels is true
    bool createHybridProfile(ProfileColorSpace space, float* matrix, int num_in, int num_out, bool ignore_base_channels=true);

    //getProfileMem and getProfileSize should only be called after createHybridProfile returns true, and the returned memory should note be freed
    unsigned char *getProfileMem();
    size_t getProfileSize();
    
    // get access to the created profile object, which will be owned by this class and
    // should not be deleted by the caller. This is provided for advanced use cases where the
    // caller may want to access the profile object.
    const CIccProfile* getHybridProfile() const { return hybrid_icc; }

  protected:
    // helper functions to create the base RGB profile and the MPE profile to be embedded in the hybrid profile
    CIccProfile* createRgbProfile(ProfileColorSpace space);
    CIccProfile* createSpecProfile(ProfileColorSpace base_space, float *matrix, int num_in, int num_out, bool ignore_base_channels=true);

    // data members
    size_t max_profile_size;
    CIccProfile* hybrid_icc;
    unsigned char* profile_mem;
    size_t profile_size;
  };

//}; // namespace btrgb

#endif

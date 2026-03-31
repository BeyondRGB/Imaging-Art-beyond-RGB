//
// Created by Taylor Lineman on 3/25/26.
//

#ifndef BEYOND_RGB_BACKEND_WRITEICCPROFILE_H
#define BEYOND_RGB_BACKEND_WRITEICCPROFILE_H

#include <IccProfLib/IccProfile.h>

namespace btrgb::icc {
class WriteIccProfile {
  public:
    static void write(CIccProfile *icc_profile, const std::string &filename);
};
} // namespace btrgb

#endif // BEYOND_RGB_BACKEND_WRITEICCPROFILE_H
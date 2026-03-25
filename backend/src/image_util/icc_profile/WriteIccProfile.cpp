//
// Created by Taylor Lineman on 3/25/26.
//

#include <image_util/icc_profile/WriteIccProfile.h>
#include <iostream>

#include "IccProfLib/IccIO.h"

namespace btrgb {
void WriteIccProfile::write(CIccProfile *icc_profile,
                            const std::string &filename) {
    auto *iccIO = new CIccFileIO();
    iccIO->Open(filename.c_str(), "wb");

    icc_profile->Write(iccIO, icVersionBasedID);

    iccIO->Close();
}
} // namespace btrgb

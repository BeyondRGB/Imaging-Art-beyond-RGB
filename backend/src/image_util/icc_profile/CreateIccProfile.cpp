#include <image_util/icc_profile/CreateIccProfile.hpp>
#include <iostream>
#include <ostream>

#include <IccProfLib/IccIO.h>
#include <IccProfLib/IccMpeBasic.h>
#include <IccProfLib/IccTagBasic.h>
#include <IccProfLib/IccTagEmbedIcc.h>
#include <IccProfLib/IccTagMPE.h>
#include <IccProfLib/IccUtil.h>

namespace btrgb {
CreateIccProfile::CreateIccProfile(size_t max_size) {
    max_profile_size = max_size;
    hybrid_icc = nullptr;
    profile_mem = nullptr;
    profile_size = 0;
}

CreateIccProfile::~CreateIccProfile() {
    delete hybrid_icc;
    delete[] profile_mem;
}

bool CreateIccProfile::createHybridProfile(const ProfileColorSpace space,
                                           const float *dataMatrix,
                                           const int numInputChannels,
                                           const int numOutputChannels,
                                           const bool ignore_base_channels,
                                           const float *inv_matrix) {
    if (hybrid_icc) {
        delete hybrid_icc;
        delete[] profile_mem;
    }

    CIccProfile *rgb_profile = createRgbProfile(space);

    if (!rgb_profile) {
        return false;
    }

    CIccProfile *spec_profile = createSpecProfile(
        space, dataMatrix, numInputChannels, numOutputChannels,
        ignore_base_channels, inv_matrix);
    if (!spec_profile) {
        delete rgb_profile;
    }

    // Allocate IO tag, attach spec_profile to it, and embed it in rgb profile
    auto *pTag = new CIccTagEmbeddedProfile();
    pTag->SetProfile(spec_profile);
    rgb_profile->AttachTag(icSigEmbeddedV5ProfileTag, pTag);

    hybrid_icc = rgb_profile;

    profile_mem = new unsigned char[max_profile_size];

    CIccMemIO io;
    io.Attach(profile_mem, max_profile_size, true);

    if (!hybrid_icc->Write(&io)) {
        delete[] profile_mem;
        delete hybrid_icc;

        return false;
    }
    profile_size = io.GetLength();

    return true;
}

unsigned char *CreateIccProfile::getProfileMem() const { return profile_mem; }

size_t CreateIccProfile::getProfileSize() const { return profile_size; }

struct RGBTags {
    CIccTagXYZ *red;
    CIccTagXYZ *green;
    CIccTagXYZ *blue;
};

void createRGBTags(RGBTags *tags, const ProfileColorSpace colorSpace) {
    switch (colorSpace) {
    case cs_Adobe_RGB_1998:
        // Attach red XYZ primary to profile
        tags->red = new CIccTagXYZ();
        tags->red->GetXYZ(0)->X = icDtoF(0.60974);
        tags->red->GetXYZ(0)->Y = icDtoF(0.31111);
        tags->red->GetXYZ(0)->Z = icDtoF(0.01947);

        // Attach green XYZ primary to profile
        tags->green = new CIccTagXYZ();
        tags->green->GetXYZ(0)->X = icDtoF(0.20528);
        tags->green->GetXYZ(0)->Y = icDtoF(0.62567);
        tags->green->GetXYZ(0)->Z = icDtoF(0.06087);

        // Attach blue XYZ primary to profile
        tags->blue = new CIccTagXYZ();
        tags->blue->GetXYZ(0)->X = icDtoF(0.14919);
        tags->blue->GetXYZ(0)->Y = icDtoF(0.06322);
        tags->blue->GetXYZ(0)->Z = icDtoF(0.74457);
        break;
    case cs_ProPhoto:
        // Attach red XYZ primary to profile
        tags->red = new CIccTagXYZ();
        tags->red->GetXYZ(0)->X = icDtoF(0.398818969727);
        tags->red->GetXYZ(0)->Y = icDtoF(0.144012451172);
        tags->red->GetXYZ(0)->Z = icDtoF(0);

        // Attach green XYZ primary to profile
        tags->green = new CIccTagXYZ();
        tags->green->GetXYZ(0)->X = icDtoF(0.067596435547);
        tags->green->GetXYZ(0)->Y = icDtoF(0.355926513672);
        tags->green->GetXYZ(0)->Z = icDtoF(0);

        // Attach blue XYZ primary to profile
        tags->blue = new CIccTagXYZ();
        tags->blue->GetXYZ(0)->X = icDtoF(0.015670776367);
        tags->blue->GetXYZ(0)->Y = icDtoF(0.000518798828);
        tags->blue->GetXYZ(0)->Z = icDtoF(0.412048339844);
        break;
    case cs_sRGB:
        // Attach red XYZ primary to profile
        tags->red = new CIccTagXYZ();
        tags->red->GetXYZ(0)->X = icDtoF(0.436065673828);
        tags->red->GetXYZ(0)->Y = icDtoF(0.222488403320);
        tags->red->GetXYZ(0)->Z = icDtoF(0.013916015625);

        // Attach green XYZ primary to profile
        tags->green = new CIccTagXYZ();
        tags->green->GetXYZ(0)->X = icDtoF(0.385147094727);
        tags->green->GetXYZ(0)->Y = icDtoF(0.716873168945);
        tags->green->GetXYZ(0)->Z = icDtoF(0.097076416016);

        // Attach blue XYZ primary to profile
        tags->blue = new CIccTagXYZ();
        tags->blue->GetXYZ(0)->X = icDtoF(0.143066406250);
        tags->blue->GetXYZ(0)->Y = icDtoF(0.060607910156);
        tags->blue->GetXYZ(0)->Z = icDtoF(0.714096069336);
        break;
    case cs_Wide_Gamut_RGB:
        // Attach red XYZ primary to profile
        tags->red = new CIccTagXYZ();
        tags->red->GetXYZ(0)->X = icDtoF(0.7579);
        tags->red->GetXYZ(0)->Y = icDtoF(0.4430);
        tags->red->GetXYZ(0)->Z = icDtoF(0);

        // Attach green XYZ primary to profile
        tags->green = new CIccTagXYZ();
        tags->green->GetXYZ(0)->X = icDtoF(0.2230);
        tags->green->GetXYZ(0)->Y = icDtoF(0.5137);
        tags->green->GetXYZ(0)->Z = icDtoF(0.1708);

        // Attach blue XYZ primary to profile
        tags->blue = new CIccTagXYZ();
        tags->blue->GetXYZ(0)->X = icDtoF(0);
        tags->blue->GetXYZ(0)->Y = icDtoF(0.0433);
        tags->blue->GetXYZ(0)->Z = icDtoF(0.6541);
        break;
    case cs_Linear_Normalized_XYZ:
        tags->red = new CIccTagXYZ();
        tags->red->GetXYZ(0)->X = icDtoF(0.96420288);
        tags->red->GetXYZ(0)->Y = icDtoF(0);
        tags->red->GetXYZ(0)->Z = icDtoF(0);

        // Attach green XYZ primary to profile
        tags->green = new CIccTagXYZ();
        tags->green->GetXYZ(0)->X = icDtoF(0);
        tags->green->GetXYZ(0)->Y = icDtoF(1);
        tags->green->GetXYZ(0)->Z = icDtoF(0);

        // Attach blue XYZ primary to profile
        tags->blue = new CIccTagXYZ();
        tags->blue->GetXYZ(0)->X = icDtoF(0);
        tags->blue->GetXYZ(0)->Y = icDtoF(0);
        tags->blue->GetXYZ(0)->Z = icDtoF(0.82487488);
        break;
    default:
        break;
    }
}

void createParametricCurve(CIccTagParametricCurve *parametricCurve,
                           const ProfileColorSpace colorSpace) {
    parametricCurve->SetFunctionType(3);
    icFloatNumber *param = parametricCurve->GetParams();

    switch (colorSpace) {
    case cs_ProPhoto:
        param[0] = 1.80000;
        param[1] = 1.00000;
        param[2] = 0.00000;
        param[3] = 0.0625;
        param[4] = 0.03125;
        break;
    case cs_sRGB:
        param[0] = 2.4; // gamma
        param[1] = 0.947867298578199; // a
        param[2] = 0.0521327014218010; // b
        param[3] = 0.077399380804954; // c
        param[4] = 0.04045; // d
        break;
    case cs_Wide_Gamut_RGB:
        param[0] = 3.0;
        param[1] = 1.121991404;
        param[2] = 0.137931034;
        param[3] = 0.144084508;
        param[4] = 0.061467064;
        break;
    default:
        break;
    }
}

std::string getProfileName(const ProfileColorSpace colorSpace) {
    switch (colorSpace) {
    case cs_Adobe_RGB_1998:
        return "Hybrid aRGB";
    case cs_ProPhoto:
        return "Hybrid Pro Photo RGB";
    case cs_sRGB:
        return "Hybrid aRGB";
    case cs_Wide_Gamut_RGB:
        return "Hybrid Wide Gamut RGB";
    case cs_Linear_Normalized_XYZ:
        return "Hybrid Linear Normalized XYZ as RGB";
    }

    return "Unkown Space";
}

CIccProfile *CreateIccProfile::createRgbProfile(const ProfileColorSpace space) {
    VALIDATE_COLOR_SPACE(space);

    const auto pIcc = new CIccProfile();

    // Set up the profile header
    pIcc->InitHeader();
    pIcc->m_Header.deviceClass = icSigDisplayClass;
    pIcc->m_Header.version = icVersionNumberV4_3;
    pIcc->m_Header.colorSpace = icSigRgbData;
    pIcc->m_Header.pcs = icSigXYZData;

    // Allocate copyright tag and attach to profile
    auto *pText = new CIccTagMultiLocalizedUnicode();
    pText->SetText("Copyright (C) 2026 BeyondRGB");

    pIcc->AttachTag(icSigCopyrightTag, pText);

    // Allocate media white point tag and attach to profile
    auto *pXYZ = new CIccTagXYZ();
    (*pXYZ)[0].X = icDtoF(0.96420288);
    (*pXYZ)[0].Y = icDtoF(1.00000000);
    (*pXYZ)[0].Z = icDtoF(0.82487488);

    pIcc->AttachTag(icSigMediaWhitePointTag, pXYZ);

    pText = new CIccTagMultiLocalizedUnicode();
    pText->SetText(getProfileName(space).c_str());

    pIcc->AttachTag(icSigProfileDescriptionTag, pText);

    auto *tags = new RGBTags();
    createRGBTags(tags, space);

    pIcc->AttachTag(icSigRedMatrixColumnTag, tags->red);
    pIcc->AttachTag(icSigGreenMatrixColumnTag, tags->green);
    pIcc->AttachTag(icSigBlueMatrixColumnTag, tags->blue);

    // These three color spaces use a parametric curves, so attach one to the profile.
    if (space == cs_ProPhoto || space == cs_sRGB || space ==
        cs_Wide_Gamut_RGB) {
        auto *pCurveParametric = new CIccTagParametricCurve();
        createParametricCurve(pCurveParametric, space);
        pIcc->AttachTag(icSigRedTRCTag, pCurveParametric);
        pIcc->AttachTag(icSigGreenTRCTag, pCurveParametric);
        pIcc->AttachTag(icSigBlueTRCTag, pCurveParametric);
    }

    // These two color spaces use a gamma curve, so attach one to the profile.
    if (space == cs_Adobe_RGB_1998 || space == cs_Linear_Normalized_XYZ) {
        auto *pCurveGamma = new CIccTagCurve();
        pCurveGamma->SetGamma(space == cs_Adobe_RGB_1998
                                  ? (563.0f / 256.0f)
                                  : 1.0f);

        pIcc->AttachTag(icSigRedTRCTag, pCurveGamma);
        pIcc->AttachTag(icSigGreenTRCTag, pCurveGamma);
        pIcc->AttachTag(icSigBlueTRCTag, pCurveGamma);
    }

    return pIcc;
}


bool loadSpectralRanges(CIccProfile *profile, int numChannelsOut) {
    switch (numChannelsOut) {
    case 36:
        profile->m_Header.spectralRange.start = icFtoF16(380.0f);
        profile->m_Header.spectralRange.end = icFtoF16(730.0f);
        profile->m_Header.spectralRange.steps = numChannelsOut;
        return true;
    case 31:
        profile->m_Header.spectralRange.start = icFtoF16(400.0f);
        profile->m_Header.spectralRange.end = icFtoF16(700.0f);
        profile->m_Header.spectralRange.steps = numChannelsOut;
        return true;
    case 17:
        profile->m_Header.spectralRange.start = icFtoF16(380.0f);
        profile->m_Header.spectralRange.end = icFtoF16(720.0f);
        profile->m_Header.spectralRange.steps = numChannelsOut;
        return true;
    case 15:
        profile->m_Header.spectralRange.start = icFtoF16(400.0f);
        profile->m_Header.spectralRange.end = icFtoF16(700.0f);
        profile->m_Header.spectralRange.steps = numChannelsOut;
        return true;
    default:
        return false;
    }
}

void attachNoBaseChannelMatrix(CIccTagMultiProcessElement *multiProcessTag,
                               const int numInputChannels,
                               const int numOutputChannels,
                               const float *dataMatrix) {
    auto *matrix = new CIccMpeMatrix();

    matrix->SetSize(numInputChannels, numOutputChannels);
    icFloatNumber *pData = matrix->GetMatrix();
    const icFloatNumber *pRow = dataMatrix;

    for (int r = 0; r < numOutputChannels; r++) {
        memset(
            pData, 0,
            3 * sizeof(icFloatNumber)); // zero out the base profile columns
        pData += 3;
        for (int c = 0; c < numInputChannels; c++) {
            *pData++ = *pRow++;
        }
    }

    multiProcessTag->Attach(matrix);
}

using FunctionParameters = std::array<icFloatNumber, 4>;

void applyFormula(CIccFormulaCurveSegment *curveFormula,
                  FunctionParameters parameters, const bool shareCurve,
                  CIccSegmentedCurve *segmentedCurve, CIccMpeCurveSet *curveSet,
                  const int otherCurves = 3) {
    curveFormula->SetFunction(0, 4, parameters.data());
    segmentedCurve->Insert(curveFormula);

    if (shareCurve) {
        // share the curve for all 3 base channels since they are the same
        for (int i = 0; i < otherCurves; i++) {
            curveSet->SetCurve(i, segmentedCurve);
        }
    }
}

struct CurveProfile {
    struct Segment {
        icFloatNumber end;
        FunctionParameters params;
    };

    Segment linear; // empty if no linear segment
    Segment gamma;
    bool hasLinear;
};

struct ColorSpaceCurveProfiles {
    CurveProfile normal;
    CurveProfile inverse;
};

static const std::unordered_map<ProfileColorSpace, ColorSpaceCurveProfiles> profiles =
{
    {cs_Adobe_RGB_1998, {
        // Normal: A gamma 2.2 curve with no offset.
         {{}, {icMaxFloat32Number, {563.0f / 256.0f, 1.0f, 0.0f, 0.0f}}, false},
        // Inverse: A gamma (1 / 2.2) curve with no offset.
         {{}, {icMaxFloat32Number, {256.0f / 563.0f, 1.0f, 0.0f, 0.0f}}, false},
     }},
    {cs_ProPhoto, {
         { // Normal Profile
             // A linear curve from 0 to 0.13 with a slope of 0.0625 (1/16) to avoid a hard cut-off at 0.0.
             {0.03125f, {1.0f, 0.0625f, 0.0f, 0.0f}},
             // A gamma 1.8 curve with no offset
             {icMaxFloat32Number, {1.8f, 1.0f, 0.0f, 0.0f}}, true
         },
         { // Inverse Profile
             // A linear curve from 0 to 0.001953125 with a slope of 16 to avoid a hard cut-off at 0.0.
             {0.001953125f, {1.0f, 16.0f, 0.0f, 0.0f}},
             // A gamma (1 / 1.8) curve with no offset
             {icMaxFloat32Number, {0.555557250977f, 1.0f, 0.0f, 0.0f}},
             true
         },
     }},
    {cs_sRGB, {
         { // Normal Profile
             // A linear curve from 0 to 0.04045 with slope of 0.077399380804954 to avoid a hard cutoff at 0.0, then apply a gamma 2.4 curve with no offset
             {0.04045f, {1.0f, 0.077399380804954f, 0.0f, 0.0f}},
             // A gamma 2.4 curve with a slight offset
             {icMaxFloat32Number, {2.4f, 0.947867298578199f, 0.0521327014218010f, 0.0f}}, true
         },
         { // Inverse Profile
             // A linear curve from 0 to 0.0031308 with slope of 12.92 to avoid a hard cut off at 0.0.
             {0.0031308f, {1.0f, 12.92f, 0.0f, 0.0f}},
             // A gamma (1 / 2.4) curve with a slight offset
             {icMaxFloat32Number, {1.0f / 2.4f, 1.055f, 0.0f, -0.055f}},
             true
         },
     }},
};

void attachGammaCurve(CIccTagMultiProcessElement *multiProcessTag,
                      const ProfileColorSpace colorSpace,
                      CIccSegmentedCurve *segmentedCurve,
                      CIccMpeCurveSet *curveSet,
                      const int totalNumberOfChannels,
                      const bool inverse = false) {
    if (colorSpace == cs_Linear_Normalized_XYZ) {
        return;
    }

    CIccFormulaCurveSegment *curveSegmentFormula;

    ProfileColorSpace adjustedColorSpace = colorSpace;

    // Wide gamut is processed the same as adobe rgb so change it for the profile lookup.
    if (colorSpace == cs_Wide_Gamut_RGB) {
        adjustedColorSpace = cs_Adobe_RGB_1998;
    }

    //.Find the ColorSpaceProfile that matches this Color Space.
    const auto iterator = profiles.find(adjustedColorSpace);
    if (iterator == profiles.end()) return;

    // If we are attaching an inverse curve, get the profile's inverse Profile or its normal profile if not inverse.
    const auto&[linearSegment, gammaSegment, hasLinear] = inverse ? iterator->second.inverse : iterator->second.normal;

    // If the color space should apply a linear segment, do that here.
    if (hasLinear) {
        curveSegmentFormula = new CIccFormulaCurveSegment(0.0f, linearSegment.end);
        applyFormula(curveSegmentFormula, linearSegment.params, false, segmentedCurve, curveSet);
    }

    // Apply the gamma curve. If there was a linear profile we need to adjust the start.
    curveSegmentFormula = new CIccFormulaCurveSegment(hasLinear ? linearSegment.end : 0.0f, gammaSegment.end);
    applyFormula(curveSegmentFormula, gammaSegment.params, true, segmentedCurve, curveSet);

    // The rest of the channels are assumed to be linear, so set them up with a simple gamma 1.0 curve with no clipping
    segmentedCurve = new CIccSegmentedCurve();
    curveSegmentFormula = new CIccFormulaCurveSegment(icMinFloat32Number, icMaxFloat32Number);
    applyFormula(curveSegmentFormula,  FunctionParameters{1.0f, 1.0f, 0.0f, 0.0f},  true, segmentedCurve, curveSet, totalNumberOfChannels);

    // Finally, attach the curve set we made.
    multiProcessTag->Attach(curveSet);
}

void attachMatrix(CIccTagMultiProcessElement *multiProcessTag,
                  const float *dataMatrix, const int totalNumberOfChannels,
                  const int numOutputChannels) {
    auto *profileDataMatrix = new CIccMpeMatrix();

    profileDataMatrix->SetSize(totalNumberOfChannels, numOutputChannels);

    const int matrixSize = totalNumberOfChannels * numOutputChannels;
    for (int i = 0; i < matrixSize; i++) {
        profileDataMatrix->GetMatrix()[i] = dataMatrix[i];
    }

    // Add the dataMatrix to the multiprocessing element tag.
    multiProcessTag->Attach(profileDataMatrix);
}

CIccProfile *CreateIccProfile::createSpecProfile(
    const ProfileColorSpace baseSpace,
    const float *dataMatrix,
    const int numInputChannels,
    const int numOutputChannels,
    const bool ignore_base_channels,
    const float *inverseMatrix) {
    VALIDATE_COLOR_SPACE(baseSpace);

    const int totalNumberOfChannels = !ignore_base_channels
                                          ? numInputChannels
                                          : numInputChannels + 3;

    const auto iccProfile = new CIccProfile();

    // Set up the profile header
    iccProfile->InitHeader();
    iccProfile->m_Header.deviceClass = icSigDisplayClass;
    iccProfile->m_Header.version = icVersionNumberV5_1;
    iccProfile->m_Header.flags |= icEmbeddedProfileTrue;
    iccProfile->m_Header.colorSpace =
        icNColorSpaceSig(icSigNChannelData, (totalNumberOfChannels & 0xffff));
    iccProfile->m_Header.pcs = static_cast<icColorSpaceSignature>(0);
    iccProfile->m_Header.spectralPCS =
        icNColorSpaceSig(icSigReflectanceSpectralData,
                         (numOutputChannels & 0xffff));

    // Allocate copyright tag and attach to profile
    auto *pText = new CIccTagMultiLocalizedUnicode();
    pText->SetText("Copyright (C) 2026 BeyondRGB");

    iccProfile->AttachTag(icSigCopyrightTag, pText);

    // Load the spectral ranges into the profile. If it fails (numOutputChannels is invalid) fail out of the function.
    if (!loadSpectralRanges(iccProfile, numOutputChannels)) {
        delete iccProfile;
        return nullptr;
    }

    // Allocate media white point tag and attach to profile
    auto *pWhiteTag = new CIccTagFloat32();
    pWhiteTag->SetSize(numOutputChannels);
    for (int i = 0; i < numOutputChannels; i++) {
        (*pWhiteTag)[i] = 1.0;
    }

    iccProfile->AttachTag(icSigSpectralWhitePointTag, pWhiteTag);

    // Allocate profile description tag and attach to profile
    pText = new CIccTagMultiLocalizedUnicode();
    pText->SetText("BeyondRGB to spectra");

    iccProfile->AttachTag(icSigProfileDescriptionTag, pText);

    // Allocate spectral viewing conditions and attach to profile
    auto *pCond = new CIccTagSpectralViewingConditions();

    icFloatNumber lum = 203.0;
    pCond->m_illuminantXYZ.X = icDtoF(0.96420288f * lum);
    pCond->m_illuminantXYZ.Y = icDtoF(1.00000000f * lum);
    pCond->m_illuminantXYZ.Z = icDtoF(0.82487488f * lum);

    pCond->m_surroundXYZ.X = icDtoF(0.96420288f * lum);
    pCond->m_surroundXYZ.Y = icDtoF(1.00000000f * lum);
    pCond->m_surroundXYZ.Z = icDtoF(0.82487488f * lum);

    icSpectralRange range;
    range.start = icFtoF16(380.0);
    range.end = icFtoF16(730.0);
    range.steps = 71;
    pCond->setIlluminant(icIlluminantD50, range, nullptr, 5000);
    pCond->setObserver(icStdObs1931TwoDegrees, range, nullptr);

    iccProfile->AttachTag(icSigSpectralViewingConditionsTag, pCond);

    // Populate DToB3 tag and attach to profile
    auto *multiProcessTag = new CIccTagMultiProcessElement(
        totalNumberOfChannels, numOutputChannels);

    if (ignore_base_channels) {
        // If we are ignoring the base profile channels, then just add a dataMatrix element with zero columns for the base channels and the provided data and no curves
        attachNoBaseChannelMatrix(multiProcessTag, numInputChannels,
                                  numOutputChannels, dataMatrix);
    } else {
        auto *curveSet = new CIccMpeCurveSet(totalNumberOfChannels);
        auto *segmentedCurve = new CIccSegmentedCurve();

        // clip the shadows to 0.0
        auto *shadowCurveFormula = new CIccFormulaCurveSegment(
            icMinFloat32Number, 0.0f);
        applyFormula(shadowCurveFormula,
                     FunctionParameters{1.0f, 0.0f, 0.0f, 0.0f},
                     false, segmentedCurve, curveSet);

        // Load gamma curves based on the color space.
        attachGammaCurve(multiProcessTag, baseSpace, segmentedCurve, curveSet,
                         totalNumberOfChannels);

        // Attach the data matrix to the end of the multi process tag.
        attachMatrix(multiProcessTag, dataMatrix, totalNumberOfChannels,
                     numOutputChannels);
    }

    iccProfile->AttachTag(icSigDToB3Tag, multiProcessTag);

    if (inverseMatrix) {
        auto *inverseMultiProcessTag = new CIccTagMultiProcessElement(
            numOutputChannels, totalNumberOfChannels);

        // Attach the data matrix to the start of the multi process tag.
        attachMatrix(inverseMultiProcessTag, inverseMatrix,
                     totalNumberOfChannels, numOutputChannels);

        auto *curveSet = new CIccMpeCurveSet(totalNumberOfChannels);
        auto *segmentedCurve = new CIccSegmentedCurve();

        // Clip shadows to 0.0.
        auto *shadowCurveFormula = new CIccFormulaCurveSegment(
            icMinFloat32Number, 0.0f);
        applyFormula(shadowCurveFormula,
                     FunctionParameters{1.0f, 0.0f, 0.0f, 0.0f},
                     false, segmentedCurve, curveSet);

        // Attach the inverse gamma curve tag to the profile
        attachGammaCurve(inverseMultiProcessTag, baseSpace, segmentedCurve,
                         curveSet, totalNumberOfChannels, true);

        // Attach the inverse multi process tag to the icc profile.
        iccProfile->AttachTag(icSigBToD3Tag, inverseMultiProcessTag);
    }

    return iccProfile;
}
} // namespace btrgb
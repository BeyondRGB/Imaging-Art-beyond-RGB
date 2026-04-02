#include <image_util/icc_profile/IccProfile.hpp>
#include <iostream>
#include <ostream>
#include <unordered_map>
#include <array>
// Needed for memset on GCC.
#include <image_util/ColorProfiles.hpp>

#include <cstring>

#include <IccProfLib/IccIO.h>
#include <IccProfLib/IccMpeBasic.h>
#include <IccProfLib/IccTagBasic.h>
#include <IccProfLib/IccTagEmbedIcc.h>
#include <IccProfLib/IccTagMPE.h>
#include <IccProfLib/IccUtil.h>

namespace btrgb::icc {
IccProfile::IccProfile(size_t max_size) {
    max_profile_size = max_size;
    hybrid_icc_profile = nullptr;
    profile_memory = nullptr;
    profile_size = 0;
}

IccProfile::~IccProfile() {
    delete hybrid_icc_profile;
    delete[] profile_memory;
}

bool IccProfile::create_hybrid_profile(const ColorSpace space,
                                     const float *data_matrix,
                                     const int num_input_channels,
                                     const int num_output_channels,
                                     const bool ignore_ignore_base_channels,
                                     const float *inverse_matrix) {
    if (hybrid_icc_profile) {
        delete hybrid_icc_profile;
        delete[] profile_memory;
    }

    CIccProfile *rgb_profile = create_rgb_profile(space);

    if (!rgb_profile) {
        return false;
    }

    CIccProfile *spec_profile = create_spec_profile(
        space, data_matrix, num_input_channels, num_output_channels,
        ignore_ignore_base_channels, inverse_matrix);
    if (!spec_profile) {
        delete rgb_profile;
    }

    // Allocate IO tag, attach spec_profile to it, and embed it in rgb profile
    auto *pTag = new CIccTagEmbeddedProfile();
    pTag->SetProfile(spec_profile);
    rgb_profile->AttachTag(icSigEmbeddedV5ProfileTag, pTag);

    hybrid_icc_profile = rgb_profile;

    // Cache the profile memory.
    get_profile_mem();

    return true;
}

struct RGBTags {
    CIccTagXYZ *red;
    CIccTagXYZ *green;
    CIccTagXYZ *blue;
};

void create_rgb_tags(RGBTags *tags, const ColorSpace color_space) {
    switch (color_space) {
    case Adobe_RGB_1998:
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
    case ProPhoto:
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
    case sRGB:
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
    case Wide_Gamut_RGB:
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
    case Linear_Normalized_XYZ:
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

void create_parametric_curve(CIccTagParametricCurve *parametric_curve,
                           const ColorSpace color_space) {
    parametric_curve->SetFunctionType(3);
    icFloatNumber *param = parametric_curve->GetParams();

    switch (color_space) {
    case ProPhoto:
        param[0] = 1.80000;
        param[1] = 1.00000;
        param[2] = 0.00000;
        param[3] = 0.0625;
        param[4] = 0.03125;
        break;
    case sRGB:
        param[0] = 2.4; // gamma
        param[1] = 0.947867298578199; // a
        param[2] = 0.0521327014218010; // b
        param[3] = 0.077399380804954; // c
        param[4] = 0.04045; // d
        break;
    case Wide_Gamut_RGB:
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

std::string get_profile_name(const ColorSpace color_space) {
    switch (color_space) {
    case Adobe_RGB_1998:
        return "Hybrid aRGB";
    case ProPhoto:
        return "Hybrid Pro Photo RGB";
    case sRGB:
        return "Hybrid aRGB";
    case Wide_Gamut_RGB:
        return "Hybrid Wide Gamut RGB";
    case Linear_Normalized_XYZ:
        return "Hybrid Linear Normalized XYZ as RGB";
    case none:
    default:
        return "Unknown Space";
    }

}

CIccProfile *IccProfile::create_rgb_profile(const ColorSpace space) {
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
    pText->SetText(get_profile_name(space).c_str());

    pIcc->AttachTag(icSigProfileDescriptionTag, pText);

    auto *tags = new RGBTags();
    create_rgb_tags(tags, space);

    pIcc->AttachTag(icSigRedMatrixColumnTag, tags->red);
    pIcc->AttachTag(icSigGreenMatrixColumnTag, tags->green);
    pIcc->AttachTag(icSigBlueMatrixColumnTag, tags->blue);

    // These three color spaces use a parametric curves, so attach one to the profile.
    if (space == ProPhoto || space == sRGB || space ==
        Wide_Gamut_RGB) {
        auto *pCurveParametric = new CIccTagParametricCurve();
        create_parametric_curve(pCurveParametric, space);
        pIcc->AttachTag(icSigRedTRCTag, pCurveParametric);
        pIcc->AttachTag(icSigGreenTRCTag, pCurveParametric);
        pIcc->AttachTag(icSigBlueTRCTag, pCurveParametric);
    }

    // These two color spaces use a gamma curve, so attach one to the profile.
    if (space == Adobe_RGB_1998 || space == Linear_Normalized_XYZ) {
        auto *pCurveGamma = new CIccTagCurve();
        pCurveGamma->SetGamma(space == Adobe_RGB_1998
                                  ? (563.0f / 256.0f)
                                  : 1.0f);

        pIcc->AttachTag(icSigRedTRCTag, pCurveGamma);
        pIcc->AttachTag(icSigGreenTRCTag, pCurveGamma);
        pIcc->AttachTag(icSigBlueTRCTag, pCurveGamma);
    }

    return pIcc;
}


bool load_spectral_ranges(CIccProfile *profile, int num_channels_out) {
    switch (num_channels_out) {
    case 36:
        profile->m_Header.spectralRange.start = icFtoF16(380.0f);
        profile->m_Header.spectralRange.end = icFtoF16(730.0f);
        profile->m_Header.spectralRange.steps = num_channels_out;
        return true;
    case 31:
        profile->m_Header.spectralRange.start = icFtoF16(400.0f);
        profile->m_Header.spectralRange.end = icFtoF16(700.0f);
        profile->m_Header.spectralRange.steps = num_channels_out;
        return true;
    case 17:
        profile->m_Header.spectralRange.start = icFtoF16(380.0f);
        profile->m_Header.spectralRange.end = icFtoF16(720.0f);
        profile->m_Header.spectralRange.steps = num_channels_out;
        return true;
    case 15:
        profile->m_Header.spectralRange.start = icFtoF16(400.0f);
        profile->m_Header.spectralRange.end = icFtoF16(700.0f);
        profile->m_Header.spectralRange.steps = num_channels_out;
        return true;
    default:
        return false;
    }
}

void attach_no_base_channel_matrix(CIccTagMultiProcessElement *multi_process_tag,
                               const int num_input_channels,
                               const int num_output_channels,
                               const float *data_matrix) {
    auto *matrix = new CIccMpeMatrix();

    matrix->SetSize(num_input_channels, num_output_channels);
    icFloatNumber *pData = matrix->GetMatrix();
    const icFloatNumber *pRow = data_matrix;

    for (int r = 0; r < num_output_channels; r++) {
        memset(
            pData, 0,
            3 * sizeof(icFloatNumber)); // zero out the base profile columns
        pData += 3;
        for (int c = 0; c < num_input_channels; c++) {
            *pData++ = *pRow++;
        }
    }

    multi_process_tag->Attach(matrix);
}

typedef std::array<icFloatNumber, 4> FunctionParameters;

void apply_formula(CIccFormulaCurveSegment *curve_formula,
                  FunctionParameters parameters, const bool share_curve,
                  CIccSegmentedCurve *segmented_curve, CIccMpeCurveSet *curve_set,
                  const int other_curves = 3) {
    curve_formula->SetFunction(0, 4, parameters.data());
    segmented_curve->Insert(curve_formula);

    if (share_curve) {
        // share the curve for all 3 base channels since they are the same
        for (int i = 0; i < other_curves; i++) {
            curve_set->SetCurve(i, segmented_curve);
        }
    }
}

struct CurveProfile {
    struct Segment {
        icFloatNumber end;
        FunctionParameters params;
    };

    Segment linear; // Empty if no linear segment
    Segment gamma;
    bool hasLinear;
};

struct ColorSpaceCurveProfiles {
    CurveProfile normal;
    CurveProfile inverse;
};


static const std::unordered_map<ColorSpace, ColorSpaceCurveProfiles>
profiles =
{
    {Adobe_RGB_1998, {
         // Normal: A gamma 2.2 curve with no offset.
         {{}, {icMaxFloat32Number, {563.0f / 256.0f, 1.0f, 0.0f, 0.0f}}, false},
         // Inverse: A gamma (1 / 2.2) curve with no offset.
         {{}, {icMaxFloat32Number, {256.0f / 563.0f, 1.0f, 0.0f, 0.0f}}, false},
     }},
    {ProPhoto, {
         {
             // Normal Profile
             // A linear curve from 0 to 0.13 with a slope of 0.0625 (1/16) to avoid a hard cut-off at 0.0.
             {0.03125f, {1.0f, 0.0625f, 0.0f, 0.0f}},
             // A gamma 1.8 curve with no offset
             {icMaxFloat32Number, {1.8f, 1.0f, 0.0f, 0.0f}}, true
         },
         {
             // Inverse Profile
             // A linear curve from 0 to 0.001953125 with a slope of 16 to avoid a hard cut-off at 0.0.
             {0.001953125f, {1.0f, 16.0f, 0.0f, 0.0f}},
             // A gamma (1 / 1.8) curve with no offset
             {icMaxFloat32Number, {0.555557250977f, 1.0f, 0.0f, 0.0f}},
             true
         },
     }},
    {sRGB, {
         {
             // Normal Profile
             // A linear curve from 0 to 0.04045 with slope of 0.077399380804954 to avoid a hard cutoff at 0.0, then apply a gamma 2.4 curve with no offset
             {0.04045f, {1.0f, 0.077399380804954f, 0.0f, 0.0f}},
             // A gamma 2.4 curve with a slight offset
             {icMaxFloat32Number, {2.4f, 0.947867298578199f,
                                   0.0521327014218010f, 0.0f}},
             true
         },
         {
             // Inverse Profile
             // A linear curve from 0 to 0.0031308 with slope of 12.92 to avoid a hard cut off at 0.0.
             {0.0031308f, {1.0f, 12.92f, 0.0f, 0.0f}},
             // A gamma (1 / 2.4) curve with a slight offset
             {icMaxFloat32Number, {1.0f / 2.4f, 1.055f, 0.0f, -0.055f}},
             true
         },
     }},
};

void attach_gamma_curve(CIccTagMultiProcessElement *multi_process_tag,
                      const ColorSpace color_space,
                      CIccSegmentedCurve *segmented_curve,
                      CIccMpeCurveSet *curve_set,
                      const int total_number_of_channels,
                      const bool inverse = false) {
    if (color_space == Linear_Normalized_XYZ) {
        return;
    }

    CIccFormulaCurveSegment *curve_formula;

    ColorSpace adjusted_color_space = color_space;

    // Wide gamut is processed the same as adobe rgb so change it for the profile lookup.
    if (color_space == Wide_Gamut_RGB) {
        adjusted_color_space = Adobe_RGB_1998;
    }

    //.Find the ColorSpaceProfile that matches this Color Space.
    const auto iterator = profiles.find(adjusted_color_space);
    if (iterator == profiles.end())
        return;

    // If we are attaching an inverse curve, get the profile's inverse Profile or its normal profile if not inverse.
    const auto &[linearSegment, gammaSegment, hasLinear] = inverse
        ? iterator->second.inverse
        : iterator->second.normal;

    // If the color space should apply a linear segment, do that here.
    if (hasLinear) {
        curve_formula = new CIccFormulaCurveSegment(
            0.0f, linearSegment.end);
        apply_formula(curve_formula, linearSegment.params, false,
                     segmented_curve, curve_set);
    }

    // Apply the gamma curve. If there was a linear profile we need to adjust the start.
    curve_formula = new CIccFormulaCurveSegment(
        hasLinear ? linearSegment.end : 0.0f, gammaSegment.end);
    apply_formula(curve_formula, gammaSegment.params, true, segmented_curve,
                 curve_set);

    // The rest of the channels are assumed to be linear, so set them up with a simple gamma 1.0 curve with no clipping
    segmented_curve = new CIccSegmentedCurve();
    curve_formula = new CIccFormulaCurveSegment(
        icMinFloat32Number, icMaxFloat32Number);
    apply_formula(curve_formula,
                 FunctionParameters{1.0f, 1.0f, 0.0f, 0.0f}, true,
                 segmented_curve, curve_set, total_number_of_channels);

    // Finally, attach the curve set we made.
    multi_process_tag->Attach(curve_set);
}

void attach_matrix(CIccTagMultiProcessElement *multi_process_tag,
                  const float *data_matrix, const int total_number_of_channels,
                  const int num_output_channels) {
    auto *profile_data_matrix = new CIccMpeMatrix();

    profile_data_matrix->SetSize(total_number_of_channels, num_output_channels);

    const int matrix_size = total_number_of_channels * num_output_channels;
    for (int i = 0; i < matrix_size; i++) {
        profile_data_matrix->GetMatrix()[i] = data_matrix[i];
    }

    // Add the data_matrix to the multiprocessing element tag.
    multi_process_tag->Attach(profile_data_matrix);
}

CIccProfile *IccProfile::create_spec_profile(
    const ColorSpace base_space,
    const float *data_matrix,
    const int num_input_channels,
    const int num_output_channels,
    const bool ignore_base_channels,
    const float *inverse_matrix) {
    VALIDATE_COLOR_SPACE(base_space);

    const int total_number_of_channels = !ignore_base_channels
                                          ? num_input_channels
                                          : num_input_channels + 3;

    const auto icc_profile = new CIccProfile();

    // Set up the profile header
    icc_profile->InitHeader();
    icc_profile->m_Header.deviceClass = icSigDisplayClass;
    icc_profile->m_Header.version = icVersionNumberV5_1;
    icc_profile->m_Header.flags |= icEmbeddedProfileTrue;
    icc_profile->m_Header.colorSpace =
        icNColorSpaceSig(icSigNChannelData, (total_number_of_channels & 0xffff));
    icc_profile->m_Header.pcs = static_cast<icColorSpaceSignature>(0);
    icc_profile->m_Header.spectralPCS =
        icNColorSpaceSig(icSigReflectanceSpectralData,
                         (num_output_channels & 0xffff));

    // Allocate copyright tag and attach to profile
    auto *pText = new CIccTagMultiLocalizedUnicode();
    pText->SetText("Copyright (C) 2026 BeyondRGB");

    icc_profile->AttachTag(icSigCopyrightTag, pText);

    // Load the spectral ranges into the profile. If it fails (num_output_channels is invalid) fail out of the function.
    if (!load_spectral_ranges(icc_profile, num_output_channels)) {
        delete icc_profile;
        return nullptr;
    }

    // Allocate media white point tag and attach to profile
    auto *pWhiteTag = new CIccTagFloat32();
    pWhiteTag->SetSize(num_output_channels);
    for (int i = 0; i < num_output_channels; i++) {
        (*pWhiteTag)[i] = 1.0;
    }

    icc_profile->AttachTag(icSigSpectralWhitePointTag, pWhiteTag);

    // Allocate profile description tag and attach to profile
    pText = new CIccTagMultiLocalizedUnicode();
    pText->SetText("BeyondRGB to spectra");

    icc_profile->AttachTag(icSigProfileDescriptionTag, pText);

    // Allocate spectral viewing conditions and attach to profile
    auto *viewing_conditions = new CIccTagSpectralViewingConditions();

    icFloatNumber lum = 203.0;
    viewing_conditions->m_illuminantXYZ.X = icDtoF(0.96420288f * lum);
    viewing_conditions->m_illuminantXYZ.Y = icDtoF(1.00000000f * lum);
    viewing_conditions->m_illuminantXYZ.Z = icDtoF(0.82487488f * lum);

    viewing_conditions->m_surroundXYZ.X = icDtoF(0.96420288f * lum);
    viewing_conditions->m_surroundXYZ.Y = icDtoF(1.00000000f * lum);
    viewing_conditions->m_surroundXYZ.Z = icDtoF(0.82487488f * lum);

    icSpectralRange range;
    range.start = icFtoF16(380.0);
    range.end = icFtoF16(730.0);
    range.steps = 71;
    viewing_conditions->setIlluminant(icIlluminantD50, range, nullptr, 5000);
    viewing_conditions->setObserver(icStdObs1931TwoDegrees, range, nullptr);

    icc_profile->AttachTag(icSigSpectralViewingConditionsTag, viewing_conditions);

    // Populate DToB3 tag and attach to profile
    auto *multi_process_tag = new CIccTagMultiProcessElement(
        total_number_of_channels, num_output_channels);

    if (ignore_base_channels) {
        // If we are ignoring the base profile channels, then just add a data_matrix element with zero columns for the base channels and the provided data and no curves
        attach_no_base_channel_matrix(multi_process_tag, num_input_channels,
                                  num_output_channels, data_matrix);
    } else {
        auto *curve_set = new CIccMpeCurveSet(total_number_of_channels);
        auto *segmented_curve = new CIccSegmentedCurve();

        // clip the shadows to 0.0
        auto *curve_formula = new CIccFormulaCurveSegment(
            icMinFloat32Number, 0.0f);
        apply_formula(curve_formula,
                     FunctionParameters{1.0f, 0.0f, 0.0f, 0.0f},
                     false, segmented_curve, curve_set);

        // Load gamma curves based on the color space.
        attach_gamma_curve(multi_process_tag, base_space, segmented_curve, curve_set,
                         total_number_of_channels);

        // Attach the data matrix to the end of the multi process tag.
        attach_matrix(multi_process_tag, data_matrix, total_number_of_channels,
                     num_output_channels);
    }

    icc_profile->AttachTag(icSigDToB3Tag, multi_process_tag);

    if (inverse_matrix) {
        auto *inverse_multi_process_tag = new CIccTagMultiProcessElement(
            num_output_channels, total_number_of_channels);

        // Attach the data matrix to the start of the multi process tag.
        attach_matrix(inverse_multi_process_tag, inverse_matrix,
                     total_number_of_channels, num_output_channels);

        auto *curve_set = new CIccMpeCurveSet(total_number_of_channels);
        auto *segmented_curve = new CIccSegmentedCurve();

        // Clip shadows to 0.0.
        auto *curve_formula = new CIccFormulaCurveSegment(
            icMinFloat32Number, 0.0f);
        apply_formula(curve_formula,
                     FunctionParameters{1.0f, 0.0f, 0.0f, 0.0f},
                     false, segmented_curve, curve_set);

        // Attach the inverse gamma curve tag to the profile
        attach_gamma_curve(inverse_multi_process_tag, base_space, segmented_curve,
                         curve_set, total_number_of_channels, true);

        // Attach the inverse multi process tag to the icc profile.
        icc_profile->AttachTag(icSigBToD3Tag, inverse_multi_process_tag);
    }

    return icc_profile;
}


unsigned char *IccProfile::get_profile_mem() {
    if (profile_memory)
        return profile_memory;
    if (!hybrid_icc_profile) {
        throw std::runtime_error(
            "No hybrid profile available to save. Run btrgb::icc::IccProfile::create_hybrid_profile.");
    }

    profile_memory = new unsigned char[max_profile_size];

    CIccMemIO io;
    io.Attach(profile_memory, max_profile_size, true);

    if (!hybrid_icc_profile->Write(&io)) {
        delete[] profile_memory;
        return nullptr;
    }

    profile_size = io.GetLength();

    return profile_memory;
}

size_t IccProfile::get_profile_size() const {
    return profile_size;
}

void IccProfile::write(const std::string &path) const {
    if (!hybrid_icc_profile) {
        throw std::runtime_error(
            "No hybrid profile available to save. Run btrgb::icc::IccProfile::create_hybrid_profile.");
    }

    auto *iccIO = new CIccFileIO();
    iccIO->Open(path.c_str(), "wb");

    hybrid_icc_profile->Write(iccIO, icVersionBasedID);

    iccIO->Close();
}

} // namespace btrgb

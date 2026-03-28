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
                                               float *matrix, const int num_in,
                                               const int num_out,
                                               bool ignore_base_channels,
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
            space, matrix, num_in, num_out, ignore_base_channels, inv_matrix);
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

    void createRGBTags(RGBTags* tags, const ProfileColorSpace colorSpace) {
        switch (colorSpace) {
            case cs_Adobe_RGB_1998:
                // Attach red XYZ primary to profile
                tags->red = new CIccTagXYZ();
                tags->red->GetXYZ(0)->X = icDtoF(0.60974);
                tags->red->GetXYZ(0)->Y = icDtoF(0.31111);
                tags->red->GetXYZ(0)->Z = icDtoF(0.01947);

                // Attach green XYZ primary to profile
                tags->green = new  CIccTagXYZ();
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

    void createParametricCurve(CIccTagParametricCurve* parametricCurve, const ProfileColorSpace colorSpace) {
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
        if (space == cs_ProPhoto || space == cs_sRGB || space == cs_Wide_Gamut_RGB) {
            auto *pCurveParametric = new CIccTagParametricCurve();
            createParametricCurve(pCurveParametric, space);
            pIcc->AttachTag(icSigRedTRCTag, pCurveParametric);
            pIcc->AttachTag(icSigGreenTRCTag, pCurveParametric);
            pIcc->AttachTag(icSigBlueTRCTag, pCurveParametric);
        }

        // These two color spaces use a gamma curve, so attach one to the profile.
        if (space == cs_Adobe_RGB_1998 || space == cs_Linear_Normalized_XYZ) {
            auto *pCurveGamma = new CIccTagCurve();
            pCurveGamma->SetGamma(space == cs_Adobe_RGB_1998 ? (563.0f / 256.0f) : 1.0f);

            pIcc->AttachTag(icSigRedTRCTag, pCurveGamma);
            pIcc->AttachTag(icSigGreenTRCTag, pCurveGamma);
            pIcc->AttachTag(icSigBlueTRCTag, pCurveGamma);

        }

        return pIcc;
    }

    CIccProfile *CreateIccProfile::createSpecProfile(ProfileColorSpace base_space,
                                                     float *matrix, int num_in,
                                                     int num_out,
                                                     bool ignore_base_channels,
                                                     const float *inv_matrix) {
        int num_chan = !ignore_base_channels ? num_in : num_in + 3;

        auto pIcc = new CIccProfile();
        CIccTagMultiLocalizedUnicode *pText;

        // Set up the profile header
        pIcc->InitHeader();
        pIcc->m_Header.deviceClass = icSigDisplayClass;
        pIcc->m_Header.version = icVersionNumberV5_1;
        pIcc->m_Header.flags |= icEmbeddedProfileTrue;
        pIcc->m_Header.colorSpace =
                icNColorSpaceSig(icSigNChannelData, (num_chan & 0xffff));
        pIcc->m_Header.pcs = static_cast<icColorSpaceSignature>(0);
        pIcc->m_Header.spectralPCS =
                icNColorSpaceSig(icSigReflectanceSpectralData, (num_out & 0xffff));

        // setup spectral range by number of rows in matrix (num_out)
        switch (num_out) {
            case 36:
                pIcc->m_Header.spectralRange.start = icFtoF16(380.0f);
                pIcc->m_Header.spectralRange.end = icFtoF16(730.0f);
                pIcc->m_Header.spectralRange.steps = num_out;
                break;
            case 31:
                pIcc->m_Header.spectralRange.start = icFtoF16(400.0f);
                pIcc->m_Header.spectralRange.end = icFtoF16(700.0f);
                pIcc->m_Header.spectralRange.steps = num_out;
                break;
            case 17:
                pIcc->m_Header.spectralRange.start = icFtoF16(380.0f);
                pIcc->m_Header.spectralRange.end = icFtoF16(720.0f);
                pIcc->m_Header.spectralRange.steps = num_out;
                break;
            case 15:
                pIcc->m_Header.spectralRange.start = icFtoF16(400.0f);
                pIcc->m_Header.spectralRange.end = icFtoF16(700.0f);
                pIcc->m_Header.spectralRange.steps = num_out;
                break;
            default:
                delete pIcc;
                return nullptr;
        }

        // Allocate copyright tag and attach to profile
        pText = new CIccTagMultiLocalizedUnicode();
        pText->SetText("Copyright (C) 2026 BeyondRGB");

        pIcc->AttachTag(icSigCopyrightTag, pText);

        // Allocate media white point tag and attach to profile
        auto *pWhiteTag = new CIccTagFloat32();
        pWhiteTag->SetSize(num_out);
        for (int i = 0; i < num_out; i++) {
            (*pWhiteTag)[i] = 1.0;
        }

        pIcc->AttachTag(icSigSpectralWhitePointTag, pWhiteTag);

        // Allocate profile description tag and attach to profile
        pText = new CIccTagMultiLocalizedUnicode();
        pText->SetText("BeyondRGB to spectra");

        pIcc->AttachTag(icSigProfileDescriptionTag, pText);

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

        pIcc->AttachTag(icSigSpectralViewingConditionsTag, pCond);

        // Populate DToB3 tag and attach to profile
        auto *pMulti = new CIccTagMultiProcessElement(num_chan, num_out);

        if (ignore_base_channels) {
            // if ignoring the base profile channels, then just add a matrix element
            // with zero columns for the base channels and the provided data and no
            // curves

            auto *pMatrix = new CIccMpeMatrix();
            pMatrix->SetSize(num_chan, num_out);
            icFloatNumber *pData = pMatrix->GetMatrix();
            icFloatNumber *pRow = matrix;

            for (int r = 0; r < num_out; r++) {
                memset(
                    pData, 0,
                    3 * sizeof(icFloatNumber)); // zero out the base profile columns
                pData += 3;
                for (int c = 0; c < num_in; c++) {
                    *pData++ = *pRow++;
                }
            }

            pMulti->Attach(pMatrix);
        } else {
            auto *pCurves = new CIccMpeCurveSet(num_chan);
            CIccSegmentedCurve *pCurve;
            CIccFormulaCurveSegment *pSegment;
            icFloatNumber zeroClip[4] = {1.00000f, 0.00000f, 0.00000f, 0.0000f};

            int i;

            switch (base_space) {
                case cs_Adobe_RGB_1998:
                    pCurve = new CIccSegmentedCurve();

                    // clip the shadows to 0.0
                    pSegment = new CIccFormulaCurveSegment(icMinFloat32Number, 0.0f);
                    pSegment->SetFunction(0, 4, zeroClip);
                    pCurve->Insert(pSegment);

                    // then apply a gamma 2.2 curve with no offset
                    pSegment = new CIccFormulaCurveSegment(0.0f, icMaxFloat32Number); {
                        icFloatNumber params[4] = {
                            563.0f / 256.0f, 1.00000f, 0.00000f,
                            0.0000f
                        };
                        pSegment->SetFunction(0, 4, params);
                    }
                    pCurve->Insert(pSegment);

                    // share the curve for all 3 base channels since they are the same
                    for (i = 0; i < 3; i++) {
                        pCurves->SetCurve(i, pCurve);
                    }

                    // The rest of the channels are assumed to be linear, so set them up
                    // with a simple gamma 1.0 curve with no clipping
                    pCurve = new CIccSegmentedCurve();
                    pSegment = new CIccFormulaCurveSegment(icMinFloat32Number,
                                                           icMaxFloat32Number); {
                        icFloatNumber params[4] = {
                            1.0000f, 1.00000f, 0.00000f,
                            0.0000f
                        };
                        pSegment->SetFunction(0, 4, params);
                    }
                    pCurve->Insert(pSegment);

                    // share the same curve for all non-base channels
                    for (; i < num_chan; i++) {
                        pCurves->SetCurve(i, pCurve);
                    }

                    pMulti->Attach(pCurves);
                    break;

                case cs_ProPhoto:
                    pCurve = new CIccSegmentedCurve();

                    // clip the shadows to 0.0
                    pSegment = new CIccFormulaCurveSegment(icMinFloat32Number, 0.0f);
                    pSegment->SetFunction(0, 4, zeroClip);
                    pCurve->Insert(pSegment);

                    // linear from 0 to 0.13 with slope of 0.0625 (1/16) to avoid a
                    // hard cut off at 0.0, then apply a gamma 1.8 curve with no offset
                    pSegment = new CIccFormulaCurveSegment(0.0f, 0.03125f); {
                        icFloatNumber params[4] = {1.0f, 0.0625f, 0.00000f, 0.0000f};
                        pSegment->SetFunction(0, 4, params);
                    }
                    pCurve->Insert(pSegment);

                    // then apply a gamma 1.8 curve with no offset
                    pSegment =
                            new CIccFormulaCurveSegment(0.03125f, icMaxFloat32Number); {
                        icFloatNumber params[4] = {1.8f, 1.00000f, 0.00000f, 0.0000f};
                        pSegment->SetFunction(0, 4, params);
                    }
                    pCurve->Insert(pSegment);

                    // share the curve for all 3 base channels since they are the same
                    for (i = 0; i < 3; i++) {
                        pCurves->SetCurve(i, pCurve);
                    }

                    // The rest of the channels are assumed to be linear, so set them up
                    // with a simple gamma 1.0 curve with no clipping
                    pCurve = new CIccSegmentedCurve();
                    pSegment = new CIccFormulaCurveSegment(icMinFloat32Number,
                                                           icMaxFloat32Number); {
                        icFloatNumber params[4] = {1.0, 1.0, 0.0, 0.0};
                        pSegment->SetFunction(0, 4, params);
                    }
                    pCurve->Insert(pSegment);

                    // share the same curve for all non-base channels
                    for (; i < num_chan; i++) {
                        pCurves->SetCurve(i, pCurve);
                    }

                    pMulti->Attach(pCurves);
                    break;

                case cs_sRGB:
                    pCurve = new CIccSegmentedCurve();

                    // clip the shadows to 0.0
                    pSegment = new CIccFormulaCurveSegment(icMinFloat32Number, 0.0f);
                    pSegment->SetFunction(0, 4, zeroClip);
                    pCurve->Insert(pSegment);

                    // linear from 0 to 0.04045 with slope of 0.077399380804954 to avoid
                    // a hard cut off at 0.0, then apply a gamma 2.4 curve with no
                    // offset
                    pSegment = new CIccFormulaCurveSegment(0.0f, 0.04045f); {
                        icFloatNumber params[4] = {
                            1.0f, 0.077399380804954, 0.00000f,
                            0.0000f
                        };
                        pSegment->SetFunction(0, 4, params);
                    }
                    pCurve->Insert(pSegment);

                    // then apply a gamma 2.4 curve with slight offset
                    pSegment =
                            new CIccFormulaCurveSegment(0.04045f, icMaxFloat32Number); {
                        icFloatNumber params[4] = {
                            2.4f, 0.947867298578199,
                            0.0521327014218010, 0.0000f
                        };
                        pSegment->SetFunction(0, 4, params);
                    }
                    pCurve->Insert(pSegment);

                    // share the curve for all 3 base channels since they are the same
                    for (i = 0; i < 3; i++) {
                        pCurves->SetCurve(i, pCurve);
                    }

                    // The rest of the channels are assumed to be linear, so set them up
                    // with a simple gamma 1.0 curve with no clipping
                    pCurve = new CIccSegmentedCurve();
                    pSegment = new CIccFormulaCurveSegment(icMinFloat32Number,
                                                           icMaxFloat32Number); {
                        icFloatNumber params[4] = {1.0f, 1.0f, 0.0f, 0.0f};
                        pSegment->SetFunction(0, 4, params);
                    }
                    pCurve->Insert(pSegment);

                    // share the same curve for all non-base channels
                    for (; i < num_chan; i++) {
                        pCurves->SetCurve(i, pCurve);
                    }

                    pMulti->Attach(pCurves);
                    break;

                case cs_Wide_Gamut_RGB:
                    pCurve = new CIccSegmentedCurve();

                    // clip the shadows to 0.0
                    pSegment = new CIccFormulaCurveSegment(icMinFloat32Number, 0.0f);
                    pSegment->SetFunction(0, 4, zeroClip);
                    pCurve->Insert(pSegment);

                    // then apply a gamma 2.2 curve with no offset
                    pSegment = new CIccFormulaCurveSegment(0.0f, icMaxFloat32Number); {
                        icFloatNumber params[4] = {
                            563.0f / 256.0f, 1.00000f, 0.00000f,
                            0.0000f
                        };
                        pSegment->SetFunction(0, 4, params);
                    }
                    pCurve->Insert(pSegment);

                    // share the curve for all 3 base channels since they are the same
                    for (i = 0; i < 3; i++) {
                        pCurves->SetCurve(i, pCurve);
                    }

                    // The rest of the channels are assumed to be linear, so set them up
                    // with a simple gamma 1.0 curve with no clipping
                    pCurve = new CIccSegmentedCurve();
                    pSegment = new CIccFormulaCurveSegment(icMinFloat32Number,
                                                           icMaxFloat32Number); {
                        icFloatNumber params[4] = {1.0f, 1.0f, 0.0f, 0.0f};
                        pSegment->SetFunction(0, 4, params);
                    }
                    pCurve->Insert(pSegment);

                    // share the same curve for all non-base channels
                    for (; i < num_chan; i++) {
                        pCurves->SetCurve(i, pCurve);
                    }

                    pMulti->Attach(pCurves);
                    break;

                case cs_Linear_Normalized_XYZ:
                    // No need for any 1-d curves since the base channels are linear and
                    // the rest of the channels are assumed to be linear as well
                    break;

                default:
                    delete pCurves;
                    delete pMulti;
                    delete pIcc;
                    return nullptr;
            }

            // allocate a matrix element to go after the curves in the MPE and
            // populate with the provided matrix data
            CIccMpeMatrix *pMatrix = new CIccMpeMatrix();
            pMatrix->SetSize(num_chan, num_out);

            icFloatNumber *pData = pMatrix->GetMatrix();
            int size_matrix = num_chan * num_out;
            for (int i = 0; i < size_matrix; i++) {
                pData[i] = matrix[i];
            }

            // add matrix to MPE
            pMulti->Attach(pMatrix);
        }

        pIcc->AttachTag(icSigDToB3Tag, pMulti);

        if (inv_matrix) {
            int i;

            // also add the inverse matrix as a BToD3 tag
            auto *pMultiInv = new CIccTagMultiProcessElement(num_out, num_chan);
            auto *pMatrixInv = new CIccMpeMatrix();
            pMatrixInv->SetSize(num_out, num_chan);
            icFloatNumber *pData = pMatrixInv->GetMatrix();
            int size_matrix = num_chan * num_out;
            for (i = 0; i < size_matrix; i++)
                pData[i] = inv_matrix[i];
            pMultiInv->Attach(pMatrixInv);

            auto *pCurves = new CIccMpeCurveSet(num_chan);
            CIccSegmentedCurve *pCurve;
            CIccFormulaCurveSegment *pSegment;
            icFloatNumber zeroClip[4] = {1.00000f, 0.00000f, 0.00000f, 0.0000f};

            switch (base_space) {
                case cs_Adobe_RGB_1998:
                    pCurve = new CIccSegmentedCurve();

                    // clip the shadows to 0.0
                    pSegment = new CIccFormulaCurveSegment(icMinFloat32Number, 0.0f);
                    pSegment->SetFunction(0, 4, zeroClip);
                    pCurve->Insert(pSegment);

                    // then apply a gamma 1/2.2 curve with no offset
                    pSegment = new CIccFormulaCurveSegment(0.0f, icMaxFloat32Number); {
                        icFloatNumber params[4] = {
                            256.0 / 563.0f, 1.00000f, 0.00000f,
                            0.0000f
                        };
                        pSegment->SetFunction(0, 4, params);
                    }
                    pCurve->Insert(pSegment);

                    // share the curve for all 3 base channels since they are the same
                    for (i = 0; i < 3; i++) {
                        pCurves->SetCurve(i, pCurve);
                    }

                    // The rest of the channels are assumed to be linear, so set them up
                    // with a simple gamma 1.0 curve with no clipping
                    pCurve = new CIccSegmentedCurve();
                    pSegment = new CIccFormulaCurveSegment(icMinFloat32Number,
                                                           icMaxFloat32Number); {
                        icFloatNumber params[4] = {
                            1.0000f, 1.00000f, 0.00000f,
                            0.0000f
                        };
                        pSegment->SetFunction(0, 4, params);
                    }
                    pCurve->Insert(pSegment);

                    // share the same curve for all non-base channels
                    for (; i < num_chan; i++) {
                        pCurves->SetCurve(i, pCurve);
                    }

                    pMultiInv->Attach(pCurves);
                    break;

                case cs_ProPhoto:
                    pCurve = new CIccSegmentedCurve();

                    // clip the shadows to 0.0
                    pSegment = new CIccFormulaCurveSegment(icMinFloat32Number, 0.0f);
                    pSegment->SetFunction(0, 4, zeroClip);
                    pCurve->Insert(pSegment);

                    // linear from 0 to 0.001953125 with slope of 16 to avoid a
                    // hard cut off at 0.0
                    pSegment = new CIccFormulaCurveSegment(0.0f, 0.001953125f); {
                        icFloatNumber params[4] = {1.0f, 16.0f, 0.00000f, 0.0000f};
                        pSegment->SetFunction(0, 4, params);
                    }
                    pCurve->Insert(pSegment);

                    // then apply a gamma 1/1.8 curve with no offset
                    pSegment =
                            new CIccFormulaCurveSegment(0.001953125f, icMaxFloat32Number); {
                        icFloatNumber params[4] = {
                            0.555557250977f, 1.00000f, 0.00000f,
                            0.0000f
                        };
                        pSegment->SetFunction(0, 4, params);
                    }
                    pCurve->Insert(pSegment);

                    // share the curve for all 3 base channels since they are the same
                    for (i = 0; i < 3; i++) {
                        pCurves->SetCurve(i, pCurve);
                    }

                    // The rest of the channels are assumed to be linear, so set them up
                    // with a simple gamma 1.0 curve with no clipping
                    pCurve = new CIccSegmentedCurve();
                    pSegment = new CIccFormulaCurveSegment(icMinFloat32Number,
                                                           icMaxFloat32Number); {
                        icFloatNumber params[4] = {1.0, 1.0, 0.0, 0.0};
                        pSegment->SetFunction(0, 4, params);
                    }
                    pCurve->Insert(pSegment);

                    // share the same curve for all non-base channels
                    for (; i < num_chan; i++) {
                        pCurves->SetCurve(i, pCurve);
                    }

                    pMulti->Attach(pCurves);
                    break;

                case cs_sRGB:
                    pCurve = new CIccSegmentedCurve();

                    // clip the shadows to 0.0
                    pSegment = new CIccFormulaCurveSegment(icMinFloat32Number, 0.0f);
                    pSegment->SetFunction(0, 4, zeroClip);
                    pCurve->Insert(pSegment);

                    // linear from 0 to 0.0031308 with slope of 12.92 to avoid a
                    // hard cut off at 0.0
                    pSegment = new CIccFormulaCurveSegment(0.0f, 0.0031308f); {
                        icFloatNumber params[4] = {1.0f, 12.92f, 0.00000f, 0.0000f};
                        pSegment->SetFunction(0, 4, params);
                    }
                    pCurve->Insert(pSegment);

                    // then apply a gamma 1/2.4 curve with slight offset
                    pSegment =
                            new CIccFormulaCurveSegment(0.0031308f, icMaxFloat32Number); {
                        icFloatNumber params[4] = {1.0f / 2.4f, 1.055f, 0.0f, -0.055f};
                        pSegment->SetFunction(0, 4, params);
                    }
                    pCurve->Insert(pSegment);

                    // share the curve for all 3 base channels since they are the same
                    for (i = 0; i < 3; i++) {
                        pCurves->SetCurve(i, pCurve);
                    }

                    // The rest of the channels are assumed to be linear, so set them up
                    // with a simple gamma 1.0 curve with no clipping
                    pCurve = new CIccSegmentedCurve();
                    pSegment = new CIccFormulaCurveSegment(icMinFloat32Number,
                                                           icMaxFloat32Number); {
                        icFloatNumber params[4] = {1.0f, 1.0f, 0.0f, 0.0f};
                        pSegment->SetFunction(0, 4, params);
                    }
                    pCurve->Insert(pSegment);

                    // share the same curve for all non-base channels
                    for (; i < num_chan; i++) {
                        pCurves->SetCurve(i, pCurve);
                    }

                    pMulti->Attach(pCurves);
                    break;

                case cs_Wide_Gamut_RGB:
                    pCurve = new CIccSegmentedCurve();

                    // clip the shadows to 0.0
                    pSegment = new CIccFormulaCurveSegment(icMinFloat32Number, 0.0f);
                    pSegment->SetFunction(0, 4, zeroClip);
                    pCurve->Insert(pSegment);

                    // then apply a gamma 1/2.2 curve with no offset
                    pSegment = new CIccFormulaCurveSegment(0.0f, icMaxFloat32Number); {
                        icFloatNumber params[4] = {
                            256.0 / 563.0f, 1.00000f, 0.00000f,
                            0.0000f
                        };
                        pSegment->SetFunction(0, 4, params);
                    }
                    pCurve->Insert(pSegment);

                    // share the curve for all 3 base channels since they are the same
                    for (i = 0; i < 3; i++) {
                        pCurves->SetCurve(i, pCurve);
                    }

                    // The rest of the channels are assumed to be linear, so set them up
                    // with a simple gamma 1.0 curve with no clipping
                    pCurve = new CIccSegmentedCurve();
                    pSegment = new CIccFormulaCurveSegment(icMinFloat32Number,
                                                           icMaxFloat32Number); {
                        icFloatNumber params[4] = {1.0f, 1.0f, 0.0f, 0.0f};
                        pSegment->SetFunction(0, 4, params);
                    }
                    pCurve->Insert(pSegment);

                    // share the same curve for all non-base channels
                    for (; i < num_chan; i++) {
                        pCurves->SetCurve(i, pCurve);
                    }

                    pMulti->Attach(pCurves);
                    break;

                case cs_Linear_Normalized_XYZ:
                    // No need for any 1-d curves since the base channels are linear and
                    // the rest of the channels are assumed to be linear as well
                    break;

                default:
                    delete pCurves;
                    delete pMulti;
                    pMultiInv = nullptr;
                    return nullptr;
            }

            if (pMultiInv) {
                pIcc->AttachTag(icSigBToD3Tag, pMultiInv);
            }
        }

        return pIcc;
    }
} // namespace btrgb

#include "CreateIccProfile.hpp"
#include "IccIo.h"
#include "IccTagBasic.h"
#include "IccTagEmbedIcc.h"
#include "IccTagMPE.h"
#include "IccMpeBasic.h"
#include "IccUtil.h"

CreateIccProfile::CreateIccProfile(size_t max_size)
{
  max_profile_size = max_size;
  hybrid_icc = nullptr;
  profile_mem = nullptr;
  profile_size = 0;
}

CreateIccProfile::~CreateIccProfile()
{
  if (hybrid_icc)
    delete hybrid_icc;

  if (profile_mem)
    delete[] profile_mem;
}

bool CreateIccProfile::createHybridProfile(ProfileColorSpace space, float* matrix, int num_in, int num_out, bool hybrid_ignore_base_channels)
{
  if (hybrid_icc)
    return false;

  CIccProfile* rgb_profile = createRgbProfile(space);
  if (!rgb_profile)
    return false;

  CIccProfile* spec_profile = createSpecProfile(space, matrix, num_in, num_out, hybrid_ignore_base_channels);
  if (!spec_profile) {
    delete rgb_profile;
  }

  //Allocate IO tag, attach spec_profile to it, and embed it in rgb profile
  CIccTagEmbeddedProfile* pTag = new CIccTagEmbeddedProfile();
  pTag->SetProfile(spec_profile);
  rgb_profile->AttachTag(icSigEmbeddedV5ProfileTag, pTag);

  hybrid_icc = rgb_profile;

  profile_mem = new unsigned char[max_profile_size];

  if (!profile_mem) {
    delete hybrid_icc;
    return false;
  }

  CIccMemIO io;
  io.Alloc(max_profile_size, true);

  if (!hybrid_icc->Write(&io)) {
    delete[] profile_mem;
    delete hybrid_icc;

    return false;
  }
  profile_size = io.GetLength();

  return true;
}

unsigned char* CreateIccProfile::getProfileMem()
{
  return profile_mem;
}

size_t CreateIccProfile::getProfileSize()
{
  return profile_size;
}

CIccProfile* CreateIccProfile::createRgbProfile(ProfileColorSpace space)
{
  CIccProfilePtr pIcc(new CIccProfile());
  CIccTagCurve* pCurveGamma;
  CIccTagParametricCurve* pCurve;
  CIccTagXYZ* pXYZ;
  CIccTagMultiLocalizedUnicode* pText;
  icFloatNumber* param;

  //Set up the profile header
  pIcc->InitHeader();
  pIcc->m_Header.deviceClass = icSigDisplayClass;
  pIcc->m_Header.version = icVersionNumberV4_3;
  pIcc->m_Header.colorSpace = icSigRgbData;
  pIcc->m_Header.pcs = icSigXYZData;

  //Allocate copyright tag and attach to profile
  pText = new CIccTagMultiLocalizedUnicode();
  pText->SetText("Copyright (C) 2026 BeyondRGB");

  pIcc->AttachTag(icSigCopyrightTag, pText);

  //Allocate media white point tag and attach to profile
  pXYZ = new CIccTagXYZ();
  (*pXYZ)[0].X = icDtoF(0.96420288);
  (*pXYZ)[0].Y = icDtoF(1.00000000);
  (*pXYZ)[0].Z = icDtoF(0.82487488);

  pIcc->AttachTag(icSigMediaWhitePointTag, pXYZ);

  switch (space) {
  case cs_Adobe_RGB_1998:
    //Allocate profile description tag and attach to profile
    pText = new CIccTagMultiLocalizedUnicode();
    pText->SetText("Hybrid aRGB");

    pIcc->AttachTag(icSigProfileDescriptionTag, pText);

    //Allocate Tone Reproduction Curve(s) and attach to profile
    pCurveGamma = new CIccTagCurve();
    pCurveGamma->SetGamma(563.0f / 256.0f);

    pIcc->AttachTag(icSigRedTRCTag, pCurveGamma);
    pIcc->AttachTag(icSigGreenTRCTag, pCurveGamma);
    pIcc->AttachTag(icSigBlueTRCTag, pCurveGamma);

    //Attach red XYZ primary to profile
    pXYZ = new CIccTagXYZ();
    (*pXYZ)[0].X = icDtoF(0.60974);
    (*pXYZ)[0].Y = icDtoF(0.31111);
    (*pXYZ)[0].Z = icDtoF(0.01947);

    pIcc->AttachTag(icSigRedMatrixColumnTag, pXYZ);

    //Attach green XYZ primary to profile
    pXYZ = new CIccTagXYZ();
    (*pXYZ)[0].X = icDtoF(0.20528);
    (*pXYZ)[0].Y = icDtoF(0.62567);
    (*pXYZ)[0].Z = icDtoF(0.06087);

    pIcc->AttachTag(icSigGreenMatrixColumnTag, pXYZ);

    //Attach blue XYZ primary to profile
    pXYZ = new CIccTagXYZ();
    (*pXYZ)[0].X = icDtoF(0.14919);
    (*pXYZ)[0].Y = icDtoF(0.06322);
    (*pXYZ)[0].Z = icDtoF(0.74457);

    pIcc->AttachTag(icSigBlueMatrixColumnTag, pXYZ);
    break;

  case cs_ProPhoto:
    //Allocate profile description tag and attach to profile
    pText = new CIccTagMultiLocalizedUnicode();
    pText->SetText("Hybrid Pro Photo RGB");

    pIcc->AttachTag(icSigProfileDescriptionTag, pText);

    //Allocate Tone Reproduction Curve(s) and attach to profile
    pCurve = new CIccTagParametricCurve();
    pCurve->SetFunctionType(3);
    param = pCurve->GetParams();
    param[0] = 1.80000;
    param[1] = 1.00000;
    param[2] = 0.00000;
    param[3] = 0.0625;
    param[4] = 0.03125;

    pIcc->AttachTag(icSigRedTRCTag, pCurve);
    pIcc->AttachTag(icSigGreenTRCTag, pCurve);
    pIcc->AttachTag(icSigBlueTRCTag, pCurve);

    //Attach red XYZ primary to profile
    pXYZ = new CIccTagXYZ();
    (*pXYZ)[0].X = icDtoF(0.398818969727);
    (*pXYZ)[0].Y = icDtoF(0.144012451172);
    (*pXYZ)[0].Z = icDtoF(0.00000);

    pIcc->AttachTag(icSigRedMatrixColumnTag, pXYZ);

    //Attach green XYZ primary to profile
    pXYZ = new CIccTagXYZ();
    (*pXYZ)[0].X = icDtoF(0.067596435547);
    (*pXYZ)[0].Y = icDtoF(0.355926513672);
    (*pXYZ)[0].Z = icDtoF(0.00000000);

    pIcc->AttachTag(icSigGreenMatrixColumnTag, pXYZ);

    //Attach blue XYZ primary to profile
    pXYZ = new CIccTagXYZ();
    (*pXYZ)[0].X = icDtoF(0.015670776367);
    (*pXYZ)[0].Y = icDtoF(0.000518798828);
    (*pXYZ)[0].Z = icDtoF(0.412048339844);

    pIcc->AttachTag(icSigBlueMatrixColumnTag, pXYZ);
    break;

  case cs_sRGB:
    //Allocate profile description tag and attach to profile
    pText = new CIccTagMultiLocalizedUnicode();
    pText->SetText("Hybrid aRGB");

    pIcc->AttachTag(icSigProfileDescriptionTag, pText);

    //Allocate Tone Reproduction Curve(s) and attach to profile
    pCurve = new CIccTagParametricCurve();
    pCurve->SetFunctionType(3);
    param = pCurve->GetParams();
    param[0] = 2.4;                  //gamma
    param[1] = 0.947867298578199;    //a
    param[2] = 0.0521327014218010;   //b
    param[3] = 0.077399380804954;    //c
    param[4] = 0.04045;              //d

    pIcc->AttachTag(icSigRedTRCTag, pCurve);
    pIcc->AttachTag(icSigGreenTRCTag, pCurve);
    pIcc->AttachTag(icSigBlueTRCTag, pCurve);

    //Attach red XYZ primary to profile
    pXYZ = new CIccTagXYZ();
    (*pXYZ)[0].X = icDtoF(0.436065673828);
    (*pXYZ)[0].Y = icDtoF(0.222488403320);
    (*pXYZ)[0].Z = icDtoF(0.013916015625);

    pIcc->AttachTag(icSigRedMatrixColumnTag, pXYZ);

    //Attach green XYZ primary to profile
    pXYZ = new CIccTagXYZ();
    (*pXYZ)[0].X = icDtoF(0.385147094727);
    (*pXYZ)[0].Y = icDtoF(0.716873168945);
    (*pXYZ)[0].Z = icDtoF(0.097076416016);

    pIcc->AttachTag(icSigGreenMatrixColumnTag, pXYZ);

    //Attach blue XYZ primary to profile
    pXYZ = new CIccTagXYZ();
    (*pXYZ)[0].X = icDtoF(0.143066406250);
    (*pXYZ)[0].Y = icDtoF(0.060607910156);
    (*pXYZ)[0].Z = icDtoF(0.714096069336);

    pIcc->AttachTag(icSigBlueMatrixColumnTag, pXYZ);
    break;

  case cs_Wide_Gamut_RGB:
    //Allocate profile description tag and attach to profile
    pText = new CIccTagMultiLocalizedUnicode();
    pText->SetText("Hybrid Wide Gamut RGB");

    pIcc->AttachTag(icSigProfileDescriptionTag, pText);

    //Allocate Tone Reproduction Curve(s) and attach to profile
    pCurve = new CIccTagParametricCurve();
    pCurve->SetFunctionType(3);
    param = pCurve->GetParams();
    param[0] = 3.0;
    param[1] = 1.121991404;
    param[2] = 0.137931034;
    param[3] = 0.144084508;
    param[4] = 0.061467064;

    pIcc->AttachTag(icSigRedTRCTag, pCurve);
    pIcc->AttachTag(icSigGreenTRCTag, pCurve);
    pIcc->AttachTag(icSigBlueTRCTag, pCurve);

    //Attach red XYZ primary to profile
    pXYZ = new CIccTagXYZ();
    (*pXYZ)[0].X = icDtoF(0.96420288);
    (*pXYZ)[0].Y = icDtoF(0.00000000);
    (*pXYZ)[0].Z = icDtoF(0.00000000);

    pIcc->AttachTag(icSigRedMatrixColumnTag, pXYZ);

    //Attach green XYZ primary to profile
    pXYZ = new CIccTagXYZ();
    (*pXYZ)[0].X = icDtoF(0.00000000);
    (*pXYZ)[0].Y = icDtoF(1.00000000);
    (*pXYZ)[0].Z = icDtoF(0.00000000);

    pIcc->AttachTag(icSigGreenMatrixColumnTag, pXYZ);

    //Attach blue XYZ primary to profile
    pXYZ = new CIccTagXYZ();
    (*pXYZ)[0].X = icDtoF(0.00000000);
    (*pXYZ)[0].Y = icDtoF(0.00000000);
    (*pXYZ)[0].Z = icDtoF(0.82487488);

    pIcc->AttachTag(icSigBlueMatrixColumnTag, pXYZ);
    break;

  case cs_Linear_Wide_Gamut_RGB:
    //Allocate profile description tag and attach to profile
    pText = new CIccTagMultiLocalizedUnicode();
    pText->SetText("Hybrid Wide Gamut RGB");

    pIcc->AttachTag(icSigProfileDescriptionTag, pText);

    //Allocate Tone Reproduction Curve(s) and attach to profile
    pCurveGamma = new CIccTagCurve();
    pCurveGamma->SetGamma(1.0f);

    pIcc->AttachTag(icSigRedTRCTag, pCurveGamma);
    pIcc->AttachTag(icSigGreenTRCTag, pCurveGamma);
    pIcc->AttachTag(icSigBlueTRCTag, pCurveGamma);

    //Attach red XYZ primary to profile
    pXYZ = new CIccTagXYZ();
    (*pXYZ)[0].X = icDtoF(0.96420288);
    (*pXYZ)[0].Y = icDtoF(0.00000000);
    (*pXYZ)[0].Z = icDtoF(0.00000000);

    pIcc->AttachTag(icSigRedMatrixColumnTag, pXYZ);

    //Attach green XYZ primary to profile
    pXYZ = new CIccTagXYZ();
    (*pXYZ)[0].X = icDtoF(0.00000000);
    (*pXYZ)[0].Y = icDtoF(1.00000000);
    (*pXYZ)[0].Z = icDtoF(0.00000000);

    pIcc->AttachTag(icSigGreenMatrixColumnTag, pXYZ);

    //Attach blue XYZ primary to profile
    pXYZ = new CIccTagXYZ();
    (*pXYZ)[0].X = icDtoF(0.00000000);
    (*pXYZ)[0].Y = icDtoF(0.00000000);
    (*pXYZ)[0].Z = icDtoF(0.82487488);

    pIcc->AttachTag(icSigBlueMatrixColumnTag, pXYZ);
    break;

  default:
    delete pIcc;
    return nullptr;
  }

  return pIcc;
}

CIccProfile* CreateIccProfile::createSpecProfile(ProfileColorSpace space, float *matrix, int num_in, int num_out, bool ignore_base)
{
  int num_chan = ignore_base ? num_in : num_in + 3;

  CIccProfilePtr pIcc(new CIccProfile());
  CIccTagMultiLocalizedUnicode* pText;

  //Set up the profile header
  pIcc->InitHeader();
  pIcc->m_Header.deviceClass = icSigDisplayClass;
  pIcc->m_Header.version = icVersionNumberV5_1;
  pIcc->m_Header.flags |= icEmbeddedProfileTrue;
  pIcc->m_Header.colorSpace = icNColorSpaceSig(icSigNChannelData, (num_chan&0xffff));
  pIcc->m_Header.spectralPCS = icNColorSpaceSig(icSigReflectanceSpectralData, (num_out&0xffff));

  //setup spectral range by number of rows in matrix (num_out)
  switch (num_out)
  {
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

  //Allocate copyright tag and attach to profile
  pText = new CIccTagMultiLocalizedUnicode();
  pText->SetText("Copyright (C) 2026 BeyondRGB");

  pIcc->AttachTag(icSigCopyrightTag, pText);

  //Allocate media white point tag and attach to profile
  CIccTagFloat32 *pWhiteTag = new CIccTagFloat32();
  pWhiteTag->SetSize(num_out);
  for (int i = 0; i < num_out; i++) {
    (*pWhiteTag)[i] = 1.0;
  }

  pIcc->AttachTag(icSigSpectralWhitePointTag, pWhiteTag);

  //Allocate profile description tag and attach to profile
  pText = new CIccTagMultiLocalizedUnicode();
  pText->SetText("BeyondRGB to spectra");

  pIcc->AttachTag(icSigProfileDescriptionTag, pText);

  //Allocate spectral viewing conditions and attach to profile
  CIccTagSpectralViewingConditions* pCond = new CIccTagSpectralViewingConditions();

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

  //Populate DToB3 tag and attach to profile
  CIccTagMultiProcessElement* pMulti = new CIccTagMultiProcessElement(num_chan, num_out);

  if (!ignore_base) {
    CIccMpeCurveSet* pCurves = new CIccMpeCurveSet(num_chan);
    CIccSegmentedCurve *pCurve;
    CIccFormulaCurveSegment* pSegment;
    icFloatNumber zeroClip[4] = { 1.00000f, 0.00000f, 0.00000f, 0.0000f };

    int i;

    switch (space) {
    case cs_Adobe_RGB_1998:
      pCurve = new CIccSegmentedCurve();

      //clip the shadows to 0.0
      pSegment = new CIccFormulaCurveSegment(icMinFloat32Number, 0.0f);
      pSegment->SetFunction(0, 4, zeroClip);
      pCurve->Insert(pSegment);

      //then apply a gamma 2.2 curve with no offset
      pSegment = new CIccFormulaCurveSegment(0.0f, icMaxFloat32Number);
      {
        icFloatNumber params[4] = { 563.0f / 256.0f, 1.00000f, 0.00000f, 0.0000f };
        pSegment->SetFunction(0, 4, params);
      }
      pCurve->Insert(pSegment);

      //share the curve for all 3 base channels since they are the same
      for (i = 0; i < 3; i++) {
        pCurves->SetCurve(i, pCurve);
      }

      //The rest of the channels are assumed to be linear, so set them up with a simple gamma 1.0 curve with no clipping
      pCurve = new CIccSegmentedCurve();
      pSegment = new CIccFormulaCurveSegment(icMinFloat32Number, icMaxFloat32Number);
      {
        icFloatNumber params[4] = { 1.0000f, 1.00000f, 0.00000f, 0.0000f };
        pSegment->SetFunction(0, 4, params);
      }
      pCurve->Insert(pSegment);

      //share the same curve for all non-base channels
      for (; i < num_chan; i++) {
        pCurves->SetCurve(i, pCurve);
      }

      pMulti->Attach(pCurves);
      break;

    case cs_ProPhoto:
      pCurve = new CIccSegmentedCurve();

      //clip the shadows to 0.0
      pSegment = new CIccFormulaCurveSegment(icMinFloat32Number, 0.0f);
      pSegment->SetFunction(0, 4, zeroClip);
      pCurve->Insert(pSegment);

      //linear from 0 to 0.13 with slope of 0.0625 (1/16) to avoid a
      //hard cut off at 0.0, then apply a gamma 1.8 curve with no offset
      pSegment = new CIccFormulaCurveSegment(0.0f, 0.03125f);
      {
        icFloatNumber params[4] = { 1.0f, 0.0625f, 0.00000f, 0.0000f };
        pSegment->SetFunction(0, 4, params);
      }
      pCurve->Insert(pSegment);

      //then apply a gamma 1.8 curve with no offset
      pSegment = new CIccFormulaCurveSegment(0.03125f, icMaxFloat32Number);
      {
        icFloatNumber params[4] = { 1.8f, 1.00000f, 0.00000f, 0.0000f };
        pSegment->SetFunction(0, 4, params);
      }
      pCurve->Insert(pSegment);

      //share the curve for all 3 base channels since they are the same
      for (i = 0; i < 3; i++) {
        pCurves->SetCurve(i, pCurve);
      }

      //The rest of the channels are assumed to be linear, so set them up with a simple gamma 1.0 curve with no clipping
      pCurve = new CIccSegmentedCurve();
      pSegment = new CIccFormulaCurveSegment(icMinFloat32Number, icMaxFloat32Number);
      {
        icFloatNumber params[5] = { 1.8, 1.0, 0.0, 0.0625, 0.03125 };
        pSegment->SetFunction(3, 5, params);
      }
      pCurve->Insert(pSegment);

      //share the same curve for all non-base channels
      for (; i < num_chan; i++) {
        pCurves->SetCurve(i, pCurve);
      }

      pMulti->Attach(pCurves);
      break;

    case cs_sRGB:
      pCurve = new CIccSegmentedCurve();

      //clip the shadows to 0.0
      pSegment = new CIccFormulaCurveSegment(icMinFloat32Number, 0.0f);
      pSegment->SetFunction(0, 4, zeroClip);
      pCurve->Insert(pSegment);

      //linear from 0 to 0.04045 with slope of 0.077399380804954 to avoid a
      //hard cut off at 0.0, then apply a gamma 2.4 curve with no offset
      pSegment = new CIccFormulaCurveSegment(0.0f, 0.04045f);
      {
        icFloatNumber params[4] = { 1.0f, 0.077399380804954, 0.00000f, 0.0000f };
        pSegment->SetFunction(0, 4, params);
      }
      pCurve->Insert(pSegment);

      //then apply a gamma 2.4 curve with slight offset
      pSegment = new CIccFormulaCurveSegment(0.04045f, icMaxFloat32Number);
      {
        icFloatNumber params[4] = {2.4f, 0.947867298578199, 0.0521327014218010, 0.0000f };
        pSegment->SetFunction(0, 4, params);
      }
      pCurve->Insert(pSegment);

      //share the curve for all 3 base channels since they are the same
      for (i = 0; i < 3; i++) {
        pCurves->SetCurve(i, pCurve);
      }

      //The rest of the channels are assumed to be linear, so set them up with a simple gamma 1.0 curve with no clipping
      pCurve = new CIccSegmentedCurve();
      pSegment = new CIccFormulaCurveSegment(icMinFloat32Number, icMaxFloat32Number);
      {
        icFloatNumber params[4] = { 1.0f, 1.0f, 0.0f, 0.0f };
        pSegment->SetFunction(0, 4, params);
      }
      pCurve->Insert(pSegment);

      //share the same curve for all non-base channels
      for (; i < num_chan; i++) {
        pCurves->SetCurve(i, pCurve);
      }

      pMulti->Attach(pCurves);
      break;

    case cs_Wide_Gamut_RGB:
      pCurve = new CIccSegmentedCurve();

      //clip the shadows to 0.0
      pSegment = new CIccFormulaCurveSegment(icMinFloat32Number, 0.0f);
      pSegment->SetFunction(0, 4, zeroClip);
      pCurve->Insert(pSegment);

      //linear from 0 to 0.04045 with slope of 0.144084508 to avoid a
      //hard cut off at 0.0, then apply a gamma 2.4 curve with no offset
      pSegment = new CIccFormulaCurveSegment(0.0f, 0.061467064f);
      {
        icFloatNumber params[4] = { 1.0f, 0.144084508, 0.00000f, 0.0000f };
        pSegment->SetFunction(0, 4, params);
      }
      pCurve->Insert(pSegment);

      //then apply a gamma 3.0 curve with no offset
      pSegment = new CIccFormulaCurveSegment(0.061467064f, icMaxFloat32Number);
      {
        icFloatNumber params[4] = { 3.0f, 1.121991404f, 0.137931034, 0.0000f };
        pSegment->SetFunction(0, 4, params);
      }
      pCurve->Insert(pSegment);

      //share the curve for all 3 base channels since they are the same
      for (i = 0; i < 3; i++) {
        pCurves->SetCurve(i, pCurve);
      }

      //The rest of the channels are assumed to be linear, so set them up with a simple gamma 1.0 curve with no clipping
      pCurve = new CIccSegmentedCurve();
      pSegment = new CIccFormulaCurveSegment(icMinFloat32Number, icMaxFloat32Number);
      {
        icFloatNumber params[4] = { 1.0f, 1.0f, 0.0f, 0.0f };
        pSegment->SetFunction(0, 4, params);
      }
      pCurve->Insert(pSegment);

      //share the same curve for all non-base channels
      for (; i < num_chan; i++) {
        pCurves->SetCurve(i, pCurve);
      }

      pMulti->Attach(pCurves);
      break;

    case cs_Linear_Wide_Gamut_RGB:
      //No need for any 1-d curves since the base channels are linear and the
      //rest of the channels are assumed to be linear as well
      break;

    default:
      delete pCurves;
      delete pMulti;
      delete pIcc;
      return nullptr;
    }

    //allocate a matrix element to go after the curves in the MPE and
    //populate with the provided matrix data
    CIccMpeMatrix* pMatrix = new CIccMpeMatrix();
    pMatrix->SetSize(num_chan, num_out);

    icFloatNumber* pData = pMatrix->GetMatrix();
    int size_matrix = num_chan * num_out;
    for (int i = 0; i < size_matrix; i++)
      pData[i] = matrix[i];

    //add matrix to MPE
    pMulti->Attach(pMatrix);
  }
  else { 
    //if ignoring the base profile channels, then just add a matrix element 
    //with zero columns for the base channels and the provided data and no curves
    
    CIccMpeMatrix* pMatrix = new CIccMpeMatrix();
    pMatrix->SetSize(num_chan, num_out);
    icFloatNumber* pData = pMatrix->GetMatrix();
    icFloatNumber* pRow = matrix;

    for (int r = 0; r < num_out; r++) {
      memset(pData, 0, 3 * sizeof(icFloatNumber)); //zero out the base profile columns
      pData += 3;
      for (int c = 0; c < num_in; c++) {
        *pData++ = *pRow++;
      }
    }

    pMulti->Attach(pMatrix);
  }

  pIcc->AttachTag(icSigDToB3Tag, pMulti);

  return pIcc;
}

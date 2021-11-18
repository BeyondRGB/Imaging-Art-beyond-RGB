#ifndef PIPELINE_H
#define PIPELINE_H

#include "image_processing/header/BitDepthScalor.h"
#include "image_processing/header/ChannelSelector.h"
#include "image_processing/header/ColorManagedCalibrator.h"
#include "image_processing/header/DarkCurrentCorrector.h"
#include "image_processing/header/FlatFeildor.h"
#include "image_processing/header/ImageCalibrator.h"
#include "image_processing/header/ImageProcessor.h"
#include "image_processing/header/ImgProcessingComponent.h"
#include "image_processing/header/PixelRegestor.h"
#include "image_processing/header/PreProcessor.h"
#include "image_processing/header/RawImageReader.h"
#include "image_processing/header/SpectralCalibrator.h"

#include "server/comunication_obj.h"

#include <iostream>
using namespace std;

class Pipeline{


private:
	CommunicationObj coms_m;
	int num_m;

	void callback(string msg);
	shared_ptr<ImgProcessingComponent> pipelineSetup();

public:
	Pipeline(CommunicationObj coms_obj, int pipelineNumber);
	void executePipeline();


};


#endif // !PIPELINE_H



	
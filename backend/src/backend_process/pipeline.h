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

#include "server/comunication_obj.hpp"
#include "backend_process.hpp"

#include <iostream>
//using namespace std;

class Pipeline: public BackendProcess{


private:
	CommunicationObj coms_m;
	int num_m;
	static int pipeline_count;

	void callback(std::string msg);
	std::shared_ptr<ImgProcessingComponent> pipelineSetup();

public:
	Pipeline();
	void executePipeline();
	void run() override;


};


#endif // !PIPELINE_H



	
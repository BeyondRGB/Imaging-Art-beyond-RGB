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

/*
Class that process's Images. Image processing includes 
Preprocessing and Calibration
*/
class Pipeline: public BackendProcess{


private:
	CommunicationObj coms_m;
	int num_m;
	static int pipeline_count;

	/*
	A callback function given to all ImgProcessingComponents.
	When a ImagProcessingComponent calls this function it will
	send a message to the frontend
	@param msg: the mesage to send to the frontend
	*/
	void callback(std::string msg);

	/*
	Sets up the pipeline
	This will likely be replaced by the PipelineFactory if and when
	it is constructed
	*/
	std::shared_ptr<ImgProcessingComponent> pipelineSetup();

public:
	Pipeline();
	
	/*
	Override of the run method inherited from BackendProcess
	This gets called by the ProcessManager to start this process 
	*/
	void run() override;


};


#endif // !PIPELINE_H



	
#ifndef PIPELINE_H
#define PIPELINE_H

#include "image_processing/header/BitDepthScaler.h"
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

#include <iostream>
//using namespace std;

#define ASIO_STANDALONE
#define _WEBSOCKETPP_CPP11_THREAD_
#define _WEBSOCKETPP_CPP11_STRICT_ 

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;
// pull out the type of messages sent by our config
typedef server::message_ptr message_ptr;

class Pipeline{


private:
	server* server_m = NULL;
	websocketpp::connection_hdl connectionHandle_m;
	message_ptr msg_m;
	int num_m;

	void callback(std::string msg);
	std::shared_ptr<ImgProcessingComponent> pipelineSetup();

public:
	Pipeline(server* s, websocketpp::connection_hdl hdl, message_ptr msg, int count);
	void executePipeline();


};


#endif // !PIPELINE_H



	
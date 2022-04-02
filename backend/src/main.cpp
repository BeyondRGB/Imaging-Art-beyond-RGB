
#include <iostream>
#include "server/request_server.hpp"
#include "utils/cmd_arg_manager.hpp"
#include "server/globals_siglton.hpp"


//Testing Includes: Remove before submiting PR
void testFunc() {

	//Set up PreProcess components
    std::vector<std::shared_ptr<ImgProcessingComponent>> pre_process_components;
    pre_process_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new ImageReader()));
    //pre_process_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new ChannelSelector()));
    pre_process_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new BitDepthScaler()));
    pre_process_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new FlatFieldor()));
    pre_process_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new PixelRegestor()));
    //Set up Calibration components
    std::vector<std::shared_ptr<ImgProcessingComponent>> calibration_components;
    calibration_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new ColorManagedCalibrator()));
    calibration_components.push_back(static_cast<const std::shared_ptr <ImgProcessingComponent>>(new SpectralCalibrator()));

    std::vector<std::shared_ptr<ImgProcessingComponent>> img_process_components;
    img_process_components.push_back(std::shared_ptr<ImgProcessingComponent>(new PreProcessor(pre_process_components)));
    img_process_components.push_back(std::shared_ptr<ImgProcessingComponent>(new ImageCalibrator(calibration_components)));
    img_process_components.push_back(std::shared_ptr<ImgProcessingComponent>(new ResultsProcessor()));

    std::shared_ptr<ImgProcessingComponent> pipeline = std::shared_ptr<ImgProcessingComponent>(new ImageProcessor(img_process_components));

	std::string list = pipeline->get_component_list();
	std::cout << list << std::endl << std::endl;
	Json list_json(list);
	std::cout << list_json.to_string(true) << std::endl;

}


int main(int argc, char** argv) {
  CMDArgManager::process_args(argc, argv);
	bool test = true; // Set to true if you want to test something and bypass the server
	if (GlobalsSinglton::get_instance()->is_test()) {
		testFunc();
		std::cout << "Exit" << std::endl;
		exit(1);
	}

	std::cout << "BeyondRBG Backend Started: listening on port " << GlobalsSinglton::get_instance()->get_port() << std::endl;
	RequestServer server;
	server.init_server();
	server.start_server();

  //Block till server shuts down
  std::cout << "Exiting.\n";
  delete GlobalsSinglton::get_instance();


}

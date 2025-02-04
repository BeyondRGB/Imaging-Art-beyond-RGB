#ifndef PIPELINE_H
#define PIPELINE_H

// Pipeline Components
#include "image_processing/header/BitDepthScaler.h"
#include "image_processing/header/ChannelSelector.h"
#include "image_processing/header/ColorManagedCalibrator.h"
#include "image_processing/header/FlatFieldor.h"
#include "image_processing/header/ImageCalibrator.h"
#include "image_processing/header/ImageProcessor.h"
#include "image_processing/header/ImgProcessingComponent.h"
#include "image_processing/header/PixelRegestor.h"
#include "image_processing/header/PreProcessor.h"
#include "image_processing/header/ImageReader.h"
#include "image_processing/header/SpectralCalibrator.h"
#include "image_processing/header/ResultsProcessor.h"
#include "image_processing/header/NoiseReduction.h"
#include "image_processing/header/Verification.h"


#include "server/comunication_obj.hpp"
#include "backend_process.hpp"
#include "reference_data/ref_data.hpp"
#include "image_processing/results/calibration_results.hpp"
#include "reference_data/ref_data.hpp"
#include "reference_data/color_patch.hpp"
#include "utils/general_utils.hpp"

#include <filesystem>
#include <iostream>
#include <ctime>

/*
Class that process's Images. Image processing includes
Preprocessing and Calibration
*/
class Pipeline: public BackendProcess{

	enum DataKey {
		ART,
		WHITE,
		DARK,
		IMAGES,
		ReferenceData,
		StandardObserver,
		Illuminants,
		TargetLocation,
		VerificationLocation,
		TARGET_IMG,
		OUT_FILE_NAME
	};
	/**
	* Maps enum values to a string
	*/
	const std::string key_map[11] = {
		"art",
		"white",
		"dark",
		"images",
		"refData",
		"standardObserver",
		"illuminants",
		"targetLocation",
		"verificationLocation",
		"target",
		"outputFileName"
	};


private:
	bool should_verify = false; // Assume there is no verification data

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

	/**
	 * @brief Initialize the ArtObject. 
	 * This will populate the art object with TargetData and the initial images it will contain
	 * 
	 * @param art_obj pointer to the ArtObject to initialize
	 * @return true if successfule
	 * @return false if falure
	 */
	bool init_art_obj(btrgb::ArtObject* art_obj);

	/**
	 * @brief Initialize the initial General Info for this Prossessing run
	 * 
	 * @param art_obj the ArtObject that contains the General Info results object.
	 */
	void init_general_info(btrgb::ArtObject* art_obj);

	/**
	 * @brief Get the illuminant type object from the request data provided by frontend
	 * 
	 * @return IlluminantType 
	 */
	IlluminantType get_illuminant_type(Json target_data);

	/**
	 * @brief Get the observer type object from the request data provided by fronend
	 * 
	 * @return ObserverType 
	 */
	ObserverType get_observer_type(Json target_data);

	/**
	 * @brief Get the ref file from the request data provided by the frontend
	 * 
	 * @return std::string 
	 */
	std::string get_ref_file(Json target_data);

	/**
	 * @brief Get the output directory.
	 * This will uses the output directory from the request data provided by the frontend as a base
	 * and create a new folder within that directory in the form
	 * 		BTRGB_<DATE>_<TIME>
	 * It then returns the new dir as a string.
	 * 
	 * @return std::string 
	 */
	std::string get_output_directory(std::string artImage);

	TargetData build_target_data(Json target_json);

	void init_verification(btrgb::ArtObject* images);
	bool verify_targets(btrgb::ArtObject *images);

	/**
	* @brief get the sharpen type
	* Gets a string corresponding to the level of sharpening needed
	* N for disabled or L M H
	* @return std::string
	*/
	std::string get_sharpen_type();


	/**
	* @brief get the registration
	* Gets a string corresponding to the level of registration needed
	* L M H | M is default
	* @return std::string
	*/
	std::string get_registration_type();


public:
	Pipeline(std::string name) : BackendProcess(name) {};

	/*
	Override of the run method inherited from BackendProcess
	This gets called by the ProcessManager to start this process
	*/
	void run() override;

};


#endif // !PIPELINE_H

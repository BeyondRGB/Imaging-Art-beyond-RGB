#include "library.h"
#include "src/image_processing/header/PreProcessor.h"
#include "src/image_processing/header/RawImageReader.h"
#include "src/image_processing/header/ImageProcessor.h"
#include "src/image_processing/header/ChannelSelector.h"
#include "src/image_processing/header/BitDepthScalor.h"
#include "src/image_processing/header/DarkCurrentCorrector.h"
#include "src/image_processing/header/FlatFeildor.h"
#include "src/image_processing/header/PixelRegestor.h"
#include "src/image_processing/header/ImageCalibrator.h"
#include "src/image_processing/ColorManagedCalibrator.h"
#include "src/image_processing/SpectralCalibrator.h"

#include <iostream>


void hello() {
    std::cout << "Hello From the Library!" << std::endl;
}

void testCallBack(string str){
    std::cout << "TestCallback: " << str << std::endl;
}


void process(){
    //TODO use factory patter to build this dynamically instead of hard coding
    //Set up PreProcess components
    vector<shared_ptr<ImgProcessingComponent>> pre_process_components;
    pre_process_components.push_back(static_cast<const shared_ptr <ImgProcessingComponent>>(new RawImageReader()));
    pre_process_components.push_back(static_cast<const shared_ptr <ImgProcessingComponent>>(new ChannelSelector()));
    pre_process_components.push_back(static_cast<const shared_ptr <ImgProcessingComponent>>(new BitDepthScalor()));
    pre_process_components.push_back(static_cast<const shared_ptr <ImgProcessingComponent>>(new DarkCurrentCorrector()));
    pre_process_components.push_back(static_cast<const shared_ptr <ImgProcessingComponent>>(new FlatFeildor()));
    pre_process_components.push_back(static_cast<const shared_ptr <ImgProcessingComponent>>(new PixelRegestor()));
    //Set up Calibration components
    vector<shared_ptr<ImgProcessingComponent>> calibration_components;
    calibration_components.push_back(static_cast<const shared_ptr <ImgProcessingComponent>>(new ColorManagedCalibrator()));
    calibration_components.push_back(static_cast<const shared_ptr <ImgProcessingComponent>>(new SpectralCalibrator()));

    auto *process = new PreProcessor(pre_process_components);
    auto *calibrator = new ImageCalibrator(calibration_components);

    vector<shared_ptr<ImgProcessingComponent>> img_process_components;
    img_process_components.push_back(shared_ptr<ImgProcessingComponent>(process));
    img_process_components.push_back(shared_ptr<ImgProcessingComponent>(calibrator));

    auto *processor = new ImageProcessor(img_process_components);


    processor->execute(testCallBack);
}
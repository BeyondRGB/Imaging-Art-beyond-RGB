#include "library.h"
#include "src/image_processing/PreProcessor.h"
#include "src/image_processing/RawImageReader.h"

#include <iostream>


void hello() {
    std::cout << "Hello From the Library!" << std::endl;
}

void process(){
    vector<shared_ptr<ImgProcessingComponent>> components;
    components.push_back(static_cast<const shared_ptr <ImgProcessingComponent>>(new RawImageReader()));

    PreProcessor process = PreProcessor(components,1);
    process.execute();
}
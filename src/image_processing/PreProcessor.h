//
// Created by ThinkPad41 on 10/10/2021.
//

#ifndef BEYOND_RGB_BACKEND_PREPROCESSOR_H
#define BEYOND_RGB_BACKEND_PREPROCESSOR_H


#include "ImgProcessingComponent.h"
using namespace std;
#include <memory>
#include <vector>

class PreProcessor: public ImgProcessingComponent {

private:

    int num_components;
    vector<shared_ptr<ImgProcessingComponent>> components;


public:
    PreProcessor(ImgProcessingComponent* components[], int num_components);
    void execute();

    PreProcessor(const vector<shared_ptr<ImgProcessingComponent>>& components, int num_components);
};


#endif //BEYOND_RGB_BACKEND_PREPROCESSOR_H

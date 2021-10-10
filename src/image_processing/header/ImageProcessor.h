//
// Created by ThinkPad41 on 10/10/2021.
//

#ifndef BEYOND_RGB_BACKEND_IMAGEPROCESSOR_H
#define BEYOND_RGB_BACKEND_IMAGEPROCESSOR_H


#include "ImgProcessingComponent.h"

class ImageProcessor: ImgProcessingComponent{
private:
    vector<shared_ptr<ImgProcessingComponent>> components;
public:
    ImageProcessor(const vector<shared_ptr<ImgProcessingComponent>>& components);
    void execute(callback func) override;
};


#endif //BEYOND_RGB_BACKEND_IMAGEPROCESSOR_H

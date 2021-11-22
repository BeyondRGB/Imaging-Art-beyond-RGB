//
// Created by ThinkPad41 on 10/10/2021.
//

#ifndef BEYOND_RGB_BACKEND_IMAGEPROCESSOR_H
#define BEYOND_RGB_BACKEND_IMAGEPROCESSOR_H


#include "ImgProcessingComponent.h"

class ImageProcessor: public ImgProcessingComponent{
private:
    vector<shared_ptr<ImgProcessingComponent>> components;
    CallBackFunction callback_func;
public:
    ImageProcessor(const vector<shared_ptr<ImgProcessingComponent>>& components);
    void execute(CallBackFunction func, ArtObject* images) override;
    void my_callback(string str);
};


#endif //BEYOND_RGB_BACKEND_IMAGEPROCESSOR_H

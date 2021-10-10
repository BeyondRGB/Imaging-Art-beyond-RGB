//
// Created by ThinkPad41 on 10/10/2021.
//

#ifndef BEYOND_RGB_BACKEND_PREPROCESSOR_H
#define BEYOND_RGB_BACKEND_PREPROCESSOR_H


#include "ImgProcessingComponent.h"


class PreProcessor: public ImgProcessingComponent {

private:

    int num_components;
    vector<shared_ptr<ImgProcessingComponent>> components;


public:
    explicit PreProcessor(const vector<shared_ptr<ImgProcessingComponent>>& components);
    void execute(callback func) override;

};


#endif //BEYOND_RGB_BACKEND_PREPROCESSOR_H

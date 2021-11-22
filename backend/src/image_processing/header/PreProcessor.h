//
// Created by ThinkPad41 on 10/10/2021.
//

#ifndef BEYOND_RGB_BACKEND_PREPROCESSOR_H
#define BEYOND_RGB_BACKEND_PREPROCESSOR_H


#include "ImgProcessingComponent.h"


class PreProcessor: public ImgProcessingComponent {

private:
    vector<shared_ptr<ImgProcessingComponent>> components;
    CallBackFunction callback_func;

public:
    explicit PreProcessor(const vector<shared_ptr<ImgProcessingComponent>>& components);
    void execute(CallBackFunction func, ArtObject* images) override;
    void my_callback(string str);

};


#endif //BEYOND_RGB_BACKEND_PREPROCESSOR_H

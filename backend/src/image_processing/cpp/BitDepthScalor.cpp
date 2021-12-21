//
// Created by ThinkPad41 on 10/10/2021.
//

#include "../header/BitDepthScalor.h"

void BitDepthScalor::execute(CallBackFunction func, btrgb::ArtObject* images) {
    func("Bit Depth Scaling");
    //TODO add Bit depth scaling logic here
    sleep_for(seconds(5));
}
//
// Created by ThinkPad41 on 10/10/2021.
//

#include "../header/DarkCurrentCorrector.h"

void DarkCurrentCorrector::execute(CallBackFunction func, btrgb::ArtObject* images) {
    func("DarkCurrentCorrector");
    //TODO add Dark current correction logic here
    sleep_for(seconds(5));
}
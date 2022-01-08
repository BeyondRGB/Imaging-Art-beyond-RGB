//
// Created by ThinkPad41 on 10/10/2021.
//

#include "../header/PixelRegestor.h"

void PixelRegestor::execute(CallBackFunction func, btrgb::ArtObject* images) {
    func("Pixel Registration");
    //TODO add Pixel Registration logic here
    sleep_for(seconds(1));
}
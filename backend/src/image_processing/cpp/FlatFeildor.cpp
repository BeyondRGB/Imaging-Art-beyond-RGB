//
// Created by ThinkPad41 on 10/10/2021.
//

#include "../header/FlatFeildor.h"

void FlatFeildor::execute(CallBackFunction func, ArtObject* images) {
    func("Flat Fielding");
    //TODO add FlatFeilding logic here
    sleep_for(seconds(5));
}
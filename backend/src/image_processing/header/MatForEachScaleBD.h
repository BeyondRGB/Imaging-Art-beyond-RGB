#ifndef MATFOREACHSCALLEBD_H
#define MATFOREACHSCALLEBD_H

#include <math.h>
#include "image_processing/header/BitDepthScalerStrategy.h"

class MatForEachScaleBD : public BitDepthScalerStrategy {
    public:
        void scale(btrgb::Image* im);
};

#endif
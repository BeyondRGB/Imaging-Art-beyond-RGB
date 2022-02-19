#ifndef COLOR_CONVERTIONS_H
#define COLOR_CONVERTIONS_H

#include "reference_data/white_points.hpp"
#include <iostream>

namespace btrgb{
    typedef struct xyz_s{
        double x, y, z;

    }XYZ_t;

    typedef struct lab_s{
        double L, a, b;
    }Lab_t;


    btrgb::Lab_t xyz_2_Lab(btrgb::XYZ_t xyz, WhitePoints* wp);

};



#endif //COLOR_CONVERTIONS_H

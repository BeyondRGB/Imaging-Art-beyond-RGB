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

    /**
     * @brief Convert xyz values to L*a*b* values
     * 
     * @param xyz xyz_t struct containing the x,y,z values
     * @param wp the whitepoints that should be used for convertion, 
     * these are based on the Illuminants and Standard observer
     * @return btrgb::Lab_t struct containgin the L*, a*, b* values
     */
    btrgb::Lab_t xyz_2_Lab(btrgb::XYZ_t xyz, WhitePoints* wp);

};



#endif //COLOR_CONVERTIONS_H

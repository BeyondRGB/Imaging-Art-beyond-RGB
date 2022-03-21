#ifndef JSONAFIABLE_H
#define JSONAFIABLE_H

#include <jsoncons/json.hpp>
#include <opencv2/opencv.hpp>

class Jsonafiable{

    public:
        virtual jsoncons::json jsonafy() = 0;

        



    


};

#endif // JSONAFIABLE_H

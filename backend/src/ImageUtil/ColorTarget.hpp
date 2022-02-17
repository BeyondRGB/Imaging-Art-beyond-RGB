#ifndef COLOR_TARGET_H
#define COLOR_TARGET_H

#include "Image.hpp"
#include <iostream>
#include <math.h>

class ColorTarget {

public:
	ColorTarget() {}
	ColorTarget(btrgb::Image* im, double top_loction,double bottom_location, double left_location, double right_location, int row_count, int col_count);
	double get_patch_avg(int row, int col, int channel);
	int get_row_count();
	int get_col_count();

private:
	btrgb::Image* im;
	int target_top_edge;
	int target_left_edge;
	int target_width;
	int target_height;
	int row_count;
	int col_count;
	int row_height;
	int col_width;

	int patch_posX(int col);
	int patch_posY(int row);
	
};
#endif // !COLOR_TARGET_H

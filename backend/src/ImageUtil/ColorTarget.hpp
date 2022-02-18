#ifndef COLOR_TARGET_H
#define COLOR_TARGET_H

#include "Image.hpp"
#include <iostream>
#include <math.h>

typedef struct target_location {
	// Normalized locations of the four edges of the Target
	// These are the values provided by the front end
	double top_loc, bot_loc, left_loc, right_loc;
	// Row and Colum count for the color Target provided by the front end
	int row_count, col_count;
}TargetData;

class ColorTarget {

public:
	ColorTarget() {}
	ColorTarget(btrgb::Image* im, TargetData location_data);
	float get_patch_avg(int row, int col, int channel, double sp=0.3);
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

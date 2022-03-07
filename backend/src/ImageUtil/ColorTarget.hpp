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

/**
 * @brief Class that represents a ColorTarget from an image
 * A Color target has a number of ColorPatches of different colors organized
 * into rows and colums.
 * 
 */
class ColorTarget {


public:
	ColorTarget() {}
	ColorTarget(btrgb::Image* im, TargetData location_data);

	/**
	 * @brief Calculate the average pixel value for specified color patch
	 * 
	 * @param row the row of the color patch
	 * @param col the col of the color patch
	 * @param channel the color channel to get the avg from
	 * @param sp the sample width as a percentage of the color patch width
	 * @return float the average pixel value
	 */
	float get_patch_avg(int row, int col, int channel, double sp=0.3);
	
	/**
	 * @brief Get the row count
	 * 
	 * @return int 
	 */
	int get_row_count();

	/**
	 * @brief Get the col count
	 * 
	 * @return int 
	 */
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

	/**
	 * @brief Calculate the center x postiton for the specified col
	 * 
	 * @param col 
	 * @return int 
	 */
	int patch_posX(int col);

	/**
	 * @brief Calculate the center y position for the specified row
	 * 
	 * @param row 
	 * @return int 
	 */
	int patch_posY(int row);
	
};
#endif // !COLOR_TARGET_H
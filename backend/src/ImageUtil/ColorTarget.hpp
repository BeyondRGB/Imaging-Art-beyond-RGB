#ifndef COLOR_TARGET_H
#define COLOR_TARGET_H

#include "Image.hpp"
#include "reference_data/ref_data.hpp"
#include <iostream>
#include <math.h>

typedef struct target_location {
	// Normalized locations of the four edges of the Target
	// These are the values provided by the front end
	double top_loc, bot_loc, left_loc, right_loc;
	// Size of the sample to take from a color patch as a percentage of the patch size
	double sample_size;
	// Row and Colum count for the color Target provided by the front end
	int row_count, col_count;
	// White Patch Info
	int w_row, w_col;
	// Ref Data
	std::string ref_base, illum_base;
	int obsv_base;
	// Rotation angle of target
	double rotation_angle;
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
	ColorTarget(btrgb::Image* im, TargetData location_data, RefData *ref_data);

	/**
	 * @brief Calculate the average pixel value for specified color patch
	 *
	 * @param row the row of the color patch
	 * @param col the col of the color patch
	 * @param channel the color channel to get the avg from
	 * @param sp the sample width as a percentage of the color patch width
	 * @return float the average pixel value
	 */
	float get_patch_avg(int row, int col, int channel);

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

	int get_white_row();

	int get_white_col();

	RefData *get_ref_data();

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
	// White Patch Loc
	int white_row;
	int white_col;
	// Ref Data Base
	std::string reference;
	ObserverType observer;
	IlluminantType illuminant;
	// Full Ref Data
	RefData* ref_data;
	// Size of the sample to take from a color patch as a percentage of the patch size
	double sample_size = 0.3;

	double rotation_angle = 0.0;

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

class ColorTargetError : public std::exception {};

class ColorTarget_MissmatchingRefData : public ColorTargetError{
	public:
	ColorTarget_MissmatchingRefData(){};
    virtual char const * what() const noexcept { return error.c_str(); }

	private:
	std::string error = "ColorTarget Error: Target size does not match RefData size";
};

#endif // !COLOR_TARGET_H

#include "ColorTarget.hpp"

ColorTarget::ColorTarget(btrgb::Image* im, TargetData location_data) {
	this->im = im;

	// The front end normalizes the location based on width, so multiply top by width instead of height
	int img_width = 1;
	try{
		img_width = im->width();
	}catch(std::exception){
		std::cout << "ColorTarget: Image Not initialized yet" << std::endl;
	}
	// Init target edge locations
	this->target_left_edge = location_data.left_loc * img_width;
	this->target_top_edge = location_data.top_loc * img_width;
	int right_edge = location_data.right_loc * img_width;
	int bottom_edge = location_data.bot_loc * img_width;
	// Init target width and hieght
	this->target_width = right_edge - this->target_left_edge;
	this->target_height = bottom_edge - this->target_top_edge;
	// Init row and col count
	this->row_count = location_data.row_count;
	this->col_count = location_data.col_count;
	// Init row width and col height
	this->row_height = this->target_height / this->row_count;
	this->col_width = this->target_width / this->col_count;
	// Init sampel size
	this->sample_size = location_data.sample_size;
	// White Patch Init
	// The TargetData has already subtraced one so this is already zero based
	this->white_row = location_data.w_row;
	this->white_col = location_data.w_col;
	// Ref Data Collection
	this->reference = location_data.ref_base;
	this->illuminant = this->set_illuminant_type(location_data.illum_base);
	this->observer = this->set_observer_type(location_data.obsv_base);
	// Make the RefData
	this->ref_data = new RefData(this->reference, this->illuminant, this->observer);
	if( this->row_count != ref_data->get_row_count() || this->col_count != ref_data->get_col_count()){
		throw ColorTarget_MissmatchingRefData();
	}
}

/**
* w = patch_width
* sw = sample_width
* sr = sample_radious (number of pixels on any side of the center pixel)
*	ie. an sr of 2 would be a total width of 5 (.. . ..)
*		an sr of 3 would be a total width of 7 (... . ...)
* sp = sample_persengate (the width of sr as a percentage of w)
*
*		sw = sp(w)
*
*		sw = 2sr + 1 // 1 for the center pixel and 2sr for pixels on either side
*		sr = (sw - 1) / 2
*
*/
float ColorTarget::get_patch_avg(int row, int col, int chan) {
	int center_pixX = this->patch_posX(col);
	int center_pixY = this->patch_posY(row);
	// The sample_size is the size of the sample to take from a color patch as a percentage of the patch size
	// Sample width/height
	int sw = this->sample_size * this->col_width;
	// Sample radious(number of pixels on either side of center)
	int sr = (sw - 1) / 2;

	// Find sume of pixel values for all pixels within sample
	float pixel_value_sum = 0;
	for (int yOffset = -sr; yOffset < sr + 1; yOffset++) {
		for (int xOffset = -sr; xOffset < sr + 1; xOffset++) {
			int col = center_pixX + xOffset;
			int row = center_pixY + yOffset;
			pixel_value_sum += im->getPixel(row, col, chan);
			//std::cout << "pixel(" << col << "," << row << "): " << im->getPixel(row, col, chan) << std::endl;
		}
	}

	// The sample pixels form a square so the number of pixels is sample_width squared
	int pixel_count = pow(sw, 2);
	float avg = pixel_value_sum / pixel_count;
	return avg;
}

int ColorTarget::patch_posX(int col) {
	int col_width = this->target_width / this->col_count;
	int offset = col * col_width;
	// left edge of target + offset gives the left edge of patch so add col_width/2 to get to center
	int patch_centerX = this->target_left_edge + offset + (col_width / 2);
	return patch_centerX;
}

int ColorTarget::patch_posY(int row) {
	int row_height = this->target_height / this->row_count;
	int offset = row * row_height;
	// top edge of target + offset gives the top edge of patch so add row_height/2 to get to center
	int patch_centerY = this->target_top_edge + offset + (row_height / 2);
	return patch_centerY;

}

int ColorTarget::get_row_count() {
	return this->row_count;
}

int ColorTarget::get_col_count() {
	return this->col_count;
}


int ColorTarget::get_white_row() {
	return this->white_row;
}

int ColorTarget::get_white_col() {
	return this->white_col;
}

IlluminantType ColorTarget::set_illuminant_type(std::string illum_str) {
    // Default to D50
    IlluminantType type = IlluminantType::D50;
    if (illum_str == "A") {
        type = IlluminantType::A;
    }
    if (illum_str == "D65") {
        type = IlluminantType::D65;
    }
    return type;
}

ObserverType ColorTarget::set_observer_type(int observer_num) {
    // Default to 1931
    ObserverType type = ObserverType::SO_1931;
    if (observer_num == 1964) {
        type = ObserverType::SO_1964;
    }
    return type;
	}

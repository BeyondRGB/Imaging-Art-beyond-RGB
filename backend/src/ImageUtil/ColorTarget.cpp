#include "ColorTarget.hpp"

ColorTarget::ColorTarget(btrgb::Image* im, double top_loction, double bottom_location, double left_location, double right_location, int row_count, int col_count) {
	this->im = im;
	std::cout << "Image: " << im->filename();
	// The front end normalizes the location based on width, so multiply top by width instead of height
	int img_width = im->width();

	this->target_left_edge = left_location * img_width;
	this->target_top_edge = top_loction * img_width;
	int right_edge = right_location * img_width;
	int bottom_edge = bottom_location * img_width;

	this->target_width = right_edge - this->target_left_edge;
	this->target_height = bottom_edge - this->target_top_edge;

	this->row_count = row_count;
	this->col_count = col_count;

	this->row_height = this->target_height / this->row_count;
	this->col_width = this->target_width / this->col_count;
	/*std::cout << "img_widht: " << img_width << " img_height: " << im->height() << std::endl;
	std::cout << "Top: " << this->target_top_edge << "  Left: " << this->target_left_edge << std::endl;
	std::cout << "Bottom: " << bottom_edge << "  Right: " << right_edge << std::endl;

	std::cout << "width: " << this->target_width << " hight: " << this->target_height << std::endl;
	std::cout << "row_count: " << this->row_count << " col_count: " << this->col_count << std::endl;*/


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
double ColorTarget::get_patch_avg(int row, int col, int chan) {
	int center_pixX = this->patch_posX(col);
	int center_pixY = this->patch_posY(row);
	// percentage of patch width that the sample width takes up
	double sp = 30.0 / 100.0; // 30%
	// Sample width/height
	int sw = 5;//sp * this->col_width;
	// Sample radious(number of pixels on either side of center)
	int sr = (sw - 1) / 2;

	// Find sume of pixel values for all pixels within sample
	double pixel_value_sum = 0;
	int loops = 0;
	for (int yOffset = -sr; yOffset < sr + 1; yOffset++) {
		for (int xOffset = -sr; xOffset < sr + 1; xOffset++) {
			int col = center_pixX + xOffset;
			int row = center_pixY + yOffset;
			//double pix_value = im->getPixel(row, col, chan);
			pixel_value_sum += im->getPixel(row, col, chan);
			//std::cout << "pix_value: " << pix_value << "(" << col << "," << row << ")" << std::endl;
			loops++;
		}
	}
	std::cout << "loops: " << loops << std::endl;
	std::cout << "centerX: " << center_pixX << " centerY: " << center_pixY << std::endl;
	std::cout << "pixel_value_sum: " << pixel_value_sum << std::endl;
	// The sample pixels form a square so the number of pixels is sample_width squared
	int pixel_count = pow(sw, 2);
	std::cout << "pix_count: " << pixel_count << std::endl;
	double avg = pixel_value_sum / pixel_count;	
	return avg;
}

int ColorTarget::patch_posX(int col) {
	int col_width = this->target_width / this->col_count;
	int offset = col * col_width;
	int patch_centerX = this->target_left_edge + offset + (col_width / 2);
	return patch_centerX;
}

int ColorTarget::patch_posY(int row) {
	int row_height = this->target_height / this->row_count;
	int offset = row * row_height;
	int patch_centerY = this->target_top_edge + offset + (row_height / 2);
	return patch_centerY;

}

int ColorTarget::row_count() {
	return this->row_count;
}

int ColorTarget::col_count() {
	return this->col_count;
}


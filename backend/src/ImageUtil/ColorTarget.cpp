#include "ColorTarget.hpp"

ColorTarget::ColorTarget(btrgb::Image* im, TargetData location_data, RefData* ref_data) {
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
    this->rotation_angle = location_data.rotation_angle;
	// Ref Data Collection
	this->reference = location_data.ref_base;
	this->illuminant = RefData::get_illuminant(location_data.illum_base);
	this->observer = RefData::get_observer(location_data.obsv_base);
	
	// Make the RefData
	this->ref_data = ref_data;
	// Ensure that the Target size matches the RefData size
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
float ColorTarget::get_patch_avg(int row, int col, int chan)
{
    // Convert the stored bounding box into absolute pixel coordinates.
    //
    //       this->target_left_edge    // in pixels
    //       this->target_top_edge     // in pixels
    //       this->target_width        // in pixels
    //       this->target_height       // in pixels
    //   and this->rotation_angle     // in degrees

    // bounding box center
    double boxCenterX = this->target_left_edge + (this->target_width / 2.0);
    double boxCenterY = this->target_top_edge  + (this->target_height / 2.0);

    // Compute the patch unrotated center
    // Without rotation the patch (row,col) center is
    double patchWidth  = (double)this->target_width  / this->col_count;
    double patchHeight = (double)this->target_height / this->row_count;

    double unrotatedCenterX = this->target_left_edge + (col + 0.5)*patchWidth;
    double unrotatedCenterY = this->target_top_edge  + (row + 0.5)*patchHeight;

    // Rotate that center around (boxCenterX, boxCenterY) by rotation_angle
    double rad = this->rotation_angle * M_PI / 180.0; // convert degrees to radians
    double dx  = unrotatedCenterX - boxCenterX;
    double dy  = unrotatedCenterY - boxCenterY;

    double rotatedX =  dx*std::cos(rad) - dy*std::sin(rad);
    double rotatedY =  dx*std::sin(rad) + dy*std::cos(rad);

    double finalCenterX = boxCenterX + rotatedX;
    double finalCenterY = boxCenterY + rotatedY;

    // sample a local region around (finalCenterX, finalCenterY)
    int sw = (int)(this->sample_size * patchWidth);
    int sr = sw / 2; // half on each side
    if (sw < 1) sw = 1; // just in case

    float pixel_value_sum = 0.0f;
    int   count           = 0;

    for (int yOffset = -sr; yOffset <= sr; yOffset++) {
        for (int xOffset = -sr; xOffset <= sr; xOffset++) {
            int sx = (int)std::round(finalCenterX + xOffset);
            int sy = (int)std::round(finalCenterY + yOffset);
            if (sx >= 0 && sx < im->width() && sy >= 0 && sy < im->height()) {
                pixel_value_sum += im->getPixel(sy, sx, chan);
                count++;
            }
        }
    }
    if (count == 0) {
        // fallback if no valid samples
        return 0.0f;
    }
    return pixel_value_sum / (float)count;
}

/*int ColorTarget::patch_posX(int col) {
    // Dynamically compute edges and assign them to local variables
    double left = static_cast<double>(this->target_left_edge) / im->width();
    double right = static_cast<double>(this->target_left_edge + this->target_width) / im->width();

    // Pass variables as lvalues to transform_edges
    double top = static_cast<double>(this->target_top_edge) / im->height(); // Needed for transform_edges
    double bottom = static_cast<double>(this->target_top_edge + this->target_height) / im->height(); // Needed for transform_edges
    transform_edges(left, top, right, bottom, this->rotation_angle, im->width(), im->height());

    int patch_width = (right - left) * im->width() / this->col_count;
    return left * im->width() + col * patch_width + patch_width / 2;
}

int ColorTarget::patch_posY(int row) {
    // Dynamically compute edges and assign them to local variables
    double top = static_cast<double>(this->target_top_edge) / im->height();
    double bottom = static_cast<double>(this->target_top_edge + this->target_height) / im->height();

    // Pass variables as lvalues to transform_edges
    double left = static_cast<double>(this->target_left_edge) / im->width(); // Needed for transform_edges
    double right = static_cast<double>(this->target_left_edge + this->target_width) / im->width(); // Needed for transform_edges
    transform_edges(left, top, right, bottom, this->rotation_angle, im->width(), im->height());

    int patch_height = (bottom - top) * im->height() / this->row_count;
    return top * im->height() + row * patch_height + patch_height / 2;
}*/



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

RefData* ColorTarget::get_ref_data(){
	return this->ref_data;
}

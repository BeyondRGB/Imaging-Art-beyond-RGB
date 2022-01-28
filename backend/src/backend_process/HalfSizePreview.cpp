#include "backend_process/HalfSizePreview.hpp"

unsigned int HalfSizePreview::id = 0;

HalfSizePreview::HalfSizePreview() {
    this->set_process_name("GetHalfSizePreview(" + std::to_string(this->id++) + ")");
}

HalfSizePreview::~HalfSizePreview() {}


void HalfSizePreview::run() {
    Json filenames = this->process_data_m->get_array("filenames");
    LibRawReader reader(LibRawReader::LOAD_HALF_SIZE);
    ManualBitDepthFinder bitDepthFinder;
    LibpngWriter pngWriter;
    int raw_bit_depth;

    
    for (int i = 0; i < filenames.get_size(); i++) {

        btrgb::image im(filenames.string_at(i));
        reader.read(&im);

        raw_bit_depth = bitDepthFinder.get_bit_depth(&im);

        std::vector<uint8_t> png_binary;
        pngWriter.write_png(&im, "", &png_binary, raw_bit_depth);
        
        /* To do: 
            - convert to base64
            - format response message
        */

    }

}



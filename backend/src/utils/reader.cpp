#include <libraw/libraw.h>
#include <string>
#include <iostream>
#include <fstream>
#include <array>
#include <vector>

void* read_image(std::string path)
{
        // Init
        LibRaw reader;
        // reader params
        libraw_output_params_t* opt = &(reader.imgdata.params);
        opt->user_black = 0;
        opt->use_camera_wb = 0;
        opt->use_auto_wb = 0;
        opt->use_camera_matrix = 0;
        opt->output_bps = 16;
        opt->no_auto_bright = 1;
        opt->no_auto_scale = 1;
        opt->gamm[0] = 1.0;
        opt->gamm[1] = 1.0;
        opt->output_color = 0;
        opt->user_qual = 0;

        // Read
        int error_code;
        error_code = reader.open_file(path.c_str());
        if(error_code) {
                reader.recycle();
                std::cout << "ERROR: failed to open file" << std::endl;
        }
        error_code = reader.unpack();
        if(error_code) 
                std::cout << "ERROR: failed to unpack file" << std::endl;
        error_code = reader.dcraw_process();
        if(error_code)
                std::cout << "ERROR: failed to process file" << std::endl;
        int width, height, channels, depth;
        reader.get_mem_image_format(&width, &height, &channels, &depth);

        std::cout << width << std::endl;
        std::cout << height << std::endl;
        std::cout << channels << std::endl;
        std::cout << depth << std::endl;

        int stride = width * channels * (depth / 8);
        ushort buf[stride*height];
        reader.copy_mem_image(&buf, stride, false);
    

        std::ofstream file;
        file.open ("t1.txt");
        for (int i = 0; i < width * height * channels; i++) {
        }
        file.close();


        // Cleanup
        reader.recycle();

        // Return
        return 0;
}


int main() 
{
        read_image("./SampleImages/targets_1.ARW");
}

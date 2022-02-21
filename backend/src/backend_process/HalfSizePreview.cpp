#include <chrono>
#include <regex>
#include "backend_process/HalfSizePreview.hpp"

unsigned int HalfSizePreview::id = 0;

HalfSizePreview::HalfSizePreview() {
    this->set_process_name("GetHalfSizePreview(" + std::to_string(HalfSizePreview::id) + ")");
    HalfSizePreview::id += 1;
}

HalfSizePreview::~HalfSizePreview() {}


void HalfSizePreview::run() {
    std::cout << "started process" << std::endl;

    Json filenames = this->process_data_m->get_array("filenames");
    const int ticket = 2345234;

    btrgb::LibRawReader* reader = new btrgb::LibRawReader(btrgb::LibRawReader::PREVIEW);
    std::vector<uchar>* png_binary = nullptr;
    std::string* rsp = nullptr;

    
    const std::vector<int> png_params = {
        cv::IMWRITE_PNG_COMPRESSION, 1,
        cv::IMWRITE_PNG_STRATEGY, cv::IMWRITE_PNG_STRATEGY_HUFFMAN_ONLY,
    };

    
        
        
    for (int i = 0; i < filenames.get_size(); i++) {
        auto img_start = std::chrono::high_resolution_clock::now();
        try {

            std::cout << "about to read raw..." << std::endl;
            reader->open(filenames.string_at(i));
            cv::Mat im(reader->height(), reader->width(), CV_8UC(reader->channels()));
            reader->copyBitmapTo( (uint8_t*) im.data, im.rows * im.cols * im.channels());
            reader->recycle();
            std::cout << "read raw" << std::endl;

            std::cout << "scaling preview..." << std::endl;
            cv::Mat im_scaled;
            double scaler = double(2000) / double(im.cols);
            std::cout << "scaling by: " << scaler << std::endl;
            cv::resize(im, im_scaled, cv::Size(), scaler, scaler, cv::INTER_AREA);
            im.release();
            std::cout << "scaling done" << std::endl;

            cv::Mat bgr_im;
            cv::cvtColor(im_scaled, bgr_im, cv::COLOR_RGB2BGR);
            im_scaled.release();
            std::cout << "writing png..." << std::endl;
            png_binary = new std::vector<uchar>;
            cv::imencode(".png", bgr_im, *png_binary, png_params);
            bgr_im.release();
            std::cout << "wrote png" << std::endl;
            
            std::cout << "writing base64 and response..." << std::endl;
            rsp = new std::string;
            rsp->reserve( png_binary->size() / 2 );
            rsp->append(R"({"RequestID":)");
            rsp->append(std::to_string(ticket));
            rsp->append(R"(,"RequestType":"HalfSizePreview","RequestData":{"filename":")");
            rsp->append(std::regex_replace(filenames.string_at(i), std::regex("\\\\"), "\\\\"));
            rsp->append(R"(","dataURL": "data:image/png;base64,)");
            rsp->append(cppcodec::base64_rfc4648::encode(*png_binary));
            rsp->append(R"("}})");

            std::cout << "base64 and json complete" << std::endl;

            this->send_msg(*rsp);


        }
        catch(const std::runtime_error& e) {
            this->send_msg(R"({"RequestID":)");
            rsp->append(std::to_string(ticket));
            rsp->append(R"(,"RequestType":"HalfSizePreview","RequestData":{"filename":")");
            rsp->append(filenames.string_at(i));
            rsp->append(R"(","dataURL": 0)");
            rsp->append(R"(}})");
        }

        auto img_end = std::chrono::high_resolution_clock::now();
        auto img_duration = std::chrono::duration_cast<std::chrono::milliseconds>(img_end - img_start);
        std::cout << "HalfSizePreview processing time: " << std::to_string(img_duration.count()) << "ms (" << filenames.string_at(i) << ")" << std::endl;

        if(rsp != nullptr) {
            delete rsp;
            rsp = nullptr;
        }
        if(png_binary != nullptr) {
            delete png_binary;
            png_binary = nullptr;
        }

    }

    delete reader;

}


    


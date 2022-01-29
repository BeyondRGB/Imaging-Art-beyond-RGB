#include <chrono>
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

    LibRawReader* reader = new LibRawReader(LibRawReader::LOAD_HALF_SIZE);
    ManualBitDepthFinder* bitDepthFinder = new ManualBitDepthFinder();;
    btrgb::LibpngWriter* pngWriter = new btrgb::LibpngWriter();

    int raw_bit_depth;
    std::string* rsp = nullptr;
    btrgb::image* im = nullptr;
    std::vector<uint8_t>* png_binary;

    
        
        
    for (int i = 0; i < filenames.get_size(); i++) {
        auto img_start = std::chrono::high_resolution_clock::now();
        try {
            std::cout << "about to read raw..." << std::endl;
            im = new btrgb::image(filenames.string_at(i));
            reader->read(im);

            std::cout << "read raw" << std::endl;
            raw_bit_depth = bitDepthFinder->get_bit_depth(im);
            std::cout << "got bit depth: " << std::to_string(raw_bit_depth) << std::endl;

            png_binary = new std::vector<uint8_t>;
            pngWriter->write_png(im, "", png_binary, raw_bit_depth);
            std::cout << "wrote png" << std::endl;
            
            rsp = new std::string;
            rsp->reserve(im->getTotalByteSize() * 2);
            std::cout << "reserved size: " << std::to_string(im->getTotalByteSize() * 2) << std::endl;

            rsp->append(R"({"RequestID":)");
            rsp->append(std::to_string(ticket));
            rsp->append(R"(,"RequestType":"HalfSizePreview","RequestData":{"filename":")");
            rsp->append(im->filename());
            rsp->append(R"(","dataURL": "data:image/png;base64,)");
            rsp->append(cppcodec::base64_rfc4648::encode((const uint8_t*) &(*png_binary)[0], png_binary->size()));
            rsp->append(R"("}})");

            std::cout << "base64 and json complete" << std::endl;

            this->send_msg(*rsp);


        }
        catch(const RawReaderStrategy_FailedToOpenFile& e) {
            this->send_msg(R"({"RequestID":)");
            rsp->append(std::to_string(ticket));
            rsp->append(R"(,"RequestType":"HalfSizePreview","RequestData":{"filename":")");
            rsp->append(im->filename());
            rsp->append(R"(","dataURL": 0)");
            rsp->append(R"(}})");
        }
        catch(...) {
            this->send_msg(R"({"RequestID":)");
            rsp->append(std::to_string(ticket));
            rsp->append(R"(,"RequestType":"HalfSizePreview","RequestData":{"filename":")");
            rsp->append(im->filename());
            rsp->append(R"(","dataURL": 0)");
            rsp->append(R"(}})");
        }
        auto img_end = std::chrono::high_resolution_clock::now();
        auto img_duration = std::chrono::duration_cast<std::chrono::milliseconds>(img_end - img_start);
        std::cout << "Image processing time: " << std::to_string(img_duration.count()) << " (" << im->filename() << ")" << std::endl;

        if(rsp != nullptr) {
            delete rsp;
            rsp = nullptr;
        }
        if(png_binary != nullptr) {
            delete png_binary;
            png_binary = nullptr;
        }
        if(im != nullptr) {
            delete im;
            im = nullptr;
        }
    }

    delete reader;
    delete bitDepthFinder;
    delete pngWriter;

}


    


#include <regex>
#include "backend_process/Thumbnail.hpp"

unsigned int Thumbnail::id = 0;

Thumbnail::Thumbnail() {
    this->set_process_name("Thumbnail(" + std::to_string(Thumbnail::id) + ")");
    Thumbnail::id += 1;
}

Thumbnail::~Thumbnail() {}


void Thumbnail::run() {
    std::cout << "started process" << std::endl;

    Json filenames = this->process_data_m->get_array("filenames");
    const int ticket = 2345234;

    btrgb::LibRawThumbnail* reader = new btrgb::LibRawThumbnail;
    std::vector<uchar>* binary = nullptr;
    std::string* rsp = nullptr;
    std::string type;
    std::string fname;
        
    for (int i = 0; i < filenames.get_size(); i++) {
        try {fname = filenames.string_at(i);

            std::cout << "about to read thumbnail..." << std::endl;
            reader->open(fname);

            if( reader->is_encoded() ) {
                type = "image/jpeg";

                binary = new std::vector<uchar>(reader->length());
                reader->copyBitmapTo(&binary[0], binary->size());
                reader->recycle();

                rsp = new std::string;
                rsp->reserve(binary->size() * 2);
                rsp->append(R"({"RequestID":)");
                rsp->append("3453456");
                rsp->append(R"(,"ResponseType":"Thumbnail","ResponseData":{"filename":")");
                rsp->append(std::regex_replace(fname, std::regex("\\\\"), "\\\\"));
                rsp->append(R"(","dataURL": "data:image/jpeg;base64,)");
                rsp->append(cppcodec::base64_rfc4648::encode(*binary));
                rsp->append(R"("}})");
                this->coms_obj_m->send_msg(*rsp);
            }
            else {
                cv::Mat im;
                reader->copyBitmapTo(im);
                reader->recycle();

                /* Wrap the Mat as an Image object. */
                btrgb::Image imObj(fname + ".Thumbnail");
                imObj.initImage(im);
  
                /* The thumbnail is small so use btrgb::FULL, otherwise the thumbnails
                 * will get upscaled to a width of 2000 pixels, wasting time and memory. */
                this->coms_obj_m->send_base64(&imObj, btrgb::PNG, btrgb::FULL);
            }

        }
        catch(std::runtime_error& e) {
            this->coms_obj_m->send_error(e.what(), "Thumbnail");
        }
        catch(...) {
            this->coms_obj_m->send_error("[Thumbnail] Unknown error.", "Thumbnail");
        }

        reader->recycle();
        if(binary != nullptr) {
            delete binary;
            binary = nullptr;
        }
        if(rsp != nullptr) {
            delete rsp;
            rsp = nullptr;
        }
    }

    delete reader;
}

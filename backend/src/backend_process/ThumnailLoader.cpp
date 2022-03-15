#include <regex>
#include "backend_process/ThumbnailLoader.hpp"

unsigned int ThumbnailLoader::id = 0;

ThumbnailLoader::ThumbnailLoader() {
    this->set_process_name("ThumbnailLoader(" + std::to_string(ThumbnailLoader::id) + ")");
    ThumbnailLoader::id += 1;
}

ThumbnailLoader::~ThumbnailLoader() {}


void ThumbnailLoader::run() {
    std::cout << "started process" << std::endl;

    Json filenames = this->process_data_m->get_array("names");
    const int ticket = 2345234;

    btrgb::LibRawThumbnail* reader = new btrgb::LibRawThumbnail;
    std::vector<uchar>* binary = nullptr;
    std::string* rsp = nullptr;
    std::string type;
    std::string fname;
        
    for (int i = 0; i < filenames.get_size(); i++) {
        try {fname = filenames.string_at(i);

            std::cout << "about to read thumbnail of " << fname << std::endl;
            reader->open(fname);

            if( reader->is_encoded() ) {
                std::cout << "Thumbnail is JPEG" << std::endl;
                type = "image/jpeg";

                binary = new std::vector<uchar>;
                reader->copyBitmapTo(*binary);
                reader->recycle();

                rsp = new std::string;
                rsp->reserve(binary->size() * 2);
                rsp->append(R"({"RequestID":)");
                rsp->append("3453456");
                rsp->append(R"(,"ResponseType":"ImageBase64","ResponseData":{"name":")");
                rsp->append(std::regex_replace(fname, std::regex("\\\\"), "\\\\"));
                rsp->append(R"(","dataURL": "data:image/jpeg;base64,)");
                rsp->append(cppcodec::base64_rfc4648::encode(*binary));
                rsp->append(R"("}})");
                this->coms_obj_m->send_msg(*rsp);
            }
            else {
                std::cout << "Thumbnail is bitmap" << std::endl;
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
        catch(btrgb::ReaderFailedToOpenFile& e) {
            this->coms_obj_m->send_error("Failed to open file " + fname, "ThumbnailLoader");
            std::cerr << "Failed to open file " + fname << std::endl;
        }
        catch(std::runtime_error& e) {
            this->coms_obj_m->send_error(e.what(), "ThumbnailLoader");
            std::cerr << e.what() << std::endl;
        }
        catch(...) {
            this->coms_obj_m->send_error("[ThumbnailLoader] Unknown error.", "ThumbnailLoader");
            std::cerr << "[ThumbnailLoader] Unknown error." << std::endl;
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
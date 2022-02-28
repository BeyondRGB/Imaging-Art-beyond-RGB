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
        
    for (int i = 0; i < filenames.get_size(); i++) {

        try {

            std::cout << "about to read thumbnail..." << std::endl;
            reader->open(filenames.string_at(i));

            if( reader->is_encoded() ) {
                type = "image/jpeg";

                binary = new std::vector<uchar>(reader->length());
                reader->copyBitmapTo(&binary[0], binary->size());

                reader->recycle();
            }
            else {
                type = "image/png";

                cv::Mat im;
                reader->copyBitmapTo(im);

                
            }



        }
        catch(std::runtime_error& e) {

        }

        reader->recycle();
        if(binary != nullptr)
            delete binary;
    }

    delete reader;
}

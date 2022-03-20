#ifndef RESULTS_PROCESSOR_H
#define RESULTS_PROCESSOR_H

#include "ImgProcessingComponent.h"
#include "image_processing/results/calibration_results.hpp"

class ResultsProcessor : public ImgProcessingComponent{
    public:
        ResultsProcessor(){};
        ~ResultsProcessor(){};
        void execute(CommunicationObj* comms, btrgb::ArtObject* images) override;

    private:
        void output_results(btrgb::ArtObject* images);
        void output_images(btrgb::ArtObject* images);
};

#endif //RESULTS_PROCESSOR_H
#include <utils/threading_statics/flatfield_static.hpp>
#include <boost/range/irange.hpp>
#include <iostream>

void btrgb::flatfield::pixelOperation(float w, int row, int col, int img_hei, 
                                      int img_wid, int chunk_hei, int chunk_wid, int c, 
                                      btrgb::Image *a, btrgb::Image *wh, 
                                      btrgb::Image *d, btrgb::Image *ac) {
    // For loop is for every pixel in the image, and gets a corrisponding pixel
    // from white and dark images Every Channel value for each pixel needs to be
    // adjusted based on the w for that group of images
    int currRow, currCol, ch;
    int stuckPixelCounter = 0;
    int uncorrectedCounter = 0;

    double wPix, dPix, aPix, newPixel;
    for (currRow = row; currRow < chunk_hei; currRow++) {
        for (currCol = col; currCol < chunk_wid; currCol++) {
            for (ch = 0; ch < c; ch++) {

                // Get pixel from all three images
                wPix = wh->getPixel(currRow, currCol, ch);
                dPix = d->getPixel(currRow, currCol, ch);
                aPix = ac->getPixel(currRow, currCol, ch);

                // If pixel in white and dark targets are equal pixel is stuck /
                // dead
                if (double(wPix == dPix)) {

                    stuckPixelCounter++;

                    // Radius to perform dead pixel correction
                    int radius = 2;

                    // Hold the final average value
                    double artPixelTotalValue = 0.0;
                    double whitePixeTotallValue = 0.0;
                    double darkPixeTotallValue = 0.0;

                    // Total number of neighboring pixels looked at
                    int pixelCount = 0;

                    // make sure the selection area doesn't go over the edge
                    int left = (currCol - radius < 0 ? 0 : currCol - radius);
                    int right =
                        (currCol + radius >= img_wid ? img_wid - 1 : currCol + radius);
                    int top = (currRow - radius < 0 ? 0 : currRow - radius);
                    int bot = (currRow + radius > img_hei ? img_hei : currRow + radius);

                    for (auto xIndex : boost::irange(left, right)) {
                        for (auto yIndex : boost::irange(top, bot)) {

                            // Grab the values
                            double artPixel = ac->getPixel(yIndex, xIndex, ch);
                            double whitePixel =
                                wh->getPixel(yIndex, xIndex, ch);
                            double darkPixel = d->getPixel(yIndex, xIndex, ch);

                            if (whitePixel != darkPixel) {
                                // Neighbor is not dead, so use to correct
                                pixelCount++;
                                artPixelTotalValue += artPixel;
                                whitePixeTotallValue += whitePixel;
                                darkPixeTotallValue += darkPixel;
                            } else {
                                // Neighbor pixel is dead do nothing
                            }
                        }
                    }

                    // Average values
                    artPixelTotalValue = artPixelTotalValue / pixelCount;
                    whitePixeTotallValue = whitePixeTotallValue / pixelCount;
                    darkPixeTotallValue = darkPixeTotallValue / pixelCount;

                    // Perform flatfielding on these new values
                    newPixel =
                        w *
                        (double(artPixelTotalValue - darkPixeTotallValue) /
                         double(whitePixeTotallValue - darkPixeTotallValue));

                    // Final sanity checks, ensure no invalid values get by
                    // NAN catch just set to 0;
                    if (newPixel != newPixel) {
                        uncorrectedCounter++;
                        newPixel = 0;
                    }
                    // INF catch just set to 0;
                    if (isinf(newPixel)) {
                        uncorrectedCounter++;
                        newPixel = 0;
                    }
                    // Done set pixel in artwork
                    a->setPixel(currRow, currCol, ch, newPixel);
                }

                // Normal pixels flatfield as normal
                else {
                    newPixel =
                        w * (double(aPix - dPix) / double(wPix - dPix));
                    a->setPixel(currRow, currCol, ch, newPixel);
                }
            }
        }
    }
    int corrected = stuckPixelCounter - uncorrectedCounter;
    std::cout << "Stuck/Dead Pixels Detected - " << stuckPixelCounter / 3
              << "\n";
    std::cout << "Stuck/Dead Pixels Corrected - " << corrected / 3 << "\n";
    std::cout << "Stuck/Dead Pixels Uncorrected - " << uncorrectedCounter / 3
              << "\n";
}
#include "LibTiffWriter.hpp"

namespace btrgb {
    
    LibTiffWriter::LibTiffWriter() {this->file_extension = ".tiff";}
    LibTiffWriter::~LibTiffWriter() {}

    void LibTiffWriter::_write(Image* im, std::string filename) {
        struct tiff* img_out;

        int width = im->width();
        int height = im->height();
        int channels = im->channels();


		/* Check if color profile is implemented. */
		switch(im->getColorProfile()) {
			case none: case ColorSpace::ProPhoto: case ColorSpace::Adobe_RGB_1998:
				break;
			default: case ColorSpace::sRGB: case ColorSpace::Wide_Gamut_RGB: 
				throw std::logic_error("[LibTiffWriter] Color profile not implemented.");
		}


		cv::Mat im_16u;
		im->getMat().convertTo(im_16u, CV_16U, 0xFFFF);
        uint16_t* bitmap = (uint16_t*) im_16u.data;



        /* ==============[ Open tiff file for writing ]================== */

		img_out = TIFFOpen(filename.c_str(), "w");
		if(!img_out) 
            throw LibTiff_OpenFileFailed();



		/* =================[ Write tiff header tags ]================== */

		TIFFSetField(img_out, TIFFTAG_IMAGEWIDTH, width);
		TIFFSetField(img_out, TIFFTAG_IMAGELENGTH, height);

		/* Set the number of channels. */
		TIFFSetField(img_out, TIFFTAG_SAMPLESPERPIXEL, channels);

		/* We want 16 bits. */
		TIFFSetField(img_out, TIFFTAG_BITSPERSAMPLE, 16);
		
		/* Image rotation. */
		TIFFSetField(img_out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);

		/* Write all channel data in one array (a bitmap) instead of separating the channels. */
		TIFFSetField(img_out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

		/* Indicate type of image. */
		if(channels == 3)
			TIFFSetField(img_out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
		else if(channels == 1)
			TIFFSetField(img_out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
		else {
			TIFFSetField(img_out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
			uint16_t tags[16] = {EXTRASAMPLE_UNSPECIFIED};
			TIFFSetField(img_out, TIFFTAG_EXTRASAMPLES, channels - 1, tags);
		}

		/* Set color profile. */
		switch(im->getColorProfile()) {

			case none: 
				break;

			case ColorSpace::ProPhoto:
				TIFFSetField(img_out, TIFFTAG_ICCPROFILE, ProPhoto_icm_size, ProPhoto_icm_data);
				break;

			case ColorSpace::Adobe_RGB_1998:
				TIFFSetField(img_out, TIFFTAG_ICCPROFILE, AdobeRGB1998_icc_size, AdobeRGB1998_icc_data);
				break;

			case ColorSpace::sRGB:
			case ColorSpace::Wide_Gamut_RGB:
			default: throw std::logic_error("[LibTiffWriter] Invalid color profile.");
		}

		/* Set custom application tags as artist. */
		try {
			std::string t = this->getCustomTag(im);
			TIFFSetField(img_out, TIFFTAG_ARTIST, t.c_str());
		}
		catch(...) {
			std::cerr << "[LibTiffWriter] Warning: could not write custom tag.\n";
		}

		/* Set software field. */
		TIFFSetField(img_out, TIFFTAG_SOFTWARE, "BTRGB v1.0.0");

		/* Set make and model if available. */
		btrgb::exif tags = im->getExifTags();
		if(tags.make != btrgb::UNSPECIFIED)
			TIFFSetField(img_out, TIFFTAG_MAKE, tags.make.c_str());
		if(tags.model != btrgb::UNSPECIFIED)
			TIFFSetField(img_out, TIFFTAG_MODEL, tags.model.c_str());

		/* The written data needs to be broken up into "Strips" to make buffering easier 
		* for TIFF readers. Rows-per-strip needs to be tagged, this is the number of 
		* physical pixel rows of the image written in each strip. A "row" here is a bitmap
		* row where the length is <image width> X <number of channels>. A strip size of 8kB is 
		* recommended (I wonder if this is for computing standars/capabilities wayback 
		* when). Anyway, since the data type of the return value and estimate rows-per-strip 
		* values are uint32: TIFFDefaultStripSize will not give strips that contain a fraction 
		* of a row, and our image rows are more than 8kB, so the closest to the recommended we 
		* can do is one row per strip. */
		/*
		int rows_per_strip = TIFFDefaultStripSize(img_out, 1);
		TIFFSetField(img_out, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
		*/
		TIFFSetField(img_out, TIFFTAG_ROWSPERSTRIP, 1);



        /* =================[ Write bitmap to file ]================== */

		/* The buffer for each strip to be copied to and then written to disk. 
		* If rows-per-strip is one, scanline size should equate to the number
		* of bytes for one physical pixel row of the image, and the number of
		* strips should equal the height of the image. */
		/*
		tmsize_t scanline_size = TIFFScanlineSize(img_out);
		unsigned short* sample_row = (unsigned short *) _TIFFmalloc(scanline_size);
		if (sample_row)
			_TIFFfree(sample_row);
		*/

		uint32_t row, row_size = width * channels;
        void* row_memory_address;

		/* Write all rows to file.
         * ASSUMPTION: The "rows per strip" tiff tag is set to one. */
		for( row = 0; row < height; row++) {
			
			row_memory_address = bitmap + row * row_size;

            /* Write row to file. */
			if (TIFFWriteScanline(img_out, row_memory_address, row, 0) < 0) {
                TIFFClose(img_out);
				throw LibTiff_WriteStripFailed();
            }

		}



		/* ==============[ Close tiff file ]================== */
		TIFFClose(img_out);
    }


    std::string LibTiffWriter::getCustomTag(Image* im) {
		jsoncons::json json_tag;

		/* Set color profile indicator. */
		switch(im->getColorProfile()) {
			case none: 
				break;
			case ColorSpace::ProPhoto: 
				json_tag["ColorSpace"] = "ProPhoto"; break;
			case ColorSpace::Adobe_RGB_1998: 
				json_tag["ColorSpace"] = "Adobe_RGB_1998"; break;
			case ColorSpace::sRGB:
				json_tag["ColorSpace"] = "sRGB"; break;
			case ColorSpace::Wide_Gamut_RGB:
				json_tag["ColorSpace"] = "Wide_Gamut_RGB"; break;
			default: throw std::logic_error("[LibTiffWriter] Invalid color profile.");
		}

		/* Set conversion matricies. */
		for(const auto& [key, m] : im->getConversionsIterator()) {
			jsoncons::json json_mat;
			
			int data_length = m.rows * m.cols;
			float* mat_data = (float*) m.data;

			json_mat["rows"] = m.rows;
			json_mat["cols"] = m.cols;
			json_mat["cv_type"] = m.type();

			json_mat["data"] = jsoncons::json::make_array<1>(data_length);
			for(int i = 0; i < data_length; i++)
				json_mat["data"][i] = mat_data[i];

			json_tag[key] = json_mat;
        }

		/* Return resulting json. */
		return json_tag.to_string();
	}
}

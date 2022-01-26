#ifndef REF_DATA_H
#define REF_DATA_H

#include <string>
//#include <iostream>
#include "csv_parser.hpp"
#include "color_patch.hpp"
#include "illuminants.hpp"
#include "standard_observer.hpp"

class RefData: public CSVParser {

public:
	RefData(std::string file_path, Illuminants::IlluminantType illum_type, StandardObserver::ObserverType so_type);
	~RefData();
	ColorPatch* get_color_patch(int row, int col);
	ColorPatch* get_white_patch();
	int get_row_size();
	int get_col_size();

	void output_xyz();


private:
	void read_in_data(std::string file_path);
	void pars_line(std::string line);
	void pars_header(std::string header);
	void identify_data_size(std::string header);
	void init_data_storage();
	std::string get_col_id(std::string header_item);

	int row_count;
	int col_count;
	ColorPatch*** color_patches;
	std::string f_name;
	StandardObserver* observer = nullptr;
	Illuminants* illuminants = nullptr;

	
};
#endif //REF_DATA_H
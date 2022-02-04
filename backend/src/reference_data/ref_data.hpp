#ifndef REF_DATA_H
#define REF_DATA_H

#include <string>
//#include <iostream>
#include "utils/csv_parser.hpp"
#include "color_patch.hpp"
#include "illuminants.hpp"
#include "standard_observer.hpp"
#include "white_points.hpp"


typedef Illuminants::IlluminantType IlluminantType;
typedef StandardObserver::ObserverType ObserverType;

/**
* Class representing a spicific set of reference data.
* This class will read in its data from given file_name.
* It can provide ColorPatches, find the white patch,
* and provide Tristimulus/CIELAB values
*/
class RefData: public CSVParser {

public:
	RefData(std::string file_path, IlluminantType illum_type = IlluminantType::D50, ObserverType so_type = ObserverType::SO_1931);
	~RefData();

	/**
	* Retrives a reference to a ColorPatch at a given row and col
	* @param row: the row to get the ColorPatch from
	* @param col: the col to get the ColorPatch from
	* @return: pointer to the ColorPatch
	*/
	ColorPatch* get_color_patch(int row, int col);

	/**
	* Finds the white ColorPatch. The whit patch is the one with
	* the hightest y value.
	* @return: a pointer to the white ColorPatch
	*/
	ColorPatch* get_white_patch();

	/**
	* Get the row the white patch is in
	* @return: white patch row as an int
	*/
	int get_white_patch_row();

	/**
	* Get the colum the white patch is in
	* @return: white patch colum as an int
	*/
	int get_white_patch_col();

	/**
	* Gets the x value from the ColorPatch at the given row and col
	* @param row: the row of the ColorPatch to get x value from
	* @param col: the colum of the ColorPatch to get x value from
	*/
	double get_x(int row, int col);

	/**
	* Gets the y value from the ColorPatch at the given row and col
	* @param row: the row of the ColorPatch to get y value from
	* @param col: the colum of the ColorPatch to get y value from
	*/
	double get_y(int row, int col);

	/**
	* Gets the z value from the ColorPatch at the given row and col
	* @param row: the row of the ColorPatch to get z value from
	* @param col: the colum of the ColorPatch to get z value from
	*/
	double get_z(int row, int col);

	/**
	* Gets the number of rows the RefData holds
	* @return: number of rows
	*/
	int get_row_count();

	/**
	* Gets the number of colums the RefData holds
	* @return: number of colums
	*/
	int get_col_count();

	/**
	* Ouputs RefData to standard out.
	* This is used to capture Reference data to verify 
	* that values are computed correctly
	* Format:
	* RefData file_name
	* Valuetype,A:1,A:2,A:3,...,B:1,B:2,B:3,...,...,...
	* X,value1,value2,value3,...
	* Y,value1,value2,value3,...
	* Z,value1,value2,value3,...
	* WhitPatch,<WhiteColorPatch_name>
	* Y Value,<White ColorPatch yvalue>,Row,<White ColorPatch row>,Col,<White ColorPatch col> 
	*/
	void output_xyz();


private:
	/**
	* Read in reference data from given file
	*/
	void read_in_data(std::string file_path);

	/**
	* Pars given csv line and append data to related ColorPatch
	*/
	void pars_line(std::string line);

	/**
	* Pars header csv line and append data to related ColorPatch
	*/
	void pars_header(std::string header);

	/**
	* Pars given header and identify number of rows and colums 
	* needed to house ColorPatches
	*/
	void identify_data_size(std::string header);

	/**
	* Initialize ColorPatch storage
	*/
	void init_data_storage();

	/**
	* Parse given item from header to extrac the letter
	* Header item format: "A:1"
	* Given "A:1" this would retur "A"
	*/
	std::string get_col_id(std::string header_item);

	ColorPatch*** color_patches;
	StandardObserver* observer = nullptr;
	Illuminants* illuminants = nullptr;
	WhitePoints* white_pts = nullptr;
	std::string f_name;
	int row_count;
	int col_count;

	
};
#endif //REF_DATA_H
#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include <string>
#include <iostream>
#include <fstream>
#include <boost/lexical_cast.hpp>

/**
* Class for parsing csv files
*/
class CSVParser {

public:
	CSVParser() {}

	/**
	* Open given file. The file steam is stored as a private 
	* member variable.
	* It is the responsibility of the caller of open to 
	* call close_file() when done
	* @param file: the file name and path to be opend
	* @return: true if opend succsessfuly 
	*		   false if not
	*/
	bool open_file(std::string file);

	/**
	* Close the file
	*/
	void close_file();

	/**
	* Counts the number of tokens in the given line
	* @param line: string to count tokens in
	* @param delimiter: string used to seperate tokens in line. 
	*	This defaults to ","
	* @return: count of tokens seperated by delimiter
	*/
	int count_line_items(std::string line, std::string delimiter = ",");
	
	/**
	* Identifies if the given line contains another token
	* @param line: string to check for additional tokens
	* @param delimiter: string used to seperate tokens in line
	* @return: true if more tokens exist
	*		   false if none exist
	*/
	bool has_next(std::string line, std::string delimiter = ",");
	
	/**
	* Gets the next line from open file
	* @return: line as a string
	*/
	std::string get_next_line();

	/**
	* Identifies if open file has another line to read
	* @return: true if there exists another line
	*		   false if not
	*/
	bool has_next_line();

	/**
	* Generic function for retriveing the next token from the given line
	* This modifies the given line, removing the token returned
	* @param line: the string to get token from
	* @param delimiter: the string seperating tokens in line
	* @return: token as type T
	*/
	template <typename T>
	T get_next(std::string &line, std::string delimiter = ",") {
		std::string token;
		size_t pos = line.find(delimiter);
		token = line.substr(0, pos);
		// Remove current token from line
		size_t erase_stop = pos + delimiter.length();
		if (this->has_one_item(line))
			// Erase entire line
			erase_stop = line.length();
		line.erase(0, erase_stop);
		return boost::lexical_cast<T>(token);
	}

private:
	/**
	* Identifies if there is only token in the given line
	*/
	bool has_one_item(std::string line, std::string delimiter = ",");
	std::ifstream file_m;


};

#endif // !CSV_PARSER_H

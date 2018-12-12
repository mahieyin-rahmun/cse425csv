#include <iostream>
#include <fstream>
#include <map>
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>
#include <sstream>

using namespace boost;

int str_to_int(std::string str_val) {
	/*
		Helper function to convert string data to integer values
	*/

	// create string stream with the supplied string values
	std::stringstream ss(str_val);

	// declare variable type
	int int_val = 0;

	// dump the string streams into the variable
	ss >> int_val;

	// voila, integers!
	return int_val;
}

float str_to_float(std::string str_val) {
	/*
		Helper function to convert string data to floating point values
	*/

	// create string stream with the supplied string values
	std::stringstream ss(str_val);

	// declare variable type
	float float_val = 0;

	// dump the string streams into the variable
	ss >> float_val;

	// voila, floating point numbers!
	return float_val;
}

bool compare_csv_data(std::vector<std::string> temp_vector, std::string key, std::string value) {
	/*
		Helper function that returns a boolean value based on whether a csv row (in the form of a vector) meets the search criteria

		temp_vector: contains contents of the row from the csv file in the order
		
		Index: 	 	0	       1		   2 		  3	 		       4						 5
 		Column:	  	Year	   Cause1	   Cause2	  State		       Number of Deaths 		 Age Adjusted Death Rate

		key: the specified parameter of search
		value: value of the search parameter

		returns: boolean
	*/

	// segregate the data of interest
	std::string year = temp_vector[0];
	std::string disease1 = temp_vector[1];
	std::string disease2 = temp_vector[2];
	std::string state = temp_vector[3];
	std::string deaths = temp_vector[4];
	std::string age_adjusted = temp_vector[5];

	// compare based on the key and value pairs
	if (key.compare("year") == 0) {
		if (value.compare(year) == 0) {
			return true;
		}
	} // compare name of the cause/disease using substring matching
	else if (key.compare("cause") == 0) {
		if (disease1.find(value) != std::string::npos || disease1.find(value) != std::string::npos) {
			return true;
		}
	} // compare the name of the states using substring matching
	else if (key.compare("state") == 0) {
		if (state.find(value) != std::string::npos) {
			return true;
		}
	} // compare number of deaths 
	else if (key.compare("deaths") == 0) {				
		int csv_deaths = str_to_int(deaths); 
		int specified_num_of_deaths = str_to_int(value);

		if (csv_deaths >= specified_num_of_deaths) {
			return true;
		}
	} // compare death rate
	else if (key.compare("age_adjusted") == 0) {				
		float csv_age_adjusted = str_to_float(age_adjusted); 
		float specified_age_adjusted = str_to_float(value);

		if (csv_age_adjusted >= specified_age_adjusted) {
			return true;
		}
	}
	
	return false;
}

std::vector<std::vector<std::string>> search_file(std::string key, std::string value) {
	/* 
		This function is fired when we are attempting the very first search on the csv file.

		key: the name of the variable we are looking for (year, cause, state, deaths, death rate)
		value: value of the variable assigned to key

		returns: a vector of vector of strings
	*/

	// open the csv file
	std::ifstream ip;
	ip.open("../NCHS_-_Leading_Causes_of_Death__United_States.csv");

	// declare the necessary variables
	std::string line;
	std::vector<std::vector<std::string>> results_vector;

	// read the file line by line
	while(ip.good()) {
		std::vector<std::string> temp_vector;

		// get the entire line first
		std::getline(ip, line);

		if (line.length() > 0) {
			// tokenize using regex to escape commas inside quotation marks
			boost::tokenizer<escaped_list_separator<char>> tk(line, escaped_list_separator<char>('\\', ',', '\"'));

			// push data of each column into a vector
			for(auto i = tk.begin(); i != tk.end(); i++) {
				temp_vector.push_back(*i);
			}

			// if this row of data matches the search criteria, put it onto the results_vector
			if (compare_csv_data(temp_vector, key, value)) {
				results_vector.push_back(temp_vector);
			}			
		}
	}

	// close file stream
	ip.close();

	return results_vector;
}

std::vector<std::vector<std::string>> search_vector(std::vector<std::vector<std::string>> searchable, std::string key, std::string value) {
	/* 
		This function is fired when we are attempting to search with multiple parameters. The first search results are stored
		in a vector. That vector is passed to this function for further searching.

		key: the name of the variable we are looking for (year, cause, state, deaths, death rate)
		value: value of the variable assigned to key

		returns: a vector of vector of strings
	*/
	std::vector<std::vector<std::string>> results_vector;

	for (auto i = searchable.begin(); i != searchable.end(); i++) {
		//  get the individual vector within the searchable vector
		std::vector<std::string> temp_results = *i;

		if (compare_csv_data(temp_results, key, value)) {
			results_vector.push_back(temp_results);
		}
	}

	// there can be cases when the search returns empty (if the user specifies a key value pair that does not exist in the csv file)
	// so we check the size of the results vector before returning it. if it is zero, we simply return the searchable vector
	return (results_vector.size() > 0) ? results_vector : searchable;
}

std::map<std::string, std::string> get_arguments( boost::program_options::variables_map vm) {
	/*
		Helper function that sets up a map with the passed arguments. This map is later iterated over
		and each search call is fired.

		vm: the variable map generated by boost that cannot be iterated over. this is why we need another c++ map.
	*/
	std::map<std::string, std::string> my_map;
	
	if (vm.count("year")) {
		std::cout << "year was set to " << vm["year"].as<int>() << ".\n";
		my_map["year"] = std::to_string(vm["year"].as<int>());
	}  
	
	if (vm.count("cause")) {
		std::cout << "cause was set to " << vm["cause"].as<std::string>() << ".\n";
		my_map["cause"] = vm["cause"].as<std::string>();
	} 
	
	if (vm.count("state")) {
		std::cout << "state was set to " << vm["state"].as<std::string>() << ".\n";
		my_map["state"] = vm["state"].as<std::string>();
	}

	if (vm.count("deaths")) {
		std::cout << "deaths was set to " << vm["deaths"].as<int>() << ".\n";
		my_map["deaths"] = std::to_string(vm["deaths"].as<int>());
	}

	if (vm.count("age_adjusted")) {
		std::cout << "age_adjusted was set to " << vm["age_adjusted"].as<float>() << ".\n";
		my_map["age_adjusted"] = std::to_string(vm["age_adjusted"].as<float>());
	}

	std::cout << "\n" << std::endl;

	return my_map;
}

int main(int argc, const char* argv[]) {
	// namesapce to shorten the name
	namespace po = boost::program_options;

	// Initialize the parser
	po::options_description desc("Allowed options");

	// add optional arguments
	desc.add_options()
		("help, h", "produce help message")
		("file, f", po::value<std::string>(), "CSV File path")
		("year, y", po::value<int>(), "Search by year")
		("cause, c", po::value<std::string>(), "Search by cause")
		("state, s", po::value<std::string>(), "Search by name of state")
		("deaths, d", po::value<int>(), "Search by number of deaths")
		("age_adjusted, a", po::value<float>(), "Search by age adjusted death rate")
	;

	// store the arguments passed from the user into the map
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	// if the user wanted to see help, show it and exit
	if (vm.count("help")) {
		std::cout << desc << "\n";
		return 1;
	}

	// get the list of arguments that were passed	
	std::map<std::string, std::string> my_map = get_arguments(vm);

	std::vector<std::vector<std::string>> results;

	// iterate over the arguments list
	if (my_map.size() > 0) {
		for(auto i = my_map.begin(); i != my_map.end(); i++) {
			// get the key and value pairs
			std::string key = i->first;
			std::string value = i->second;

			// if this is the first search, we should search the file. 
			if (results.size() == 0) {
				results = search_file(key, value);
			}
			// searches after the first search will be done on the returing result_vectors from the previous searches 
			else {
				results = search_vector(results, key, value);
			}
		}
	}

	// iterate over final results vector and print out the results
	for (auto i = results.begin(); i != results.end(); i++) {
		std::vector<std::string> temp_vector = *i;

		std::cout << temp_vector[0] << " " << temp_vector[1] << " " << temp_vector[2] << " " << temp_vector[3] << " " << temp_vector[4] << " " << temp_vector[5] << std::endl;
	}
}
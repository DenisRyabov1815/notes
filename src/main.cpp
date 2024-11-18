
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>
#include <cstdarg>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <fstream>
#include <getopt.h>
#include <algorithm>
#include <cctype>

#define VERSION "1.0"

// The name of the file where the notes will be stored.
#define FILE_NAME ".notes"

// A function to terminate the program. It is used throughout the
// program for error handling.
void die(const char* __format, ...){
	va_list args;
	va_start(args, __format);
	vfprintf(stderr, __format, args);
	va_end(args);
	exit(EXIT_FAILURE);
}

void help(){
	auto print_key = [](std::string key, std::string desc){
		std::cout << "\t" << key << ": " << desc << std::endl;
	};
	std::cout << "\nA simple note-taking program for the terminal on the Linux operating system, written in C++.\n";
	std::cout << "Usage: notes [COMMAND 1]  [COMMAND 2]  ...\n\n";
	print_key("-a, --add \"NOTES\"", "Add a note.");
	print_key("-d, --delete \"INDEX\"", "Delete the note with the specified index INDEX.");
	print_key("-D, --delete-all Yes", "Delete all notes.");
	print_key("-s, --show", "Display all notes.");
	print_key("-v, --version", "Display the program version.");
	print_key("-h, --help \"NOTES\"", "Show help information.");
	std::cout << std::endl;
}

// The main class of the entire application.
class Notes{
	private:
		std::vector <std::string> __notes;
		std::string __ptf;

		std::string __gen_path_user_home(){
			std::ostringstream ptf;
			char* user_name;
			if (getuid() == 0){
				ptf << "/root/";
			}
			else{
				user_name = getenv("USER");
				if (user_name == nullptr){
					die("Could not retrieve the environment variable USER.\n");
				}
				ptf << "/home/" << user_name << "/";
			}
			return ptf.str();
		}

		void __check_dir(std::string __ptd){
			if (!std::filesystem::exists(__ptd)){
				die("Could not find the user directory.\n");
			}
			else if (!std::filesystem::is_directory(__ptd)){
				die("The path to the user directory is a file.\n");
			}
		}

		void __gen_path(){
			std::ostringstream ptf;
			ptf << __gen_path_user_home();
			__check_dir(ptf.str());
			ptf << FILE_NAME;
			__ptf = ptf.str();
		}

		void __create_file(std::string __ptf){
			std::ofstream file(__ptf);
			if (!file){
				die("Could not create the notes file: %s\n", strerror(errno));
			}
			file.close();
		}

		void __load_notes(){
			std::ifstream file(__ptf);
			std::string buffer;

			if (!file){
				die("Could not open the file for reading: %s\n", strerror(errno));
			}

			while (std::getline(file, buffer)){
				__notes.push_back(buffer);
			}

			file.close();
		}

		void __unload_notes(){
			std::ofstream file(__ptf);
			if (!file){
				die("Could not open the file for writing: %s\n", strerror(errno));
			}
			for (size_t i = 0; i < __notes.size(); i++){
				file << __notes[i]<< std::endl;
			}
			file.close();
		}

		void __check_index(size_t __index){
			if (__notes.size() == 0){
				die("There are no notes.");
			}
			if (__index >= __notes.size())
				die("Index too large. Maximum index: %li\n", __notes.size() - 1);
		}

	public:
		Notes(){
			__gen_path();
			if (!std::filesystem::exists(__ptf))
				__create_file(__ptf);
			__load_notes();
		}

		void add_note(std::string __note){
			__notes.push_back(__note);
		}

		void delete_note(size_t __index){
			__check_index(__index);
			__notes.erase(__notes.begin() + __index);
		}

		void delete_all_notes(std::string __confirm){
			if (__confirm != "Yes"){
				die("The confirmation was entered incorrectly.\n");
			}
			__notes.clear();
		}

		void show_notes(){
			for (size_t i = 0; i < __notes.size(); i++){
				std::cout << "[" << i << "]: " << __notes[i] << std::endl;
			}
		}

		~Notes(){
			__unload_notes();
		}
};

void str_is_digits(std::string str) {
	if (!std::all_of(str.begin(), str.end(), [](unsigned char c) { return std::isdigit(c); })){
		die("The format specified in the field where a number is required is incorrect.\n");
	}
}

int main(int argc, char** argv){
	Notes notes;
	const char* short_options = "a:d:D:svh";
	const struct option long_options[] = {
		{"add", required_argument, NULL, 'a'},
		{"delete", required_argument, NULL, 'd'},
		{"delete-all", required_argument, NULL, 'D'},
		{"show", no_argument, NULL, 's'},
		{"version", no_argument, NULL, 'v'},
		{"help", no_argument, NULL, 'h'},
		{0, 0, 0, 0}
	};
	int result_option = 0;
	int index_option = 0;
	size_t index = 0;

	while ((result_option = getopt_long(argc, argv, short_options,
					long_options, &index_option)) != -1){
		switch (result_option){
			case 'a':
				notes.add_note(optarg);
				break;
			case 'd':
				str_is_digits(optarg);
				index = atol(optarg);
				notes.delete_note(index);
				break;
			case 'D':
				notes.delete_all_notes(optarg);
				break;
			case 's':
				notes.show_notes();
				break;
			case 'v':
				std::cout << "Notes version: " << VERSION << std::endl;
				break;
			case 'h':
				help();
				break;
		}
	}
}

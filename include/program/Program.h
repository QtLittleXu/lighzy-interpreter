#pragma once

#include <argparse/argparse.hpp>
#include <fstream>
#include <filesystem>
#include "object/Environment.hpp"

namespace li::program
{

using namespace std;


class Program
{
public:
	static const filesystem::path PREREAD_SOURCES_PATH;

public:
	Program(int argc, char* argv[]);
	int run();

private:
	string read_file(const string& fileName);
	string read_folder_sources(const filesystem::path& folder);
	void change_write_file(const string& fileName);
	string get_source_file_name();
	void parse_source(const string& input, shared_ptr<Environment> env);
	int repl();

private:
	vector<string> _argv;

	argparse::ArgumentParser _program;
	ostream* _out;
	ofstream _outFile;
};


}
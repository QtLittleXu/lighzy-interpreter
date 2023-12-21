#pragma once

#include <argparse/argparse.hpp>
#include <fstream>

namespace li::program
{

using namespace std;


class Program
{
public:
	Program();
	int run(int argc, char* argv[]);

private:
	string read_file(const string& fileName);
	void change_write_file(const string& fileName);
	string get_source_file_name();
	void parse_source(const string& input);
	int repl();

private:
	argparse::ArgumentParser _program;
	ostream* _out;
	ofstream _outFile;
};


}
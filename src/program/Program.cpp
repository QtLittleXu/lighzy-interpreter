#include <fstream>
#include <filesystem>
#include "program/Program.h"
#include "program/initialization.h"
#include "parser/Parser.h"
#include "evaluator/Evaluator.h"
#include "config.h"

namespace li::program
{


const filesystem::path Program::PREREAD_SOURCES_PATH = executable_path().parent_path().parent_path() / PREREAD_SOURCES_DIR;

Program::Program(int argc, char* argv[]) :
	_program(EXECUTABLE_NAME, PROJECT_VERSION),
	_out(&cout)
{
	_program.add_description(EXECUTABLE_DESCRIPTION);

	_program.add_argument("source")
		.remaining()
		.nargs(argparse::nargs_pattern::optional)
		.help("input source");

	_program.add_argument("--repl", "-r")
		.flag()
		.help("run in Read-Evaluate-Print-Loop mode");
	
	_program.add_argument("--output", "-o")
		.help("specify the output file, the default is standard output");

	for (int i = 0; i < argc; i++)
	{
		_argv.push_back(argv[i]);
	}
}

string Program::get_source_file_name()
{
	string fileName;
	try
	{
		fileName = _program.get<string>("source");
	}
	catch (const logic_error& err)
	{
		cerr << "No input source! \n";
		cerr << _program;
		exit(-1);
	}
	return fileName;
}

int Program::run()
{
    try
    {
        _program.parse_args(_argv);
    }
    catch (const exception& err)
    {
		cerr << err.what() << '\n';
		cerr << _program;
		return -1;
    }

	if (_program["--repl"] == true)
	{
		return repl();
	}

	string fileName = get_source_file_name();
	string input = read_file(fileName);

	if (auto outFileName = _program.present("--output"))
	{
		change_write_file(outFileName.value());
	}

	parse_source(input, make_shared<Environment>());
	return 0;
}

void Program::change_write_file(const string& fileName)
{
	_outFile.open(fileName, ios::out | ios::trunc);
	if (!_outFile)
	{
		cerr << "File \"" << fileName << "\" write failed! \n";
		exit(-1);
	}
	_out = &_outFile;
}

void Program::parse_source(const string& input, shared_ptr<Environment> env)
{
	auto lexer = make_shared<li::Lexer>(read_folder_sources(PREREAD_SOURCES_PATH) + input);
	auto parser = make_shared<li::Parser>(lexer);
	auto program = parser->parseProgram();

	if (!parser->outputs().empty())
	{
		cerr << "parser has " << parser->outputs().size() << " outputs: " << '\n';
		for (const auto& o : parser->outputs())
		{
			cerr << o << '\n';
		}
		return;
	}

	auto evaluator = make_shared<li::Evaluator>();
	auto obj = evaluator->evaluate(program, env);
	
	if (!obj || obj->type == Object::Type::Null)
	{
		return;
	}

	if (obj->type == Object::Type::Error)
	{
		cerr << "evaluator has an error: " << '\n';
	}

	*_out << obj->inspect() << '\n';
}

string Program::read_file(const string& fileName)
{
	string output;
	string buffer;
	ifstream in(fileName, ios::in);
	if (!in)
	{
		cerr << "File \"" << fileName << "\" read failed! \n";
		exit(-1);
	}

	while (getline(in, buffer))
	{
		output += buffer + '\n';
	}
	return output;
}

string Program::read_folder_sources(const filesystem::path& folder)
{
    if (!filesystem::is_directory(folder))
	{
		cerr << "cannot read standard library! " << '\n';
		return "";
	}

	string sources;
	for (const auto& entry : filesystem::directory_iterator(folder))
	{
		if (filesystem::is_directory(entry))
        {
            sources += read_folder_sources(entry.path());
            continue;
        }

		string fileName = entry.path().string();
		sources += read_file(fileName);
	}
	return sources;
}

int Program::repl()
{
	string input;
	const string PROMPT = ">> ";
	auto env = make_shared<Environment>();
	
	cout << "Welcome to lighzy-interpreter! \n";
	cout << PROMPT;
	while (getline(cin, input))
	{
		if (input == "exit")
		{
			break;
		}

		parse_source(input, env);
		cout << PROMPT;
	}

	return 0;
}


}
#include <fstream>
#include <filesystem>
#include "program/Program.h"
#include "parser/Parser.h"
#include "evaluator/Evaluator.h"
#include "evaluator/BuiltinFuns.h"
#include "config.h"

namespace li::program
{


const string Program::PREREAD_SOURCES_PATH = "../preread-sources";

Program::Program() : _program(EXECUTABLE_NAME, PROJECT_VERSION), _out(&cout)
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

	auto& group = _program.add_mutually_exclusive_group();
	group.add_argument("--lexer", "-l")
		.flag()
		.help("only run lexer to generate tokens (json format)");

	group.add_argument("--parser", "-p")
		.flag()
		.help("only run lexer and parser to generate ast (json format)");
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

int Program::run(int argc, char* argv[])
{
    try
    {
        _program.parse_args(argc, argv);
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

void Program::parse_source(const string& input, const shared_ptr<Environment>& env)
{
	auto lexer = make_shared<li::Lexer>(read_preread_sources(PREREAD_SOURCES_PATH) + input);
	auto parser = make_shared<li::Parser>(lexer);
	auto program = parser->parseProgram();

	if (!parser->outputs().empty())
	{
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

string Program::read_preread_sources(const string& folderName)
{
	filesystem::path path(folderName);
    if (!filesystem::exists(path) ||
		!filesystem::is_directory(path))
	{
		stringstream read_error;
		read_error << "_builtin_(" << BuiltinFuns::Print << R"(, "cannot read preread sources! ", true))";
		return read_error.str();
	}

	string sources;
	for (const auto& entry : filesystem::directory_iterator(path))
	{
		if (filesystem::is_directory(entry))
        {
            sources += read_preread_sources(entry.path());
            continue;
        }

		string fileName = entry.path();
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
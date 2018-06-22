#include <iostream>
#include <fstream>

#include "program.hpp"
#include "skipper.hpp"
#include "compiler.hpp"

using namespace std;

int main(int argc, char* argv[])
{
   char const* filename;
    if (argc > 1)
    {
        filename = argv[1];
    }
    else {
		std::cerr << "Error: No input file provided." << std::endl;
		return 1;
    }

    std::ifstream in(filename, std::ios_base::in);

    if (!in)
    {
        std::cerr << "Error: Could not open input file: "
            << filename << std::endl;
        return 1;
    }

   std::string source_code; 		// We will read the contents here.
   in.unsetf(std::ios::skipws); 	// No white space skipping!
   std::copy(
      std::istream_iterator<char>(in),
      std::istream_iterator<char>(),
      std::back_inserter(source_code));

   using boost::spirit::ascii::space;
	using boost::spirit::qi::phrase_parse;
	typedef std::string::const_iterator iterator_type;

   iterator_type iter = source_code.begin();
   iterator_type end = source_code.end();

	algc::vmachine vm;
   algc::ast::program ast;														// Our AST
   algc::error_handler<iterator_type> error_handler(iter, end);	// Our error handler
	algc::parser::program<iterator_type> program(error_handler);	//	Our parser
   algc::parser::skipper<iterator_type> skipper;						// Our skipper
	algc::code_gen::compiler compiler(error_handler);					// Our compiler

	bool success = phrase_parse(iter, end, program, skipper, ast);


	if (success && iter == end ) {
		if (compiler(ast)) {
			/*
			std::cout << "Compile Success\n\n";
			std::cout << ast.program_name.name << "------------------\n";
			std::cout << "-----------------------------------\n";
			std::cout << "Assembler--------------------------\n\n";
			compiler.print_assembler();
			*/

			std::cout << "\n\n-----------------------------------\n";
			std::cout << "Execution : " << ast.program_name.name << std::endl;
			std::cout << "-----------------------------------\n\n";

			try {
				algc::data r = vm.execute(compiler.get_code());
			}
			catch (std::exception& e) {
				std::cout << "Error : " << e.what();
			}
			std::cout << "\n\n-----------------------------------\n";
			std::cout << "Terminated\n";
			std::cout << "-----------------------------------\n\n";
		}
		else {
			std::cout << "Compile failure\n";
		}
	} else {
		std::cout << "Parse Failure." << std::endl;
	}

}

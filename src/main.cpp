#include <iostream>
#include <fstream>

#include "program.hpp"
#include "skipper.hpp"

using namespace std;

int main(int argc, char* argv[])
{
   char const* filename;
    if (argc > 1)
    {
        filename = argv[1];
    }
    else
    {
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

   std::string source_code; // We will read the contents here.
   in.unsetf(std::ios::skipws); // No white space skipping!
   std::copy(
      std::istream_iterator<char>(in),
      std::istream_iterator<char>(),
      std::back_inserter(source_code));

   using boost::spirit::ascii::space;
	using boost::spirit::qi::phrase_parse;
	typedef std::string::const_iterator iterator_type;

   iterator_type iter = source_code.begin();
   iterator_type end = source_code.end();

   client::error_handler<iterator_type> error_handler(iter, end);
   client::parser::skipper<iterator_type> skipper;
	client::parser::program<iterator_type> prog(error_handler);

	bool success = phrase_parse(iter, end, prog, skipper);

	if (success && iter == end ) {
		std::cout << "Parsing success." << std::endl;
	} else {
		std::cout << "Parsing Failure." << std::endl;
	}

}

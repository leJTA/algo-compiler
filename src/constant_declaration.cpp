#include "constant_declaration.hpp"

typedef std::string::const_iterator iterator_type;
template struct algc::parser::constant_declaration<iterator_type>;

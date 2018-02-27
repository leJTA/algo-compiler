#ifndef __FUNCTION_DEFINITION_HPP__
#define __FUNCTION_DEFINITION_HPP__

#include <boost/spirit/include/qi.hpp>

#include "statement.hpp"
#include "variable_declaration.hpp"

namespace algc {
   namespace parser {
      ///////////////////////////////////////////////////////////////////////////////
      //  The function grammar
      ///////////////////////////////////////////////////////////////////////////////
      template <typename Iterator>
      struct function_definition : qi::grammar<Iterator, ast::function_definition_list(), skipper<Iterator> >
      {
        function_definition(error_handler<Iterator>& error_handler);

        qi::rule<Iterator, ast::function_definition_list(), skipper<Iterator> > function_definition_list;
        qi::rule<Iterator, ast::function_definition(), skipper<Iterator> > function_definition_;
        qi::rule<Iterator, std::string(), skipper<Iterator> > name;
        qi::rule<Iterator, ast::identifier(), skipper<Iterator> > identifier;
        qi::rule<Iterator, ast::args_list_type(), skipper<Iterator> > argument_list;
        qi::rule<Iterator, ast::type(), skipper<Iterator> > type;
        variable_declaration<Iterator> vars;
        statement<Iterator> body;
      };
   }
}

#endif // __FUNCTION_DEFINITION_HPP__

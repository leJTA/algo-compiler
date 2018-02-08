#ifndef __PROGRAM_HPP__
#define __PROGRAM_HPP__

#include <boost/spirit/include/qi.hpp>

#include "expression.hpp"
#include "skipper.hpp"
#include "error_handler.hpp"

namespace client {
   namespace parser {
      namespace qi = boost::spirit::qi;
      namespace ascii = boost::spirit::ascii;

      template <typename Iterator>
      struct program : qi::grammar<Iterator, ast::program(), skipper<Iterator> > {
         program(error_handler<Iterator>& error_handler);

         expression<Iterator> expr;
         qi::rule<Iterator, ast::program(), skipper<Iterator> > start;
         qi::rule<Iterator, std::string(), skipper<Iterator> > identifier;
      };
   }
}

#endif // __PROGRAM_HPP__

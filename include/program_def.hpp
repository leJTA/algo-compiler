#ifndef __PROGRAM_DEF_HPP__
#define __PROGRAM_DEF_HPP__

#include "program.hpp"
#include "error_handler.hpp"

namespace client {
   namespace parser {

      template <typename Iterator>
      program<Iterator>::program(error_handler<Iterator>& error_handler)
      : program::base_type(start), expr(error_handler)
      {
         qi::_1_type _1;
         qi::_2_type _2;
         qi::_3_type _3;
         qi::_4_type _4;

         qi::lexeme_type lexeme;
         qi::alpha_type alpha;
         qi::alnum_type alnum;
         qi::raw_type raw;

         using qi::on_error;
         using qi::fail;
         using boost::phoenix::function;

         typedef function<client::error_handler<Iterator> > error_handler_function;

         identifier = !expr.keywords
            >> raw[lexeme[(alpha | '_') >> *(alnum | '_')]];

         start =
               lexeme["algorithm" >> !(alnum | '_')] > identifier
            > -(lexeme["constants" >> !(alnum | '_')])
            > -(lexeme["variables" >> !(alnum | '_')])
            > -(lexeme["functions" >> !(alnum | '_')])
            > lexeme["begin" >> !(alnum | '_')] > +(identifier) > "end";

            // Error handling: on error in statement_list, call error_handler.
            on_error<fail>(start,
               error_handler_function(error_handler)(
                  "Error! Expecting ", _4, _3));
      }
   }
}

#endif // __PROGRAM_DEF_HPP__

#ifndef __FUNCTION_DEFINITION_DEF_HPP__
#define __FUNCTION_DEFINITION_DEF_HPP__

#include "function_definition.hpp"
#include "error_handler.hpp"

namespace algc {
   namespace parser {

      template<typename Iterator>
      function_definition<Iterator>::function_definition(error_handler<Iterator>& error_handler)
         : function_definition::base_type(function_definition_list), body(error_handler), vars(error_handler)
      {
         qi::_1_type _1;
         qi::_2_type _2;
         qi::_3_type _3;
         qi::_4_type _4;

         qi::_val_type _val;
         qi::lit_type lit;
         qi::raw_type raw;
         qi::lexeme_type lexeme;
         qi::alpha_type alpha;
         qi::alnum_type alnum;

         using qi::on_error;
         using qi::on_success;
         using qi::fail;
         using boost::phoenix::function;

         typedef function<algc::error_handler<Iterator> > error_handler_function;

         name =
               !lexeme[body.expr.keywords >> !(alnum | '_')]
            >> raw[lexeme[(alpha | '_') >> *(alnum | '_')]]
            ;

         identifier = name;

         argument_list = -((type > identifier) % ',');

         type =
              lit("integer")
            | lit("real")
            | lit("boolean")
            | lit("character")
            | lit("string")
            | lit("array_of_integer")
            | lit("array_of_real")
            | lit("array_of_boolean")
            | lit("array_of_character")
            | lit("array_of_string")
            ;

         function_definition_list =
              lexeme["functions_and_procedures" >> !(alnum | '_')]
            > +function_definition_
            ;

         function_definition_ =
            (
               (
                    lexeme["function" >> !(alnum | '_')]
                  > identifier
                  > '(' >> argument_list >> ')'
                  > lexeme["returns" >> !(alnum | '_')]
                  > type
               )
               |
               (
                    lexeme["procedure" >> !(alnum | '_')]
                  > identifier
                  > '(' >> argument_list >> ')'
               )
            )
            > -(vars)
            > lexeme["begin" >> !(alnum | '_')] > body > lexeme["end" >> !(alnum | '_')]
            ;

         // Debugging and error handling and reporting support.
        BOOST_SPIRIT_DEBUG_NODES(
            (identifier)
            (type)
            (argument_list)
            (function_definition_)
        );

        // Error handling: on error in start, call error_handler.
        on_error<fail>(function_definition_list,
            error_handler_function(error_handler)(
                "Error! Expecting ", _4, _3));
      }
   }
}

#endif // __FUNCTION_DEFINITION_DEF_HPP__

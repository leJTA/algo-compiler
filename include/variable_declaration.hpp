#ifndef __VARIABLE_DECLARATION_HPP__
#define __VARIABLE_DECLARATION_HPP__

#include "ast.hpp"
#include "skipper.hpp"
#include "error_handler.hpp"
#include "expression.hpp"

namespace client {
   namespace parser {
      namespace qi = boost::spirit::qi;
      namespace ascii = boost::spirit::ascii;

      template<typename Iterator>
      struct variable_declaration
         : qi::grammar<Iterator, client::ast::variable_declaration_list(), skipper<Iterator> >
      {
         variable_declaration(error_handler<Iterator>& error_handler)
            : variable_declaration::base_type(variable_declaration_list), expr(error_handler)
         {
            qi::_1_type _1;
            qi::_2_type _2;
            qi::_3_type _3;
            qi::_4_type _4;

            qi::lit_type lit;
            qi::raw_type raw;
            qi::lexeme_type lexeme;
            qi::alpha_type alpha;
            qi::alnum_type alnum;
            qi::blank_type blank;

            using qi::on_error;
            using qi::fail;
            using qi::eol;
            using qi::no_skip;
            using boost::phoenix::function;

            typedef function<client::error_handler<Iterator> > error_handler_function;

            name =
                 !lexeme[expr.keywords >> !(alnum | '_')]
               >> raw[lexeme[(alpha | '_') >> *(alnum | '_')]]
               ;

            identifier = name;

            type = (
                 lit("integer")
               | lit("real")
               | lit("boolean")
               | lit("string")
               | lit("array_of_integer")
               | lit("array_of_double")
               | lit("array_of_character")
               | lit("array_of_string")
               )
               ;

            variable_declaration_list =
                 lexeme["variables" >> !(alnum | '_')]
               > +variable_declaration_
               ;

            variable_declaration_ =
                 type
               > (identifier % ',')
               > no_skip[*blank >> eol]
               ;

            BOOST_SPIRIT_DEBUG_NODES(
               (variable_declaration_)
               (type)
               (identifier)
            );


            ///////////////////////////////////////////////////////////////////////
            // Error handling: on error in expr, call error_handler.
            on_error<fail>(variable_declaration_list,
            error_handler_function(error_handler)(
                "Error! Expecting ", _4, _3));

         }

         qi::rule<Iterator, ast::variable_declaration(), skipper<Iterator> > variable_declaration_;
         qi::rule<Iterator, ast::variable_declaration_list(), skipper<Iterator> > variable_declaration_list;
         qi::rule<Iterator, ast::type(), skipper<Iterator> > type;
         qi::rule<Iterator, std::string(), skipper<Iterator> > name;
         qi::rule<Iterator, ast::identifier(), skipper<Iterator> > identifier;

         expression<Iterator> expr; /// used only to retrieve keywords :(
      };
   }
}


#endif // __VARIABLE_DECLARATION_HPP__

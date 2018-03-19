#ifndef __CONSTANT_DECLARATION_HPP__
#define __CONSTANT_DECLARATION_HPP__

#include "ast.hpp"
#include "skipper.hpp"
#include "error_handler.hpp"
#include "annotation.hpp"
#include "expression.hpp"

namespace algc {
   namespace parser {
      namespace qi = boost::spirit::qi;
      namespace ascii = boost::spirit::ascii;

      template<typename Iterator>
      struct constant_declaration
         : qi::grammar<Iterator, algc::ast::constant_declaration_list(), skipper<Iterator> >
      {
         constant_declaration(error_handler<Iterator>& error_handler)
            : constant_declaration::base_type(constant_declaration_list), expr(error_handler)
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
            qi::uint_type uint_;
            qi::blank_type blank;

            using qi::on_error;
            using qi::fail;
            using qi::eol;
            using qi::no_skip;
            using boost::phoenix::function;

            typedef function<algc::error_handler<Iterator> > error_handler_function;
            typedef function<algc::annotation<Iterator> > annotation_function;

            name =
                 !lexeme[expr.keywords >> !(alnum | '_')]
               >> raw[lexeme[(alpha | '_') >> *(alnum | '_')]]
               ;

            identifier = name;

            primitive_type =
                 lit("integer")
               | lit("real")
               | lit("boolean")
               | lit("character")
               | lit("string")
               ;

            array_type =
                 lit("array_of_integer")
               | lit("array_of_real")
               | lit("array_of_boolean")
               | lit("array_of_character")
               | lit("array_of_string")
               ;

            constant_declaration_list =
                 lexeme["constants" >> !(alnum | '_')]
               > +constant_declaration_
               ;

            constant_declaration_ =
               (
                  (
                       primitive_type
                     > ((identifier > '=' >> expr) % ',')
                  )
                  |
                  (
                       array_type
                     > ((identifier > lit('=') >> '[' >> (expr % ',') >> ']') % ',')
                  )
               )
               > no_skip[*blank >> eol]
               ;

            BOOST_SPIRIT_DEBUG_NODES(
               (constant_declaration_)
               (primitive_type)
               (array_type)
               (identifier)
            );


            ///////////////////////////////////////////////////////////////////////
            // Error handling: on error in expr, call error_handler.
            on_error<fail>(constant_declaration_list,
            error_handler_function(error_handler)(
                "Error! Expecting ", _4, _3));

            // Annotation: on success in start, call annotation.
            on_success(identifier,
               annotation_function(error_handler.iters)(_val, _1));

         }

         qi::rule<Iterator, ast::constant_declaration(), skipper<Iterator> > constant_declaration_;
         qi::rule<Iterator, ast::constant_declaration_list(), skipper<Iterator> > constant_declaration_list;
         qi::rule<Iterator, ast::type_id(), skipper<Iterator> > primitive_type;
         qi::rule<Iterator, ast::type_id(), skipper<Iterator> > array_type;
         qi::rule<Iterator, std::string(), skipper<Iterator> > name;
         qi::rule<Iterator, ast::identifier(), skipper<Iterator> > identifier;

         expression<Iterator> expr;
      };
   }
}

#endif // __CONSTANT_DECLARATION_HPP__

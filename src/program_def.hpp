#ifndef __PROGRAM_DEF_HPP__
#define __PROGRAM_DEF_HPP__

#include "program.hpp"
#include "error_handler.hpp"
#include "annotation.hpp"

namespace algc {
   namespace parser {

      template <typename Iterator>
      program<Iterator>::program(error_handler<Iterator>& error_handler)
      : program::base_type(start), body(error_handler), consts(error_handler)
        ,vars(error_handler), funcs(error_handler)
      {
         qi::_1_type _1;
         qi::_2_type _2;
         qi::_3_type _3;
         qi::_4_type _4;

         qi::_val_type _val;

         qi::lexeme_type lexeme;
         qi::alpha_type alpha;
         qi::alnum_type alnum;
         qi::raw_type raw;
         qi::blank_type blank;
         qi::lit_type lit;
         qi::double_type double_;
         qi::bool_type bool_;

         using qi::on_error;
         using qi::fail;
         using qi::eol;
         using qi::no_skip;
         using boost::phoenix::function;

         typedef function<algc::error_handler<Iterator> > error_handler_function;
         typedef function<algc::annotation<Iterator> > annotation_function;

         name =
               !lexeme[body.expr.keywords >> !(alnum | '_')]
            >> raw[lexeme[(alpha | '_') >> *(alnum | '_')]]
            ;

         identifier = name;

         start =
               lexeme["algorithm" >> !(alnum | '_')] > identifier
            > -(consts)
            > -(vars)
            > -(funcs)
            > lexeme["begin" >> !(alnum | '_')] > body > lexeme["end" >> !(alnum | '_')]
            ;

         BOOST_SPIRIT_DEBUG_NODES(
            (body)
            (identifier)
         );

         // Error handling: on error in statement_list, call error_handler.
         on_error<fail>(start,
            error_handler_function(error_handler)(
               "Error! Expecting ", _4, _3));

         // Annotation: on success in start, call annotation.
         on_success(identifier,
            annotation_function(error_handler.iters)(_val, _1));
      }
   }
}

#endif // __PROGRAM_DEF_HPP__

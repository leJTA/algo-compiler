#ifndef __EXPRESSION_DEF_HPP__
#define __EXPRESSION_DEF_HPP__

#include "expression.hpp"
#include "error_handler.hpp"
#include <boost/spirit/include/phoenix_function.hpp>

namespace client {
   namespace parser {
      template <typename Iterator>
      expression<Iterator>::expression(error_handler<Iterator>& error_handler) : expression::base_type(expr)
      {
         qi::_1_type _1;
         qi::_2_type _2;
         qi::_3_type _3;
         qi::_4_type _4;

         qi::char_type char_;
         qi::uint_type uint_;
         qi::_val_type _val;
         qi::raw_type raw;
         qi::lexeme_type lexeme;
         qi::alpha_type alpha;
         qi::alnum_type alnum;
         qi::bool_type bool_;

         using qi::on_error;
         using qi::on_success;
         using qi::fail;
         using boost::phoenix::function;

         typedef function<client::error_handler<Iterator> > error_handler_function;
         //typedef function<client::annotation<Iterator> > annotation_function;

         ///////////////////////////////////////////////////////////////////////
         // Tokens
         logical_or_op.add
            ("||", ast::op_or)
            ;

         logical_and_op.add
            ("&&", ast::op_and)
            ;

         equality_op.add
            ("==", ast::op_equal)
            ("!=", ast::op_not_equal)
            ;

         relational_op.add
            ("<", ast::op_less)
            ("<=", ast::op_less_equal)
            (">", ast::op_greater)
            (">=", ast::op_greater_equal)
            ;

         additive_op.add
            ("+", ast::op_plus)
            ("-", ast::op_minus)
            ;

         multiplicative_op.add
            ("*", ast::op_times)
            ("/", ast::op_divide)
            ("%", ast::op_modulo)
            ;

         unary_op.add
            ("+", ast::op_positive)
            ("-", ast::op_negative)
            ("!", ast::op_not)
            ;

         keywords.add
            ("algorithm")
            ("constants")
            ("variables")
            ("functions_and_procedures")
            ("function")
            ("procedure")
            ("begin")
            ("end")
            ("integer")
            ("real")
            ("boolean")
            ("character")
            ("string")
            ("array_of_int")
            ("array_of_double")
            ("array_of_character")
            ("array_of_string")
            ("true")
            ("false")
            ("if")
            ("then")
            ("else")
            ("endif")
            ("while")
            ("do")
            ("endwhile")
            ("for")
            ("to")
            ("by")
            ("endfor")
            ("repeat")
            ("until")
            ("read")
            ("write")
            ("return")
            ;

         ///////////////////////////////////////////////////////////////////////
         // Main expression grammar
         expr =
            logical_or_expr.alias()
            ;

         logical_or_expr =
                logical_and_expr
            >> *(logical_or_op > logical_and_expr)
            ;

         logical_and_expr =
                equality_expr
            >> *(logical_and_op > equality_expr)
            ;

         equality_expr =
                relational_expr
            >> *(equality_op > relational_expr)
            ;

         relational_expr =
                additive_expr
            >> *(relational_op > additive_expr)
            ;

         additive_expr =
                multiplicative_expr
            >> *(additive_op > multiplicative_expr)
            ;

         multiplicative_expr =
                unary_expr
            >> *(multiplicative_op > unary_expr)
            ;

         unary_expr =
                primary_expr
            |   (unary_op > unary_expr)
            ;

         primary_expr =
                uint_
            |   identifier
            |   bool_
            |   '(' > expr > ')'
            ;

         identifier =
                !lexeme[keywords >> !(alnum | '_')]
            >>  raw[lexeme[(alpha | '_') >> *(alnum | '_')]]
            ;

         ///////////////////////////////////////////////////////////////////////
         // Debugging and error handling and reporting support.
         BOOST_SPIRIT_DEBUG_NODES(
            (expr)
            (logical_or_expr)
            (logical_and_expr)
            (equality_expr)
            (relational_expr)
            (additive_expr)
            (multiplicative_expr)
            (unary_expr)
            (primary_expr)
            (identifier)
         );

         ///////////////////////////////////////////////////////////////////////
         // Error handling: on error in expr, call error_handler.
         on_error<fail>(expr,
            error_handler_function(error_handler)(
                "Error! Expecting ", _4, _3));

         ///////////////////////////////////////////////////////////////////////
         // Annotation: on success in primary_expr, call annotation.
         //on_success(primary_expr,
         //   annotation_function(error_handler.iters)(_val, _1));
      }
   }
}


#endif // __EXPRESSION_DEF_HPP__

#ifndef __EXPRESSION_HPP__
#define __EXPRESSION_HPP__

#include <boost/spirit/include/qi.hpp>
#include "ast.hpp"
#include "error_handler.hpp"
#include "skipper.hpp"
#include <vector>

namespace client {
   namespace parser {
      namespace qi = boost::spirit::qi;
      namespace ascii = boost::spirit::ascii;

      ///////////////////////////////////////////////////////////////////////////////
      //  The expression grammar
      ///////////////////////////////////////////////////////////////////////////////
      template <typename Iterator>
      struct expression : qi::grammar<Iterator, ast::expression(), skipper<Iterator> >
      {
         expression(error_handler<Iterator>& error_handler);

         qi::rule<Iterator, ast::expression(), skipper<Iterator> >
            expr, equality_expr, relational_expr,
            logical_or_expr, logical_and_expr,
            additive_expr, multiplicative_expr
            ;

         qi::rule<Iterator, ast::operand(), skipper<Iterator> > unary_expr, primary_expr;
         //qi::rule<Iterator, ast::function_call, skipper<Iterator> > function_call;
         //qi::rule<Iterator, std::list<ast::expression>(), skipper<Iterator> > argument_list;
         qi::rule<Iterator, std::string(), skipper<Iterator> > identifier;
         qi::symbols<char, ast::optoken>
            logical_or_op, logical_and_op,
            equality_op, relational_op,
            additive_op, multiplicative_op, unary_op
            ;

         qi::symbols<char> keywords;
      };
   }
}

#endif // __EXPRESSION_HPP__

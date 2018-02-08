#ifndef __STATEMENT_HPP__
#define __STATEMENT_HPP__

#include "expression.hpp"
/*
namespace client {
   namespace parser {
      ///////////////////////////////////////////////////////////////////////////////
      //  The statement grammar
      ///////////////////////////////////////////////////////////////////////////////
      template <typename Iterator>
      struct statement : qi::grammar<Iterator, ast::statement_list(), skipper<Iterator> >
      {
         statement(error_handler<Iterator>& error_handler);

         expression<Iterator> expr;
         qi::rule<Iterator, ast::statement_list(), skipper<Iterator> >
            statement_list, compound_statement;

         qi::rule<Iterator, ast::statement(), skipper<Iterator> > statement_;
         qi::rule<Iterator, ast::variable_declaration(), skipper<Iterator> > variable_declaration;
         qi::rule<Iterator, ast::assignment(), skipper<Iterator> > assignment;
         qi::rule<Iterator, ast::if_statement(), skipper<Iterator> > if_statement;
         qi::rule<Iterator, ast::while_statement(), skipper<Iterator> > while_statement;
         qi::rule<Iterator, ast::identifier(), skipper<Iterator> > identifier;
      };
   }
}*/

#endif // __STATEMENT_HPP__

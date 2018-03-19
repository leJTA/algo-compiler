#ifndef __STATEMENT_HPP__
#define __STATEMENT_HPP__

#include "expression.hpp"

namespace algc {
   namespace parser {
      using boost::spirit::qi::ascii::blank_type;
      ///////////////////////////////////////////////////////////////////////////////
      //  The statement grammar
      ///////////////////////////////////////////////////////////////////////////////
      template <typename Iterator>
      struct statement : qi::grammar<Iterator, ast::statement_list(), skipper<Iterator> >
      {
         statement(error_handler<Iterator>& error_handler);

         expression<Iterator> expr;
         qi::rule<Iterator, ast::statement_list(), skipper<Iterator> >
            statement_list;

         qi::rule<Iterator, ast::statement(), skipper<Iterator> > statement_;
         qi::rule<Iterator, ast::assignment(), skipper<Iterator> > assignment;
         qi::rule<Iterator, ast::read_statement(), skipper<Iterator> > read_statement;
         qi::rule<Iterator, ast::write_statement(), skipper<Iterator> > write_statement;
         qi::rule<Iterator, ast::if_statement(), skipper<Iterator> > if_statement;
         qi::rule<Iterator, ast::while_statement(), skipper<Iterator> > while_statement;
         qi::rule<Iterator, ast::repeat_until_statement(), skipper<Iterator> > repeat_until_statement;
         qi::rule<Iterator, ast::for_statement(), skipper<Iterator> > for_statement;
         qi::rule<Iterator, ast::return_statement(), skipper<Iterator> > return_statement;
         qi::rule<Iterator, ast::procedure_call_statement(), skipper<Iterator> > procedure_call_statement;
         qi::rule<Iterator, std::string(), skipper<Iterator> > identifier;
      };
   }
}

#endif // __STATEMENT_HPP__

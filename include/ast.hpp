#ifndef __AST_HPP__
#define __AST_HPP__

#include <boost/config/warning_disable.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/optional.hpp>
#include <list>

namespace client {
   namespace ast {
      ///////////////////////////////////////////////////////////////////////////
      //  The AST
      ///////////////////////////////////////////////////////////////////////////
      struct tagged
      {
        int id; // Used to annotate the AST with the iterator position.
                // This id is used as a key to a map<int, Iterator>
                // (not really part of the AST.)
      };

      struct nil {};
      struct unary;
      struct expression;

      struct identifier : tagged
      {
         identifier(std::string const& name = "") : name(name) {}
         std::string name;
      };

      struct type {
         type(std::string const& name = ""): name(name){}
         std::string name;
      };

      typedef boost::variant<
            nil
          , bool
          , double
          , int
          , std::string
          , identifier
          , boost::recursive_wrapper<unary>
          , boost::recursive_wrapper<expression>
        >
      operand;

      enum optoken
      {
        op_plus,
        op_minus,
        op_times,
        op_power,
        op_divide,
        op_modulo,
        op_positive,
        op_negative,
        op_not,
        op_equal,
        op_not_equal,
        op_less,
        op_less_equal,
        op_greater,
        op_greater_equal,
        op_and,
        op_or
      };

      struct unary
      {
        optoken operator_;
        operand operand_;
      };

      struct operation
      {
        optoken operator_;
        operand operand_;
      };

      struct expression
      {
        operand first;
        std::list<operation> rest;
      };

      struct assignment
      {
        identifier lhs;
        expression rhs;
      };

      typedef boost::variant<
          boost::fusion::vector<type, std::list<identifier> >
         ,boost::fusion::vector<type, std::list<boost::fusion::vector<identifier, unsigned int> > >
         >
         variable_declaration_type;

      struct variable_declaration
      {
         variable_declaration_type variable_list;
      };

      struct variable_declaration_list : std::list<variable_declaration> {};

      typedef boost::variant<
           boost::fusion::vector<type, std::list<boost::fusion::vector<identifier, expression> > >
         , boost::fusion::vector<type, std::list<boost::fusion::vector<identifier, std::list<expression> > > >
      >
      constant_declaration_type;

      struct constant_declaration {
          constant_declaration_type constant_list;
      };

      struct constant_declaration_list : std::list<constant_declaration> {};

      struct read_statement;
      struct write_statement;
      struct if_statement;
      struct while_statement;
      struct repeat_until_statement;
      struct for_statement;
      struct return_statement;
      struct statement_list;

      typedef boost::variant<
           variable_declaration
         , constant_declaration
         , assignment
         , read_statement
         , write_statement
         , boost::recursive_wrapper<if_statement>
         , boost::recursive_wrapper<while_statement>
         , boost::recursive_wrapper<repeat_until_statement>
         , boost::recursive_wrapper<for_statement>
         , boost::recursive_wrapper<return_statement>
         , boost::recursive_wrapper<statement_list>
        >
      statement;

      struct statement_list : std::list<statement> {};

      struct read_statement {
         identifier variable;
      };

      struct write_statement {
         std::list<expression> expression_list;
      };

      struct if_statement
      {
        expression condition;
        statement then;
        boost::optional<statement> else_;
      };

      struct while_statement
      {
        expression condition;
        statement body;
      };

      struct repeat_until_statement {
         statement body;
         expression condition;
      };

      struct for_statement {
         identifier counter;
         expression start;
         expression end;
         boost::optional<expression> step;
         statement body;
      };

      struct return_statement : tagged
      {
         expression expr;
      };

      typedef std::list<boost::fusion::vector<type, identifier> > args_list_type;
      typedef boost::variant<
           boost::fusion::vector<identifier, args_list_type, type>
         , boost::fusion::vector<identifier, args_list_type>
      >
      header_type;

      struct function_definition {
         header_type header;
         boost::optional<variable_declaration_list> local_vars;
         statement body;
      };

      struct function_definition_list : std::list<function_definition> {};

      struct program {
         identifier program_name;
         boost::optional<constant_declaration_list> consts;
         boost::optional<variable_declaration_list> vars;
         boost::optional<function_definition_list> funcs;
         statement body;
      };

      // print functions for debugging
      inline std::ostream& operator<<(std::ostream& out, nil)
      {
         out << "nil";
         return out;
      }

      inline std::ostream& operator<<(std::ostream& out, identifier const& id)
      {
         out << id.name;
         return out;
      }
   }
}

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::unary,
    (client::ast::optoken, operator_)
    (client::ast::operand, operand_)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::operation,
    (client::ast::optoken, operator_)
    (client::ast::operand, operand_)
)


BOOST_FUSION_ADAPT_STRUCT(
    client::ast::expression,
    (client::ast::operand, first)
    (std::list<client::ast::operation>, rest)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::variable_declaration,
    (client::ast::variable_declaration_type, variable_list)
)

BOOST_FUSION_ADAPT_STRUCT(
   client::ast::constant_declaration,
   (client::ast::constant_declaration_type, constant_list)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::assignment,
    (client::ast::identifier, lhs)
    (client::ast::expression, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::read_statement,
    (client::ast::identifier, variable)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::write_statement,
    (std::list<client::ast::expression>, expression_list)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::if_statement,
    (client::ast::expression, condition)
    (client::ast::statement, then)
    (boost::optional<client::ast::statement>, else_)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::while_statement,
    (client::ast::expression, condition)
    (client::ast::statement, body)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::repeat_until_statement,
    (client::ast::statement, body)
    (client::ast::expression, condition)
)

BOOST_FUSION_ADAPT_STRUCT(
   client::ast::for_statement,
   (client::ast::identifier, counter)
   (client::ast::expression, start)
   (client::ast::expression, end)
   (boost::optional<client::ast::expression>, step)
   (client::ast::statement, body)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::return_statement,
    (client::ast::expression, expr)
)

BOOST_FUSION_ADAPT_STRUCT(
   client::ast::function_definition,
   (client::ast::header_type, header)
   (boost::optional<client::ast::variable_declaration_list>, local_vars)
   (client::ast::statement, body)
)

BOOST_FUSION_ADAPT_STRUCT(
   client::ast::program,
   (client::ast::identifier, program_name)
   (boost::optional<client::ast::constant_declaration_list>, consts)
   (boost::optional<client::ast::variable_declaration_list>, vars)
   (boost::optional<client::ast::function_definition_list>, funcs)
   (client::ast::statement, body)
)

#endif // __AST_HPP__

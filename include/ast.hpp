#ifndef __AST_HPP__
#define __AST_HPP__

#include <boost/config/warning_disable.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/optional.hpp>
#include <list>

namespace algc {
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
      struct array_element_access;
      struct function_call;
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
          , identifier
          , char
          , std::string
          , boost::recursive_wrapper<unary>
          , boost::recursive_wrapper<array_element_access>
          , boost::recursive_wrapper<function_call>
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

      struct array_element_access {
         identifier array_name;
         operand index;
      };

      struct function_call
      {
         identifier function_name;
         std::list<expression> args;
      };

      struct expression
      {
        operand first;
        std::list<operation> rest;
      };

      struct assignment
      {
        identifier lhs;
        boost::optional<expression> index;
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
      struct procedure_call_statement;
      struct statement_list;

      typedef boost::variant<
           assignment
         , read_statement
         , write_statement
         , boost::recursive_wrapper<if_statement>
         , boost::recursive_wrapper<while_statement>
         , boost::recursive_wrapper<repeat_until_statement>
         , boost::recursive_wrapper<for_statement>
         , boost::recursive_wrapper<return_statement>
         , boost::recursive_wrapper<procedure_call_statement>
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

      struct procedure_call_statement {
         identifier procedure_name;
         std::list<expression> args;
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
    algc::ast::unary,
    (algc::ast::optoken, operator_)
    (algc::ast::operand, operand_)
)

BOOST_FUSION_ADAPT_STRUCT(
    algc::ast::operation,
    (algc::ast::optoken, operator_)
    (algc::ast::operand, operand_)
)

BOOST_FUSION_ADAPT_STRUCT(
   algc::ast::array_element_access,
   (algc::ast::identifier, array_name)
   (algc::ast::operand, index)
)

BOOST_FUSION_ADAPT_STRUCT(
   algc::ast::function_call,
   (algc::ast::identifier, function_name)
   (std::list<algc::ast::expression>, args)
)

BOOST_FUSION_ADAPT_STRUCT(
    algc::ast::expression,
    (algc::ast::operand, first)
    (std::list<algc::ast::operation>, rest)
)

BOOST_FUSION_ADAPT_STRUCT(
    algc::ast::variable_declaration,
    (algc::ast::variable_declaration_type, variable_list)
)

BOOST_FUSION_ADAPT_STRUCT(
   algc::ast::constant_declaration,
   (algc::ast::constant_declaration_type, constant_list)
)

BOOST_FUSION_ADAPT_STRUCT(
   algc::ast::assignment,
   (algc::ast::identifier, lhs)
   (boost::optional<algc::ast::expression>, index)
   (algc::ast::expression, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(
    algc::ast::read_statement,
    (algc::ast::identifier, variable)
)

BOOST_FUSION_ADAPT_STRUCT(
    algc::ast::write_statement,
    (std::list<algc::ast::expression>, expression_list)
)

BOOST_FUSION_ADAPT_STRUCT(
    algc::ast::if_statement,
    (algc::ast::expression, condition)
    (algc::ast::statement, then)
    (boost::optional<algc::ast::statement>, else_)
)

BOOST_FUSION_ADAPT_STRUCT(
    algc::ast::while_statement,
    (algc::ast::expression, condition)
    (algc::ast::statement, body)
)

BOOST_FUSION_ADAPT_STRUCT(
    algc::ast::repeat_until_statement,
    (algc::ast::statement, body)
    (algc::ast::expression, condition)
)

BOOST_FUSION_ADAPT_STRUCT(
   algc::ast::for_statement,
   (algc::ast::identifier, counter)
   (algc::ast::expression, start)
   (algc::ast::expression, end)
   (boost::optional<algc::ast::expression>, step)
   (algc::ast::statement, body)
)

BOOST_FUSION_ADAPT_STRUCT(
    algc::ast::return_statement,
    (algc::ast::expression, expr)
)

BOOST_FUSION_ADAPT_STRUCT(
   algc::ast::procedure_call_statement,
   (algc::ast::identifier, procedure_name)
   (std::list<algc::ast::expression>, args)
)

BOOST_FUSION_ADAPT_STRUCT(
   algc::ast::function_definition,
   (algc::ast::header_type, header)
   (boost::optional<algc::ast::variable_declaration_list>, local_vars)
   (algc::ast::statement, body)
)

BOOST_FUSION_ADAPT_STRUCT(
   algc::ast::program,
   (algc::ast::identifier, program_name)
   (boost::optional<algc::ast::constant_declaration_list>, consts)
   (boost::optional<algc::ast::variable_declaration_list>, vars)
   (boost::optional<algc::ast::function_definition_list>, funcs)
   (algc::ast::statement, body)
)

#endif // __AST_HPP__

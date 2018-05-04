#ifndef __TYPE_SETTING_HPP__
#define __TYPE_SETTING_HPP__

#include <boost/variant/apply_visitor.hpp>
#include <boost/fusion/container/vector.hpp>
#include <type_traits>
#include <boost/fusion/include/at_c.hpp>
#include "ast.hpp"

namespace algc {

   template<typename Iterator>
   struct type_setting {

      template <typename, typename>
      struct result { typedef void type; };
      type_setting(std::vector<Iterator>& iters) : iters(iters) {}

      struct set_type {
         typedef void result_type;

         // primitive types
         void operator()(boost::fusion::vector<ast::type_name, std::list<ast::identifier> >& x) const
         {
            ast::type t = get_type(boost::fusion::at_c<0>(x));
            std::cout << "variables : " << t << std::endl;
            for(ast::identifier& id : boost::fusion::at_c<1>(x)) {
               id.t = t;
               std::cout << id.name << ", ";
            }
            std::cout << std::endl;
         }

         // arrays
         void operator()(boost::fusion::vector<ast::type_name, std::list<boost::fusion::vector<ast::identifier, unsigned int> > >& x) const
         {
            ast::type t = get_type(boost::fusion::at_c<0>(x));
            std::cout << "variables(arrays) : " << t << std::endl;
            for(auto& id : boost::fusion::at_c<1>(x)) {
               boost::fusion::at_c<0>(id).t = t;
               std::cout << boost::fusion::at_c<0>(id).name << ", ";
            }
            std::cout << std::endl;
         }

         // primitives types (constants)
         void operator()(boost::fusion::vector<ast::type_name, std::list<boost::fusion::vector<ast::identifier, ast::expression> > >& x) const
         {
            ast::type t = get_type(boost::fusion::at_c<0>(x));
            std::cout << "constants : " << t << std::endl;
            for(auto& id : boost::fusion::at_c<1>(x)) {
               boost::fusion::at_c<0>(id).t = t;
               std::cout << boost::fusion::at_c<0>(id).name << ", ";
            }
            std::cout << std::endl;
         }

         // arrays (constants)
         void operator()(boost::fusion::vector<ast::type_name, std::list<boost::fusion::vector<ast::identifier, std::list<ast::expression> > > >& x) const
         {
            ast::type t = get_type(boost::fusion::at_c<0>(x));
            std::cout << "constants(arrays) : " << t << std::endl;
            for(auto& id : boost::fusion::at_c<1>(x)) {
               boost::fusion::at_c<0>(id).t = t;
               std::cout << boost::fusion::at_c<0>(id).name << ", ";
            }
            std::cout << std::endl;
         }

         template<typename T>
         void operator()(T& x) const
         {
            // no-op
         }

         //returns the type according to the type name
         ast::type get_type(const std::string& type_name) const
         {
            if (type_name.compare("boolean") == 0) {
               return ast::type::t_boolean;
            } else if (type_name.compare("integer") == 0) {
               return ast::type::t_boolean;
            } else if (type_name.compare("character") == 0) {
               return ast::type::t_character;
            } else if (type_name.compare("real") == 0) {
               return ast::type::t_real;
            } else if (type_name.compare("string") == 0) {
               return ast::type::t_string;
            } else if (type_name.compare("array_of_integer") == 0) {
               return ast::type::t_array_of_integer;
            } else if (type_name.compare("array_of_real") == 0) {
               return ast::type::t_array_of_real;
            } else if (type_name.compare("array_of_boolean") == 0) {
               return ast::type::t_array_boolean;
            } else if (type_name.compare("array_of_character") == 0) {
               return ast::type::t_array_of_character;
            } else if (type_name.compare("array_of_string") == 0) {
               return ast::type::t_array_of_string;
            } else {
               return ast::type::t_invalid;
            }
         }
      };

      void operator()(ast::variable_declaration& ast, Iterator pos) const
      {
         boost::apply_visitor(set_type(), ast.variable_list);
      }

      void operator()(ast::constant_declaration& ast, Iterator pos) const
      {
         boost::apply_visitor(set_type(), ast.constant_list);
      }

      std::vector<Iterator>& iters;
   };
}

#endif // __TYPE_SETTING_HPP__

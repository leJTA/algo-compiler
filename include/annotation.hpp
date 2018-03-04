#ifndef __ANNOTATION_HPP__
#define __ANNOTATION_HPP__

#include <map>
#include <boost/variant/apply_visitor.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <type_traits>
#include <boost/mpl/bool.hpp>
#include "ast.hpp"

namespace algc {
   template<typename Iterator>
   struct annotation {

      template <typename, typename>
      struct result { typedef void type; };
      annotation(std::vector<Iterator>& iters) : iters(iters) {}

      struct set_id {
         typedef void result_type;

         int id;
         set_id(int id) : id(id) {}

         void operator()(ast::function_call& x) const
         {
            x.function_name.id = id;
         }

         void operator()(ast::identifier& x) const
         {
            x.id = id;
         }

         void operator()(ast::array_element_access& x) const
         {
            x.array_name.id = id;
         }

         template <typename T>
         void operator()(T& x) const
         {
            // no-op
         }
      };

      void operator()(ast::operand& ast, Iterator pos) const
      {
         int id = iters.size();
         iters.push_back(pos);
         boost::apply_visitor(set_id(id), ast);
      }

      /*
      void operator()(ast::variable_declaration& ast, Iterator pos) const
      {
         boost::apply_visitor([&ast, &pos](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, boost::fusion::vector<type, std::list<identifier> > >) {
               for (ast::identifier& ident, boost::fusion::at_c<1>(arg)) {
                  int id = iters.size();
                  iters.push_back(pos);
                  ident.id = id;
               }
            } else { /// std::is_same_v<T, boost::fusion::vector<type, std::list<boost::fusion::vector<identifier, unsigned int> > > >)
               for (fusion::vector<identifier, unsigned int>& id_size, boost::fusion::at_c<1>(arg)) {
                  int id = iters.size();
                  iters.push_back(pos);
                  boost::fusion::at_c<0>(id_size).id = id;
               }
            }
         }, ast.variable_list);
      }

      void operator()(ast::constant_declaration& ast, Iterator pos)
      {
         boost::apply_visitor([&ast, &pos](auto&& arg) {
            for (ast::identifier& id_expr, boost::fusion::at_c<1>(arg)) {
               int id = iters.size();
               iters.push_back(pos);
               boost::fusion::at_c<0>(id_expr).id = id;
            }
         }, ast.constant_list);
      }
      */
      void operator()(ast::assignment& ast, Iterator pos) const
      {
         int id = iters.size();
         iters.push_back(pos);
         ast.lhs.id = id;
      }

      void operator()(ast::read_statement& ast, Iterator pos) const
      {
         int id = iters.size();
         iters.push_back(pos);
         ast.variable.id = id;
      }

      void operator()(ast::procedure_call_statement& ast, Iterator pos) const
      {
         int id = iters.size();
         iters.push_back(pos);
         ast.procedure_name.id = id;
      }

      void operator()(ast::return_statement& ast, Iterator pos) const
      {
         int id = iters.size();
         iters.push_back(pos);
         ast.id = id;
      }

      void operator()(ast::for_statement& ast, Iterator pos) const
      {
         int id = iters.size();
         iters.push_back(pos);
         ast.counter.id = id;
      }

      void operator()(ast::identifier& ast, Iterator pos) const
      {
         int id = iters.size();
         iters.push_back(pos);
         ast.id = id;
      }

      std::vector<Iterator>& iters;
   };
}

#endif // __ANNOTATION_HPP__

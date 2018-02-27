#ifndef __ANNOTATION_HPP__
#define __ANNOTATION_HPP__

#include <map>
#include <boost/variant/apply_visitor.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/bool.hpp>
#include "ast.hpp"

namespace algc {
   template<typedef Iterator>
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

      void operator()(ast::variable_declaration& ast, Iterator pos) const
      {
         int id = iters.size();
         iters.push_back(pos);
         boost::apply_visitor(set_id(id), ast);
         //ast.lhs.id = id;
      }

      void operator()(ast::assignment& ast, Iterator pos) const
      {
         int id = iters.size();
         iters.push_back(pos);
         ast.lhs.id = id;
      }

      std::vector<Iterator>& iters;
   };
}

#endif // __ANNOTATION_HPP__

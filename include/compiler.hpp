#ifndef __COMPILER_HPP__
#define __COMPILER_HPP__

#include "ast.hpp"
#include "error_handler.hpp"
#include "vm.hpp"
#include <memory>
#include <variant>
#include <vector>
#include <map>
#include <functional>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_function.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

namespace algc {
   namespace code_gen {
   	using data = std::variant<
			  byte_code
			, int			// integer value
			, size_t		// offset
			, double		// float value
			, const char*	// string value
			>;

      struct function {

      };

      struct compiler {
			template <typename ErrorHandler>
			compiler(ErrorHandler& error_handler_) : current(0)
			{
				using namespace boost::phoenix::arg_names;
				namespace phx = boost::phoenix;
				using boost::phoenix::function;

				error_handler = function<ErrorHandler>(error_handler_)(
					 "Error! ", _2, phx::cref(error_handler_.iters)[_1]);
			}

			bool operator()(ast::nil) { BOOST_ASSERT(0); return false; }
			bool operator()(bool x);
			bool operator()(double x);
			bool operator()(unsigned int x);
			bool operator()(char x);
			bool operator()(ast::identifier& x);
			bool operator()(ast::unary& x);
			bool operator()(ast::array_access& x);
			bool operator()(ast::function_call& x);
			bool operator()(ast::expression& x);
			bool operator()(ast::operation& x);
			bool operator()(ast::assignment& x);
			bool operator()(ast::read_statement& x);
			bool operator()(ast::write_statement& x);
			bool operator()(ast::if_statement& x);
			bool operator()(ast::while_statement& x);
			bool operator()(ast::repeat_until_statement& x);
			bool operator()(ast::for_statement& x);
			bool operator()(ast::return_statement& x);
			bool operator()(ast::variable_declaration& x);
			bool operator()(ast::constant_declaration& x);
			bool operator()(ast::procedure_call_statement& x);
			bool operator()(ast::function_definition& x);
			bool operator()(ast::program& x);

			using function_table = std::map<std::string, std::shared_ptr<code_gen::function> >;

			std::vector<data> code;
			code_gen::function* current;
			std::string current_function_name;
			function_table functions;
			std::function<void(int tag, const std::string& what)> error_handler;

      };
   }

}

#endif // __COMPILER_HPP__

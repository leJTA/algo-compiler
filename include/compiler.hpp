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
			  byte_code	// opcode
			, bool		// boolean value
			, char		// char value
			, int			// integer value
			, size_t		// offset
			, double		// float value
			, const char*	// string value
			>;

		using value = std::variant<
			  bool		// boolean_type
			, char		// character_type
			, int			// integer_type
			, double		// real_type
			, std::string		// string_type
			, std::vector<bool>	// array_of_boolean_type
			, std::vector<char>	// array_of_integer_type
			, std::vector<int>	// array_of_real_type
			, std::vector<double>// array_of_character_type
			, std::vector<std::string>	// array_of_string_type
			>;

      struct function {
			function(std::vector<data>& code, int nargs)
			 : code(code), address(code.size()), size_(0), nargs_(nargs) {}

			template<typename T> void op(T a);
			template<typename T> void op(T a, T b);
			template<typename T> void op(T a, T b, T c);

			data& operator[](std::size_t i) { return code[address+i]; }
			const data& operator[](std::size_t i) const { return code[address+i]; }
			std::size_t size() const { return size_; }
			std::size_t get_address() const { return address; }

			int nargs() const { return nargs_; }
			int nvars() const { return variables.size(); }
			int const* find_var(std::string const& name) const;
			void add_var(std::string const& name);
			void link_to(std::string const& name, std::size_t address);

			void print_assembler() const;

			private:

			std::map<std::string, value> variables;
			std::map<std::size_t, std::string> function_calls;
			std::vector<data>& code;
			std::size_t address;
			std::size_t size_;
			std::size_t nargs_;
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
			std::map<std::string, value> global_constants;

			code_gen::function* current;
			std::string current_function_name;
			function_table functions;
			std::function<void(int tag, const std::string& what)> error_handler;

      };
   }

}

#endif // __COMPILER_HPP__

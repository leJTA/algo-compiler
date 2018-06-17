#ifndef __COMPILER_HPP__
#define __COMPILER_HPP__

#include "ast.hpp"
#include "error_handler.hpp"
#include "vm.hpp"
#include <vector>
#include <map>
#include <functional>
#include <boost/variant.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_function.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

namespace algc {
   namespace code_gen {
   	using data = boost::variant<
			  byte_code	// opcode
			, bool		// boolean value
			, char		// char value
			, int			// integer value
			, size_t		// offset
			, double		// float value
			, const char*	// string value
			>;

		using value = boost::variant<
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

		const data initialization_values[5] = {
			  false
			, '\0'
			, 0
			, 0.0
			, ""
			};

      struct function {
		public:
			function(std::vector<data>& code, int nargs, bool returns_value = false)
			 : returns_value(returns_value), code(code), address(code.size()), size_(0), nargs_(nargs) {}

			void op(byte_code a);
			template<typename T> void op(byte_code a, T b);
			template<typename T, typename U> void op(byte_code a, T b, U c);

			data& operator[](std::size_t i) { return code[address+i]; }
			const data& operator[](std::size_t i) const { return code[address+i]; }
			std::size_t size() const { return size_; }
			std::size_t get_address() const { return address; }

			int nargs() const { return nargs_; }
			int nvars() const { return variables.size(); }
			const int* find_var(const std::string& name) const;
			void add_var(const std::string& name);
			void link_to(const std::string& name, std::size_t address);

			void print_assembler() const;

			bool returns_value;

		private:

			std::map<std::string, int> variables;
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
			bool operator()(int x);
			bool operator()(char x);
			bool operator()(const std::string& x);
			bool operator()(const ast::identifier& x);
			bool operator()(const ast::unary& x);
			bool operator()(const ast::array_access& x);
			bool operator()(const ast::array_size_access& x);
			bool operator()(const ast::function_call& x);
			bool operator()(const ast::expression& x);
			bool operator()(const ast::operation& x);
			bool operator()(const ast::statement_list& x);
			bool operator()(const ast::statement& x);
			bool operator()(const ast::assignment& x);
			bool operator()(const ast::read_statement& x);
			bool operator()(const ast::write_statement& x);
			bool operator()(const ast::if_statement& x);
			bool operator()(const ast::while_statement& x);
			bool operator()(const ast::repeat_until_statement& x);
			bool operator()(const ast::for_statement& x);
			bool operator()(const ast::return_statement& x);
			bool operator()(const ast::variable_declaration_list& x);
			bool operator()(const ast::variable_declaration& x);
			bool operator()(const ast::constant_declaration_list& x);
			bool operator()(const ast::constant_declaration& x);
			bool operator()(const ast::procedure_call_statement& x);
			bool operator()(const ast::function_definition_list& x);
			bool operator()(const ast::function_definition& x);
			bool operator()(const ast::program& x);

			void print_assembler() const;

			byte_code get_opcode(ast::type& t);
			bool operator()(const boost::fusion::vector<ast::type_name, std::list<ast::identifier> >& x);
			bool operator()(const boost::fusion::vector<ast::type_name, std::list<boost::fusion::vector<ast::identifier, unsigned int> > >& x);

			using function_table = std::map<std::string, boost::shared_ptr<code_gen::function> >;

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

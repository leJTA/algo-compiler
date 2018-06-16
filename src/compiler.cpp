#include "compiler.hpp"
#include "vm.hpp"
#include <boost/assert.hpp>
#include <boost/lexical_cast.hpp>
#include <set>

namespace algc {
	namespace code_gen {

		void function::op(byte_code a)
		{
			code.push_back(a);
			size_ += 1;
		}

		template<typename T>
		void function::op(byte_code a, T b)
		{
			code.push_back(a);
			code.push_back(b);
			size_ += 2;
		}

		template<typename T, typename U>
		void function::op(byte_code a, T b, U c)
		{
			code.push_back(a);
			code.push_back(b);
			code.push_back(c);
			size_ += 3;
		}

		const int* function::find_var(const std::string& name) const
		{
			std::map<std::string, int>::const_iterator i = variables.find(name);
			if (i == variables.end())
				return nullptr;
			return &i->second;
		}

		void function::add_var(const std::string& name)
		{
			std::size_t n = variables.size();
			variables[name] = n;
		}

		void function::link_to(const std::string& name, std::size_t address)
		{
			function_calls[address] = name;
		}

		void function::print_assembler() const
		{
			std::vector<data>::const_iterator pc = code.begin() + address;

			std::vector<std::string> locals(variables.size());
			typedef std::pair<std::string, int> pair;

			for (pair const& p : variables)
			{
				locals[p.second] = p.first;
				std::cout << "      local       "
					 << p.first << ", @" << p.second << std::endl;
			}

			std::map<std::size_t, std::string> lines;
			std::set<std::size_t> jumps;

			while (pc != (code.begin() + address + size_))
			{
				std::string line;
				std::size_t address = pc - code.begin();

				switch ( boost::get<byte_code>(*pc++)) {
				case op_neg:
					line += "      op_neg";
					break;

				case op_not:
					line += "      op_not";
					break;

				case op_add:
					line += "      op_add";
					break;

				case op_sub:
					line += "      op_sub";
					break;

				case op_mul:
					line += "      op_mul";
					break;

				case op_div:
					line += "      op_div";
					break;

				case op_mod:
					line += "      op_mod";
					break;

				case op_eq:
					line += "      op_eq";
					break;

				case op_neq:
					line += "      op_neq";
					break;

				case op_lt:
					line += "      op_lt";
					break;

				case op_lte:
					line += "      op_lte";
					break;

				case op_gt:
					line += "      op_gt";
					break;

				case op_gte:
					line += "      op_gte";
					break;

				case op_and:
					line += "      op_and";
					break;

				case op_or:
					line += "      op_or";
					break;

				case op_load:
					line += "      op_load        ";
					line += boost::lexical_cast<std::string>(locals[boost::get<int>(*pc++)]);
					break;

				case op_aload:
					line += "      op_aload       ";
					line += boost::lexical_cast<std::string>(locals[boost::get<int>(*pc++)]);
					break;

				case op_store:
					line += "      op_store       ";
					line += boost::lexical_cast<std::string>(locals[boost::get<int>(*pc++)]);
					break;

				case op_astore:
					line += "      op_astore      ";
					line += boost::lexical_cast<std::string>(locals[boost::get<int>(*pc++)]);
					break;

				case op_push_char:
					line += "      op_push_char   ";
					line += "\'" + boost::lexical_cast<std::string>(boost::get<char>(*pc++)) + "\'";
					break;

				case op_push_int:
					line += "      op_push_int    ";
					line += boost::lexical_cast<std::string>(boost::get<int>(*pc++));
					break;

				case op_push_float:
					line += "      op_push_float  ";
					line += boost::lexical_cast<std::string>(boost::get<double>(*pc++));
					break;

				case op_push_str:
					line += "      op_push_str    ";
					line += "\"" + boost::lexical_cast<std::string>(boost::get<const char*>(*pc++)) + "\"";
					break;

				case op_push_true:
					line += "      op_push_true";
					break;

				case op_push_false:
					line += "      op_push_false";
					break;

				case op_jump:
				{
					line += "      op_jump        ";
					std::size_t pos = (pc - code.begin()) + boost::get<size_t>(*pc++);
					if (pos == code.size())
						 line += "end";
					else
						 line += boost::lexical_cast<std::string>(pos);
					jumps.insert(pos);
				}
				break;

				case op_jump_if:
					{
						line += "      op_jump_if     ";
						std::size_t pos = (pc - code.begin()) + boost::get<size_t>(*pc++);
						if (pos == code.size())
							 line += "end";
						else
							 line += boost::lexical_cast<std::string>(pos);
						jumps.insert(pos);
					}
					break;

				case op_read:
					line += "      op_read        ";
					line += boost::lexical_cast<std::string>(locals[boost::get<int>(*pc++)]);
					break;

				case op_print:
					line += "      op_print";
					break;

				case op_call:
					{
						line += "      op_call        ";
						int nargs = boost::get<int>(*pc++);
						std::size_t jump = boost::get<size_t>(*pc++);
						line += boost::lexical_cast<std::string>(nargs) + ", ";
						BOOST_ASSERT(function_calls.find(jump) != function_calls.end());
						line += function_calls.find(jump)->second;
					}
					break;

				case op_stk_adj:
					line += "      op_stk_adj     ";
					line += boost::lexical_cast<std::string>(boost::get<int>(*pc++));
					break;


				case op_return:
					line += "      op_return";
                    break;
				}
				lines[address] = line;
			}

            std::cout << "start:" << std::endl;
            typedef std::pair<std::size_t, std::string> line_info;
            for (line_info const& l : lines)
            {
                std::size_t pos = l.first;
                if (jumps.find(pos) != jumps.end())
                    std::cout << pos << ':' << std::endl;
                std::cout << l.second << std::endl;
            }

            std::cout << "end:" << std::endl << std::endl;
		}

		bool compiler::operator()(bool x)
		{
			BOOST_ASSERT(current != 0);
			current->op(x ? op_push_true : op_push_false);
			return true;
		}

		bool compiler::operator()(int x)
		{
			BOOST_ASSERT(current != 0);
			current->op(op_push_int, x);
			return true;
		}

		bool compiler::operator()(double x)
		{
			BOOST_ASSERT(current != 0);
			current->op(op_push_float, x);
			return true;
		}

		bool compiler::operator()(char x)
		{
			BOOST_ASSERT(current != 0);
			current->op(op_push_char, x);
			return true;
		}

		bool compiler::operator()(const std::string& x)
		{
			BOOST_ASSERT(current != 0);
			current->op(op_push_str, x.c_str());
			return true;
		}

		bool compiler::operator()(const ast::identifier& x)
		{
			BOOST_ASSERT(current != 0);
			const int* p = current->find_var(x.name);
			if (p == nullptr)
			{
				error_handler(x.id, "Undeclared variable: " + x.name);
				return false;
			}
			current->op(op_load, *p);
			return true;
		}

		bool compiler::operator()(const ast::unary& x)
		{
			BOOST_ASSERT(current != 0);
			if (!boost::apply_visitor(*this, x.operand_))
				return false;
			switch (x.operator_)
			{
				case ast::op_negative: current->op(op_neg); break;
				case ast::op_not: current->op(op_not); break;
				case ast::op_positive: break;
				default: BOOST_ASSERT(0); return false;
			}
			return true;
		}

		bool compiler::operator()(const ast::array_access& x)
		{
			BOOST_ASSERT(current != 0);
			const int* p = current->find_var(x.array_name.name);
			if (p == nullptr)
			{
				error_handler(x.array_name.id, "Undeclared variable: " + x.array_name.name);
				return false;
			}
			if (!boost::apply_visitor((*this), x.index))
				return false;
			current->op(op_aload, *p);
			return true;
		}

		bool compiler::operator()(const ast::expression& x)
		{
			BOOST_ASSERT(current != 0);
			if (!boost::apply_visitor(*this, x.first))
				return false;
			for (const ast::operation& oper : x.rest)
			{
				if (!(*this)(oper))
					 return false;
			}
			return true;
		}

		bool compiler::operator()(const ast::operation& x)
		{
			BOOST_ASSERT(current != 0);
			if (!boost::apply_visitor(*this, x.operand_))
				return false;
			switch (x.operator_)
			{
				case ast::op_plus: current->op(op_add); break;
				case ast::op_minus: current->op(op_sub); break;
				case ast::op_times: current->op(op_mul); break;
				case ast::op_divide: current->op(op_div); break;
				case ast::op_modulo: current->op(op_mod); break;

				case ast::op_equal: current->op(op_eq); break;
				case ast::op_not_equal: current->op(op_neq); break;
				case ast::op_less: current->op(op_lt); break;
				case ast::op_less_equal: current->op(op_lte); break;
				case ast::op_greater: current->op(op_gt); break;
				case ast::op_greater_equal: current->op(op_gte); break;

				case ast::op_and: current->op(op_and); break;
				case ast::op_or: current->op(op_or); break;
				default: BOOST_ASSERT(0); return false;
			}
			return true;
		}

		bool compiler::operator()(const ast::statement& x)
		{
			BOOST_ASSERT(current != 0);
			return boost::apply_visitor(*this, x);
		}

		bool compiler::operator()(const ast::statement_list& x)
		{
			BOOST_ASSERT(current != 0);
			for (const ast::statement& s : x) {
				if (!(*this)(s))
					return false;
			}
			return true;
		}

		bool compiler::operator()(const ast::assignment& x)
		{
			BOOST_ASSERT(current != 0);
			if (!(*this)(x.rhs))
				return false;
			const int* p = current->find_var(x.lhs.name);
			if (p == nullptr)
			{
				if (global_constants.find(x.lhs.name) != global_constants.end()) {
					error_handler(x.lhs.id, x.lhs.name + " is a constant and cannot be modified");
				}
				else {
					error_handler(x.lhs.id, "Undeclared variable: " + x.lhs.name);
				}
				return false;
			}
			if (x.index) {
				if (!(*this)(*x.index))
					return false;
				current->op(op_astore, *p);
			}
            else {
				current->op(op_store, *p);
			}
			return true;
		}

		bool compiler::operator()(const ast::read_statement& x)
		{
			BOOST_ASSERT(current != 0);
			const int* p = current->find_var(x.variable.name);
			if (p == nullptr)
			{
				if (global_constants.find(x.variable.name) != global_constants.end()) {
					error_handler(x.variable.id, x.variable.name + " is a constant and cannot be modified");
				}
				else {
					error_handler(x.variable.id, "Undeclared variable: " + x.variable.name);
				}
				return false;
			}
			current->op(op_read, *p);
			return true;
		}

		bool compiler::operator()(const ast::write_statement& x)
		{
			BOOST_ASSERT(current != 0);
			for (const ast::expression& e : x.expression_list) {
				if (!(*this)(e))
					return false;
				current->op(op_print);
			}
			return true;
		}

		bool compiler::operator()(const ast::if_statement& x)
		{
			BOOST_ASSERT(current != 0);
			if (!(*this)(x.condition))
				return false;
			current->op(op_jump_if, size_t(0));                 // we shall fill this (0) in later
			std::size_t skip = current->size()-1;       // mark its position
			if (!(*this)(x.then))
				return false;
			(*current)[skip] = current->size() - skip;    // now we know where to jump to (after the if branch)

			if (x.else_)											// We got an else
			{
				boost::get<size_t>((*current)[skip]) += 2;	// adjust for the "else" jump
				current->op(op_jump, size_t(0));						// we shall fill this (0) in later
				std::size_t exit = current->size()-1;		// mark its position
				if (!(*this)(*x.else_))
					 return false;
					 (*current)[exit] = current->size()-exit;// now we know where to jump to (after the else branch)
			}

			return true;
		}

		bool compiler::operator()(const ast::while_statement& x)
		{
			BOOST_ASSERT(current != 0);
			std::size_t loop = current->size();         // mark our position
			if (!(*this)(x.condition))
				return false;
			current->op(op_jump_if, size_t(0));                 // we shall fill this (0) in later
			std::size_t exit = current->size()-1;       // mark its position
			if (!(*this)(x.body))
				return false;
			current->op(op_jump, (loop-1) - current->size());    // loop back
				(*current)[exit] = current->size() - exit;    // now we know where to jump to (to exit the loop)
			return true;
		}

		bool compiler::operator()(const ast::repeat_until_statement& x)
		{
			/// TODO add code
			return true;
		}

		bool compiler::operator()(const ast::for_statement& x)
		{
			/// TODO add code
			return true;
		}

		bool compiler::operator()(const ast::return_statement& x)
		{
			if (!current->returns_value)
			{
				if (current_function_name == "%algorithm%") {
					error_handler(x.id, " an algorithm must not return a value ");
				}
				else {
					error_handler(x.id, current_function_name + " is a procedure and must not return a value ");
				}
				 return false;
			}
			else {
				if (!(*this)(x.expr))
					 return false;
			}
			current->op(op_return);
			return true;
		}

		bool compiler::operator()(const ast::variable_declaration_list& x)
		{
			BOOST_ASSERT(current != 0);
			for (const ast::variable_declaration& v : x) {
				if (!(*this)(v))
					return false;
			}
			return true;
		}

		bool compiler::operator()(const ast::variable_declaration& x)
		{
			BOOST_ASSERT(current != 0);
			return boost::apply_visitor(*this, x.variable_list);
		}

		bool compiler::operator()(const ast::constant_declaration& x)
		{
			/// TODO add code
			return true;
		}

		bool compiler::operator()(const ast::constant_declaration_list& x)
		{
			/// TODO add code
			return true;
		}

		bool compiler::operator()(const ast::procedure_call_statement& x)
		{
			BOOST_ASSERT(current != 0);

			if (functions.find(x.procedure_name.name) == functions.end())
			{
				error_handler(x.procedure_name.id, "Procedure not found: " + x.procedure_name.name);
				return false;
			}

			boost::shared_ptr<code_gen::function> p = functions[x.procedure_name.name];

			if (p->returns_value) {
				error_handler(x.procedure_name.id, x.procedure_name.name + " is not a procedure");
				return false;
			}

			if (p->nargs() != x.args.size())
			{
				error_handler(x.procedure_name.id, "Wrong number of arguments: " + x.procedure_name.name);
				return false;
			}

			for (const ast::expression& expr : x.args) {
				if (!(*this)(expr))
					 return false;
			}

			current->op(
				op_call,
				p->nargs(),
				p->get_address());
			current->link_to(x.procedure_name.name, p->get_address());

			return true;
		}

		bool compiler::operator()(const ast::function_call& x)
		{
			BOOST_ASSERT(current != 0);

			if (functions.find(x.function_name.name) == functions.end())
			{
				error_handler(x.function_name.id, "Function not found: " + x.function_name.name);
				return false;
			}

			boost::shared_ptr<code_gen::function> p = functions[x.function_name.name];

			if (!p->returns_value) {
				error_handler(x.function_name.id, x.function_name.name + " is not a function");
				return false;
			}

			if (p->nargs() != x.args.size())
			{
				error_handler(x.function_name.id, "Wrong number of arguments: " + x.function_name.name);
				return false;
			}

			for (const ast::expression& expr : x.args) {
				if (!(*this)(expr))
					 return false;
			}

			current->op(
				op_call,
				p->nargs(),
				p->get_address());
			current->link_to(x.function_name.name, p->get_address());

			return true;
		}

		bool compiler::operator()(const ast::function_definition_list& x)
        {
			for (const ast::function_definition& f : x) {
				if (!(*this)(f))
				{
					code.clear();
					return false;
				}
			}
			return true;
		}

		bool compiler::operator()(const ast::function_definition& x)
		{
			const ast::identifier& function_name = [&x](){
				if (x.header.which() == 0) {
					using T = boost::fusion::vector<ast::identifier, ast::args_list_type>;
					return boost::fusion::at_c<0>(boost::get<T>(x.header));
				}
				else {	// == 1
					using T = boost::fusion::vector<ast::identifier, ast::args_list_type, ast::type_name>;
					return boost::fusion::at_c<0>(boost::get<T>(x.header));
				}
			}();

			if (functions.find(function_name.name) != functions.end()) {
				error_handler(function_name.id, "Duplicate function: " + function_name.name);
				return false;
			}

			boost::shared_ptr<code_gen::function>& p = functions[function_name.name];

			const ast::args_list_type& args = [&x](){
				if (x.header.which() == 0) {
					using T = boost::fusion::vector<ast::identifier, ast::args_list_type>;
					return boost::fusion::at_c<1>(boost::get<T>(x.header));
				}
				else {	// == 1
					using T = boost::fusion::vector<ast::identifier, ast::args_list_type, ast::type_name>;
					return boost::fusion::at_c<1>(boost::get<T>(x.header));
				}
			}();

			p.reset(new code_gen::function(code, args.size(), x.header.which() == 1));
			current = p.get();
			current_function_name = function_name.name;

			// op_stk_adj 0 for now. we'll know how many variables
			// we'll have later and add them
			current->op(op_stk_adj, 0);

			for (const boost::fusion::vector<ast::type_name, ast::identifier>& arg : args) {
				current->add_var(boost::fusion::at_c<1>(arg).name);
			}

			if (x.local_vars) {
				if (!(*this)(*x.local_vars))
					return false;
			}

			if (!(*this)(x.body))
				return false;
			(*current)[1] = current->nvars();   // now store the actual number of variables
														  // this includes the arguments
			return true;
		}

		bool compiler::operator()(const ast::program& x)
		{
            //if (x.consts) {
				//return true;
            //}

            // Jump to the main function
            code.push_back(op_jump);
				code.push_back(size_t(0)); // we will fill this in later when we finish compiling
                                   // and we know where the main function is

            if (x.funcs) {
                if (!(*this)(*x.funcs)) {
                    return false;
                }
            }

				code[1] = code.size() - 1; // jump to begin of the algorithm

            boost::shared_ptr<code_gen::function>& p = functions["%algorithm%"];
            p.reset(new code_gen::function(code, 0));
            current = p.get();
            current_function_name = "%algorithm%";

            if (x.vars) {
                if (!(*this)(*x.vars)) {
                    return false;
                }
            }

            return ((*this)(x.body));
        }

        void compiler::print_assembler() const
        {
            typedef std::pair<std::string, boost::shared_ptr<code_gen::function> > pair;
            for (pair const& p : functions)
            {
				std::cout << ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;" << std::endl;
				std::cout << p.second->get_address() << ": function " << p.first << std::endl;
				p.second->print_assembler();
			}
		}

		bool compiler::operator()(const boost::fusion::vector<ast::type_name, std::list<ast::identifier> >& x)
		{
			ast::type t = boost::fusion::at_c<1>(x).front().t;
			byte_code opcode = get_opcode(t);
			for (const ast::identifier& v : boost::fusion::at_c<1>(x)) {
				const int* p = current->find_var(v.name);
				if (p != nullptr)
				{
					error_handler(v.id, "Duplicate variable: " + v.name);
					return false;
				}

				current->add_var(v.name);
                if (opcode == op_push_false) {
                    current->op(opcode);
                }
                else {
                    current->op(opcode, initialization_values[t]);
                }
				current->op(op_store, *current->find_var(v.name));
			}
			return true;
		}

		bool compiler::operator()(const boost::fusion::vector<ast::type_name, std::list<boost::fusion::vector<ast::identifier, unsigned int> > >& x)
		{
			/// TODO arrays

			return true;
		}

		byte_code compiler::get_opcode(ast::type& t)
		{
			switch (t) {
			case ast::boolean_type:
				return op_push_false;
				break;
			case ast::character_type:
				return op_push_char;
				break;
			case ast::integer_type:
				return op_push_int;
				break;
			case ast::real_type:
				return op_push_float;
				break;
			case ast::string_type:
				return op_push_str;
				break;
			}
		}

	}
}

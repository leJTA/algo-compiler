#include "vm.hpp"
#include <boost/variant.hpp>
#include <string>
#include <exception>

namespace algc
{
	using std::string;

	data vmachine::execute(
	  std::vector<data> const& code
	, std::vector<data>::const_iterator pc
	, std::vector<data>::iterator frame_ptr
	)
	{
		std::vector<data>::iterator stack_ptr = frame_ptr;

		while (true)
		{
			switch (boost::get<byte_code>(*pc++))
			{
			case op_neg:
				 neg(stack_ptr[-1]);
				 break;

			case op_not:
				 not_(stack_ptr[-1]);
				 break;

			case op_add:
				 --stack_ptr;
				 add(stack_ptr[-1], stack_ptr[0]);
				 break;

			case op_sub:
				 --stack_ptr;
				 sub(stack_ptr[-1], stack_ptr[0]);
				 break;

			case op_mul:
				 --stack_ptr;
				 mul(stack_ptr[-1], stack_ptr[0]);
				 break;

			case op_div:
				 --stack_ptr;
				 div(stack_ptr[-1], stack_ptr[0]);
				 break;

			case op_mod:
				 --stack_ptr;
				 mod(stack_ptr[-1], stack_ptr[0]);
				 break;

			case op_eq:
				--stack_ptr;
				//
				break;

			case op_neq:
				--stack_ptr;
				//boost::apply_visitor(op_visitor(op_neq), stack_ptr[-1], stack_ptr[0]);
				break;

			case op_lt:
				--stack_ptr;
				//boost::apply_visitor(op_visitor(op_lt), stack_ptr[-1], stack_ptr[0]);
				break;

			case op_lte:
				 --stack_ptr;
				 //boost::apply_visitor(op_visitor(op_lte), stack_ptr[-1], stack_ptr[0]);
				 break;

			case op_gt:
				 --stack_ptr;
				 //boost::apply_visitor(op_visitor(op_gt), stack_ptr[-1], stack_ptr[0]);
				 break;

			case op_gte:
				 --stack_ptr;
				 //boost::apply_visitor(op_visitor(op_gte), stack_ptr[-1], stack_ptr[0]);
				 break;

			case op_and:
				 --stack_ptr;
				 //boost::apply_visitor(op_visitor(op_and), stack_ptr[-1], stack_ptr[0]);
				 break;

			case op_or:
				 --stack_ptr;
				 //boost::apply_visitor(op_visitor(op_or), stack_ptr[-1], stack_ptr[0]);
				 break;

			case op_load:
				 //boost::apply_visitor(op_visitor(op_load), frame_ptr[boost::get<int>(*pc++)], *stack_ptr++);
				 break;

			case op_store:
				 --stack_ptr;
				 //boost::apply_visitor(op_visitor(op_store), frame_ptr[boost::get<int>(*pc++)], stack_ptr[0]);
				 break;

			case op_aload:
				break;

			case op_astore:
				break;

			case op_push_char:
				 *stack_ptr++ = boost::get<char>(*pc++);
				 break;

			case op_push_int:
				 *stack_ptr++ = boost::get<int>(*pc++);
				 break;

			case op_push_float:
				 *stack_ptr++ = boost::get<double>(*pc++);
				 break;

			case op_push_str:
				 *stack_ptr++ = boost::get<const char*>(*pc++);
				 break;

			case op_push_true:
				 *stack_ptr++ = true;
				 break;

			case op_push_false:
				 *stack_ptr++ = false;
				 break;

			case op_jump:
				 pc += boost::get<size_t>(*pc);
				 break;

			case op_jump_if:
				 if (!bool(boost::get<bool>(stack_ptr[-1])))
					  pc += boost::get<size_t>(*pc);
				 else
					  ++pc;
				 --stack_ptr;
				 break;

			case op_print:
				--stack_ptr;
				print(stack_ptr[0]);
				break;

			case op_read:
				 break;

			case op_stk_adj:
				 stack_ptr += boost::get<int>(*pc++);
				 break;

			case op_call:
				 {
					  int nargs = boost::get<int>(*pc++);
					  int jump = boost::get<int>(*pc++);

					  // a function call is a recursive call to execute
					  data r = execute(
							code
						 , code.begin() + jump
						 , stack_ptr - nargs
					  );

					  // cleanup after return from function
					  stack_ptr[-nargs] = r;      //  get return value
					  stack_ptr -= (nargs - 1);   //  the stack will now contain the return value
				 }
				 break;

			case op_return:
				return stack_ptr[-1];
			}
		}
	}

	const char* type_str[] = {
		  "byte_code"
		, "boolean"
		, "character"
		, "integer"
		, "size_t"
		, "real"
		, "string"
	};

	void vmachine::neg(data& x)
	{
		if (x.which() == 3) {	// int
			boost::get<int>(x) = -boost::get<int>(x);
		}
		else if (x.which() == 5) {	// double
			boost::get<double>(x) = -boost::get<double>(x);
		}
		else {
			throw std::runtime_error(string("negation cannot be applied to type : ") + type_str[x.which()]);
		}
	 }

	 void vmachine::not_(data& x)
	 {
		 if (x.which() == 1) { // bool
			 boost::get<bool>(x) = !boost::get<bool>(x);
		 }
		 else {
			 throw std::runtime_error(string("logical 'not' cannot be applied to type : ") + type_str[x.which()]);
		 }
	 }

	void vmachine::add(data& x, data& y)
	{
		if (binary_operation(x, y, [](auto x, auto y){return x + y;})) { // addition for int and double
			return;
		}
		else if (x.which() == 6 and y.which() == 6) {	// addition between two string
			boost::get<const char*>(x) =
					strdup(( string(boost::get<const char*>(x)) + string(boost::get<const char*>(y)) ).c_str());
		}
		else {
			throw std::runtime_error(string("operation '+' cannot be applied between types : ")
											 + type_str[x.which()] + " and " + type_str[y.which()]);
		}
	}

	void vmachine::sub(data& x, data& y)
	{
		if (binary_operation(x, y, [](auto x, auto y){return x - y;})) { // substraction for int and double
			return;
		}
		else {
			throw std::runtime_error(string("operation '-' cannot be applied between types : ")
											 + type_str[x.which()] + " and " + type_str[y.which()]);
		}
	}

	void vmachine::mul(data& x, data& y)
	{
		if (binary_operation(x, y, [](auto x, auto y){return x * y;})) { // multiplication for int and double
			return;
		}
		else {
			throw std::runtime_error(string("operation '*' cannot be applied between types : ")
											 + type_str[x.which()] + " and " + type_str[y.which()]);
		}
	}

	void vmachine::div(data& x, data& y)
	{
		if ((y.which() == 3 && boost::get<int>(y) == 0)
			 || (y.which() == 5 && boost::get<double>(y) == 0.0)) {	// check for division by zero
			throw std::runtime_error("Error division by zero");
		}

		if (binary_operation(x, y, [](auto x, auto y){return x / y;})) { // division for int and double
			return;
		}
		else {
			throw std::runtime_error(string("operation '/' cannot be applied between types : ")
											 + type_str[x.which()] + " and " + type_str[y.which()]);
		}
	}

	void vmachine::mod(data& x, data& y)
	{
		if ((y.which() == 3 && boost::get<int>(y) == 0)
			 || (y.which() == 5 && boost::get<double>(y) == 0.0)) {	// check for division by zero
			throw std::runtime_error("Error division by zero");
		}
		if (binary_operation(x, y, [](auto x, auto y){return x / y;})) { // division for int and double
			return;
		}
		else {
			throw std::runtime_error(string("operation '/' cannot be applied between types : ")
											 + type_str[x.which()] + " and " + type_str[y.which()]);
		}
	}

	void vmachine::eq(data& x, data& y)
	{

	}

	void vmachine::neq(data& x, data& y)
	{

	}

	void vmachine::lt(data& x, data& y)
	{

	}

	void vmachine::lte(data& x, data& y)
	{

	}

	void vmachine::gt(data& x, data& y)
	{

	}

	void vmachine::gte(data& x, data& y)
	{

	}

	void vmachine::and_(data& x, data& y)
	{

	}

	void vmachine::or_(data& x, data& y)
	{

	}

	void vmachine::read(data& x)
	{

	}

	void vmachine::print(data& x)
	{
		boost::apply_visitor([](auto& arg){std::cout << arg;}, x);
	}

	void vmachine::assign(data& x, data& y)
	{
		if (x.which() == 0 || (x.which() == y.which()) ) {
				x = y;
		}
		else if (x.which() == 5 && y.which() == 3) {
			boost::get<double>(x) = boost::get<int>(y);
		}
		else {
			throw std::runtime_error(string("cannot assign ")
											 + type_str[y.which()] + " value to " + type_str[x.which()] + " value");
		}
	}
}

#include "vm.hpp"
#include <boost/variant.hpp>
#include <boost/lexical_cast.hpp>
#include <exception>

namespace algc
{
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
				eq(stack_ptr[-1], stack_ptr[0]);
				break;

			case op_neq:
				--stack_ptr;
				neq(stack_ptr[-1], stack_ptr[0]);
				break;

			case op_lt:
				--stack_ptr;
				lt(stack_ptr[-1], stack_ptr[0]);
				break;

			case op_lte:
				--stack_ptr;
				lte(stack_ptr[-1], stack_ptr[0]);
				break;

			case op_gt:
				--stack_ptr;
				gt(stack_ptr[-1], stack_ptr[0]);
				break;

			case op_gte:
				--stack_ptr;
				gte(stack_ptr[-1], stack_ptr[0]);
				break;

			case op_and:
				--stack_ptr;
				and_(stack_ptr[-1], stack_ptr[0]);
				break;

			case op_or:
				--stack_ptr;
				or_(stack_ptr[-1], stack_ptr[0]);
				break;

			case op_load:
				*stack_ptr++ = frame_ptr[boost::get<int>(*pc++)];
				break;

			case op_store:
				--stack_ptr;
				assign(frame_ptr[boost::get<int>(*pc++)], stack_ptr[0]);
				break;

			case op_new_bool_array:
				--stack_ptr;
				new_bool_array(frame_ptr[boost::get<int>(*pc++)], boost::get<int>(stack_ptr[0]));
				break;

			case op_new_char_array:
				--stack_ptr;
				new_char_array(frame_ptr[boost::get<int>(*pc++)], boost::get<int>(stack_ptr[0]));
				break;

			case op_new_int_array:
				--stack_ptr;
				new_int_array(frame_ptr[boost::get<int>(*pc++)], boost::get<int>(stack_ptr[0]));
				break;

			case op_new_float_array:
				--stack_ptr;
				new_float_array(frame_ptr[boost::get<int>(*pc++)], boost::get<int>(stack_ptr[0]));
				break;

			case op_new_string_array:
				--stack_ptr;
				new_string_array(frame_ptr[boost::get<int>(*pc++)], boost::get<int>(stack_ptr[0]));
				break;

			case op_array_length:
				array_length(*stack_ptr++, frame_ptr[boost::get<int>(*pc++)]);
				break;

			case op_aload:
				aload(stack_ptr[-1], frame_ptr[boost::get<int>(*pc++)]);
				break;

			case op_astore:
				stack_ptr -= 2;
				astore(frame_ptr[boost::get<int>(*pc++)], stack_ptr[1], stack_ptr[0]);
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
				read(frame_ptr[boost::get<int>(*pc++)]);
				break;

			case op_stk_adj:
				 stack_ptr += boost::get<int>(*pc++);
				 break;

			case op_call:
				 {
					  int nargs = boost::get<int>(*pc++);
					  int jump = boost::get<size_t>(*pc++);

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
		, "array"
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
		if (x.which() == 3 && y.which() == 3) { // modulo for int
			boost::get<int>(x) %= boost::get<int>(y);
		}
		else {
			throw std::runtime_error(string("operation '/' cannot be applied between types : ")
											 + type_str[x.which()] + " and " + type_str[y.which()]);
		}
	}

	void vmachine::eq(data& x, data& y)
	{
		if (compare(x, y, [](auto x, auto y){return x == y;})) {
			return;
		}
		else {
			throw std::runtime_error(string("comparison '=' cannot be applied between types : ")
											 + type_str[x.which()] + " and " + type_str[y.which()]);
		}
	}

	void vmachine::neq(data& x, data& y)
	{
		if (compare(x, y, [](auto x, auto y){return x != y;})) {
			return;
		}
		else {
			throw std::runtime_error(string("comparison '!=' cannot be applied between types : ")
											 + type_str[x.which()] + " and " + type_str[y.which()]);
		}
	}

	void vmachine::lt(data& x, data& y)
	{
		if (compare(x, y, [](auto x, auto y){return x < y;})) {
			return;
		}
		else {
			throw std::runtime_error(string("comparison '<' cannot be applied between types : ")
											 + type_str[x.which()] + " and " + type_str[y.which()]);
		}
	}

	void vmachine::lte(data& x, data& y)
	{
		if (compare(x, y, [](auto x, auto y){return x <= y;})) {
			return;
		}
		else {
			throw std::runtime_error(string("comparison '<=' cannot be applied between types : ")
											 + type_str[x.which()] + " and " + type_str[y.which()]);
		}
	}

	void vmachine::gt(data& x, data& y)
	{
		if (compare(x, y, [](auto x, auto y){return x > y;})) {
			return;
		}
		else {
			throw std::runtime_error(string("comparison '>' cannot be applied between types : ")
											 + type_str[x.which()] + " and " + type_str[y.which()]);
		}
	}

	void vmachine::gte(data& x, data& y)
	{
		if (compare(x, y, [](auto x, auto y){return x >= y;})) {
			return;
		}
		else {
			throw std::runtime_error(string("comparison '>=' cannot be applied between types : ")
											 + type_str[x.which()] + " and " + type_str[y.which()]);
		}
	}

	void vmachine::and_(data& x, data& y)
	{
		if (x.which() == 1 && y.which() == 1) {
			x = boost::get<bool>(x) && boost::get<bool>(y);
		}
		else {
			throw std::runtime_error(string("operation 'logical and' cannot be applied between types : ")
											 + type_str[x.which()] + " and " + type_str[y.which()]);
		}
	}

	void vmachine::or_(data& x, data& y)
	{
		if (x.which() == 1 && y.which() == 1) {
			x = boost::get<bool>(x) || boost::get<bool>(y);
		}
		else {
			throw std::runtime_error(string("operation 'logical or' cannot be applied between types : ")
											 + type_str[x.which()] + " and " + type_str[y.which()]);
		}
	}

	void vmachine::read(data& x)
	{
		switch (x.which()) {
		case 1:	// boolean
			{
				bool input;
				std::cin >> input;
				x = input;
				break;
			}
		case 2:	// char
			{
				char input;
				std::cin >> input;
				x = input;
				break;
			}
		case 3:	// integer
			{
				int input;
				std::cin >> input;
				x = input;
				break;
			}
		case 5: // double
			{
				double input;
				std::cin >> input;
				x = input;
				break;
			}
		case 6: // string
			{
				string input;
				std::cin >> input;
				x = strdup(input.c_str());
				break;
			}
		default:
			throw std::runtime_error("Input error");
		}
	}

	void vmachine::print(const data& x)
	{
		if (x.which() == 1) {
			std::cout << ((boost::get<bool>(x) == true) ? "True" : "False");
		}
		else {
			boost::apply_visitor([](auto& arg){std::cout << arg;}, x);
		}
	}

	void vmachine::new_bool_array(data& x, int sz)
	{
		if (sz == 0) {
			throw std::runtime_error(string("size of array must be greater than zero"));
		}
		x = std::make_shared<array>(std::vector<bool>(sz));
	}

	void vmachine::new_char_array(data& x, int sz)
	{
		if (sz == 0) {
			throw std::runtime_error(string("size of array must be greater than zero"));
		}
		x = std::make_shared<array>(std::vector<char>(sz));
	}

	void vmachine::new_int_array(data& x, int sz)
	{
		if (sz == 0) {
			throw std::runtime_error(string("size of array must be greater than zero"));
		}
		x = std::make_shared<array>(std::vector<int>(sz));
	}

	void vmachine::new_float_array(data& x, int sz)
	{
		if (sz == 0) {
			throw std::runtime_error(string("size of array must be greater than zero"));
		}
		x = std::make_shared<array>(std::vector<double>(sz));
	}

	void vmachine::new_string_array(data& x, int sz)
	{
		if (sz == 0) {
			throw std::runtime_error(string("size of array must be greater than zero"));
		}
		x = std::make_shared<array>(std::vector<const char*>(sz));
	}

	void vmachine::aload(data& x, data& y)
	{
		if (y.which() == 7) {	// if it is an array
			if (x.which() == 3) {	// if the index in an integer
				array& p(*boost::get<std::shared_ptr<array> >(y));
				int index = boost::get<int>(x);
				boost::apply_visitor([&x, index](auto& t){
					if (index <= 0 || index > t.size()) {
						throw std::runtime_error(string("index overflow : index = ")
														 + boost::lexical_cast<string>(index)
														 + " and size = " + boost::lexical_cast<string>(t.size()));
					}
					x = t[index - 1];
				}, p);
			}
			else {
				throw std::runtime_error(string("index of array must be of the integer type not : ")
												 + type_str[x.which()]);
			}
		}
		else {
			throw std::runtime_error(string("array access operation cannot be applied on type : ")
											 + type_str[y.which()]);
		}
	}

	void vmachine::astore(data& x, data& i, data& y)
	{
		if (x.which() == 7) {	// if it is an array
			if (i.which() == 3) {	// if the index in an integer
				array& p(*boost::get<std::shared_ptr<array> >(x));	// get the content of the pointer
				int index = boost::get<int>(i);	// get the index at the top of the stack
				int sz = boost::apply_visitor([](auto& t){return int(t.size());}, p);

				if (index > 0 && index <= sz) {
					if (p.which() == 0 && y.which() == 1) { // boolean <- boolean
						boost::get<std::vector<bool> >(p)[index - 1] = boost::get<bool>(y);
					}
					else if (p.which() == 1 && y.which() == 2) {	// char <- char
						boost::get<std::vector<char> >(p)[index - 1] = boost::get<char>(y);
					}
					else if (p.which() == 2 && y.which() == 3) {	// integer <- integer
						boost::get<std::vector<int> >(p)[index - 1] = boost::get<int>(y);
					}
					else if (p.which() == 3 && y.which() == 3) {	// float <- integer
						boost::get<std::vector<double> >(p)[index - 1] = boost::get<int>(y);
					}
					else if (p.which() == 3 && y.which() == 5) {	// float <- float
						boost::get<std::vector<double> >(p)[index - 1] = boost::get<double>(y);
					}
					else if (p.which() == 4 && y.which() == 6) {	// string <- string
						boost::get<std::vector<const char*> >(p)[index - 1]
								= strdup(boost::get<const char*>(y));
					}
					else {
						throw std::runtime_error(string("cannot assign ")
												+ type_str[y.which()] + " value to " + type_str[p.which()] + " value");
					}
				}
				else {
					throw std::runtime_error(string("index overflow : index = ")
													 + boost::lexical_cast<string>(index)
													 + " and size = " + boost::lexical_cast<string>(sz));
				}
			}
			else {
				throw std::runtime_error(string("index of array must be of the integer type not : ")
												 + type_str[x.which()]);
			}
		}
		else {
			throw std::runtime_error(string("array access operation cannot be applied on type : ")
											 + type_str[y.which()]);
		}
	}

	void vmachine::array_length(data &x, data& y)
	{
		if (y.which() == 6) {
			x = strlen(boost::get<const char*>(x));
		}
		else if (y.which() == 7) {
			x = boost::apply_visitor([](auto& t){return int(t.size());}, *boost::get<std::shared_ptr<array> >(y));
		}
		else {
			throw std::runtime_error(string("cannot get size of varialble of type ") + type_str[x.which()]);
		}
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

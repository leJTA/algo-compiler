#ifndef __VM_HPP__
#define __VM_HPP__

#include <iostream>
#include <vector>
#include <string>
#include <boost/variant.hpp>

namespace algc {
	using std::string;
   ///////////////////////////////////////////////////////////////////////////
   //  The Virtual Machine
   ///////////////////////////////////////////////////////////////////////////
   enum byte_code {
      op_neg,         //  negate the top stack entry
      op_add,         //  add top two stack entries
      op_sub,         //  subtract top two stack entries
      op_mul,         //  multiply top two stack entries
      op_div,         //  divide top two stack entries
      op_mod,

      op_not,         //  boolean negate the top stack entry
      op_eq,          //  compare the top two stack entries for ==
      op_neq,         //  compare the top two stack entries for !=
      op_lt,          //  compare the top two stack entries for <
      op_lte,         //  compare the top two stack entries for <=
      op_gt,          //  compare the top two stack entries for >
      op_gte,         //  compare the top two stack entries for >=

      op_and,         //  logical and top two stack entries
      op_or,          //  logical or top two stack entries

      op_load,        //  load a variable
      op_store,       //  store a variable

		op_new_array,	 //  allocates memory space for a new array whose size is the top stack entry
		op_array_length,//  get the size of the array

      op_aload,       //  load an array
      op_astore,      //  store an array

      op_push_char,	 //  push constant character into the stack
      op_push_int,    //  push constant integer into the stack
      op_push_float,  //  push constant float into the stack
      op_push_str,	 //  push constant string into the stack
		op_push_true,   //  push true into the stack
		op_push_false,  //  push false into the stack

      op_jump_if,     //  jump to a relative position in the code if top stack evaluates to false
      op_jump,        //  jump to a relative position in the code

      op_read,			 //  read from the standard input
      op_print,		 //  print on the standard output

      op_stk_adj,     //  adjust the stack (for args and locals)
      op_call,        //  function call
      op_return       //  return from function
   };

	using data = boost::variant<
		  byte_code
		, bool		// boolean value
		, char		// char value
		, int			// integer value
		, size_t		// offset
		, double		// float value
		, const char*	// string value
		>;

	extern const char* type_str[];

	class vmachine {
	public:

	  vmachine(unsigned stackSize = 4096) : stack(stackSize)
	  {
	  }

	  data execute(std::vector<data> const& code)
	  {
			return execute(code, code.begin(), stack.begin());
	  }

	  std::vector<data> const& get_stack() const { return stack; }
	  std::vector<data>& get_stack() { return stack; }

	private:

	  data execute(
			std::vector<data> const& code            // the program code
		 , std::vector<data>::const_iterator pc     // program counter
		 , std::vector<data>::iterator frame_ptr    // start of arguments and locals
	  );

	  template<typename Func>
	  bool binary_operation(data& x, data& y, Func f) // x = f(x,y)
	  {
		  if (x.which() == 3 && y.which() == 3) {	// int, int
			  x = f(boost::get<int>(x), boost::get<int>(y));
			  return true;
		  }
		  else if (x.which() == 3 && y.which() == 5) {	// int, double
			  x = f(boost::get<int>(x), boost::get<double>(y));
			  return true;
		  }
		  else if (x.which() == 5 && y.which() == 3) {	// double, int
			  x = f(boost::get<double>(x), boost::get<int>(y));
			  return true;
		  }
		  else if (x.which() == 5 && y.which() == 5) {	// double, double
			  x = f(boost::get<double>(x), boost::get<double>(y));
			  return true;
		  }
		  else {
			  return false;
		  }
	  }

		template<typename Func>
		bool compare(data& x, data& y, Func f)	// f returns a boolean value
		{
			if (x.which() == 3 && y.which() == 3) {	// int, int
				x = f(boost::get<int>(x), boost::get<int>(y));
				return true;
			}
			else if (x.which() == 3 && y.which() == 5) {	// int, double
				x = f(boost::get<int>(x), boost::get<double>(y));
				return true;
			}
			else if (x.which() == 5 && y.which() == 3) {	// double, int
				x = f(boost::get<double>(x), boost::get<int>(y));
				return true;
			}
			else if (x.which() == 5 && y.which() == 5) {	// double, double
				x = f(boost::get<double>(x), boost::get<double>(y));
				return true;
			}
			else if (x.which() == 2 && y.which() == 2){	// char, char
				x = f(boost::get<char>(x), boost::get<char>(y));
				return true;
			}
			else if (x.which() == 6 && y.which() == 6) {	// string, string
				x = f(string(boost::get<const char*>(x)), string(boost::get<const char*>(y)));
				return true;
			}
			else if (x.which() == 1 && y.which() == 1) { // bool, bool
				x = f(boost::get<bool>(x), boost::get<bool>(y));
				return true;
			}
			else {
				return false;
			}
		}

		void neg(data&);
		void add(data&, data&);
		void sub(data&, data&);
		void mul(data&, data&);
		void div(data&, data&);
		void mod(data&, data&);

		void not_(data&);
		void eq(data&, data&);
		void neq(data&, data&);
		void lt(data&, data&);
		void lte(data&, data&);
		void gt(data&, data&);
		void gte(data&, data&);

		void and_(data&, data&);
		void or_(data&, data&);

		void read(data&);

		void print(const data &);

		void assign(data&, data&);

		std::vector<data> stack;
	};
}
#endif // __VM_HPP__

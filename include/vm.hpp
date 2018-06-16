#ifndef __VM_HPP__
#define __VM_HPP__

#include <iostream>
#include <vector>
#include <boost/variant.hpp>

namespace algc {

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

      op_aload,       //  load an array
      op_astore,      //  store an array

      op_push_char,	 //  push constant character into the stack
      op_push_int,    //  push constant integer into the stack
      op_push_float,  //  push constant float into the stack
      op_push_str,	 //  push constant string into the stack
      op_push_true,  //  push constant 1 into the stack
      op_push_false,   //  push constant 0 into the stack

      op_jump_if,     //  jump to a relative position in the code if top stack evaluates to false
      op_jump,        //  jump to a relative position in the code

      op_read,			 //  read from the standard input
      op_print,		 //  print on the standard output

      op_stk_adj,     //  adjust the stack (for args and locals)
      op_call,        //  function call
      op_return       //  return from function
   };
/*
   using data = boost::variant<
             byte_code	// opcode
           , bool		// boolean value
           , char		// char value
           , int			// integer value
           , size_t		// offset
           , double		// float value
           , const char*	// string value
           >;

   class vmachine
   {
   public:

       vmachine(unsigned stackSize = 4096)
        : stack(stackSize)
       {
       }

       int execute(std::vector<data> const& code)
       {
           return execute(code, code.begin(), stack.begin());
       }

       std::vector<data> const& get_stack() const { return stack; }
       std::vector<data>& get_stack() { return stack; }

   private:

       int execute(
           std::vector<data> const& code            // the program code
         , std::vector<data>::const_iterator pc     // program counter
         , std::vector<data>::iterator frame_ptr    // start of arguments and locals
       );

       std::vector<data> stack;

   };

   struct op_visitor {
       op_visitor(byte_code op) : opcode(op){}

       void operator ()(int& x)
       {
           switch (opcode) {
           case op_neg:
               x = -x;
               break;
           case op_print:
               std::cout << x;
               break;
           case op_read:
               std::cin >> x;
               break;
           }

       }

       void operator ()(int& x)
       {
           switch(opcode) {
           case op_not:
               x = !x;
               break;
           }
       }

       template<typename T, typename U>
       void operator ()(T& x, U& y)
       {
           switch (opcode) {
           case op_add:
               x += y;
               break;
           case op_sub:
               x -= y;
               break;
           case op_mul:
               x *= y;
               break;
           case op_div:
               x /= y;
               break;
           case op_mod:
               x %= y;
               break;
           case op_eq:
               x = bool(x == y);
               break;
           case op_neq:
               x = bool(x != y);
               break;
           case op_lt:
               x = bool(x < y);
               break;
           case op_lte:
               x = bool(x <= y);
               break;
           case op_gt:
               x = bool(x > y);
               break;
           case op_gte:
               x = bool(x >= y);
               break;
           case op_and:
               x = bool(x) && bool(y);
               break;
           case op_or:
               x = bool(x) || bool(y);
               break;
           case op_store:
               x = y;
               break;
           case op_load:
               y = x;
               break;
           }
       }

       byte_code opcode;
   };
*/
}
#endif // __VM_HPP__

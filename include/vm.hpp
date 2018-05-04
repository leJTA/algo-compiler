#ifndef __VM_HPP__
#define __VM_HPP__

#include <vector>

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

      op_sload,       //  load an array
      op_sstore,      //  store an array

      op_int8,        //  push constant char into the stack
      op_int32,       //  push constant integer into the stack
      op_float64      //  push constant float into the stack
      op_true,        //  push constant 0 into the stack
      op_false,       //  push constant 1 into the stack

      op_jump_if,     //  jump to a relative position in the code if top stack
                     //  evaluates to false
      op_jump,        //  jump to a relative position in the code

      op_stk_adj,     //  adjust the stack (for args and locals)
      op_call,        //  function call
      op_return       //  return from function
   };
}

#endif // __VM_HPP__

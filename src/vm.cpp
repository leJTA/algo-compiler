#include "vm.hpp"
#include <boost/variant.hpp>
/*
namespace algc
{
    int vmachine::execute(
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
                boost::apply_visitor(op_visitor(op_neg), stack_ptr[-1]);
                break;

            case op_not:
                boost::apply_visitor(op_visitor(op_not), stack_ptr[-1]);
                break;

            case op_add:
                --stack_ptr;
                boost::apply_visitor(op_visitor(op_add), stack_ptr[-1], stack_ptr[0]);
                break;

            case op_sub:
                --stack_ptr;
                boost::apply_visitor(op_visitor(op_sub), stack_ptr[-1], stack_ptr[0]);
                break;

            case op_mul:
                --stack_ptr;
                boost::apply_visitor(op_visitor(op_mul), stack_ptr[-1], stack_ptr[0]);
                break;

            case op_div:
                --stack_ptr;
                boost::apply_visitor(op_visitor(op_div), stack_ptr[-1], stack_ptr[0]);
                break;

            case op_mod:
                --stack_ptr;
                boost::apply_visitor(op_visitor(op_mod), stack_ptr[-1], stack_ptr[0]);
                break;

            case op_eq:
                --stack_ptr;
                boost::apply_visitor(op_visitor(op_eq), stack_ptr[-1], stack_ptr[0]);
                break;

            case op_neq:
                --stack_ptr;
                boost::apply_visitor(op_visitor(op_neq), stack_ptr[-1], stack_ptr[0]);
                break;

            case op_lt:
                --stack_ptr;
                boost::apply_visitor(op_visitor(op_lt), stack_ptr[-1], stack_ptr[0]);
                break;

            case op_lte:
                --stack_ptr;
                boost::apply_visitor(op_visitor(op_lte), stack_ptr[-1], stack_ptr[0]);
                break;

            case op_gt:
                --stack_ptr;
                boost::apply_visitor(op_visitor(op_gt), stack_ptr[-1], stack_ptr[0]);
                break;

            case op_gte:
                --stack_ptr;
                boost::apply_visitor(op_visitor(op_gte), stack_ptr[-1], stack_ptr[0]);
                break;

            case op_and:
                --stack_ptr;
                boost::apply_visitor(op_visitor(op_and), stack_ptr[-1], stack_ptr[0]);
                break;

            case op_or:
                --stack_ptr;
                boost::apply_visitor(op_visitor(op_or), stack_ptr[-1], stack_ptr[0]);
                break;

            case op_load:
                boost::apply_visitor(op_visitor(op_load), frame_ptr[boost::get<int>(*pc++)], *stack_ptr++);
                break;

            case op_store:
                --stack_ptr;
                boost::apply_visitor(op_visitor(op_store), frame_ptr[boost::get<int>(*pc++)], stack_ptr[0]);
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
                pc += *stack_ptr++ = boost::get<int>(*pc);
                break;

            case op_jump_if:
                if (!bool(boost::get<bool>(stack_ptr[-1])))
                    pc += boost::get<int>(*pc);
                else
                    ++pc;
                --stack_ptr;
                break;

            case op_print:
                --stack_ptr;
                boost::apply_visitor(add_visitor(op_print), stack_ptr[-1]);
                break;

            case op_read:
                boost::apply_visitor(add_visitor(op_read), stack_ptr[-1]);
                boost::apply_visitor(add_visitor(op_store), frame_ptr[boost::get<int>(*pc++)], stack_ptr[-1]);

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
                    stack_ptr -= (nargs - 1);   //  the stack will now contain
                                                //  the return value
                }
                break;

            case op_return:
                    return stack_ptr[-1];
            }
        }
    }
}
*/

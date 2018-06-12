#include "compiler.hpp"
#include "vm.hpp"
#include <boost/variant/apply_visitor.hpp>
#include <boost/assert.hpp>
#include <boost/lexical_cast.hpp>
#include <set>

namespace algc {
	namespace code_gen {

		template<typename T>
		void function::op(T a)
		{
			code.push_back(a);
			size_ += 1;
		}

		template<typename T>
		void function::op(T a, T b)
		{
			code.push_back(a);
			code.push_back(b);
			size_ += 2;
		}

		template<typename T>
		void function::op(T a, T b, T c)
		{
			code.push_back(a);
			code.push_back(b);
			code.push_back(c);
			size_ += 3;
		}

		bool compiler::operator()(bool x)
		{
			return true;
		}
	}
}

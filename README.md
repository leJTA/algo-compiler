# algo_compiler

algo_compiler is a program that compiles and executes source code written in an algorithmic language.

## Installation

The algo compiler requires the `boost.spirit` library to be compiled.

Clone GitHub repository and use CMake and Make to build the project.

```
$ git clone https://bitbucket.org/le_JTA/algo_compiler/
$ cd algo_compiler
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make
```

## Usage

Go to the location of the binary and run it passing the file path of the file that contain the code as the argument.

```
$ cd bin/Release
$ ./algo_compiler <file_path>
```

## Example

Here is an example of a program that calculates **x** to the power of **n**:

```
/* file power.txt */
algorithm power_calculation
variables
	real x
	integer n
functions_and_procedures
	function power(real x, integer n) returns real
	variables
		real result
		integer i
	begin
		if n = 0 then
			return 1
		endif
		i := 0
		result := 1
		while i < n do
			result := x * result
		endwhile
		return result

	end	/* end function */

begin
	write("Please enter a number and an integer")
	read(x)
	read(n)
	write(x, "^", n, " = ", power(x, n))

end	/* end algorithm */
```

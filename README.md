# algo_compiler

algo_compiler is a program that compiles and executes source code written in an algorithmic language. It was done in C ++ using the **boost.spirit** (1.66.0) library.

1. Requirement 

**boost.spirit** include files.

2. How to build

Run in terminal the following commands: 
```
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make
```

3. How to execute
```
$ cd bin/Release
$ ./algo_compiler <file_name>
```

# Example

Here is an example of a preogram that calculates **x** to the power of **n**:

```
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
4. Completed tasks
- [x] parsing
- [ ] type_checking (in progress)
- [ ] execution

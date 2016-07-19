How to compile and run the compiler and virtual machine:

1.) Open your development environment of choice. Load the source code of the project.
2.) Compile the C project using the compiler of your choice. Using gcc in a Unix environment such as Eustis, this command is

gcc main.c -o main

You now have compiled the compiler and virtual machine into an executable file.

3.) Before running the compiler and virtual machine, ensure your input file is named "input.txt" and is in the same directory as the executable. "input.txt" is the only filename the compiler will look for, and the same directory as the compiler is the only place it will look for it.

4.) You are now ready to run the compiler and virtual machine. To do this on a Unix command line environment like Eustis, simply type "main" (or whatever you told gcc to name the output file). The program will now begin execution.

5.) After execution, output will be placed in the file "output.txt" in the same directory as the compiler and virtual machine executable. In addition, outputs for each stage of the compilation process with that stage's errors (if there are any) will be printed.

These output files are as follows:
scanout.txt: The result of the scanner operation. Used as input for the parser.
parseout.txt: The result of the parser operation. Used for debugging.
genout.txt: The result of the code generation operation. Used for input to the virtual machine.


const
  m = 8,
  n = 2;
int  a, b, c;
/*procedure bec;
begin
a := 1+1;
end;*/
begin
a:=2;
/*call bec;
	if n <= m then
		b := n + m;
	while a < b do
		begin
			a := a + 3;
		end;
	in c; in put an integer to c
	out a;
	out b;*/
end.

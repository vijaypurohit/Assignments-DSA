DS LAB CS 513
Assignment 01 (BST)
Roll No: 214101058
----------------------------------------------------
----------------------------------------------------
DEV C++
---------
Before Executing in Dev C++ : enable option -std=c++11
>tools Tab
	>compiler option
		>general section
		> tick mark option => "Add the following commands when calling the compiler"
			> add there -std=c++1
--------------------------------------------------------
Execution in DEV C++
-------------------
To Execute the file name "assignment_01_bst.cpp"
1. Open it in Dev C++
2. Compile and Run it.
3. Console window will be opened.
4. using interactive menu, you can execute all the operations, by pressing appropriate key.

--------------------------------------------------------
To Print Tree using graphViz
-----------------------------
1. First generate the file using the option given in menu. press 'p'.
2. execute the commands in separte cmd or powershell, where file is being generated.
		dot -Tpng a_print_thbst.gv -o a_print_bst.png
3. "a_print_bst.png" will be generated to see the tree.

Command for Split Tree 1 (if generated)
	dot -Tpng a_print_thbst_split1.gv -o a_print_bst.png
Command for Split Tree 2 (if generated)
	dot -Tpng a_print_thbst_split2.gv -o a_print_bst.png

----------------------------------
Exit
-----------------
1. Use key 'n' to exit and deallocate the memory.

--------------------THE END------------------------------------------------------------
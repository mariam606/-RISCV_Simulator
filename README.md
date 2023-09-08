# -RISCV_Simulator
This is a C++ program to simulate how  RISC-V RV32I works

To make the project more efficient, we encapsulated the implementation of all instructions in one class called Execution. This class contains some functions to perform the following tasks:

Execution: This is the constructor that saves the memory file name, the instructions file name and the starting address of the program given by the user as private data members in order to use them in the following functions. 

executeInstruction: This function executes the instruction that the program counter points to by looping over the 40 instructions, deciding which one is the correct and then execute it and reassigning the program counter according the nature of the instruction. This is the function where the implementation of the 40 instructions is placed.

storeInstructions: This function is a helper function used to read the instructions from the text file and save them into the instructions vector. In this function, the loops and function names are detected and passed to a label map<int, int>; this map saves the index of the label in order to return back to this index once this label is called in complexity O(log n).

storeMemory(): This function is a helper function used to read the memory from the text file and save them into the memory map. The memory is used in a map not an array in order to optimize the memory during the insertion and optimize the time during searching for a specific memory address.

executeInstructions: This function is used to loop over the instructions and determine which instruction should be executed next. Then it calls “executeInstruction” function to execute specific instruction. 

to_binary(): This function takes a decimal number and converts it into a binary number in the string form in order to print it.

to_hexadecimal(): This function takes a decimal number and converts it into a hexa decimal number in the string form in order to print it.

void print(): This is the function that is responsible for printing the program counter, memory and registers after executing every instruction.

The flow of the project:
The user just enters the memory file name, the instruction file name and address at which the program starts. These parameters are used to create an object of class Execution. Then this object calls the store and execute functions. The function “executeInstructions” organizes the call of instructions one by one and then printing the output after executing every instructions.

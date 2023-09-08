#pragma once
#include<bits/stdc++.h>

using namespace std;

class Execution
{
private:
	string insFileName;
	string memFileName;
	fstream instructionsFile;
	vector<pair<string, vector<string>>> instructions;
	map<int, int> memory;
	vector<int> registers;
	map<string, int> label;
	int pc; //program counter
	int pcu; //program counter unofficial
	void print();
	int memAddress; //The starting index of the memory

public:
	Execution();
	Execution(string insFileName, string memFileName, int memAddress);
	void executeInstruction();
	void storeInstructions(); //This is a helper function used to read the instructions from the text file and save them into the instructions vector
	void storeMemory();
	void executeInstructions(); //This function is used to loop over the instructions and determine which instruction should be excuted
	string to_binary(int x);
	string to_hexadecimal(int x);
	string findTwoscomplement(string str);
};


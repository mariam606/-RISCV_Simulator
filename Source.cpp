#include<bits/stdc++.h>
#include"Execution.h"

using namespace std;

int main() {

	//Getting the input from the user
	string insFileName, memFileName;
	int memAddress;
	cout << "Please enter the instructions file name:\n";
	//cin >> insFileName;
	cout << "Please enter the  program starting address:\n";
	//cin >> memAddress;
	cout << "Please enter the memory file name:\n";
	//cin >> memFileName;
	//Starting Execution
	insFileName = "instructions.txt";
	memFileName = "memory.txt";
	memAddress = 1024;
	Execution(insFileName, memFileName, memAddress);
	
	return 0;
}
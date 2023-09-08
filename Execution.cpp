#include "Execution.h"

Execution::Execution() {
}

Execution::Execution(string insFileName, string memFileName, int memAddress)
{
	this->insFileName = insFileName;
	this->memFileName = memFileName;
	this->memAddress = memAddress;
	pcu = 0;
	registers.resize(32);

	//Calling the store and execute functions
	storeInstructions();
	storeMemory();
	//Printing the instructions vector
	cout << "The instructions: \n";
	for (auto it : instructions) {
		cout << it.first << " ";
		for (auto it2 : it.second)
			cout << it2 << " ";
		cout << endl;
	}
	executeInstructions();
}


void Execution::storeInstructions()
{
	//Opening the file
	instructionsFile.open(insFileName);

	//Validating that the text file containing the instructions was open.
	if (!instructionsFile.is_open())
	{
		cout << "Error Opening the Instructions File! " << endl;
		exit(0);
	}

	string line, word;

	//Reading the file
	while (!instructionsFile.eof())
	{

		string instr_temp; //instruction word
		vector<string> operands_temp; //operands

		int i = 0;

		//Saving the instruction into the variable line
		getline(instructionsFile, line);

		//Converting the line string into stream
		stringstream s(line);

		//Reading the instructions word by word and storing them in the instructions vector.
		while (getline(s, word, ' '))
		{
			if (i == 0)
			{
				instr_temp = word;
				operands_temp.clear();
				i++;
				//Handling the loops
				if (instr_temp[instr_temp.length() - 1] == ':') {
					instr_temp.erase(instr_temp.length() - 1, instr_temp.length());
					label[instr_temp] = instructions.size();
					i = 0;
				}
			}
			else
			{
				//Deleting the x from the register name
				if (word[0] == 'x')
					word.erase(0, 1);
				//cout << word[word.length()-1] << endl;
				if (word[word.length() - 1] == ')') {
					int offset;
					//string part1, part2;
					for (int l = 0; l < word.length(); l++) {
						if (word[l] == '(')  //&& word[l] != ')' && word[l] != 'x')
							offset = l;
					}
					string part1(word, 0, offset);
					string part2(word, offset + 2, word.length() - 1);
					part2.erase(part2.length() - 1, part2.length());
					operands_temp.push_back(part1);
					operands_temp.push_back(part2);
				}
				else
					operands_temp.push_back(word);
			}
		}
		instructions.push_back({ instr_temp, operands_temp });
	}
	instructionsFile.close();
}

void Execution::storeMemory() {
	//Opening the file
	instructionsFile.open(memFileName);

	//Validating that the text file containing the instructions was open.
	if (!instructionsFile.is_open())
	{
		cout << "Error Opening the Memory File! " << endl;
		exit(0);
	}

	string line, word;

	//Reading the file
	while (!instructionsFile.eof())
	{

		int x1, x2; //Address and Value

		int i = 0;

		//Saving the memory into the variable line
		getline(instructionsFile, line);

		//Converting the line string into stream
		stringstream s(line);

		//Reading the memory word by word and storing them in the instructions vector.
		while (getline(s, word, ' '))
		{
			if (i == 0)
			{
				x1 = stoi(word);
				i++;
			}
			else
			{
				x2 = stoi(word);
			}
		}
		memory[x1] = x2;
	}
	instructionsFile.close();
}


void Execution::executeInstructions()
{
	while (instructions[pcu].first != "ECALL") {
		registers[0] = 0;
		executeInstruction();
		pc = (pcu - 1) * 4 + memAddress;
		print();
	}
}

void Execution::executeInstruction()
{
	if (instructions[pcu].first == "ADDI")
	{
		registers[stoi(instructions[pcu].second[0])] = registers[stoi(instructions[pcu].second[1])] + stoi(instructions[pcu].second[2]);
		pcu++;
		return;
	}

	if (instructions[pcu].first == "ADD")
	{
		registers[stoi(instructions[pcu].second[0])] = registers[stoi(instructions[pcu].second[1])] + registers[stoi(instructions[pcu].second[2])];
		pcu++;
		return;
	}

	if (instructions[pcu].first == "SUB")
	{
		registers[stoi(instructions[pcu].second[0])] = registers[stoi(instructions[pcu].second[1])] - registers[stoi(instructions[pcu].second[2])];
		pcu++;
		return;
	}

	if (instructions[pcu].first == "SLT")
	{
		if (registers[stoi(instructions[pcu].second[1])] < registers[stoi(instructions[pcu].second[2])])
		{
			registers[stoi(instructions[pcu].second[0])] = 1;
			pcu++;
		}
		else
		{
			registers[stoi(instructions[pcu].second[0])] = 0;
			pcu++;
		}
		return;
	}


	if (instructions[pcu].first == "SLTI")
	{
		if (registers[stoi(instructions[pcu].second[1])] < stoi(instructions[pcu].second[2]))
		{
			registers[stoi(instructions[pcu].second[0])] = 1;
			pcu++;
		}
		else
		{
			registers[stoi(instructions[pcu].second[0])] = 0;
			pcu++;
		}
		return;
	}


	if (instructions[pcu].first == "SLTIU")
	{
		if (abs(registers[stoi(instructions[pcu].second[1])]) < abs(stoi(instructions[pcu].second[2])))
		{
			registers[stoi(instructions[pcu].second[0])] = 1;
			pcu++;
		}
		else
		{
			registers[stoi(instructions[pcu].second[0])] = 0;
			pcu++;
		}
		return;
	}

	if (instructions[pcu].first == "SLTU")
	{
		if (unsigned int(registers[stoi(instructions[pcu].second[1])]) < unsigned int(registers[stoi(instructions[pcu].second[2])]))
		{
			registers[stoi(instructions[pcu].second[0])] = 1;
			pcu++;
		}
		else
		{
			registers[stoi(instructions[pcu].second[0])] = 0;
			pcu++;
		}
		return;
	}

	if (instructions[pcu].first == "XORI")
	{
		string rs1;
		rs1 = bitset<32>(registers[stoi(instructions[pcu].second[1])]).to_string();

		string imm;
		imm = bitset<32>(stoi(instructions[pcu].second[2])).to_string();

		string rd;

		char e;
		for (int i = 0; i < rs1.length(); i++)
		{
			if (rs1[i] == '1' && imm[i] == '1')
			{
				char e = '0';
				rd += e;
				continue;
			}
			if (rs1[i] == '0' && imm[i] == '0')
			{
				char e = '0';
				rd += e;
				continue;
			}
			else
			{
				char e = '1';
				rd += e;
			}
		}

		int number = 0;
		int counter = 0;
		for (int i = rd.length() - 1; i >= 0; i--) {

			if (rd[i] == '1') {
				number += pow(2, counter);
			}
			counter++;
		}
		registers[stoi(instructions[pcu].second[0])] = number;
		pcu++;
		return;
	}

	if (instructions[pcu].first == "XOR")
	{
		string rs1;
		rs1 = bitset<32>(registers[stoi(instructions[pcu].second[1])]).to_string();

		string rs2;
		rs2 = bitset<32>(registers[stoi(instructions[pcu].second[2])]).to_string();

		string rd;

		char e;
		for (int i = 0; i < rs1.length(); i++)
		{
			if (rs1[i] == '1' && rs2[i] == '1')
			{
				char e = '0';
				rd += e;
				continue;
			}
			if (rs1[i] == '0' && rs2[i] == '0')
			{
				char e = '0';
				rd += e;
				continue;
			}
			else
			{
				char e = '1';
				rd += e;
			}
		}

		int number = 0;
		int counter = 0;
		for (int i = rd.length() - 1; i >= 0; i--) {

			if (rd[i] == '1') {
				number += pow(2, counter);
			}
			counter++;
		}
		registers[stoi(instructions[pcu].second[0])] = number;
		pcu++;
		return;
	}

	if (instructions[pcu].first == "ORI")
	{
		string rs1;

		rs1 = bitset<32>(registers[stoi(instructions[pcu].second[1])]).to_string();

		string imm;
		imm = bitset<32>(stoi(instructions[pcu].second[2])).to_string();

		string rd;

		char e;
		for (int i = 0; i < rs1.length(); i++)
		{
			if (rs1[i] == '0' && imm[i] == '0')
			{
				char e = '0';
				rd += e;
			}
			else
			{
				char e = '1';
				rd += e;
			}
		}

		int number = 0;
		int counter = 0;
		for (int i = rd.length() - 1; i >= 0; i--) {

			if (rd[i] == '1') {
				number += pow(2, counter);
			}
			counter++;
		}
		cout << number << endl;
		registers[stoi(instructions[pcu].second[0])] = number;
		pcu++;
		return;
	}

	if (instructions[pcu].first == "OR")
	{
		string rs1;

		rs1 = bitset<32>(registers[stoi(instructions[pcu].second[1])]).to_string();

		string rs2;
		rs2 = bitset<32>(registers[stoi(instructions[pcu].second[2])]).to_string();

		string rd;

		char e;
		for (int i = 0; i < rs1.length(); i++)
		{
			if (rs1[i] == '0' && rs2[i] == '0')
			{
				char e = '0';
				rd += e;
			}
			else
			{
				char e = '1';
				rd += e;
			}
		}

		int number = 0;
		int counter = 0;
		for (int i = rd.length() - 1; i >= 0; i--) {

			if (rd[i] == '1') {
				number += pow(2, counter);
			}
			counter++;
		}
		registers[stoi(instructions[pcu].second[0])] = number;
		pcu++;
		return;
	}

	if (instructions[pcu].first == "ANDI")
	{
		string rs1;

		rs1 = bitset<32>(registers[stoi(instructions[pcu].second[1])]).to_string();

		string imm;
		imm = bitset<32>(stoi(instructions[pcu].second[2])).to_string();

		string rd;

		char e;
		for (int i = 0; i < rs1.length(); i++)
		{
			if (rs1[i] == '1' && imm[i] == '1')
			{
				char e = '1';
				rd += e;
			}
			else
			{
				char e = '0';
				rd += e;
			}
		}

		int number = 0;
		int counter = 0;
		for (int i = rd.length() - 1; i >= 0; i--) {

			if (rd[i] == '1') {
				number += pow(2, counter);
			}
			counter++;
		}
		registers[stoi(instructions[pcu].second[0])] = number;
		pcu++;
		return;
	}

	if (instructions[pcu].first == "SLLI")
	{
		registers[stoi(instructions[pcu].second[0])] = unsigned int(registers[stoi(instructions[pcu].second[1])]) * unsigned int(pow(2, stoi(instructions[pcu].second[2])));
		pcu++;
		return;
	}

	if (instructions[pcu].first == "SLL")
	{
		registers[stoi(instructions[pcu].second[0])] = unsigned int(registers[stoi(instructions[pcu].second[1])]) * unsigned int(pow(2, registers[stoi(instructions[pcu].second[2])]));
		pcu++;
		return;
	}


	if (instructions[pcu].first == "SRLI")
	{
		registers[stoi(instructions[pcu].second[0])] = unsigned int(registers[stoi(instructions[pcu].second[1])]) / unsigned int(pow(2, stoi(instructions[pcu].second[2])));
		pcu++;
		return;
	}

	if (instructions[pcu].first == "SRL")
	{
		registers[stoi(instructions[pcu].second[0])] = unsigned int(registers[stoi(instructions[pcu].second[1])]) / unsigned int(pow(2, registers[stoi(instructions[pcu].second[2])]));
		pcu++;
		return;
	}

	if (instructions[pcu].first == "SRAI")
	{
		registers[stoi(instructions[pcu].second[0])] = registers[stoi(instructions[pcu].second[1])] / pow(2, stoi(instructions[pcu].second[2]));
		pcu++;
		return;
	}


	if (instructions[pcu].first == "SRA")
	{
		registers[stoi(instructions[pcu].second[0])] = registers[stoi(instructions[pcu].second[1])] / pow(2, registers[stoi(instructions[pcu].second[2])]);
		pcu++;
		return;
	}




	if (instructions[pcu].first == "AND")
	{
		string rs1;

		rs1 = bitset<32>(registers[stoi(instructions[pcu].second[1])]).to_string();

		string rs2;
		rs2 = bitset<32>(registers[stoi(instructions[pcu].second[2])]).to_string();

		string rd;

		char e;
		for (int i = 0; i < rs1.length(); i++)
		{
			if (rs1[i] == '1' && rs2[i] == '1')
			{
				char e = '1';
				rd += e;
			}
			else
			{
				char e = '0';
				rd += e;
			}
		}

		int number = 0;
		int counter = 0;
		for (int i = rd.length() - 1; i >= 0; i--) {

			if (rd[i] == '1') {
				number += pow(2, counter);
			}
			counter++;
		}
		registers[stoi(instructions[pcu].second[0])] = number;
		pcu++;
		return;
	}

	//LUI
	if (instructions[pcu].first == "LUI") {
		registers[stoi(instructions[pcu].second[0])] = (stoi(instructions[pcu].second[1]) << 12); //rd = imm<<12 -- imm*2^12
		pcu++;
		return;
	}

	//AUIPC
	if (instructions[pcu].first == "AUIPC") {
		registers[stoi(instructions[pcu].second[0])] = pcu + (stoi(instructions[pcu].second[1]) << 12); //rd = PC + imm<<12
		pcu++;
		return;
	}


	//JAL
	if (instructions[pcu].first == "JAL") {
		registers[stoi(instructions[pcu].second[0])] = pcu + 1; //rd = next address
		cout << pcu;
		pcu = label[instructions[pcu].second[1]];//new PC = label ------------------------------------
		return;
	}

	//JALR
	if (instructions[pcu].first == "JALR") {
		registers[stoi(instructions[pcu].second[0])] = pcu + 1; //rd = x0? don't modify ---------------------- we can set it at the end instead
		registers[0] = 0;
		int offset = stoi(instructions[pcu].second[1]);
		int rs1 = registers[stoi(instructions[pcu].second[2])];
		pcu = rs1 + offset; //set the least significant bit to 0 ---------------------------??	
		return;
	}

	//BEQ 
	if (instructions[pcu].first == "BEQ") {
		if (registers[stoi(instructions[pcu].second[0])] == registers[stoi(instructions[pcu].second[1])]) {
			pcu = label[instructions[pcu].second[2]];//new PC = label ------------------------------------
		}
		else pcu++;
		return;
	}
	//BNE
	if (instructions[pcu].first == "BNE") {
		if (registers[stoi(instructions[pcu].second[0])] != registers[stoi(instructions[pcu].second[1])]) {
			pcu = label[instructions[pcu].second[2]];//new PC = label ------------------------------------
		}
		else pcu++;
		return;
	}

	//BLT
	if (instructions[pcu].first == "BLT") {
		if (registers[stoi(instructions[pcu].second[0])] < registers[stoi(instructions[pcu].second[1])]) {
			pcu = label[instructions[pcu].second[2]];//new PC = label ------------------------------------
		}
		else pcu++;
		return;
	}
	//BGE
	if (instructions[pcu].first == "BGE") {
		if (registers[stoi(instructions[pcu].second[0])] > registers[stoi(instructions[pcu].second[1])]) {
			pcu = label[instructions[pcu].second[2]];//new PC = label ------------------------------------
		}
		else pcu++;
		return;
	}
	//BLTU
	if (instructions[pcu].first == "BLTU") {
		int rs1 = registers[stoi(instructions[pcu].second[0])],
			rs2 = registers[stoi(instructions[pcu].second[1])];
		if (rs1 < rs2) {
			pcu = label[instructions[pcu].second[2]];//new PC = label ------------------------------------
		}
		else pcu++;
		return;
	}
	//BGEU
	if (instructions[pcu].first == "BGEU") {
		unsigned int rs1 = registers[stoi(instructions[pcu].second[0])],
			rs2 = registers[stoi(instructions[pcu].second[1])];
		if (rs1 >= rs2) {
			pcu = label[instructions[pcu].second[2]];//new PC = label ------------------------------------
		}
		else pcu++;
		return;
	}
	//from memory to reg 
	//LB -- int8_t
	if (instructions[pcu].first == "LB") {//LB X1, 0(X2)
		int rs1 = registers[stoi(instructions[pcu].second[2])],
			offset = stoi(instructions[pcu].second[1]);
		int8_t temp = memory[rs1 + offset]; //int8_t to load just 8 bits 
		registers[stoi(instructions[pcu].second[0])] = temp; //rd = memory[rs1 + offset] 	
		pcu++;
		return;
	}
	//LH -- int - 16 bits
	if (instructions[pcu].first == "LH") {
		int rs1 = registers[stoi(instructions[pcu].second[2])],
			offset = stoi(instructions[pcu].second[1]);
		int16_t temp = memory[rs1 + offset]; //int8_t to load just 8 bits 
		registers[stoi(instructions[pcu].second[0])] = temp; //rd = memory[rs1 + offset] 	
		pcu++;
		return;
	}
	//LW 32 bits
	if (instructions[pcu].first == "LW") {
		int rs1 = registers[stoi(instructions[pcu].second[2])],
			offset = stoi(instructions[pcu].second[1]);
		registers[stoi(instructions[pcu].second[0])] = memory[rs1 + offset]; //rd = memory[rs1 + offset] 	
		pcu++;
		return;
	}
	//LBU - uint8_t
	if (instructions[pcu].first == "LBU") {
		int rs1 = registers[stoi(instructions[pcu].second[2])],
			offset = stoi(instructions[pcu].second[1]);
		uint8_t temp = memory[rs1 + offset]; //uint8_t to load just 8 unsigned bits 
		registers[stoi(instructions[pcu].second[0])] = temp; //rd = memory[rs1 + offset] 	 
		pcu++;
		return;
	}
	//LHU
	if (instructions[pcu].first == "LHU") {
		int rs1 = registers[stoi(instructions[pcu].second[2])],
			offset = stoi(instructions[pcu].second[1]);
		uint16_t temp = memory[rs1 + offset]; //unsigned short int to load just 16 bits 
		registers[stoi(instructions[pcu].second[0])] = temp; //rd = memory[rs1 + offset] 	 
		pcu++;
		return;
	}

	//from reg to memory
	//SB - //SB X1, 0(X2)
	if (instructions[pcu].first == "SB") {
		int rs1 = registers[stoi(instructions[pcu].second[2])],
			offset = stoi(instructions[pcu].second[1]);
		int8_t temp = registers[stoi(instructions[pcu].second[0])]; //int8 to store just 8 bits 
		memory[rs1 + offset] = temp;
		pcu++;
		return;
	}
	//SH
	if (instructions[pcu].first == "SH") {
		int rs1 = registers[stoi(instructions[pcu].second[2])],
			offset = stoi(instructions[pcu].second[1]);
		int16_t temp = registers[stoi(instructions[pcu].second[0])]; //int8 to store just 16 bits 
		memory[rs1 + offset] = temp;
		pcu++;
		return;
	}
	//SW
	if (instructions[pcu].first == "SW") {
		int rs1 = registers[stoi(instructions[pcu].second[2])],
			offset = stoi(instructions[pcu].second[1]);
		memory[rs1 + offset] = registers[stoi(instructions[pcu].second[0])]; //storing the whole 32 bits 
		pcu++;
		return;
	}
}

string Execution::to_binary(int n) {
	bool convert = 0;
	if (n < 0) {
		convert = 1;
		n = -1 * n;
	}

	vector<int> binaryNum(32, 0);
	int i = 0;
	while (n > 0) {
		binaryNum[i] = n % 2;
		n = n / 2;
		i++;
	}
	stringstream ss;
	copy(binaryNum.begin(), binaryNum.end(), ostream_iterator<int>(ss, " "));
	string s = ss.str();
	s = s.substr(0, s.length() - 1);
	if (convert) s[s.size() - 1] = '-';
	reverse(s.begin(), s.end());
	return s;
}

string Execution::findTwoscomplement(string str)
{
	int n = str.length();
	int i;
	for (i = n - 1; i >= 0; i--)
		if (str[i] == '1')
			break;
	if (i == -1)
		return '1' + str;
	for (int k = i - 1; k >= 0; k--)
	{
		if (str[k] == '1')
			str[k] = '0';
		else
			str[k] = '1';
	}
	return str;
}

string Execution::to_hexadecimal(int n) {
	bool convert = 0;
	if (n < 0) {
		convert = 1;
		n = -1 * n;
	}
	if (n < 0) {
		cout << "-";
		n = -1 * n;
	}
	vector<char> hexaDeciNum(1, 0);
	int i = 0;
	while (n != 0) {
		int temp = 0;
		temp = n % 16;
		if (temp < 10) {
			hexaDeciNum.push_back(temp + 48);
			i++;
		}
		else {
			hexaDeciNum.push_back(temp + 55);
			i++;
		}
		n = n / 16;
	}
	string str(hexaDeciNum.begin(), hexaDeciNum.end());
	if (convert) str[str.size() - 1] = '-';
	reverse(str.begin(), str.end());
	return str;
}

void Execution::print() {

	//printing the program counter
	cout << "Program Counter: " << pc << "    " << to_binary(pc) << "    " << to_hexadecimal(pc) << endl;
	cout << "Registers:\n";
	//printing the registers
	for (int i = 0; i < registers.size(); i++)
		cout << "x" << i << "  " << registers[i] << "    " << to_binary(registers[i]) << "    " << to_hexadecimal(registers[i]) << endl;
	//printing the memory
	cout << "Memory\n";
	for (auto it : memory)
		cout << it.first << " : " << it.second << "    " << to_binary(it.second) << "    " << to_hexadecimal(it.second) << endl;

	//Finalizing the test case
	for (int i = 0; i < 80; i++)
		cout << "-";
	cout << endl;
}


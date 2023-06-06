#pragma once
#ifndef PARSELOAD_CPP
#define PARSELOAD_CPP
#include "global.h"
#include <fstream>
#include <iostream>

using namespace std;


Inst parse_code_line(string instruction)
{
    Inst inst;
    string line = instruction;
    string opcode = line.substr(0, line.find(" "));
    line = line.substr(line.find(" ") + 1);
    if(opcode == "load" || opcode == "store")
    {
        string destination = line.substr(0, line.find(","));
        line = line.substr(line.find(",") + 1);
        string offset = line.substr(0, line.find("("));
        line = line.substr(line.find("(") + 1);
        string source = line.substr(0, line.find(")"));
        inst = Inst(opcode, stoi(destination), stoi(source), -1, stoi(offset));
    }
    else if(opcode == "jal")
    {
        string destination = line;
        inst = Inst("jal", -1, -1, -1, stoi(destination));
    }
    else if (opcode == "ret")
    {
        inst = Inst("ret", -1, -1, -1, -1);
    }
    else if(opcode == "add" || opcode == "nand" || opcode == "sll")
    {
        string destination = line.substr(0, line.find(","));
        line = line.substr(line.find(",") + 1);
        string source1 = line.substr(0, line.find(","));
        line = line.substr(line.find(",") + 1);
        string source2 = line;
        inst = Inst(opcode, stoi(destination), stoi(source1), stoi(source2), -1);
    }
    else if (opcode == "addi")
    {
        string destination = line.substr(0, line.find(","));
        line = line.substr(line.find(",") + 1);
        string source1 = line.substr(0, line.find(","));
        line = line.substr(line.find(",") + 1);
        string immediate = line;
        inst = Inst(opcode, stoi(destination), stoi(source1), -1, stoi(immediate));
    }
    else if(opcode == "bne")
    {
        string source1 = line.substr(0, line.find(","));
        line = line.substr(line.find(",") + 1);
        string source2 = line.substr(0, line.find(","));
        line = line.substr(line.find(",") + 1);
        string immediate = line;
        inst = Inst(opcode, stoi(source1), stoi(source2), -1, stoi(immediate));
    }
    else if(opcode == "neg")
    {
        string destination = line.substr(0, line.find(","));
        line = line.substr(line.find(",") + 1);
        string source = line;
        inst = Inst(opcode, stoi(destination), stoi(source), -1, -1);
    }
    else
    {
        cout << "Error: Invalid opcode" << endl;
    }

    return inst;
}

void load_code(string file)
{
    ifstream code_file;
    string line;
    code_file.open(file);
    if ( code_file.is_open() ) {
        // read line by line from file
        while ( getline(code_file, line) ) {
            Inst inst = parse_code_line(line);
            inst.index = instructions.size();
            instructions.push_back(inst);
        }
    } else {
        cout<<"Error: Code File couldn't be opened."<<endl;
        exit(1);
    }
}

void load_memory(string file)
{
    ifstream memory_file;
    string line, key, value;
    memory_file.open(file);
    if ( memory_file.is_open() ) {
        while ( memory_file ) {
            memory_file>>key; memory_file>>value;
            memory[stoi(key)] = stoi(value);
        }
    } else {
        cout<<"Error: Memory File couldn't be opened."<<endl;
        exit(1);
    }
}


#endif // !PARSELOAD_CPP
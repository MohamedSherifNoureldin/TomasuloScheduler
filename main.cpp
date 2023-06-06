#ifndef MAIN_CPP
#define MAIN_CPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <bits/stdc++.h>

#include "global.h"
#include "parseload.cpp"
#include "issue.cpp"
#include "execute.cpp"
#include "write.cpp"

using namespace std;

int main()
{    
    int loadUnits = 2, storeUnits = 2, bneUnits = 1, jalRetUnits = 1, addAddiUnits = 3, nandUnits = 1, sllUnits = 1, negUnits = 1;
    int loadUnitsCycles = 2, storeUnitsCycles = 2, bneUnitsCycles = 1, jalRetUnitsCycles = 1, addAddiUnitsCycles = 2, nandUnitsCycles = 1, sllUnitsCycles = 8, negUnitsCycles = 2;
    string choice;
    int pc_start;
    cout<<"Welcome to the Tomasulo Scheduling Algorithm Simulator!"<<endl;

    // loading functional units inforamtion
    cout<<"Default simulator has the following functional units:"<<endl;
    cout<<"- 2 LOAD units -> 2 cycles"<<endl;
    cout<<"- 2 STORE units -> 2 cycles"<<endl;
    cout<<"- 1 BNE unit -> 1 cycle"<<endl;
    cout<<"- 1 JAL/RET unit -> 1 cycle"<<endl;
    cout<<"- 3 ADD/ADDI unit -> 2 cycles"<<endl;
    cout<<"- 1 NEG unit -> 2 cycles"<<endl;
    cout<<"- 1 NAND unit -> 1 cycle"<<endl;
    cout<<"- 1 SLL unit -> 8 cycles"<<endl;

    cout<<"Would you like to change the default functional units? (y or n): ";
    cin>>choice;
    if(choice=="y")
    {
        cout<<"Please input the number of LOAD units: ";
        cin>>choice;
        loadUnits = stoi(choice);
        cout<<"Please input the number of cycles for LOAD units: ";
        cin>>choice;
        loadUnitsCycles = stoi(choice);

        cout<<"Please input the number of STORE units: ";
        cin>>choice;
        storeUnits = stoi(choice);
        cout<<"Please input the number of cycles for STORE units: ";
        cin>>choice;
        storeUnitsCycles = stoi(choice);

        cout<<"Please input the number of BNE units: ";
        cin>>choice;
        bneUnits = stoi(choice);
        cout<<"Please input the number of cycles for BNE units: ";
        cin>>choice;
        bneUnitsCycles = stoi(choice);

        cout<<"Please input the number of JAL/RET units: ";
        cin>>choice;
        jalRetUnits = stoi(choice);
        cout<<"Please input the number of cycles for JAL/RET units: ";
        cin>>choice;
        jalRetUnitsCycles = stoi(choice);

        cout<<"Please input the number of ADD/ADDI units: ";
        cin>>choice;
        addAddiUnits = stoi(choice);
        cout<<"Please input the number of cycles for ADD/ADDI units: ";
        cin>>choice;
        addAddiUnitsCycles = stoi(choice);

        cout<<"Please input the number of NEG units: ";
        cin>>choice;
        negUnits = stoi(choice);
        cout<<"Please input the number of cycles for NEG units: ";
        cin>>choice;
        negUnitsCycles = stoi(choice);

        cout<<"Please input the number of NAND units: ";
        cin>>choice;
        nandUnits = stoi(choice);
        cout<<"Please input the number of cycles for NAND units: ";
        cin>>choice;
        nandUnitsCycles = stoi(choice);

        cout<<"Please input the number of SLL units: ";
        cin>>choice;
        sllUnits = stoi(choice);
        cout<<"Please input the number of cycles for SLL units: ";
        cin>>choice;
        sllUnitsCycles = stoi(choice);
        
        cout<<endl<<endl;
    }

    init_RS(loadUnits, storeUnits, bneUnits, jalRetUnits, addAddiUnits, nandUnits, sllUnits, negUnits, loadUnitsCycles, storeUnitsCycles, bneUnitsCycles, jalRetUnitsCycles, addAddiUnitsCycles, nandUnitsCycles, sllUnitsCycles, negUnitsCycles);

    // loading code 
    cout<<"Please input code filename with full absolute path: ";
    cin>>choice;
    load_code(choice);

    // loading data to memory
    cout<<"Would you like to load data into memory from file? (y or n): ";
    cin>>choice;
    if(choice=="y")
    {
        cout<<"Please input filename with full absolute path: ";
        cin>>choice;
        load_memory(choice);
        // print memory content
        cout<<"Memory content:"<<endl;
        for(int i=0; i<memory.size(); i++)
        {
            cout<<memory[i]<<endl;
        }
        cout<<"Finished memory content"<<endl;
    }

    init_registers();
    
    PC = 0;

    while(!checkAllFinished() || cycles == 1)
    {
        // issue
        if(PC < instructions.size())
        {
            if(instructions[PC].status == NOTYET || instructions[PC].status == WRITTEN)
            {
                if(canIssue(instructions[PC], cycles))
                    PC++;
            }
        }

        // execute
        for(int i = 0; i < instructions.size(); i++)
        {
            // check if the instruction is issued
            if(instructions[i].status == ISSUED && instructions[i].issue != cycles)
            {
                if (canExecute(instructions[i], cycles))
                    execute(instructions[i], cycles);
            } 
            else if(instructions[i].status == EXECUTING)
            {
                if(instructions[i].execFinishTime == cycles)
                {
                    instructions[i].status = EXECUTED;
                }
            }
        }

        // write
        for(int i = 0; i < instructions.size(); i++)
        {
            if(instructions[i].status == EXECUTED && instructions[i].execFinishTime != cycles)
            {
               write(instructions[i], cycles);
               break;
            }
        }

        cout<<"Cycle "<<cycles<<endl;
        print_updates();
        cout<<"----------------------------------------------------------------------------------------------------------------------------------"<<endl;

        cycles++;  
    }

    cout<<"END: Simulation finished"<<endl;
    // print formated table of columns: instruction, issue time, start execution time, end execution time, write result time
    for(int i=0; i<92; i++)
        cout<<"-";
    cout<<endl;
    cout<<"|"<<setw(15)<<"Instruction"<<setw(15)<<"Issue Time"<<setw(15)<<"Start Exec"<<setw(15)<<"End Exec"<<setw(15)<<"Write Result"<<setw(15)<<"Status"<<"|"<<endl;
    for(int i=0; i<instructions.size(); i++)
    {
        Inst inst = instructions[i];
        string status = "";
        if(inst.status == 0)
            status = "NOTYET";
        else if(inst.status == 1)
            status = "ISSUED";
        else if(inst.status == 2)
            status = "EXECUTING";
        else if(inst.status == 3)
            status = "EXECUTED";
        else if(inst.status == 4)
            status = "WRITTEN";

        cout<<"|"<<setw(15)<<inst.type<<setw(15)<<inst.issue<<setw(15)<<inst.execStartTime<<setw(15)<<inst.execFinishTime<<setw(15)<<inst.write<<setw(15)<<status<<"|"<<endl;
    }
    for(int i=0; i<92; i++)
        cout<<"-";
    cout<<endl;
    cout<<endl;

    cout<<"Total Execution time as cycles spanned : "<<cycles-1<<" cycles"<<endl;
    cout<<"IPC: "<<numberOfFinishedInstructions<<"/"<<(cycles-1)<<" = "<<(float)numberOfFinishedInstructions/(cycles-1)<<endl;
    if(numberOfBranchesEncountered == 0)
        cout<<"Branch Misprediction Percentage: 0%"<<endl;
    else
        cout<<"Branch Misprediction Percentage: "<<(float)(numberOfBranchesMispredicted*100)/numberOfBranchesEncountered<<"%"<<endl;
}

#endif // MAIN_CPP
#pragma once
#ifndef DEPS_CPP
#define DEPS_CPP
#include "global.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <bits/stdc++.h>
using namespace std;


int computeTwosComplement(int num) {
    // Check if the number is negative
    if (num < 0) {
        // Invert all the bits
        num = ~num;

        // Add 1 to the inverted number
        num += 1;
    }


    return num;
}

bool checkAllFinished()
{
    // for(int i = 0; i < instructions.size(); i++)
    // {
    //     if(instructions[i].write == -1)
    //     {
    //         return false;
    //     }
    // }
    // return true;
    // return (instructions[instructions.size() - 1].status == WRITTEN);

    // loop over all reservation fields and check if they are empty
    for (int i = 0; i < loadRS.size(); i++)
    {
        if (loadRS[i].busy)
        {
            return false;
        }
    }
    for (int i = 0; i < storeRS.size(); i++)
    {
        if (storeRS[i].busy)
        {
            return false;
        }
    }
    for (int i = 0; i < bneRS.size(); i++)
    {
        if (bneRS[i].busy)
        {
            return false;
        }
    }
    for (int i = 0; i < jalRetRS.size(); i++)
    {
        if (jalRetRS[i].busy)
        {
            return false;
        }
    }
    for (int i = 0; i < addAddiRS.size(); i++)
    {
        if (addAddiRS[i].busy)
        {
            return false;
        }
    }
    for (int i = 0; i < negRS.size(); i++)
    {
        if (negRS[i].busy)
        {
            return false;
        }
    }
    for (int i = 0; i < nandRS.size(); i++)
    {
        if (nandRS[i].busy)
        {
            return false;
        }
    }
    for (int i = 0; i < sllRS.size(); i++)
    {
        if (sllRS[i].busy)
        {
            return false;
        }
    }
    return (instructions[instructions.size() - 1].status == WRITTEN);
}


void emptyRS(int rsIndex, enum rsType rsVector)
{
    RS* row;
    switch (rsVector)
    {
    case LOAD:
        row = &loadRS[rsIndex];
        break;
    case STORE:
        row = &storeRS[rsIndex];
        break;
    case BNE:
        row = &bneRS[rsIndex];
        break;
    case JALRET:
        row = &jalRetRS[rsIndex];
        break;
    case ADDADDI:
        row = &addAddiRS[rsIndex];
        break;
    case NAND:
        row = &nandRS[rsIndex];
        break;
    case SLL:
        row = &sllRS[rsIndex];
        break;
    case NEG:
        row = &negRS[rsIndex];
        break;
    default:
        return;
    }

    row->busy = false;
    row->op = "";
    row->vj = -1;
    row->vk = -1;
    row->qj = "";
    row->qk = "";
    row->A = -1;
    row->instructionIndex = -1;
    row->result = -1;
}

void notifyAll(string finishedRsName, int resultValue)
{
    // notify load RS
    for(int i = 0; i < loadRS.size(); i++)
    {
        if(loadRS[i].qj == finishedRsName)
        {
            if(regMap[instructions[loadRS[i].instructionIndex].rB] == "")
            {
                loadRS[i].vj = resultValue;
                loadRS[i].qj = "";
            } else {
                loadRS[i].vj = -1;
                loadRS[i].qj = regMap[instructions[loadRS[i].instructionIndex].rB];
            }
        }
    }

    // notify store RS
    for(int i = 0; i < storeRS.size(); i++)
    {
        if(storeRS[i].qj == finishedRsName)
        {
            if(regMap[instructions[storeRS[i].instructionIndex].rB] == "")
            {
                storeRS[i].vj = resultValue;
                storeRS[i].qj = "";
            } else {
                storeRS[i].vj = -1;
                storeRS[i].qj = regMap[instructions[storeRS[i].instructionIndex].rB];
            }
        }
        if(storeRS[i].qk == finishedRsName)
        {
            if(regMap[instructions[storeRS[i].instructionIndex].rA] == "")
            {
                storeRS[i].vk = resultValue;
                storeRS[i].qk = "";
            } else {
                storeRS[i].vk = -1;
                storeRS[i].qk = regMap[instructions[storeRS[i].instructionIndex].rA];
            }
        }
    }

    // notify bne RS
    for(int i = 0; i < bneRS.size(); i++)
    {
        if(bneRS[i].qj == finishedRsName)
        {
            if(regMap[instructions[bneRS[i].instructionIndex].rA] == "")
            {
                bneRS[i].vj = resultValue;
                bneRS[i].qj = "";
            } else {
                bneRS[i].vj = -1;
                bneRS[i].qj = regMap[instructions[bneRS[i].instructionIndex].rA];
            }
        }
        if(bneRS[i].qk == finishedRsName)
        {
            if(regMap[instructions[bneRS[i].instructionIndex].rB] == "")
            {
                bneRS[i].vk = resultValue;
                bneRS[i].qk = "";
            } else {
                bneRS[i].vk = -1;
                bneRS[i].qk = regMap[instructions[bneRS[i].instructionIndex].rB];
            }
        }
    }

    // notify jalRet RS
    for(int i = 0; i < jalRetRS.size(); i++)
    {
        if(jalRetRS[i].qj == finishedRsName)
        {
            if(regMap[1] == "")
            {
                jalRetRS[i].vj = resultValue;
                jalRetRS[i].qj = "";
            } else {
                jalRetRS[i].vj = -1;
                jalRetRS[i].qj = regMap[1];
            }
        }
    }

    // notify addAddi RS
    for(int i = 0; i < addAddiRS.size(); i++)
    {
        if(addAddiRS[i].qj == finishedRsName)
        {
            if(regMap[instructions[addAddiRS[i].instructionIndex].rB] == "")
            {
                addAddiRS[i].vj = resultValue;
                addAddiRS[i].qj = "";
            } else {
                addAddiRS[i].vj = -1;
                addAddiRS[i].qj = regMap[instructions[addAddiRS[i].instructionIndex].rB];
            }
        }
        if(addAddiRS[i].qk == finishedRsName)
        {
            if(regMap[instructions[addAddiRS[i].instructionIndex].rC] == "")
            {
                addAddiRS[i].vk = resultValue;
                addAddiRS[i].qk = "";
            } else {
                addAddiRS[i].vk = -1;
                addAddiRS[i].qk = regMap[instructions[addAddiRS[i].instructionIndex].rC];
            }
        }
    }

    // notify nand RS
    for(int i = 0; i < nandRS.size(); i++)
    {
        if(nandRS[i].qj == finishedRsName)
        {
            if(regMap[instructions[nandRS[i].instructionIndex].rB] == "")
            {
                nandRS[i].vj = resultValue;
                nandRS[i].qj = "";
            } else {
                nandRS[i].vj = -1;
                nandRS[i].qj = regMap[instructions[nandRS[i].instructionIndex].rB];
            }
        }
        if(nandRS[i].qk == finishedRsName)
        {
            if(regMap[instructions[nandRS[i].instructionIndex].rC] == "")
            {
                nandRS[i].vk = resultValue;
                nandRS[i].qk = "";
            } else {
                nandRS[i].vk = -1;
                nandRS[i].qk = regMap[instructions[nandRS[i].instructionIndex].rC];
            }
        }
    }

    // notify sll RS
    for(int i = 0; i < sllRS.size(); i++)
    {
        if(sllRS[i].qj == finishedRsName)
        {
            if(regMap[instructions[sllRS[i].instructionIndex].rB] == "")
            {
                sllRS[i].vj = resultValue;
                sllRS[i].qj = "";
            } else {
                sllRS[i].vj = -1;
                sllRS[i].qj = regMap[instructions[sllRS[i].instructionIndex].rB];
            }
        }
        if(sllRS[i].qk == finishedRsName)
        {
            if(regMap[instructions[sllRS[i].instructionIndex].rC] == "")
            {
                sllRS[i].vk = resultValue;
                sllRS[i].qk = "";
            } else {
                sllRS[i].vk = -1;
                sllRS[i].qk = regMap[instructions[sllRS[i].instructionIndex].rC];
            }
        }
    }

    // notify neg RS
    for(int i = 0; i < negRS.size(); i++)
    {
        if(negRS[i].qj == finishedRsName)
        {
            if(regMap[instructions[negRS[i].instructionIndex].rB] == "")
            {
                negRS[i].vj = resultValue;
                negRS[i].qj = "";
            } else {
                negRS[i].vj = -1;
                negRS[i].qj = regMap[instructions[negRS[i].instructionIndex].rB];
            }
        }
    }    
}


void init_RS(int loadUnits, int storeUnits, int bneUnits, int jalRetUnits, int addAddiUnits, int nandUnits, int sllUnits, int negUnits, int loadUnitsCycles, int storeUnitsCycles, int bneUnitsCycles, int jalRetUnitsCycles, int addAddiUnitsCycles, int nandUnitsCycles, int sllUnitsCycles, int negUnitsCycles)
{
    // load RS
    for(int i=0; i<loadUnits; i++)
    {
        RS row("load"+to_string(i+1), loadUnitsCycles);
        loadRS.push_back(row);
    }
    // store RS
    for(int i=0; i<storeUnits; i++)
    {
        RS row("store"+to_string(i+1), storeUnitsCycles);
        storeRS.push_back(row);
    }
    // bne RS
    for(int i=0; i<bneUnits; i++)
    {
        RS row("bne"+to_string(i+1), bneUnitsCycles);
        bneRS.push_back(row);
    }
    // jalRet RS
    for(int i=0; i<jalRetUnits; i++)
    {
        RS row("jalRet"+to_string(i+1), jalRetUnitsCycles);
        jalRetRS.push_back(row);
    }
    // addAddi RS
    for(int i=0; i<addAddiUnits; i++)
    {
        RS row("addAddi"+to_string(i+1), addAddiUnitsCycles);
        addAddiRS.push_back(row);
    }
    // nand RS
    for(int i=0; i<nandUnits; i++)
    {
        RS row("nand"+to_string(i+1), nandUnitsCycles);
        nandRS.push_back(row);
    }
    // sll RS
    for(int i=0; i<sllUnits; i++)
    {
        RS row("sll"+to_string(i+1), sllUnitsCycles);
        sllRS.push_back(row);
    }
    // neg RS
    for(int i=0; i<negUnits; i++)
    {
        RS row("neg"+to_string(i+1), negUnitsCycles);
        negRS.push_back(row);
    }
}

void init_registers()
{
    for(int i=0; i<8; i++)
    {
        registers.push_back(0);
        regMap[i] = "";
        regMapQueue[i] = queue<string>();
    }
}

void print_updates()
{
    cout<<"- Registers:"<<endl;
    for(int i=0; i<8; i++)
    {
        cout<<"R"<<i<<" = "<<registers[i];
        if(i != 7)
            cout<<", ";
    }
    cout<<endl;
    cout<<"- Memory:"<<endl;
    for(auto it=memory.begin(); it!=memory.end(); it++)
    {
        cout<<"M"<<it->first<<" = "<<it->second<<endl;
    }
    cout<<"- Instruction Table:"<<endl;
    for(int i=0; i<92; i++)
        cout<<"-";
    cout<<endl;
    // print formated table of columns: instruction, issue time, start execution time, end execution time, write result time
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

    cout<<"- Reservation Stations Table:"<<endl;
    // print formated table of columns: RS name, busy, op, vj, vk, qj, qk, A
    for(int i=0; i<122; i++)
        cout<<"-";
    cout<<endl;
    cout<<"|"<<setw(15)<<"RS Name"<<setw(15)<<"Busy"<<setw(15)<<"Op"<<setw(15)<<"Vj"<<setw(15)<<"Vk"<<setw(15)<<"Qj"<<setw(15)<<"Qk"<<setw(15)<<"A"<<"|"<<endl;
    for(int i=0; i<loadRS.size(); i++)
    {
        RS row = loadRS[i];
        cout<<"|"<<setw(15)<<row.name<<setw(15)<<row.busy<<setw(15)<<row.op<<setw(15)<<row.vj<<setw(15)<<row.vk<<setw(15)<<row.qj<<setw(15)<<row.qk<<setw(15)<<row.A<<"|"<<endl;
    }
    for(int i=0; i<storeRS.size(); i++)
    {
        RS row = storeRS[i];
        cout<<"|"<<setw(15)<<row.name<<setw(15)<<row.busy<<setw(15)<<row.op<<setw(15)<<row.vj<<setw(15)<<row.vk<<setw(15)<<row.qj<<setw(15)<<row.qk<<setw(15)<<row.A<<"|"<<endl;
    }
    for(int i=0; i<bneRS.size(); i++)
    {
        RS row = bneRS[i];
        cout<<"|"<<setw(15)<<row.name<<setw(15)<<row.busy<<setw(15)<<row.op<<setw(15)<<row.vj<<setw(15)<<row.vk<<setw(15)<<row.qj<<setw(15)<<row.qk<<setw(15)<<row.A<<"|"<<endl;
    }
    for(int i=0; i<jalRetRS.size(); i++)
    {
        RS row = jalRetRS[i];
        cout<<"|"<<setw(15)<<row.name<<setw(15)<<row.busy<<setw(15)<<row.op<<setw(15)<<row.vj<<setw(15)<<row.vk<<setw(15)<<row.qj<<setw(15)<<row.qk<<setw(15)<<row.A<<"|"<<endl;
    }
    for(int i=0; i<addAddiRS.size(); i++)
    {
        RS row = addAddiRS[i];
        cout<<"|"<<setw(15)<<row.name<<setw(15)<<row.busy<<setw(15)<<row.op<<setw(15)<<row.vj<<setw(15)<<row.vk<<setw(15)<<row.qj<<setw(15)<<row.qk<<setw(15)<<row.A<<"|"<<endl;
    }
    for(int i=0; i<nandRS.size(); i++)
    {
        RS row = nandRS[i];
        cout<<"|"<<setw(15)<<row.name<<setw(15)<<row.busy<<setw(15)<<row.op<<setw(15)<<row.vj<<setw(15)<<row.vk<<setw(15)<<row.qj<<setw(15)<<row.qk<<setw(15)<<row.A<<"|"<<endl;
    }
    for(int i=0; i<sllRS.size(); i++)
    {
        RS row = sllRS[i];
        cout<<"|"<<setw(15)<<row.name<<setw(15)<<row.busy<<setw(15)<<row.op<<setw(15)<<row.vj<<setw(15)<<row.vk<<setw(15)<<row.qj<<setw(15)<<row.qk<<setw(15)<<row.A<<"|"<<endl;
    }
    for(int i=0; i<negRS.size(); i++)
    {
        RS row = negRS[i];
        cout<<"|"<<setw(15)<<row.name<<setw(15)<<row.busy<<setw(15)<<row.op<<setw(15)<<row.vj<<setw(15)<<row.vk<<setw(15)<<row.qj<<setw(15)<<row.qk<<setw(15)<<row.A<<"|"<<endl;
    }
    for(int i=0; i<122; i++)
        cout<<"-";
    cout<<endl;

    cout<<endl;

    cout<<"- Register Status Table:"<<endl;
    // print formated horizontal table 2 rows and 8 columns of registers and their reservation stations
    for(int i=0; i<47; i++)
        cout<<"-";
    cout<<endl;
    cout<<"|"<<setw(15)<<"Register"<<setw(30)<<"Reservation Station"<<"|"<<endl;
    for(int i=0; i<8; i++)
    {
        string registerName = "R"+to_string(i);
        cout<<"|"<<setw(15)<<"R"+to_string(i)<<setw(30)<<regMap[i]<<"|"<<endl;
    }
    for(int i=0; i<47; i++)
        cout<<"-";
    cout<<endl;
    cout<<endl;
    cout<<"- PC: "<<PC<<endl;
    cout<<"- Number of Instructions Completed: "<<numberOfFinishedInstructions<<endl;
    cout<<"- Number of Branches Encountered: "<<numberOfBranchesEncountered<<endl;
    cout<<"- Number of Cycles Spanned: "<<cycles<<endl;
    cout<<"- Number of Branch Mispredictions: "<<numberOfBranchesMispredicted<<endl;

}


#endif
#pragma once
#ifndef WRITE_CPP
#define WRITE_CPP
#include "global.h"
#include "deps.cpp"
#include "execute.cpp"

using namespace std;
void write(Inst& inst, int t)
{
    numberOfFinishedInstructions++;
    inst.status = WRITTEN;
    inst.write = t;

    if(inst.type == "load")
    {
        RS row = loadRS[inst.rsIndex];
        if(inst.rA != 0)
            registers[inst.rA] = row.result;
        if(regMapQueue[inst.rA].size() > 0)
        {
            string regName = regMapQueue[inst.rA].front();
            regMapQueue[inst.rA].pop();
            regMap[inst.rA] = regName;
        } else {
            regMap[inst.rA] = "";
        }
        notifyAll("load" + to_string(inst.rsIndex + 1), row.result);
        emptyRS(inst.rsIndex, LOAD);
    }
    else if(inst.type == "store")
    {
        RS row = storeRS[inst.rsIndex];
        memory[row.A + row.vj] = row.vk;
        emptyRS(inst.rsIndex, STORE);
    }
    else if(inst.type == "bne")
    {
        RS row = bneRS[inst.rsIndex];
        if(row.vj != row.vk)
        {
            // branch
            PC = row.result;
            executeAfterBranch(true);
            numberOfBranchesMispredicted++;
        } else {
            // not branch
            executeAfterBranch(false);
        }
        emptyRS(inst.rsIndex, BNE);
    }
    else if(inst.type == "jal")
    {
        RS row = jalRetRS[inst.rsIndex];
        registers[1] = row.result;
        if(regMapQueue[1].size() > 0)
        {
            string regName = regMapQueue[1].front();
            regMapQueue[1].pop();
            regMap[1] = regName;
        } else {
            regMap[1] = "";
        }
        // jump
        PC = row.A;
        notifyAll("jalRet" + to_string(inst.rsIndex + 1), row.result);
        executeAfterBranch(true);
        emptyRS(inst.rsIndex, JALRET);
    }
    else if(inst.type == "ret")
    {
        RS row = jalRetRS[inst.rsIndex];
        PC = row.vj;
        executeAfterBranch(true);
        emptyRS(inst.rsIndex, JALRET);
    }
    else if(inst.type == "add")
    {
        RS row = addAddiRS[inst.rsIndex];
        if(inst.rA != 0)
            registers[inst.rA] = row.result;
        if(regMapQueue[inst.rA].size() > 0)
        {
            string regName = regMapQueue[inst.rA].front();
            regMapQueue[inst.rA].pop();
            regMap[inst.rA] = regName;
        } else {
            regMap[inst.rA] = "";
        }
        notifyAll("addAddi" + to_string(inst.rsIndex + 1), row.result);
        emptyRS(inst.rsIndex, ADDADDI);
    }
    else if(inst.type == "nand")
    {
        RS row = nandRS[inst.rsIndex];
        if(inst.rA != 0)
            registers[inst.rA] = row.result;
        if(regMapQueue[inst.rA].size() > 0)
        {
            string regName = regMapQueue[inst.rA].front();
            regMapQueue[inst.rA].pop();
            regMap[inst.rA] = regName;
        } else {
            regMap[inst.rA] = "";
        }

        notifyAll("nand" + to_string(inst.rsIndex + 1), row.result);
        emptyRS(inst.rsIndex, NAND);
    }
    else if(inst.type == "sll")
    {
        RS row = sllRS[inst.rsIndex];
        if(inst.rA != 0)
            registers[inst.rA] = row.result;
        if(regMapQueue[inst.rA].size() > 0)
        {
            string regName = regMapQueue[inst.rA].front();
            regMapQueue[inst.rA].pop();
            regMap[inst.rA] = regName;
        } else {
            regMap[inst.rA] = "";
        }

        notifyAll("sll" + to_string(inst.rsIndex + 1), row.result);
        emptyRS(inst.rsIndex, SLL);
    }
    else if(inst.type == "addi")
    {
        RS row = addAddiRS[inst.rsIndex];
        if(inst.rA != 0)
            registers[inst.rA] = row.result;
        if(regMapQueue[inst.rA].size() > 0)
        {
            string regName = regMapQueue[inst.rA].front();
            regMapQueue[inst.rA].pop();
            regMap[inst.rA] = regName;
        } else {
            regMap[inst.rA] = "";
        }

        notifyAll("addAddi" + to_string(inst.rsIndex + 1), row.result);
        emptyRS(inst.rsIndex, ADDADDI);
    }
    else if(inst.type == "neg")
    {
        RS row = negRS[inst.rsIndex];
        if(inst.rA != 0)
            registers[inst.rA] = row.result;
        if(regMapQueue[inst.rA].size() > 0)
        {
            string regName = regMapQueue[inst.rA].front();
            regMapQueue[inst.rA].pop();
            regMap[inst.rA] = regName;
        } else {
            regMap[inst.rA] = "";
        }

        notifyAll("neg" + to_string(inst.rsIndex + 1), row.result);
        emptyRS(inst.rsIndex, NEG);
    }
}

#endif
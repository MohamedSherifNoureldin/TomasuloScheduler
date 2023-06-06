#pragma once
#ifndef EXECUTE_CPP
#define EXECUTE_CPP
#include "global.h"
#include "deps.cpp"
#include <algorithm>
using namespace std;

bool canExecute(Inst inst, int t)
{
    vector<int>::iterator it = find(waitingForBranch.begin(), waitingForBranch.end(), inst.index);
    if(it != waitingForBranch.end())
    {
        return false;
    }

    if (inst.type == "load")
    {
        // rB not ready
        if (regMap[inst.rB] != "" && regMap[inst.rB] != loadRS[inst.rsIndex].name)
        {
            return false;
        } 
        int effectiveAddress = loadRS[inst.rsIndex].A + loadRS[inst.rsIndex].vj;
        // examine all the store buffers
        for (int i = 0; i < storeRS.size(); i++)
        {
            if (storeRS[i].busy == true && (storeRS[i].A + storeRS[i].vj == effectiveAddress) )
            {
                return false;
            }
        }
    }
    else if (inst.type == "store")
    {
        // rA or rB not ready
        if (regMap[inst.rA] != "" || regMap[inst.rB] != "")
        {
            return false;
        }

        int effectiveAddress = storeRS[inst.rsIndex].A + storeRS[inst.rsIndex].vj;
        // examine all the store buffers
        for (int i = 0; i < storeRS.size(); i++)
        {
            if (storeRS[i].busy == true && (storeRS[i].A + storeRS[i].vj == effectiveAddress) )
            {
                return false;
            }
        }
        // examine all the load buffers
        for (int i = 0; i < loadRS.size(); i++)
        {
            if (loadRS[i].busy == true && (loadRS[i].A + loadRS[i].vj == effectiveAddress) )
            {
                return false;
            }
        }
    }
    else if (inst.type == "bne")
    {
        if (regMap[inst.rA] != "" || regMap[inst.rB] != "")
        {
            return false;
        }
    }
    else if (inst.type == "jal" || inst.type == "ret")
    {
        if(regMap[1] != "" && regMap[1] != jalRetRS[inst.rsIndex].name)
        {
            return false;
        }
    }
    else if (inst.type == "add")
    {
        if ((regMap[inst.rB] != "" && regMap[inst.rB] != addAddiRS[inst.rsIndex].name) || (regMap[inst.rC] != "" && regMap[inst.rC] != addAddiRS[inst.rsIndex].name))
        {
            return false;
        }
    }
    else if (inst.type == "nand") 
    {
        if ((regMap[inst.rB] != "" && regMap[inst.rB] != nandRS[inst.rsIndex].name) || (regMap[inst.rC] != "" && regMap[inst.rC] != nandRS[inst.rsIndex].name))
        {
            return false;
        }

    }
    else if (inst.type == "sll")
    {
        if ((regMap[inst.rB] != "" && regMap[inst.rB] != sllRS[inst.rsIndex].name) || (regMap[inst.rC] != "" && regMap[inst.rC] != sllRS[inst.rsIndex].name))
        {
            return false;
        }
    }
    else if (inst.type == "addi")
    {
        if (regMap[inst.rB] != "" && regMap[inst.rB] != addAddiRS[inst.rsIndex].name)
        {
            return false;
        }
    }
    else if (inst.type == "neg")
    {
        if (regMap[inst.rB] != "" && regMap[inst.rB] != negRS[inst.rsIndex].name)
        {
            return false;
        }
    }
    
    return true;
}

void execute(Inst& inst, int t)
{
    inst.execStartTime = t;
    inst.status = EXECUTING;

    if(inst.type == "load")
    {
        RS row = loadRS[inst.rsIndex];
        row.result = memory[row.A + row.vj];
        loadRS[inst.rsIndex] = row;
        inst.execFinishTime = t + row.execDuration;
    } 
    else if(inst.type == "store") 
    {
        RS row = storeRS[inst.rsIndex];
        memory[row.A + row.vj] = row.vk;
        storeRS[inst.rsIndex] = row;
        inst.execFinishTime = t + row.execDuration;
    }
    else if(inst.type == "bne")
    {
        RS row = bneRS[inst.rsIndex];
        inst.execFinishTime = t + row.execDuration;
        numberOfBranchesEncountered++;
    }
    else if(inst.type == "jal")
    {
        RS row = jalRetRS[inst.rsIndex];
        jalRetRS[inst.rsIndex] = row;
        inst.execFinishTime = t + row.execDuration;
        // jump
    }
    else if(inst.type == "ret")
    {
        RS row = jalRetRS[inst.rsIndex];
        inst.execFinishTime = t + row.execDuration;
        // return
    }
    else if(inst.type == "add")
    {
        RS row = addAddiRS[inst.rsIndex];
        row.result = row.vj + row.vk;
        addAddiRS[inst.rsIndex] = row;
        inst.execFinishTime = t + row.execDuration;
    }
    else if(inst.type == "nand")
    {
        RS row = nandRS[inst.rsIndex];
        row.result = ~(row.vj & row.vk);//computeTwosComplement(row.vj);
        nandRS[inst.rsIndex] = row;
        inst.execFinishTime = t + row.execDuration;
    }
    else if(inst.type == "sll")
    {
        RS row = sllRS[inst.rsIndex];
        row.result = row.vj << row.vk;
        sllRS[inst.rsIndex] = row;
        inst.execFinishTime = t + row.execDuration;
    }
    else if(inst.type == "addi")
    {
        RS row = addAddiRS[inst.rsIndex];
        row.result = row.vj + row.A;
        addAddiRS[inst.rsIndex] = row;
        inst.execFinishTime = t + row.execDuration;
    }
    else if(inst.type == "neg")
    {
        RS row = negRS[inst.rsIndex];
        row.result = -row.vj;
        negRS[inst.rsIndex] = row;
        inst.execFinishTime = t + row.execDuration;
    }
}


void executeAfterBranch(bool branched)
{
    afterBranch = false;
    if(branched)
    {
        // flush all issued instructions 
        for(int i=0; i<waitingForBranch.size(); i++)
        {
            Inst inst = instructions[waitingForBranch[i]];
            instructions[waitingForBranch[i]].status = NOTYET;
            if(inst.type == "load")
            {
                if(regMapQueue[inst.rA].size() > 0)
                {
                    string regName = regMapQueue[inst.rA].front();
                    regMapQueue[inst.rA].pop();
                    regMap[inst.rA] = regName;
                } else {
                    regMap[inst.rA] = "";
                }

                notifyAll("load" + to_string(inst.rsIndex + 1), -1);
                emptyRS(inst.rsIndex, LOAD);
            } 
            else if(inst.type == "store")
            {
                emptyRS(inst.rsIndex, STORE);
            } 
            else if(inst.type == "bne")
            {
                emptyRS(inst.rsIndex, BNE);
            } 
            else if(inst.type == "jal" || inst.type == "ret")
            {
                if(inst.type == "jal")
                {
                    if(regMapQueue[1].size() > 0)
                    {
                        string regName = regMapQueue[1].front();
                        regMapQueue[1].pop();
                        regMap[1] = regName;
                    } else {
                        regMap[1] = "";
                    }
                    notifyAll("jalRet" + to_string(inst.rsIndex + 1), -1);
                }

                emptyRS(inst.rsIndex, JALRET);
            } 
            else if(inst.type == "add" || inst.type == "addi")
            {
                if(regMapQueue[inst.rA].size() > 0)
                {
                    string regName = regMapQueue[inst.rA].front();
                    regMapQueue[inst.rA].pop();
                    regMap[inst.rA] = regName;
                } else {
                    regMap[inst.rA] = "";
                }

                notifyAll("addAddi" + to_string(inst.rsIndex + 1), -1);
                emptyRS(inst.rsIndex, ADDADDI);
            }
            else if(inst.type == "nand")
            {
                if(regMapQueue[inst.rA].size() > 0)
                {
                    string regName = regMapQueue[inst.rA].front();
                    regMapQueue[inst.rA].pop();
                    regMap[inst.rA] = regName;
                } else {
                    regMap[inst.rA] = "";
                }
                notifyAll("nand" + to_string(inst.rsIndex + 1), -1);
                emptyRS(inst.rsIndex, NAND);
            }
            else if(inst.type == "sll")
            {
                if(regMapQueue[inst.rA].size() > 0)
                {
                    string regName = regMapQueue[inst.rA].front();
                    regMapQueue[inst.rA].pop();
                    regMap[inst.rA] = regName;
                } else {
                    regMap[inst.rA] = "";
                }
                notifyAll("sll" + to_string(inst.rsIndex + 1), -1);
                emptyRS(inst.rsIndex, SLL);
            }
            else if(inst.type == "neg")
            {
                if(regMapQueue[inst.rA].size() > 0)
                {
                    string regName = regMapQueue[inst.rA].front();
                    regMapQueue[inst.rA].pop();
                    regMap[inst.rA] = regName;
                } else {
                    regMap[inst.rA] = "";
                }
                notifyAll("neg" + to_string(inst.rsIndex + 1), -1);
                emptyRS(inst.rsIndex, NEG);
            }
        }
    }
    
    waitingForBranch.clear();
}


#endif
#pragma once
#ifndef ISSUE_CPP
#define ISSUE_CPP
#include "global.h"

using namespace std;

bool canIssue(Inst& inst, int t)
{
    if(afterBranch)
    {
        waitingForBranch.push_back(inst.index);
    }

    if (inst.type == "load")
    {
        int r = 0;
        for (r; r < loadRS.size(); r++)
        {
            if (loadRS[r].busy == false)
                break;
        }
        
        if (r < loadRS.size())
        {
            if (regMap[inst.rB] != "")
            {
                loadRS[r].qj = regMap[inst.rB]; 
            }
            else
            {
                loadRS[r].vj = registers[inst.rB];
                loadRS[r].qj = "";
            }
            loadRS[r].A = inst.imm;
            loadRS[r].busy = true;
            if(regMap[inst.rA] == "")
            {
                regMap[inst.rA] = loadRS[r].name;
            }
            else
            {
                regMapQueue[inst.rA].push(loadRS[r].name);
            }

            inst.status = ISSUED;
            inst.issue = t;
            inst.rsIndex = r;

            loadRS[r].instructionIndex = inst.index;

            return true;
        }
        else
        {
            return false;
        }
    }
    else if (inst.type == "store")
    {
        int r = 0;
        for (r; r < storeRS.size(); r++)
        {
            if (storeRS[r].busy == false)
                break;
        }
        
        if (r < storeRS.size())
        {
            if (regMap[inst.rB] != "")
            {
                storeRS[r].qj = regMap[inst.rB]; 
            }
            else
            {
                storeRS[r].vj = registers[inst.rB];
                storeRS[r].qj = "";
            }
            storeRS[r].A = inst.imm;
            storeRS[r].busy = true;

            if (regMap[inst.rA] != "")
            {
                storeRS[r].qk = regMap[inst.rA];
            }
            else
            {
                storeRS[r].vk = registers[inst.rA];
                storeRS[r].qk = "";
            }

            inst.status = ISSUED;
            inst.issue = t;
            inst.rsIndex = r;

            storeRS[r].instructionIndex = inst.index;
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (inst.type == "jal")
    {
        int r = 0;
        for (r; r < jalRetRS.size(); r++)
        {
            if (jalRetRS[r].busy == false)
                break;
        }
        
        if (r < jalRetRS.size())
        {
            jalRetRS[r].A = inst.imm;
            jalRetRS[r].busy = true;
            if(regMap[1] == "")
            {
                regMap[1] = jalRetRS[r].name;
            }
            else
            {
                regMapQueue[1].push(jalRetRS[r].name);
            }
            jalRetRS[r].result = PC + 1;
            inst.status = ISSUED;
            inst.issue = t;
            inst.rsIndex = r;
            afterBranch = true;

            jalRetRS[r].instructionIndex = inst.index;
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (inst.type == "ret")
    {
        int r = 0;
        for (r; r < jalRetRS.size(); r++)
        {
            if (jalRetRS[r].busy == false)
                break;
        }
        
        if (r < jalRetRS.size())
        {
            if (regMap[1] != "")
            {
                jalRetRS[r].qj = regMap[1]; 
            }
            else
            {
                jalRetRS[r].vj = registers[1];
                jalRetRS[r].qj = "";
            }
            jalRetRS[r].busy = true;

            inst.status = ISSUED;
            inst.issue = t;
            inst.rsIndex = r;
            afterBranch = true;

            jalRetRS[r].instructionIndex = inst.index;
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (inst.type == "nand")
    {
        int r = 0;
        for (r; r < nandRS.size(); r++)
        {
            if (nandRS[r].busy == false)
                break;
        }
        
        if (r < nandRS.size())
        {
            if (regMap[inst.rB] != "")
            {
                nandRS[r].qj = regMap[inst.rB]; 
            }
            else
            {
                nandRS[r].vj = registers[inst.rB];
                nandRS[r].qj = "";
            }
            if (regMap[inst.rC] != "")
            {
                nandRS[r].qk = regMap[inst.rC]; 
            }
            else
            {
                nandRS[r].vk = registers[inst.rC];
                nandRS[r].qk = "";
            }
            nandRS[r].busy = true;
            if(regMap[inst.rA] == "")
            {
                regMap[inst.rA] = nandRS[r].name;
            }
            else
            {
                regMapQueue[inst.rA].push(nandRS[r].name);
            }

            inst.status = ISSUED;
            inst.issue = t;
            inst.rsIndex = r;

            nandRS[r].instructionIndex = inst.index;
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (inst.type == "add")
    {
        int r = 0;
        for (r; r < addAddiRS.size(); r++)
        {
            if (addAddiRS[r].busy == false)
                break;
        }
        
        if (r < addAddiRS.size())
        {
            if (regMap[inst.rB] != "")
            {
                addAddiRS[r].qj = regMap[inst.rB]; 
            }
            else
            {
                addAddiRS[r].vj = registers[inst.rB];
                addAddiRS[r].qj = "";
            }
            if (regMap[inst.rC] != "")
            {
                addAddiRS[r].qk = regMap[inst.rC]; 
            }
            else
            {
                addAddiRS[r].vk = registers[inst.rC];
                addAddiRS[r].qk = "";
            }
            addAddiRS[r].busy = true;
            if(regMap[inst.rA] == "")
            {
                regMap[inst.rA] = addAddiRS[r].name;
            }
            else
            {
                regMapQueue[inst.rA].push(addAddiRS[r].name);
            }

            inst.status = ISSUED;
            inst.issue = t;
            inst.rsIndex = r;

            addAddiRS[r].instructionIndex = inst.index;
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (inst.type == "sll")
    {
        int r = 0;
        for (r; r < sllRS.size(); r++)
        {
            if (sllRS[r].busy == false)
                break;
        }
        
        if (r < sllRS.size())
        {
            if (regMap[inst.rB] != "")
            {
                sllRS[r].qj = regMap[inst.rB]; 
            }
            else
            {
                sllRS[r].vj = registers[inst.rB];
                sllRS[r].qj = "";
            }
            if (regMap[inst.rC] != "")
            {
                sllRS[r].qk = regMap[inst.rC]; 
            }
            else
            {
                sllRS[r].vk = registers[inst.rC];
                sllRS[r].qk = "";
            }
            sllRS[r].busy = true;
            if(regMap[inst.rA] == "")
            {
                regMap[inst.rA] = sllRS[r].name;
            }
            else
            {
                regMapQueue[inst.rA].push(sllRS[r].name);
            }

            inst.status = ISSUED;
            inst.issue = t;
            inst.rsIndex = r;

            sllRS[r].instructionIndex = inst.index;
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (inst.type == "neg")
    {
        int r = 0;
        for (r; r < negRS.size(); r++)
        {
            if (negRS[r].busy == false)
                break;
        }
        
        if (r < negRS.size())
        {
            if (regMap[inst.rB] != "")
            {
                negRS[r].qj = regMap[inst.rB]; 
            }
            else
            {
                negRS[r].vj = registers[inst.rB];
                negRS[r].qj = "";
            }
            negRS[r].busy = true;
            if(regMap[inst.rA] == "")
            {
                regMap[inst.rA] = negRS[r].name;
            }
            else
            {
                regMapQueue[inst.rA].push(negRS[r].name);
            }

            inst.status = ISSUED;
            inst.issue = t;
            inst.rsIndex = r;

            negRS[r].instructionIndex = inst.index;
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (inst.type == "addi")
    {
        int r = 0;
        for (r; r < addAddiRS.size(); r++)
        {
            if (addAddiRS[r].busy == false)
                break;
        }
        
        if (r < addAddiRS.size())
        {
            if (regMap[inst.rB] != "")
            {
                addAddiRS[r].qj = regMap[inst.rB]; 
            }
            else
            {
                addAddiRS[r].vj = registers[inst.rB];
                addAddiRS[r].qj = "";
            }
            addAddiRS[r].A = inst.imm;
            addAddiRS[r].busy = true;
            if(regMap[inst.rA] == "")
            {
                regMap[inst.rA] = addAddiRS[r].name;
            }
            else
            {
                regMapQueue[inst.rA].push(addAddiRS[r].name);
            }

            inst.status = ISSUED;
            inst.issue = t;
            inst.rsIndex = r;

            addAddiRS[r].instructionIndex = inst.index;
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (inst.type == "bne")
    {
        int r = 0;
        for (r; r < bneRS.size(); r++)
        {
            if (bneRS[r].busy == false)
                break;
        }
        
        if (r < bneRS.size())
        {
            if (regMap[inst.rA] != "")
            {
                bneRS[r].qj = regMap[inst.rA]; 
            }
            else
            {
                bneRS[r].vj = registers[inst.rA];
                bneRS[r].qj = "";
            }
            if (regMap[inst.rB] != "")
            {
                bneRS[r].qk = regMap[inst.rB]; 
            }
            else
            {
                bneRS[r].vk = registers[inst.rB];
                bneRS[r].qk = "";
            }
            bneRS[r].A = inst.imm;
            bneRS[r].result = PC + 1 + inst.imm;
            bneRS[r].busy = true;

            inst.status = ISSUED;
            inst.issue = t;
            inst.rsIndex = r;
            afterBranch = true;

            bneRS[r].instructionIndex = inst.index;
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

#endif
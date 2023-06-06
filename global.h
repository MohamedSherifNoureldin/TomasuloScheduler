#pragma once
#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
using namespace std;

enum status {NOTYET, ISSUED, EXECUTING, EXECUTED, WRITTEN};
enum rsType {LOAD, STORE, BNE, JALRET, ADDADDI, NEG, NAND, SLL};

struct RS{
    string name;
    bool busy;
    string op;
    int vj;
    int vk;
    string qj;
    string qk;
    int A;
    int instructionIndex;
    int execDuration;
    int result;

    RS() {
        this->name = "";
        this->busy = false;
        this->op = "";
        this->vj = -1;
        this->vk = -1;
        this->qj = "";
        this->qk = "";
        this->A = -1;
        this->instructionIndex = -1;
        this->execDuration = -1;
        this->result = -1;
    }
    RS(string name, int execDuration) {
        this->name = name;
        this->busy = false;
        this->op = "";
        this->vj = -1;
        this->vk = -1;
        this->qj = "";
        this->qk = "";
        this->A = -1;
        this->instructionIndex = -1;
        this->execDuration = execDuration;
        this->result = -1;
    }
};

struct Inst{
    string type;
    enum status status;
    int issue;
    int execFinishTime;
    int execStartTime;
    int execRemainingDuration;
    int write;
    int rsIndex;
    int rA;
    int rB;
    int rC;
    int imm;
    int index;

    Inst() {
        this->type = "";
        this->rA = -1;
        this->rB = -1;
        this->rC = -1;
        this->imm = -1;
        this->status = NOTYET;
        this->issue = -1;
        this->execFinishTime = -1;
        this->execStartTime = -1;
        this->execRemainingDuration = -1;
        this->write = -1;
        this->rsIndex = -1;
    }

    Inst(string type, int rA, int rB, int rC, int imm) {
        this->type = type;
        this->rA = rA;
        this->rB = rB;
        this->rC = rC;
        this->imm = imm;
        this->status = NOTYET;
        this->issue = -1;
        this->execFinishTime = -1;
        this->execStartTime = -1;
        this->execRemainingDuration = -1;
        this->write = -1;
        this->rsIndex = -1;
    }
};

vector<Inst> instructions;

vector<RS> loadRS;
vector<RS> storeRS;
vector<RS> bneRS;
vector<RS> jalRetRS;
vector<RS> addAddiRS;
vector<RS> negRS;
vector<RS> nandRS;
vector<RS> sllRS;

unordered_map<int, string> regMap;
unordered_map<int, queue<string>> regMapQueue;
vector<int> registers;

unordered_map<int, int> memory; //memory is word addressable (made it this way so we dont initialize large amount for no reason)

int PC = 0;

int cycles = 1;

bool afterBranch = false;
vector<int> waitingForBranch;

int numberOfFinishedInstructions = 0;
int numberOfBranchesEncountered = 0;
int numberOfBranchesMispredicted = 0;

#endif // GLOBAL_H
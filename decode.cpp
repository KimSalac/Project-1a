#include <iostream>
#include <stdint.h>
#include <cstdint>
using namespace std;

void decode(uint32_t instruction);

void decode(uint32_t instruction) {
    uint32_t opcode = instruction >> 26;
    uint32_t funct = instruction & 0b111111;
    uint32_t rs = instruction << 6 >> 27;
    uint32_t rt = instruction << 11 >> 27;
    uint32_t rd = (instruction & 0xffff) >> 11;
    uint32_t shamt = instruction << 21 >> 27;
    uint32_t imm = instruction << 16 >> 16;
    if (opcode == 0) {
      switch(funct) {
        case 32:
          cout << "add $" << rd << ", $" << rs << ", $" << rt << endl; 
          break;
        case 33:
          cout << "addu $" << rd << ", $" << rs << ", $" << rt << endl;
          break;
        case 34:
          cout << "sub $" << rd << ", $" << rs << ", $" << rt << endl;
          break;
        case 35:
          cout << "subu $" << rd << ", $" << rs << ", $" << rt << endl;
          break;
        case 36:
          cout << "and $" << rd << ", $" << rs << ", $" << rt << endl;
          break;
        case 37:
          cout << "or $" << rd << ", $" << rs << ", $" << rt << endl;
          break;
        case 39:
          cout << "nor $" << rd << ", $" << rs << ", $" << rt << endl;
          break;
      }
      if (funct == 8) {
        cout << "jump register " << rs << endl; }
    }
    switch (opcode) {
      case 4:
        cout << "beq $" << rs << ", $" << rt << ", " << imm << endl;
        break;
      case 5: 
        cout << "bne $" << rs << ", $" << rt << ", " << imm << endl;
        break;
      case 8:
        cout << "addi $" << rt << ", $" << rs << ", " << imm << endl; 
        break;
      case 9:
        cout << "addiu $" << rt << ", $" << rs << ", " << imm << endl; 
        break;
      case 10:
        cout << "slt $" << rt << ", $" << rs << ", " << imm << endl; 
        break;
      case 11:
        cout << "slti $" << rt << ", $" << rs << ", " << imm << endl; 
        break;
      case 12:
        cout << "andi $" << rt << ", $" << rs << ", " << imm << endl; 
        break;
      case 13:
        cout << "ori $" << rt << ", $" << rs << ", " << imm << endl; 
        break;
      case 14:
        cout << "lui $" << rt << ", " << imm << endl; 
        break;
      case 35:
        cout << "lw $" << rt << ", " << imm << "($" << rs << ")" << endl;
        break;
      case 36:
        cout << "lbu $" << rt << ", " << imm << "($" << rs << ")" << endl;
        break;
      case 37:
        cout << "lhu $" << rt << ", " << imm << "($" << rs << ")" << endl;
        break;
      case 40:
        cout << "sb $" << rt << ", " << imm << "($" << rs << ")" << endl;
        break;
      case 41:
        cout << "sh $" << rt << ", " << imm << "($" << rs << ")" << endl;
        break;
      case 43:
        cout << "sw $" << rt << ", " << imm << "($" << rs << ")" << endl;
        break;
      case 48:
        cout << "ll $" << rt << ", " << imm << "($" << rs << ")" << endl;
        break;
    }
    
}
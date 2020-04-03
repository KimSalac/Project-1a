#ifndef STATE
#define STATE
#include <vector>
#include <cstdint>
#include <iostream>
#include "control.h"
//
// Pipeline registers implementation
//
// TODO:
// IFID Pipeline register, only contains instruction and pc + 1
struct IFID {
    uint32_t instruction;
    uint32_t pc; //pc+1?
};

// TODO:
// IDEX Pipeline register
//needs the read register data and extention of the imm to send to alu
struct IDEX {
    control_t control;
    uint32_t data_reg_1; //reg 1 data 
    uint32_t data_reg_2; // reg 2 data
    uint32_t imm; //32 bit extended imm
    int write_reg; //for reg write
};

// TODO:
// EXMEM Pipeline register
//needs the data of the read regs to access memory
struct EXMEM {
    control_t control;
    uint32_t alu_result; //take the result of ALU to use
    uint32_t mem_read_data; //the data that should be read into memory
    uint32_t mem_write_data; //the data that should be written into memory
    int write_reg; //for reg write
};

// TODO:
// MEMWB Pipwline register
//needs data to write to reg and write reg #
struct MEMWB {
    control_t control;
    uint32_t alu_result; //data to write to reg from alu??
    int write_reg; //for reg write

};

struct state_t {
    uint32_t pc;
    IFID ifid;
    IDEX idex;
    EXMEM exmem;
    MEMWB memwb;
};


#endif

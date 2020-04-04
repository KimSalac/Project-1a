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
    bool ifid_write = 0; //write to ifid if 1; 0 do not
};

// TODO:
// IDEX Pipeline register
//needs the read register data and extention of the imm to send to alu
struct IDEX {
    control_t control;
    uint32_t data_rs; //reg 1 data 
    uint32_t data_rt; // reg 2 data
    int rs; //rs register for hazard
    int rt; //rt register
    int rd; //rd register for hazard
    uint32_t imm; //32 bit extended imm
    int shamt; //for shifts
    uint32_t write_data; // for things like stores 
    uint32_t funct; //for alu execute
    uint32_t op; //for alu execute
    int write_reg; //for reg write
    bool idex_write = 0; //bool writing to idex = 1, not write if 0
    void print(){
        cout<<"control: "<<endl;
        control.print();
        cout<<"data_rs: "<<data_rs<<endl;
        cout<<"data_rt: "<<data_rt<<endl;
        cout<<"rs: "<<rs<<endl;
        cout<<"rt: "<<rt<<endl;
        cout<<"rd: "<<rd<<endl;
        cout<<"imm: "<<imm<<endl;
        cout<<"shamt: "<<shamt<<endl;
        cout<<"write_data: "<<write_data<<endl;
        cout<<"funct: "<<funct<<endl;
        cout<<"op: "<<op<<endl;
        cout<<"write_reg: "<<write_reg<<endl;
    }
};

// TODO:
// EXMEM Pipeline register
//needs the data of the read regs to access memory
struct EXMEM {
    control_t control;
    uint32_t alu_result; //take the result of ALU to use
    uint32_t write_data; //the data that should be written into memory
    int rs; //rs register for hazard
    int rd; //rd register for hazard
    int rt; //rt register
    uint32_t data_rs; //reg 1 data 
    uint32_t data_rt; // reg 2 data
    int write_reg; //for reg write
    bool exmem_write = 0; //bool writing to idex = 1, not write if 0
};

// TODO:
// MEMWB Pipwline register
//needs data to write to reg and write reg #
struct MEMWB {
    control_t control;
    uint32_t alu_result; //data to write to reg from alu??
    int rs; //rs register for hazard
    int rd; //rd register for hazard
    int rt; //rt register
    uint32_t data_rs; //reg 1 data 
    uint32_t data_rt; // reg 2 data
    uint32_t write_data; //for data for write
    bool memwb_write = 0; //bool writing to idex = 1, not write if 0

};

struct state_t {
    //string state; //name of the state currently in
    uint32_t pc;
    IFID ifid;
    IDEX idex;
    EXMEM exmem;
    MEMWB memwb;
    bool pc_write = 1; //1 to write to pc (fetch), 0 do not fetch
    
};        
        
#endif

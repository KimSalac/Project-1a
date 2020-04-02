#include <cstdint>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "memory.h"
#include "reg_file.h"
#include "ALU.h"
#include "control.h"

using namespace std;

void processor_main_loop(Registers &reg_file, Memory &memory, uint32_t end_pc) {

    // Initialize ALU
    ALU alu;
    //Initialize Control
    control_t control = {.reg_dest = 0, 
                        .jump = 0,
                        .branch = 0,
                        .mem_read = 0,
                        .mem_to_reg = 0,
                        .ALU_op = 0,
                        .mem_write = 0,
                        .ALU_src = 0,
                        .reg_write = 0,
                        .store_reg = 0,
                        .sign_zero = 0};
    
    uint32_t num_cycles = 0;
    uint32_t num_instrs = 0; 

    while (reg_file.pc != end_pc) {

        // fetch
        uint32_t instruction;
        memory.access(reg_file.pc, instruction, 0, 1, 0);
        cout << "\nPC: 0x" << std::hex << reg_file.pc << std::dec << "\n";
        // increment pc
        reg_file.pc += 4;
        
        // TODO: fill in the function argument
        // decode into contol signals
        control.decode(instruction);
        cout<< "Intstructuon: "<< instruction<<endl;
        control.print(); // used for autograding
        uint32_t data_write; //varable for data to be written 
        
        // TODO: fill in the function argument
        // Read from reg file
        
        //get rs
        uint32_t rs_b = instruction << 6; //get rid of opcode
        //cout<<"RS_B: "<<rs_b<<endl;
        rs_b = rs_b >> 27; //get rs
        //cout<<"RS_B: "<<rs_b<<endl;
          //rs_b = rs_b >> 6;
          //cout<<"RS_B: "<<rs_b<<endl;
        int32_t rs_num = (int32_t) rs_b; //convert rs to int
        cout<<"RS_num: "<<rs_num<<endl;
        
        //get rt
        uint32_t rt_b = instruction << 11; //get rid of opcode and rs
          //cout<<"Rt_B: "<<rt_b<<endl;
        rt_b = rt_b >>27; //get rt
          //cout<<"Rt_B: "<<rt_b<<endl;
          //  rt_b = rt_b >>16;
        int rt_num = (int32_t) rt_b; //convert rt to int
        cout<<"Rt_num: "<<rt_num<<endl;
        uint32_t data_rs; //varaible for data of rs
        uint32_t data_rt; //varaible for data of rt
        uint32_t data_i; //variable for data of imm
       
        //get opcode
        uint32_t op = instruction >> 26;
        cout<<"op: "<<op<<endl;
        uint32_t funct = instruction << 26;
        funct = funct >>26;
        cout<<"funct: "<<funct<<endl;
        if (op == 0){ //if r-type -- NOT COVER SHIFT LEFT OR RIGHT LOGICAL, OR JUMP REG
            uint32_t rd_b = instruction <<16; //get rid of op, rs, rt
            rd_b = rd_b >>27; //get rid of shamt, funct
            //cout<<"Rd_b: "<<rd_b<<endl;
            int rd_num = (int32_t) rd_b; //convert rd to int
            cout<<"Rd_num: "<<rd_num<<endl;
            reg_file.access(rs_num, rt_num, data_rs, data_rt, rd_num, 0, data_write);
            cout<<"rs_data: "<<data_rs<<endl;
            cout<<"rt_data: "<<data_rt<<endl;
        }
        else{ //if I type - NOT COVER BEQ, LUI, STORES
          data_i = instruction <<16; //THIS IS NOT COMPETELY CORRECT
          data_i = data_i >>16;
          cout<<"data_i: "<<data_i<<endl;
          reg_file.access(rs_num, 0, data_rs, data_rt, rt_num, 0, data_write);
          cout<<"rs_data: "<<data_rs<<endl;
          //cout<<"rt_data: "<<data_rt<<endl;
        }
        
        // TODO: fill in the function argument
        // Execution 
        alu.generate_control_inputs( control.ALU_op, funct, op);
        //cout<<"alu control: "<<alu.ALU_control_inputs<<endl;
        alu.print();
        
        
        // TODO: fill in the function argument
        uint32_t alu_zero;
        uint32_t alu_result;
        if (op == 0){ //if rtype
          alu_result = alu.execute(data_rs, data_rt, alu_zero);
        }
        else{//if itype
          alu_result = alu.execute(data_rs, data_i, alu_zero);
        }
        int32_t r = (int32_t) alu_result;
        cout<<"alu result: "<<r<<endl;
        
        // Memory
        // TODO: fill in the function argument
        //memory.access(  );
        
        // Write Back
        // TODO: fill in the function argument
        //reg_file.access(  );
        
        
        // TODO: Update PC


        //cout << "CYCLE" << num_cycles << "\n";
        //reg_file.print(); // used for automated testing

        num_cycles++;
        num_instrs++; 
        cout<<"# in: "<< num_instrs<<endl;

    }

    //cout << "CPI = " << (double)num_cycles/(double)num_instrs << "\n";
    
    /*Temporary Tests for control signals*/
    /* uint32_t instruct;
    
    ifstream inFile;
    inFile.open("controlhex.txt");

    if(!inFile)
      {
	cout << "Unable to open file";
	exit(1);
      }
    string temp;
    while (std::getline(inFile, temp))
      {
	std::stringstream ss(temp);
	ss >> hex >> instruct;
	cout << "----Instruction: " << temp << "----" << endl;
	control.decode(instruct);
	control.print();
    }
    inFile.close(); */
}

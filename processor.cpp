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
                        .reg_write = 0};
    
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

        
        // TODO: fill in the function argument
        // Read from reg file
        uint32_t rs = instruction >> 21;
        rs = rs << 6;
        
         uint32_t rt = instruction >> 16;
         rt = rt <<11;
         //reg_file.
        //reg_file.access();
        
        
        // TODO: fill in the function argument
        // Execution 
        //alu.generate_control_inputs(  );
        
        
        // TODO: fill in the function argument
        //uint32_t alu_result = alu.execute(  );
        
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

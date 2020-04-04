#include <cstdint>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "memory.h"
#include "reg_file.h"
#include "ALU.h"
#include "control.h"
#include <bitset>

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
                        .load_reg = 0,
                        .sign_zero = 0,
                        .beq = 0};
    
    uint32_t num_cycles = 0;
    uint32_t num_instrs = 0; 
    
    while (reg_file.pc != end_pc) {

        // fetch: good
        uint32_t instruction;
        memory.access(reg_file.pc, instruction, 0, 1, 0); 
        cout << "\nPC: 0x" << std::hex << reg_file.pc << std::dec << "\n";
        // increment pc
        reg_file.pc += 4;
        
        // TODO: fill in the function argument
        // decode into contol signals
        control.decode(instruction);
        //cout<< "Instruction: " << instruction <<endl;
        control.print(); // used for autograding 
        
        // TODO: fill in the function argument
        // Read from reg file
        
        /****get rs: good****/
        uint32_t rs_b = instruction << 6; //get rid of opcode
        rs_b = rs_b >> 27; //get rs
        int rs_num = (int32_t) rs_b; //convert rs to int
        //cout<< "RS_num: " << rs_num <<endl; // prints rs value
        
        /****get rt: good****/
        uint32_t rt_b = instruction << 11; //get rid of opcode and rs
        rt_b = rt_b >>27; //get rt
        int rt_num = (int32_t) rt_b; //convert rt to int
        //cout<<"Rt_num: "<<rt_num<<endl; // prints rt value

        uint32_t data_rs = 0; //variable for data of rs
        uint32_t data_rt = 0; //varaible for data of rt
        uint32_t data_i = 0; //variable for data of imm
        uint32_t data_write = 0; //varable for data to be written
        int rd_num = 0; //var for rd reg #
        uint32_t shamt = 0; //var for shamt

        /****get opcode & funct: good ****/
        uint32_t op = instruction >> 26; // gets op
        //cout << "op: "<< op <<endl; //prints opcode
        uint32_t funct = instruction << 26; // gets funct (see next too)
        funct = funct >>26;
        //cout<<"funct: "<<funct<<endl; //prints funct

        if (op == 0){ //if r-type
            uint32_t rd_b = instruction << 16; //get rid of op, rs, rt
            rd_b = rd_b >> 27; // isolate rd
            rd_num = (int32_t) rd_b; //convert rd to int
            //cout<< "Rd_num: " << rd_num <<endl; //prints out rd

            reg_file.access(rs_num, rt_num, data_rs, data_rt, rd_num, 0, data_write); // gets rs, rt values
            //cout<<"rs_data: "<< data_rs <<endl;
            //cout<<"rt_data: "<< data_rt <<endl;

            if(funct == 2 || funct == 0) // checks to see if it's either shifts
            {
              shamt =  instruction << 21;
              shamt = shamt >> 27; //isolate shamt
              //cout<< "shamt: " << shamt <<endl; //prints out shamt
            }
          
        }
        else
        { //if I type
          int16_t i = instruction & 0xFFFF;
          //i = instruction >> 8;
           //cout<<"i: "<<i<<endl;
          //data_i = instruction << 16; //gets immediate vales
          data_i = (int32_t) i;
          //cout<<"data_i: "<<data_i<<endl;

          if(control.sign_zero == 1) //logic operations
          {
            data_i = instruction << 16; //gets immediate vales - zero extended
            data_i = data_i >> 16;
            reg_file.access(rs_num, 0, data_rs, data_rt, rt_num, 0, data_write);
            data_i = data_i & 0x0000ffff; // zero extends first 16 bits
          }
          else if(op == 0b001111) // lui
          {
            data_i = data_i << 16;
            data_i = data_i & 0xffff0000;
            std::bitset<32>  x(data_i);
            //cout << "data_i: " << x << endl;
          }
          else if(op == 0b101000 || op == 0b101001 || op == 0b101011) // sb, sh, sw
          {
            reg_file.access(rs_num, rt_num, data_rs, data_rt, rt_num, 0, data_write); //acess reg_file to get the data of rt
            if(control.store_reg == 0b01) // sb
            {
              data_rt = data_rt & 0x000000ff; // only takes lower 8 bits
              //cout << "data_rt from sb" << data_rt << endl;
            }
            else if(control.store_reg == 0b00) // sh
            {
              data_rt = data_rt & 0x0000ffff; // only takes lower 16 bits
              //cout << "data_rt from sb" << data_rt << endl;
            }  
            //cout << "rt_data for stores: " << data_rt << endl;       
          }
          else // rest of regular arithmetic and loads
          {
            reg_file.access(rs_num, rt_num, data_rs, data_rt, rt_num, 0, data_write);
          }
          
          //cout<< "data_i: " << data_i <<endl; //prints immediate value 
         // cout<< "rs_data: "<< data_rs <<endl;
         // cout<<"rt_data: "<< (int32_t) data_rt <<endl;
        }
      
        // TODO: fill in the function argument
        // Execution 
        alu.generate_control_inputs(control.ALU_op, funct, op);
        //alu.print();

        uint32_t alu_zero = 0;
        uint32_t alu_result = 0;

        if (control.ALU_src == 0 && !(funct == 8)) //if rtype except jumpReg
        { 
          if(funct == 2 || funct == 0) // if shift
          {
            alu_result = alu.execute(data_rt, shamt, alu_zero);
          }
          else // if regular r-type
          {
            alu_result = alu.execute(data_rs, data_rt, alu_zero);
          }
        }
        else //if itype
        {
          if(!control.branch)
          {
            alu_result = alu.execute(data_rs, data_i, alu_zero);
          }
          else
          {
            //cout << "got here" << endl;
            alu_result = alu.execute(data_rs, data_rt, alu_zero);
          }
          
        }

        //int32_t r = (int32_t) alu_result;
        //cout<< "alu result: " << r << endl;
        data_write = alu_result;

        
        // Memory
        // TODO: fill in the function argument
        if(control.mem_read == 1 || control.mem_write == 1)
        {
          if(control.store_reg < 2) //stores
          {
            if(control.store_reg == 1) // sb
            {
              //cout << "was sb" << endl;
              memory.access(alu_result, data_write, data_rt, 1, 0); // take value from memory
              //cout << "Value from memory: " << (int32_t) data_write << endl;
              data_write = data_write & 0xffffff00; // get rid of rightmost 8 bits
              //cout << "Value zeroed: " << (int32_t) data_write << endl;
              data_write = data_write | data_rt; // replace rightmost 8 bits with rt
              //cout << "Value modified: " << (int32_t) data_write << endl;

              memory.access(alu_result, data_rt, data_write, control.mem_read, control.mem_write); // write modified value to memory
            }
            if(control.store_reg == 0) // sh
            {
              //cout << "was sh" << endl;
              memory.access(alu_result, data_write, data_rt, 1, 0); // take value from memory
              //cout << "Value from memory: " << (int32_t) data_write << endl;
              data_write = data_write & 0xffff0000; // get rid of rightmost 16 bits
              //cout << "Value from memory: " << (int32_t) data_write << endl;
              data_write = data_write | data_rt; // replace rightmost 16 bits with rt
              //cout << "Value from memory: " << (int32_t) data_write << endl;
              memory.access(alu_result, data_rt, data_write, control.mem_read, control.mem_write); // write modified value to memory
            }
          }
          else if(control.mem_to_reg == 1) //loads
          {
            //cout << "got here" << endl;
            //cout << "passed into lw" << endl;
            memory.access(alu_result, data_write, data_rt, control.mem_read, control.mem_write); // regular load word
            if(control.load_reg == 0b10) // lbu
            {
             // cout << "was lbu" << endl;
              data_write = data_write & 0x000000ff;
            }
            if(control.load_reg == 0b11) // lhu
            {
              //cout << "was lhu" << endl;
              data_write = data_write & 0x0000ffff;
            }
          }
          else // regular store word
          {
            //cout << "was sw" << endl;
            memory.access(alu_result, data_write, data_rt, control.mem_read, control.mem_write);
          }
          //memory.print(alu_result, 10);
        }

        
        // Write Back
        // TODO: fill in the function argument
        if (control.reg_write == 1)
        {
          if(control.reg_dest == 0) // write to rt
          {
            if(op == 0b001111) //lui
            {
              reg_file.access(0, 0, data_rs, data_rs, rt_num, 1, data_i);
            }
            else // all other operations
            {
              reg_file.access(0, 0, data_rs, data_rs, rt_num, 1, data_write);
            }
            //cout<< "wrote to rt" <<endl;
          }
          else if(control.reg_dest == 1) //write to rd
          { 
            //cout<< "wrote to rd" <<endl;
            reg_file.access(rs_num, rt_num, data_rs, data_rt, rd_num, 1, data_write);
          }
        }
        
        
        // TODO: Update PC
        if(control.branch == 1) // update proper branch address
        {
          if(control.beq == alu_zero) // true if beq = 0 & alu_zero = 0 OR beq = 1 & alu_zero = 1
          {
            reg_file.pc = reg_file.pc + (data_i << 2);
          }
        }
        else if(op == 0 && funct == 8) // checks to see if it's jumpReg
        {
          reg_file.pc = data_rs; // PC=R[rs]
        }

        cout << "CYCLE" << num_cycles << "\n";
        reg_file.print(); // used for automated testing

        num_cycles++;
        num_instrs++; 
        //cout<<"# in: "<< num_instrs<<endl;

    }

    cout << "CPI = " << (double)num_cycles/(double)num_instrs << "\n";
    
    /*Temporary Tests for control signals
    
    uint32_t instruction;
    uint32_t op;
    uint32_t funct;
    uint32_t ALU_zero = 0;
    uint32_t result;
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
	      ss >> hex >> instruction;
	      cout << "----Instruction: " << temp << "----" << endl;
	      control.decode(instruction); //decodes instruction
        //control.print(); //prints control signals


        op = instruction >> 26;
        cout<<"op: "<< op <<endl;
        funct = instruction << 26;
        funct = funct >> 26;
        cout<<"funct: "<< funct <<endl;
        alu.generate_control_inputs(control.ALU_op, funct, op);
        result = alu.execute(1, 1, ALU_zero);

        alu.print();
        cout << "ALU_zero: " << ALU_zero << endl;
        cout << "Result of ALU operation: " << result << endl;
        cout  << "---------------------------" << endl;
        ALU_zero = 0;
    }
    inFile.close();*/
}

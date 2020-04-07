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
#include "state.h"
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
        //cout << "\nPC2: 0x" << std::hex << reg_file.pc << std::dec << "\n";
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
            reg_file.access(rs_num, rt_num, data_rs, data_rt, 0, 0, data_write); //acess reg_file to get the data of rt
            if(control.store_reg == 0b01) // sb
            {
              data_rt = data_rt & 0x000000ff; // only takes lower 8 bits
            }
            else if(control.store_reg == 0b00) // sh
            {
              data_rt = data_rt & 0x0000ffff; // only takes lower 16 bits
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
            /*if(control.mem_write == 1 || control.mem_read == 1)
            {
              data_i = data_i << 2;
            }*/
            alu_result = alu.execute(data_rs, data_i, alu_zero);
            //cout << "data_rs: " << data_rs << endl;
            //cout << "data_i: " << data_i << endl;
            //cout << "alu_result: " << alu_result << endl;
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
          if(control.mem_write == 1) //stores
          {
            if(control.store_reg == 1) // sb
            {
              memory.access(alu_result, data_write, data_rt, 1, 0); // take value from memory
              //cout << "value at memory before change: " << data_write << endl;
              data_write = data_write & 0xffffff00; // get rid of rightmost 8 bits
              //cout << "data_write after zeroing 8 bits: " << data_write << endl;
              data_write = data_write | data_rt; // replace rightmost 8 bits with rt
              //cout << "new value to store in memory: " << data_write << endl;
              memory.access(alu_result, data_write, data_write, control.mem_read, control.mem_write); // write modified value to memory
            }
            else if(control.store_reg == 0) // sh
            {
              memory.access(alu_result, data_write, data_rt, 1, 0); // take value from memory
              //cout << "value at memory before change: " << data_write << endl;
              data_write = data_write & 0xffff0000; // get rid of rightmost 16 bits
              //cout << "data_write after zeroing 8 bits: " << data_write << endl;
              data_write = data_write | data_rt; // replace rightmost 16 bits with rt
              //cout << "new value to store in memory: " << data_write << endl;
              memory.access(alu_result, data_write, data_write, control.mem_read, control.mem_write); // write modified value to memory
            }
            else
            {
              //cout << "data_rt: " << data_rt << endl;
              memory.access(alu_result, data_write, data_rt, control.mem_read, control.mem_write);
            } 
            //memory.print(alu_result/4, 1);
          }
          else //loads
          {
            //cout << "got here" << endl;
            memory.access(alu_result, data_write, data_rt, control.mem_read, control.mem_write); // regular load word
            if(control.load_reg == 0b10) // lbu
            {
              data_write = data_write & 0x000000ff;
            }
            if(control.load_reg == 0b11) // lhu
            {
              data_write = data_write & 0x0000ffff;
            }
          }
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
          //cout<<"jump reg"<<endl;
          reg_file.pc = data_rs; // PC=R[rs]
        }

        cout << "CYCLE" << num_cycles << "\n";
        reg_file.print(); // used for automated testing

        num_cycles++;
        num_instrs++; 
        //cout << "\nPC-END: 0x" << std::hex << reg_file.pc << std::dec << "\n";
        //cout<<"# in: "<< num_instrs<<endl;

    }

    cout << "CPI = " << (double)num_cycles/(double)num_instrs << "\n";
    

};

void processor_main_loop_pipeline(Registers &reg_file, Memory &memory, uint32_t end_pc) { //NEED TO FIX HOW THIS WORKS!

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
    state_t current_state; 
    state_t next_state; 
    int n = 0;
    while (current_state.memwb.complete == 0 ) {
      //state_t next_state; 
      //cout<<"\n------n: "<<n<<endl;
      //cout<<"current state pc B: "<<current_state.pc<<endl;
      //cout<<"current state ifid in: "<<current_state.ifid.instruction<<endl;
     // cout<<"current state pc_write in: "<<current_state.pc_write<<endl;
     if (current_state.pc  != end_pc){ //if the last insturction has been fetched  
          cout<<"fetch set"<<endl;
          current_state.pc_write = 1;
        }
      if (current_state.pc_write == 1){//(current_state.ifid.instruction == 0){ //check for fetch stage? (or use pc write)
        cout<<"--fetch"<<endl;
        //fetch
        //cout<< "Instruction1: " << current_state.ifid.instruction <<endl;
        uint32_t in = current_state.ifid.instruction;
        memory.access(current_state.pc, in, 0, 1, 0);  //from the current state's pc, grab the insturction
        // increment pc
        cout << "\nPC: 0x" << std::hex << current_state.pc << std::dec << "\n";
        cout<< "Instruction: " << in <<endl;
        next_state.ifid.instruction = in; //set next stages's ifid instruction value (since we will be going to that stage next)
        //cout<< "Instruction2: " << next_state.ifid.instruction <<endl;
        next_state.pc += 4; //set up for next instruction fetch
        if (next_state.pc  == end_pc){ //if the last insturction has been fetched  
          cout<<"--------------Next stage = pc"<<endl;
          next_state.pc_write == 0;
        }
        num_instrs++; 
        //cout<<"# in: "<< num_instrs<<endl;
       // cout<<"current state ifid write: "<<current_state.ifid.ifid_write<<endl;
       //cout<<"change ifid write"<<endl;
        next_state.ifid.ifid_write = 1; //next cycle do next stage (id)

      }
      else{
        next_state.ifid.ifid_write = 0; //else dont decode the next stage
      }

      //id stage
      //cout<<"next state ifid write: "<<next_state.ifid.ifid_write<<endl;
      //cout<<"current state ifid write: "<<current_state.ifid.ifid_write<<endl;
      if(current_state.ifid.ifid_write == 1){ //if this is id stage time
      cout<<"---decode"<<endl;
      //cout<<"--n: "<<n<<endl;
      cout<< "Instruction: " << current_state.ifid.instruction <<endl;
        uint32_t instruction = current_state.ifid.instruction;
        control.decode(instruction);
        control.print(); // used for autograding 
        
        next_state.idex.control = control; //set the next state's control inputs 
       
        /****get rs: good****/
        uint32_t rs_b = instruction << 6; //get rid of opcode
        rs_b = rs_b >> 27; //get rs
        int32_t rs_num = (int32_t) rs_b; //convert rs to int
        
        next_state.idex.rs = rs_num; //set the rs reg # in the next state
        
        /****get rt: good****/
        uint32_t rt_b = instruction << 11; //get rid of opcode and rs
        rt_b = rt_b >>27; //get rt
        int rt_num = (int32_t) rt_b; //convert rt to int
        
        next_state.idex.rt = rt_num; //set the rs reg # in the next state

         uint32_t data_rs = 0; //variable for data of rs
        uint32_t data_rt = 0; //varaible for data of rt
        uint32_t data_i = 0; //variable for data of imm
        uint32_t data_write = 0; //varable for data to be written
        int rd_num = 0; //var for rd reg #
        uint32_t shamt = 0; //var for shamt

        /****get opcode & funct: good ****/
        uint32_t op = instruction >> 26; // gets op
        //cout << "op: "<< op <<endl; //prints opcode
        
        next_state.idex.op = op;
        
        uint32_t funct = instruction << 26; // gets funct (see next too)
        funct = funct >>26;
        
        next_state.idex.funct = funct;
        
        //cout<<"funct: "<<funct<<endl; //prints funct

        if (op == 0){ //if r-type
            uint32_t rd_b = instruction << 16; //get rid of op, rs, rt
            rd_b = rd_b >> 27; // isolate rd
            rd_num = (int32_t) rd_b; //convert rd to int
            reg_file.access(rs_num, rt_num, data_rs, data_rt, rd_num, 0, data_write); // gets rs, rt values
            if(funct == 2 || funct == 0) // checks to see if it's either shifts
            {
              shamt = instruction << 21;
              shamt = shamt >> 27; //isolate shamt
              
              next_state.idex.shamt = shamt; //set the shamt varaible 
            }
          
        }
        else{ //if I type 

          int16_t i = instruction & 0xFFFF;
          data_i = (int32_t) i;
          
          next_state.idex.imm = data_i; //put in the imm for next state

         if(control.sign_zero == 1) //logic operations
          {
            data_i = instruction << 16; //gets immediate vales - zero extended
            data_i = data_i >> 16;
            reg_file.access(rs_num, 0, data_rs, data_rt, rt_num, 0, data_write);
            data_i = data_i & 0x0000ffff; // zero extends first 16 bits

            next_state.idex.imm = data_i; //put in the imm for next state
          }
          else if(op == 0b001111) // lui
          {
            data_i = data_i << 16;
            data_i = data_i & 0xffff0000;
             std::bitset<32>  x(data_i);
             
             next_state.idex.imm = data_i; //put in the imm for next state
          }
          else if(op == 0b101000 || op == 0b101001 || op == 0b101011) // sb, sh, sw
          {
            reg_file.access(rs_num, rt_num, data_rs, data_rt, 0, 0, data_write); //acess reg_file to get the data of rt
            
            next_state.idex.write_data = data_rt; //put in value for next state
            
            if(control.store_reg == 0b01) // sb
            {
              data_rt = data_rt & 0x000000ff; // only takes lower 8 bits
              
              next_state.idex.write_data = data_rt; //put in value for next state
            }
            else if(control.store_reg == 0b00) // sh
            {
              data_rt = data_rt & 0x0000ffff; // only takes lower 16 bits
              
              next_state.idex.write_data = data_rt; //put in value for next state
            }  
           // cout << "rt_data for stores: " << data_rt << endl;       
          }
          else // rest of regular arithmetic and and loads
          {
            reg_file.access(rs_num, 0, data_rs, data_rt, rt_num, 0, data_write);
            
          }
         
        
       }
       cout<<"data_rs!!: "<<data_rs<<endl;
       next_state.idex.data_rs = data_rs;
       next_state.idex.data_rt = data_rt;
          next_state.idex.idex_write = 1; //go to next stage
          next_state.idex.print();
      }
       else{
        next_state.idex.idex_write = 0; //else dont do this stage again stage
      }


      //execution
      if(current_state.idex.idex_write == 1){
        cout<<"--execution"<<endl;
        uint32_t funct = current_state.idex.funct;
        uint32_t op = current_state.idex.op;
        uint32_t data_rs = current_state.idex.data_rs;
        uint32_t data_rt = current_state.idex.data_rt;
        uint32_t data_i = current_state.idex.imm;
        uint32_t shamt = current_state.idex.shamt;
        uint32_t alu_op = current_state.idex.control.ALU_op;
        alu.generate_control_inputs(alu_op, funct, op);
        uint32_t alu_zero = 0;
        uint32_t alu_result = 0;
        bool alu_source = current_state.idex.control.ALU_src;

        if (alu_source == 0 && !(funct == 8)) //if rtype except jumpreg
        { 
          if(funct == 2 || funct == 0) // if shift
          {
            alu_result = alu.execute(data_rs, shamt, alu_zero);
          }
          else // if regular r-type
          {
            alu_result = alu.execute(data_rs, data_rt, alu_zero);
          }
        }
        else //if itype
        {
          //cout<<"itype"<<endl;
         if(!current_state.idex.control.branch)
          {
            /*if(control.mem_write == 1 || control.mem_read == 1)
            {
              data_i = data_i << 2;
            }*/
            cout<<"data_rs: "<<data_rs<<endl;
            cout<<"data_i: "<<data_i<<endl;
            alu_result = alu.execute(data_rs, data_i, alu_zero);
            //cout<<"alu result: "<<alu_result<<endl;
            //cout << "data_rs: " << data_rs << endl;
            //cout << "data_i: " << data_i << endl;
            //cout << "alu_result: " << alu_result << endl;
          }
          else
          {
            //cout << "got here" << endl;
            alu_result = alu.execute(data_rs, data_rt, alu_zero);
            
          }
          
        }

        //data_write = alu_result;
        //cout<<"data_write exe == "<<current_state.idex.write_data<<endl;
        cout<<"alu_result: "<<alu_result<<endl;
        next_state.exmem.control = current_state.idex.control; //copy controls
        next_state.exmem.alu_result = alu_result; //put the data that needs to be written in alu_result
        next_state.exmem.write_data = alu_result; //put the data that needs to be written
        next_state.exmem.rt = current_state.idex.rt; //copy registers
        next_state.exmem.rs = current_state.idex.rs; //copy registers
        next_state.exmem.rd = current_state.idex.rd; //copy registers
        next_state.exmem.op = current_state.idex.op; //copy opcode 
        next_state.exmem.imm =  current_state.idex.imm; //copy imm
        next_state.exmem.funct = current_state.idex.funct; //copy funct
        next_state.exmem.data_rs = current_state.idex.data_rs; //copy data of registers for reg access
        next_state.exmem.data_rt = current_state.idex.data_rt; //copy data of registers for reg access
        next_state.exmem.exmem_write = 1; //do next stage next cycle
      }
       else{
        next_state.exmem.exmem_write = 0; //else dont do the this stage again
      }



      //memory
      if(current_state.exmem.exmem_write == 1){
        cout<<"--memory"<<endl;
        uint32_t alu_result = current_state.exmem.alu_result;
        uint32_t data_rs = current_state.exmem.data_rs;
        uint32_t data_rt = current_state.exmem.data_rt;
        uint32_t data_write = current_state.exmem.write_data;
        cout<<"write data = "<<current_state.exmem.write_data<<endl;
        next_state.memwb.write_data = data_write;
        next_state.memwb.data_rs = data_rs; //put the value for rs into the reg
         if(current_state.exmem.control.mem_read == 1 || current_state.exmem.control.mem_write == 1)
        {
          cout<<"mem staging"<<endl;
          if(current_state.exmem.control.mem_write == 1) //stores
          {
            if(current_state.exmem.control.store_reg == 1) // sb
            {
              memory.access(alu_result, data_write, data_rt, 1, 0); // take value from memory
              data_write = data_write & 0xffffff00; // get rid of rightmost 8 bits
              data_write = data_write | data_rt; // replace rightmost 8 bits with rt
              memory.access(alu_result, data_write, data_write, current_state.exmem.control.mem_read, current_state.exmem.control.mem_write); // write modified value to memory
            }
            else if(current_state.exmem.control.store_reg == 0) // sh
            { 
              memory.access(alu_result, data_write, data_rt, 1, 0); // take value from memory
              data_write = data_write & 0xffff0000; // get rid of rightmost 16 bits
              data_write = data_write | data_rt; // replace rightmost 16 bits with rt
              memory.access(alu_result, data_write, data_write, current_state.exmem.control.mem_read, current_state.exmem.control.mem_write); // write modified value to memory
            }
            //next_state.memwb.data_rs = data_rs; //put the value for rs into the reg
            else {
              //cout << "data_rt: " << data_rt << endl;
              memory.access(alu_result, data_write, data_rt, current_state.exmem.control.mem_read, current_state.exmem.control.mem_write); 
            } 
            next_state.memwb.write_data = data_write; //put the value for rs into the reg
          }
          else // if(current_state.exmem.control.mem_to_reg > 0) //loads
          {
            cout<<"alu_result: "<<alu_result<<endl;
            cout<<"data_write: "<<data_write<<endl;
            cout<<"data_rt: "<<data_rt<<endl;
            memory.access(alu_result, data_write, data_rt, current_state.exmem.control.mem_read, current_state.exmem.control.mem_write); // regular load word
            //??????? flip data_rt and data_write
             cout<<"write data2 = "<<data_write<<endl;
            next_state.memwb.write_data = data_write;
            if(current_state.exmem.control.load_reg == 0b10) // lbu
            {
              data_write = data_write & 0x000000ff;
              next_state.memwb.write_data = data_write; //put the value for rs into the reg
            }
            if(current_state.exmem.control.mem_to_reg == 0b11) // lhu
            {
              data_write = data_write & 0x0000ffff;
              next_state.memwb.write_data = data_write; //put the value for rs into the reg
            }
          }
        }
        //cout<<"data_write == "<<current_state.exmem.write_data<<endl;
        //cout<<"set mem controls"<<endl;
        next_state.memwb.control = current_state.exmem.control; //copy controls
        next_state.memwb.alu_result = current_state.exmem.alu_result; //put the data that needs to be written in alu_result
        next_state.memwb.data_rs = current_state.exmem.data_rs; //put the data that needs to be written
        next_state.memwb.rt = current_state.exmem.rt; //copy registers
        next_state.memwb.rs = current_state.exmem.rs; //copy registers
        next_state.memwb.rd = current_state.exmem.rd; //copy registers
        next_state.memwb.op = current_state.exmem.op; //copy op code
        next_state.memwb.funct = current_state.exmem.funct; //copy funct
        next_state.memwb.imm =  current_state.exmem.imm; //copy imm
        next_state.memwb.data_rt = current_state.exmem.data_rt; //copy data of registers for reg access
        next_state.memwb.memwb_write = 1; //do next stage next cycle
      }
      else{
        next_state.memwb.memwb_write = 0; //else dont do the this stage again
      }

      //write back
       if(current_state.memwb.memwb_write == 1){ //if we should write to this stage
       cout<<"--write back"<<endl;
        //cout<<"data_write == "<<current_state.exmem.write_data<<endl;
        uint32_t data_rs = current_state.memwb.data_rs;
        uint32_t data_rt = current_state.memwb.data_rt;
        uint32_t rt_num = current_state.memwb.rt;
        uint32_t rs_num = current_state.memwb.rs;
        uint32_t rd_num = current_state.memwb.rd;
        uint32_t op = current_state.memwb.op;
        uint32_t funct = current_state.memwb.funct;
        uint32_t data_write = current_state.memwb.write_data;
        uint32_t data_i = current_state.memwb.imm;
        if (current_state.memwb.control.reg_write == 1) {// might need to fix
          if(current_state.memwb.control.reg_dest == 0){ //write to rt
           if(current_state.memwb.op  == 0b001111) { //lui
              reg_file.access(0, 0, data_rs, data_rs, rt_num, 1, data_i);
            }
            else // all other operations
            {
              reg_file.access(0, 0, data_rs, data_rs, rt_num, 1, data_write);
            }
            //cout<< "wrote to rt" <<endl;
            //cout<<"data_write == "<<data_write<<endl;
            
            //current_state.memwb.print();
          
          }
          else if(control.reg_dest == 1) //write to rd
          { 
            //cout<< "wrote to rd" <<endl;
            reg_file.access(rs_num, rt_num, data_rs, data_rt, rd_num, 1, data_write);
          }
        }

       
        
          
        
         //update pc
        if(current_state.memwb.control.branch == 1) // update proper branch address
        {
          if(current_state.memwb.control.beq == (uint32_t) 0) // MIGHT BE WRONG! // true if beq = 0 & alu_zero = 0 OR beq = 1 & alu_zero = 1
          {
            next_state.pc = current_state.pc + (data_i << 2);
          }
        }
        else if(op == 0 && funct == 8) // checks to see if it's jumpReg
        {
          //cout<<"jump reg"<<endl;
          next_state.pc = data_rs; // PC=R[rs]
        }
        if (next_state.memwb.memwb_write == 0){ //if we will not perform this stage again end the cycle
            cout<<"current state pc!!!!!!"<<endl;
            next_state.memwb.complete = 1; //stop the loop
        }
       }

        cout << "CYCLE" << num_cycles << "\n";
        reg_file.print(); // used for automated testing
        num_cycles++;
        //cout<<"current state pc: "<<current_state.pc<<endl;
        //cout<<"next state pc: "<<next_state.pc<<endl;
        current_state = next_state;
        //cout<<"next state pc2: "<<next_state.pc<<endl;
        //cout<<"current state pc2: "<<current_state.pc<<endl;
        n++;
        //cout<<"next stage comp: "<< next_state.memwb.complete<<endl;

    }

    cout << "CPI = " << (double)num_cycles/(double)num_instrs << "\n";
}


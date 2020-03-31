#ifndef CONTROL_CLASS
#define CONTROL_CLASS
#include <vector>
#include <cstdint>
#include <iostream>
using namespace std;

// Control signals for the processor
struct control_t {
    bool reg_dest;           // 0 if rt, 1 if rd
    bool jump;               // 1 if jummp
    bool branch;             // 1 if branch
    bool mem_read;           // 1 if memory needs to be read
    bool mem_to_reg;         // 1 if memory needs to written to reg
    unsigned ALU_op : 2;     // 10 for R-type, 00 for LW/SW, 01 for BEQ/BNE, 11 for others
    bool mem_write;          // 1 if needs to be written to memory
    bool ALU_src;            // 0 if second operand is from reg_file, 1 if imm
    bool reg_write;          // 1 if need to write back to reg file
    
    void print() {      // Prints the generated contol signals
        cout << "REG_DEST: " << reg_dest << "\n";
        cout << "JUMP: " << jump << "\n";
        cout << "BRANCH: " << branch << "\n";
        cout << "MEM_READ: " << mem_read << "\n";
        cout << "MEM_TO_REG: " << mem_to_reg << "\n";
        cout << "ALU_OP: " << ALU_op << "\n";
        cout << "MEM_WRITE: " << mem_write << "\n";
        cout << "ALU_SRC: " << ALU_src << "\n";
        cout << "REG_WRITE: " << reg_write << "\n";
    }
    // TODO:
    // Decode instructions into control signals
    void decode(uint32_t instruction) {

      //reset all signals

    reg_dest = 0;
    jump = 0;
    branch = 0;
    mem_read = 0;
    mem_to_reg = 0;
    ALU_op = 0b10;
    mem_write = 0;
    ALU_src = 0;
    reg_write = 0;
      
      bool rType = !(instruction >> 26);
      bool jr = rType && (((instruction >> 16) & 0x1f) == 0x08);
      bool load = ((instruction >> 26) == 0b100100) || ((instruction >> 26) == 0b100101) || ((instruction >> 26) == 0b001111) || ((instruction >> 26) == 0b100011);
      bool store = ((instruction >> 26) == 0b001011) || ((instruction >> 26) == 0b111000) || ((instruction >> 26) == 0b101001) || ((instruction >> 26) == 0b101011);
      bool jumps = ((instruction >> 26) == 0x02) || ((instruction >> 26) == 0x03);
      bool beqne = ((instruction >> 26) == 0b100) || ((instruction >> 26) == 0b101);
      bool iType = !(rType || load || store || jumps || beqne);

      if(rType) // sets signals for all r-type instructions
	{
	  if(jr) // detects exception of jumpregister
	    {
	      reg_dest = 0;
	    }
	  else
	    {
	      reg_dest = 1;
	    }
	  reg_write = 1;
	  ALU_op = 0b10;
	}

      if(jumps) //sets signals for all jump  instructions
	{
	  jump = 1;
	  if((instruction >> 26) == 0x02)
	    {
	      reg_write = 0;
	    }
	  else
	    {
	      reg_write = 1;
	    }
	  ALU_op = 0b11;
	}

      if(beqne) //sets signals for all branch  instructions
	{
	  branch = 1;
	  ALU_op = 0b01;
	}

      if(load) //sets signals for all loads  instructions
	{
	  mem_read = 1;
	  mem_to_reg = 1;
	  ALU_src = 1;
	  reg_write = 1;
	  
	  if(((instruction >> 26) == 0b100011)) //except lw
	    {
	      ALU_op = 0b00;
	    }
	  else
	    {
	      ALU_op = 0b11;
	    }
	}

      if(store) //sets signals for all store  instructions
	{
	  mem_write = 1;
	  ALU_src = 1;
	  if(((instruction >> 26) == 0b101011)) //except sw
	    {
	      ALU_op = 0b00;
	    }
	  else
	    {
	      ALU_op = 0b11;
	    }
	}

      if(iType)
	{
	  ALU_op = 0b11;
	  ALU_src = 1;
	  reg_write = 1;
	}
    }	
};




#endif

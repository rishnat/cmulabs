#include <stdio.h>
#include "shell.h"

void process_instruction()
{
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. */
	 
	 // Define required variables 
	 uint32_t instruction_reg;
	 uint16_t immediate, offset;
	 uint32_t jump_target, branch_target, virt_addr;
	 uint32_t rs, rt, rd, base, sa, instr_regimm, instr_special, tempu;
	 int32_t temp;
	 uint32_t jump_flag=0;
	 uint32_t opcode;
	 int64_t mult_i;
	 uint64_t mult_u;
	 
	 instruction_reg=mem_read_32(CURRENT_STATE.PC);
	 
	 opcode=(instruction_reg&0xFC000000)>>26;
	 
	 switch(opcode)
	 {
		 case OP_J:
		 jump_target=instruction_reg&0x03FFFFFF;
		 NEXT_STATE.PC=(CURRENT_STATE.PC&0xF0000000)|(jump_target<<2);
		 break;
		 
		 case OP_JAL:
		 jump_target=instruction_reg&0x03FFFFFF;
		 NEXT_STATE.PC=(CURRENT_STATE.PC&0xF0000000)|(jump_target<<2);
		 NEXT_STATE.REGS[31]=CURRENT_STATE.PC+4;	//In actual hardware it is PC+8 because of branch delay slot 
		 break;
		 
		 case OP_BEQ:
		 offset=instruction_reg&0x0000FFFF;
		 branch_target=((offset<<16)>>14);
		 rs=(instruction_reg&0x03E00000)>>21;
		 rt=(instruction_reg&0x001F0000)>>16;
		 if(CURRENT_STATE.REGS[rs]==CURRENT_STATE.REGS[rt])
		 {
			 NEXT_STATE.PC=CURRENT_STATE.PC+branch_target;
		 }
		 else
		 {
			 NEXT_STATE.PC=CURRENT_STATE.PC+4;
		 }
		 break;
		 
		 case OP_BNE:
		 offset=instruction_reg&0x0000FFFF;
		 branch_target=((offset<<16)>>14);
		 rs=(instruction_reg&0x03E00000)>>21;
		 rt=(instruction_reg&0x001F0000)>>16;
		 if(CURRENT_STATE.REGS[rs]!=CURRENT_STATE.REGS[rt])
		 {
			 NEXT_STATE.PC=CURRENT_STATE.PC+branch_target;
		 }
		 else
		 {
			 NEXT_STATE.PC=CURRENT_STATE.PC+4;
		 }
		 break;
		 
		 case OP_BLEZ:
		 offset=instruction_reg&0x0000FFFF;
		 branch_target=((offset<<16)>>14);
		 rs=(instruction_reg&0x03E00000)>>21;
		 if((CURRENT_STATE.REGS[rs]&0x80000000)==0x80000000 || CURRENT_STATE.REGS[rs]==0)
		 {
			 NEXT_STATE.PC=CURRENT_STATE.PC+branch_target;
		 }
		 else
		 {
			 NEXT_STATE.PC=CURRENT_STATE.PC+4;
		 }
		 break;
		 
		 case OP_BGTZ:
		 offset=instruction_reg&0x0000FFFF;
		 branch_target=((offset<<16)>>14);
		 rs=(instruction_reg&0x03E00000)>>21;
		 if((CURRENT_STATE.REGS[rs]&0x80000000)==0x00000000 && CURRENT_STATE.REGS[rs]>0)
		 {
			 NEXT_STATE.PC=CURRENT_STATE.PC+branch_target;
		 }
		 else
		 {
			 NEXT_STATE.PC=CURRENT_STATE.PC+4;
		 }
		 break;
		 
		 case OP_ADDI:
		 immediate=instruction_reg&0x0000FFFF;
		 rs=(instruction_reg&0x03E00000)>>21;
		 rt=(instruction_reg&0x001F0000)>>16;
		 tempu=CURRENT_STATE.REGS[rs]+((immediate<<16)>>16);
		 
		 if((CURRENT_STATE.REGS[rs]&0x80000000)^(((immediate<<16)>>16)&0x80000000) || 
		 (!((tempu&0x80000000)^(CURRENT_STATE.REGS[rs]&0x80000000)) && !((tempu&0x80000000)^(CURRENT_STATE.REGS[rs]&0x80000000))))
		 {
			NEXT_STATE.REGS[rt]=tempu;
		 }
		 NEXT_STATE.PC=CURRENT_STATE.PC+4;
		 break;
		 
		 case OP_ADDIU:
		 immediate=instruction_reg&0x0000FFFF;
		 rs=(instruction_reg&0x03E00000)>>21;
		 rt=(instruction_reg&0x001F0000)>>16;
		 
		 NEXT_STATE.REGS[rt]=CURRENT_STATE.REGS[rs]+((immediate<<16)>>16);
		 NEXT_STATE.PC=CURRENT_STATE.PC+4;
		 break;
		 
		 case OP_SLTI:
		 immediate=instruction_reg&0x0000FFFF;
		 rs=(instruction_reg&0x03E00000)>>21;
		 rt=(instruction_reg&0x001F0000)>>16;
		 immediate=(immediate<<16)>>16;
		 
		 temp=(int32_t)CURRENT_STATE.REGS[rs]-(int32_t)immediate;
		 if(temp<0)
		 {
			 NEXT_STATE.REGS[rt]=1;
		 }
		 else
		 {
			 NEXT_STATE.REGS[rt]=0;
		 }
		 NEXT_STATE.PC=CURRENT_STATE.PC+4;
		 break;
		 
		 case OP_SLTIU:
		 immediate=instruction_reg&0x0000FFFF;
		 rs=(instruction_reg&0x03E00000)>>21;
		 rt=(instruction_reg&0x001F0000)>>16;
		 immediate=(immediate<<16)>>16;
		 
		 if(CURRENT_STATE.REGS[rs]<(uint32_t)immediate)
		 {
			 NEXT_STATE.REGS[rt]=1;
		 }
		 else
		 {
			 NEXT_STATE.REGS[rt]=0;
		 }
		 NEXT_STATE.PC=CURRENT_STATE.PC+4;
		 
		 break;
		 
		 case OP_ANDI:
		 immediate=instruction_reg&0x0000FFFF;
		 rs=(instruction_reg&0x03E00000)>>21;
		 rt=(instruction_reg&0x001F0000)>>16;
		 
		 NEXT_STATE.REGS[rt]=CURRENT_STATE.REGS[rs]&(immediate&0x0000FFFF);
		 NEXT_STATE.PC=CURRENT_STATE.PC+4;
		 break;
		 
		 case OP_ORI:
		 immediate=instruction_reg&0x0000FFFF;
		 rs=(instruction_reg&0x03E00000)>>21;
		 rt=(instruction_reg&0x001F0000)>>16;
		 
		 NEXT_STATE.REGS[rt]=CURRENT_STATE.REGS[rs]|(immediate&0x0000FFFF);
		 NEXT_STATE.PC=CURRENT_STATE.PC+4;
		 break;
		 
		 case OP_XORI:
		 immediate=instruction_reg&0x0000FFFF;
		 rs=(instruction_reg&0x03E00000)>>21;
		 rt=(instruction_reg&0x001F0000)>>16;
		 
		 NEXT_STATE.REGS[rt]=(CURRENT_STATE.REGS[rs])^(immediate&0x0000FFFF);
		 NEXT_STATE.PC=CURRENT_STATE.PC+4;
		 break;
		 
		 case OP_LUI:
		 immediate=instruction_reg&0x0000FFFF;
		 rt=(instruction_reg&0x001F0000)>>16;
		 
		 NEXT_STATE.REGS[rt]=(immediate<<16);
		 NEXT_STATE.PC=CURRENT_STATE.PC+4;
		 break;
		 
		 case OP_LB:
		 offset=instruction_reg&0x0000FFFF;
		 base=(instruction_reg&0x03E00000)>>21;
		 rt=(instruction_reg&0x001F0000)>>16;
		 
		 virt_addr=CURRENT_STATE.REGS[base]+((offset<<16)>>16);
		 NEXT_STATE.REGS[rt]=(((int8_t)mem_read_32(virt_addr))<<24)>>24;
		 NEXT_STATE.PC=CURRENT_STATE.PC+4;
		 break;
		 
		 case OP_LH:
		 offset=instruction_reg&0x0000FFFF;
		 base=(instruction_reg&0x03E00000)>>21;
		 rt=(instruction_reg&0x001F0000)>>16;
		 
		 virt_addr=CURRENT_STATE.REGS[base]+((offset<<16)>>16);
		 NEXT_STATE.REGS[rt]=(((int16_t)mem_read_32(virt_addr))<<16)>>16;
		 NEXT_STATE.PC=CURRENT_STATE.PC+4;
		 break;
		 
		 case OP_LW:
		 offset=instruction_reg&0x0000FFFF;
		 base=(instruction_reg&0x03E00000)>>21;
		 rt=(instruction_reg&0x001F0000)>>16;
		 
		 virt_addr=CURRENT_STATE.REGS[base]+((offset<<16)>>16);
		 NEXT_STATE.REGS[rt]=mem_read_32(virt_addr);
		 NEXT_STATE.PC=CURRENT_STATE.PC+4;
		 break;

		 case OP_LBU:
		 offset=instruction_reg&0x0000FFFF;
		 base=(instruction_reg&0x03E00000)>>21;
		 rt=(instruction_reg&0x001F0000)>>16;
		 
		 virt_addr=CURRENT_STATE.REGS[base]+((offset<<16)>>16);
		 NEXT_STATE.REGS[rt]=((mem_read_32(virt_addr)&0x000000FF));
		 NEXT_STATE.PC=CURRENT_STATE.PC+4;
		 break;
		 
		 case OP_LHU:
		 offset=instruction_reg&0x0000FFFF;
		 base=(instruction_reg&0x03E00000)>>21;
		 rt=(instruction_reg&0x001F0000)>>16;
		 
		 virt_addr=CURRENT_STATE.REGS[base]+((offset<<16)>>16);
		 NEXT_STATE.REGS[rt]=((mem_read_32(virt_addr)&0x0000FFFF));
		 NEXT_STATE.PC=CURRENT_STATE.PC+4;
		 break;
		 
		 case OP_SB:
		 offset=instruction_reg&0x0000FFFF;
		 base=(instruction_reg&0x03E00000)>>21;
		 rt=(instruction_reg&0x001F0000)>>16;
		 
		 virt_addr=CURRENT_STATE.REGS[base]+((offset<<16)>>16);
		 mem_write_8(virt_addr,(uint8_t)CURRENT_STATE.REGS[rt]);
		 NEXT_STATE.PC=CURRENT_STATE.PC+4;
		 break;
		 
		 case OP_SH:
		 offset=instruction_reg&0x0000FFFF;
		 base=(instruction_reg&0x03E00000)>>21;
		 rt=(instruction_reg&0x001F0000)>>16;
		 
		 virt_addr=CURRENT_STATE.REGS[base]+((offset<<16)>>16);
		 mem_write_16(virt_addr,(uint16_t)CURRENT_STATE.REGS[rt]);
		 NEXT_STATE.PC=CURRENT_STATE.PC+4;
		 break;
		 
		 case OP_SW:
		 offset=instruction_reg&0x0000FFFF;
		 base=(instruction_reg&0x03E00000)>>21;
		 rt=(instruction_reg&0x001F0000)>>16;
		 
		 virt_addr=CURRENT_STATE.REGS[base]+((offset<<16)>>16);
		 mem_write_32(virt_addr, CURRENT_STATE.REGS[rt]);
		 NEXT_STATE.PC=CURRENT_STATE.PC+4;
		 break;

		 case OP_REGIMM:
		 offset=instruction_reg&0x0000FFFF;
		 instr_regimm=(instruction_reg&0x001F0000)>>16;
		 rs=(instruction_reg&0x03E00000)>>21;
		 branch_target=CURRENT_STATE.PC+((offset<<16)>>14);
		 
		 switch(instr_regimm)
		 {
		 case OP_BLTZ:
			NEXT_STATE.PC=((CURRENT_STATE.REGS[rs]&0x80000000)==0x80000000)?branch_target:CURRENT_STATE.PC+4;
			break;
		 
		 case OP_BGEZ:
			NEXT_STATE.PC=((CURRENT_STATE.REGS[rs]&0x80000000)==0x00000000)?branch_target:CURRENT_STATE.PC+4;
			break;
			
		 case OP_BLTZAL:
			NEXT_STATE.PC=((CURRENT_STATE.REGS[rs]&0x80000000)==0x80000000)?branch_target:CURRENT_STATE.PC+4;
			NEXT_STATE.REGS[31]=CURRENT_STATE.PC+4;
			break;
			
		 case OP_BGEZAL:
			NEXT_STATE.PC=((CURRENT_STATE.REGS[rs]&0x80000000)==0x00000000)?branch_target:CURRENT_STATE.PC+4;
			NEXT_STATE.REGS[31]=CURRENT_STATE.PC+4;
			break;
			
		 default:
			printf("Illegal Instruction");
			
		 }
		 
		 break;
		 
		 case OP_SPECIAL:
		 instr_special=instruction_reg&0x0000003F;
		 immediate=instruction_reg&0x0000FFFF;
		 rs=(instruction_reg&0x03E00000)>>21;
		 rt=(instruction_reg&0x001F0000)>>16;
		 rd=(instruction_reg&0x0000F800)>>11;
		 sa=(instruction_reg&0x000007C0)>>6;
		 
		 
		 switch(instr_special)
		 {
			 
		 case OP_SLL:
			NEXT_STATE.REGS[rd]=CURRENT_STATE.REGS[rt]<<sa;
			break;
			
		 case OP_SRL:
			NEXT_STATE.REGS[rd]=(CURRENT_STATE.REGS[rt]>>sa)& ~(((0x1 << sizeof(uint32_t)) >> sa) << 1);
			break;
			
		 case OP_SRA:
			NEXT_STATE.REGS[rd]=CURRENT_STATE.REGS[rt]>>sa;
			break;

		 case OP_SLLV:
			NEXT_STATE.REGS[rd]=CURRENT_STATE.REGS[rt]<<(CURRENT_STATE.REGS[rs]&0x0000001F);
			break;
			
		 case OP_SRLV:
			NEXT_STATE.REGS[rd]=CURRENT_STATE.REGS[rt]>>(CURRENT_STATE.REGS[rs]&0x0000001F)& 
			~(((0x1 << sizeof(uint32_t)) >> (CURRENT_STATE.REGS[rs]&0x0000001F)) << 1);;
			break;
		 
		 case OP_SRAV:
			NEXT_STATE.REGS[rd]=CURRENT_STATE.REGS[rt]>>(CURRENT_STATE.REGS[rs]&0x0000001F);
			break;
			
		 case OP_JR:
			NEXT_STATE.PC=CURRENT_STATE.REGS[rs];
			jump_flag=1;
			break;
			
		 case OP_JALR:
			NEXT_STATE.PC=CURRENT_STATE.REGS[rs];
			NEXT_STATE.REGS[rd]=CURRENT_STATE.PC+4;
			jump_flag=1;
			break;
			
		 case OP_ADD:
			tempu=CURRENT_STATE.REGS[rs]+CURRENT_STATE.REGS[rt];
			 
			if((CURRENT_STATE.REGS[rs]&0x80000000)^(((immediate<<16)>>16)&0x80000000) || 
			(!((tempu&0x80000000)^(CURRENT_STATE.REGS[rs]&0x80000000)) && !((tempu&0x80000000)^(CURRENT_STATE.REGS[rs]&0x80000000))))
			NEXT_STATE.REGS[rd]=tempu;
			break;
			
		 case OP_ADDU:
			NEXT_STATE.REGS[rd]=CURRENT_STATE.REGS[rs]+CURRENT_STATE.REGS[rt];
			break;
			
		 case OP_SUB:
			NEXT_STATE.REGS[rd]=CURRENT_STATE.REGS[rs]-CURRENT_STATE.REGS[rt];
			break;
			
		 case OP_SUBU:
			tempu=CURRENT_STATE.REGS[rs]-CURRENT_STATE.REGS[rt];
			 
			if((CURRENT_STATE.REGS[rs]&0x80000000)^(((immediate<<16)>>16)&0x80000000) || 
			(!((tempu&0x80000000)^(CURRENT_STATE.REGS[rs]&0x80000000)) && !((tempu&0x80000000)^(CURRENT_STATE.REGS[rs]&0x80000000))))
			NEXT_STATE.REGS[rd]=tempu;
			
			break;
			
		 case OP_AND:
			NEXT_STATE.REGS[rd]=CURRENT_STATE.REGS[rs]&CURRENT_STATE.REGS[rt];
			break;
			
		 case OP_OR:
			NEXT_STATE.REGS[rd]=CURRENT_STATE.REGS[rs]|CURRENT_STATE.REGS[rt];
			break;
			
		 case OP_XOR:
			NEXT_STATE.REGS[rd]=CURRENT_STATE.REGS[rs]^CURRENT_STATE.REGS[rt];
			break;
			
		 case OP_NOR:
			NEXT_STATE.REGS[rd]=~(CURRENT_STATE.REGS[rs]|CURRENT_STATE.REGS[rt]);
			break;
			
		 case OP_SLT:
			temp=(int32_t)(CURRENT_STATE.REGS[rs]-CURRENT_STATE.REGS[rt]);
			NEXT_STATE.REGS[rd] = (temp<0)?1:0;
			break;
			
		 case OP_SLTU:
			NEXT_STATE.REGS[rd]=CURRENT_STATE.REGS[rs]<CURRENT_STATE.REGS[rt]?1:0;
			break;
			 
		 case OP_MULT:
		    mult_i=(int64_t)CURRENT_STATE.REGS[rs]*(int64_t)CURRENT_STATE.REGS[rt];
			NEXT_STATE.HI=(mult_i)&0xFFFFFFFF00000000;
			NEXT_STATE.LO=(mult_i)&0x00000000FFFFFFFF;
			break;
			
		 case OP_MFHI:
			NEXT_STATE.REGS[rd]=CURRENT_STATE.HI;
			break;
			 
		 case OP_MFLO:
			NEXT_STATE.REGS[rd]=CURRENT_STATE.LO;
			break;
			
		 case OP_MTHI:
		    NEXT_STATE.HI=CURRENT_STATE.REGS[rs];
			break;
			
		 case OP_MTLO:
		    NEXT_STATE.LO=CURRENT_STATE.REGS[rs];
			break;
			 
		 case OP_MULTU:
		    mult_u=(uint64_t)CURRENT_STATE.REGS[rs]*(uint64_t)CURRENT_STATE.REGS[rt];
			NEXT_STATE.HI=(mult_u)&0xFFFFFFFF00000000;
			NEXT_STATE.LO=(mult_u)&0x00000000FFFFFFFF;
			break;
			
		 case OP_DIV:
			NEXT_STATE.HI=((int32_t)CURRENT_STATE.REGS[rs]%(int32_t)CURRENT_STATE.REGS[rt]);
			NEXT_STATE.LO=((int32_t)CURRENT_STATE.REGS[rs]/(int32_t)CURRENT_STATE.REGS[rt]);
			break;
			
		 case OP_DIVU:
			NEXT_STATE.HI=(CURRENT_STATE.REGS[rs]%CURRENT_STATE.REGS[rt]);
			NEXT_STATE.LO=(CURRENT_STATE.REGS[rs]/CURRENT_STATE.REGS[rt]);
			break;
			
		 case OP_SYSCALL:
			if(CURRENT_STATE.REGS[2]==0x0A)
			{
				RUN_BIT=FALSE;
			}
			jump_flag=1;
			break;
			
		 default:
			printf("Illegal Instruction");
		 }
		 
		 if(jump_flag==0)
		 {
			 NEXT_STATE.PC=CURRENT_STATE.PC+4;
		 }
		 break;
		 
		 default:
		 printf("Illegal Instruction");

		 
		 
	 }
	 
	 
}

/**
	@file
	@author Andrew D. Zonenberg
	@brief The core of the simulator
 */
#include "sim.h"
#include "string.h"

/**
	@brief Read logic for instruction fetch and load instructions
	
	Address must be aligned
 */
uint32_t FetchWordFromVirtualMemory(uint32_t address, struct virtual_mem_region* memory, FILE* output)
{
	//Traverse the linked list until we find the range of interest

	while(memory != NULL)
	{
		//Not in range? Try next one
		if( (address < memory->vaddr) || (address >= (memory->vaddr + memory->len)) )
		{
			memory = memory->next;
			continue;
		}
		
		//Align check
		uint32_t offset = address - memory->vaddr;
		if(offset & 3)
		{
			fprintf(output, "SEGFAULT: address %08x is not aligned\n", address);
			exit(1);	
		}
		return memory->data[offset/4];
	}
	
	//Didn't find anything! Give up
	fprintf(output, "SEGFAULT: attempted to read word from nonexistent virtual address %08x\n", address);
	exit(1);
}

uint32_t FetchDataFromVirtualMemory(uint32_t address, struct virtual_mem_region* memory){
	// simealer to fetchWord, but returns the entire bock of data from that memory location instead of just a chunk (used for loading data into regesters)
	while(memory != NULL){
		if( (address < memory->vaddr) || (address >= (memory->vaddr + memory->len)) ){
			memory = memory->next;
			continue;
		}
		uint32_t offset = address - memory->vaddr;
		if(offset & 3){
			printf("SEGFAULT: address %08x is not alligned\n", address);
			exit(1);
		}
		return *memory->data;
	}
	printf("SEGFAULT: attempted to read word from nonexistent virtual address %08x\n", address);
	exit(1);
}

/**
	@brief Write logic for store instructions.
	
	Stores an entire 32-bit word. sh/sb instructions will need to do a read-modify-write structure
 */
void StoreWordToVirtualMemory(uint32_t address, uint32_t value, struct virtual_mem_region* memory, FILE* output)
{
	//Traverse the linked list until we find the range of interest
	while(memory != NULL)
	{
		//Not in range? Try next one
		if( (address < memory->vaddr) || (address >= (memory->vaddr + memory->len)) )
		{
			memory = memory->next;
			continue;
		}
		
		//Align check
		uint32_t offset = address - memory->vaddr;
		if(offset & 3)
		{
			fprintf(output, "SEGFAULT: address %08x is not aligned\n", address);
			exit(1);	
		}
		
		memory->data[offset/4] = value;
		return;
	}
	
	//Didn't find anything! Give up
	fprintf(output, "SEGFAULT: attempted to write word to nonexistent virtual address %08x\n", address);
	exit(1);
}

/**
	@brief Runs the actual simulation
 */
void RunSimulator(struct virtual_mem_region* memory, struct context* ctx, FILE* output)
{
	fprintf(output, "Starting simulation...\n");
	ctx->instrCnt = 0;
	union mips_instruction inst;
	ctx->regs[26] = ctx->pc; // store the memory location of the begining of the stack in the frame pointer regester ($fp)
	while(1)
	{
		inst.word = FetchWordFromVirtualMemory(ctx->pc, memory, output);
		if(!SimulateInstruction(&inst, memory, ctx, output))
			break;
		else
			ctx->instrCnt++;		
	}
}

/**
	@brief Simulates a single instruction
	
	Return 0 to exit the program (for syscall/invalid instruction) and 1 to keep going
 */
int SimulateInstruction(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx, FILE* output)
{
	//TODO: Switch on opcode, if R-type instruction call SimulateRTypeInstruction()
	//otherwise it's I/J type
	//printf("the word in this instance is: %x, where the opcode is: %x\n", inst->word, inst->itype.opcode);
	
	enum opcodes rtype = OP_RTYPE;	
	if(inst->rtype.opcode == OP_RTYPE){
		if(!SimulateRtypeInstruction(inst, memory, ctx, output))
			return 0;
	}
	else if(inst->itype.opcode == OP_BRANCH){
		if(inst->itype.rt == 0){ // BLTZ
			if(ctx->regs[inst->itype.rs] < 0)
                        	ctx->pc = (inst->itype.imm << 2);
		}
		else if(inst->itype.rt == 1){ // BGEZ
			if(ctx->regs[inst->itype.rs] >= 0)
                        	ctx->pc = (inst->itype.imm << 2);
		}
		else if(inst->itype.rt == 10){ // BLTZAL
			if(ctx->regs[inst->itype.rs] < 0){
	                        ctx->regs[REGID_RA] = ctx->pc + 4;
				ctx->pc = (inst->itype.imm << 2);
			}

		}
		else if(inst->itype.rt == 11){ // BGEZAL
			if(ctx->regs[inst->itype.rs] >= 0){
				ctx->regs[REGID_RA] = ctx->pc + 4;
				ctx->pc = (inst->itype.imm << 2);
			}
		}
	}
	else if(inst->itype.opcode == OP_J){
		ctx->pc = (ctx->pc & 0xf0000000) | (inst->jtype.addr << 2);
		return 1; 
	}
	else if(inst->itype.opcode == OP_JAL){
		ctx->regs[REGID_RA] = ctx->pc + 4;
		ctx->pc = (ctx->pc & 0xf0000000) | (inst->jtype.addr << 2);
		//printf("	Jump requested: target addr is [%08x], preforming...\n", ctx->pc);
		return 1;
	}
	else if(inst->itype.opcode == OP_BEQ){
		if(ctx->regs[inst->itype.rs] == ctx->regs[inst->itype.rt])
			ctx->pc = (inst->itype.imm << 2);
	}
	else if(inst->itype.opcode == OP_BNE){
		if(ctx->regs[inst->itype.rs] != ctx->regs[inst->itype.rt])
                        ctx->pc = (inst->itype.imm << 2);
	}
	else if(inst->itype.opcode == OP_BLEZ){
		if(ctx->regs[inst->itype.rs] <= 0)
                        ctx->pc = (inst->itype.imm << 2);	
	}
	else if(inst->itype.opcode == OP_BGTZ){
		if(ctx->regs[inst->itype.rs] > 0)
                        ctx->pc = (inst->itype.imm << 2);
	}
	else
		SimulateItypeInstruction(inst, memory, ctx, output);
	ctx->pc += 4;
	
	return 1;
}

int SimulateRtypeInstruction(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx, FILE* output)
{
	//TODO: switch on func, if syscall call SimulateSyscall()
	//else process instruction normally
	enum functions funct = FUNC_SYSCALL;
	enum regids regid = REGID_ZERO;	
	if(inst->rtype.func == FUNC_SLL){
		ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rt] << ctx->regs[inst->rtype.shamt];
	}
	else if(inst->rtype.func == FUNC_SRL){
		 ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rt] >> ctx->regs[inst->rtype.shamt];
	}
	else if(inst->rtype.func == FUNC_SRA){ 
		 ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rt] >> ctx->regs[inst->rtype.shamt];	
	}
	else if(inst->rtype.func == FUNC_SLLV){
		ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rt] << ctx->regs[inst->rtype.rs];
	}
	else if(inst->rtype.func == FUNC_SRLV){
		 ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rt] >> ctx->regs[inst->rtype.rs];
	}
	else if(inst->rtype.func == FUNC_JR){ // jump to regester in rs
		uint32_t returnAddress = FetchDataFromVirtualMemory( ctx->regs[inst->rtype.rs], memory);
		//printf("	jump requested to address [%x], where we should be returning to [%x]\n", returnAddress, ctx->regs[REGID_RA]);
		ctx->pc = ctx->regs[inst->rtype.rs];
	}
	else if(inst->rtype.func == FUNC_SYSCALL){//if its a system call, execute it and return
		if(!SimulateSyscall(inst->word, memory, ctx, output))
			return 0;
		else
			return 1;
	}
	else if(inst->rtype.func == FUNC_MFHI){
		ctx->regs[inst->rtype.rd] = ctx->hi;
	}
	else if(inst->rtype.func == FUNC_MFLO){
		ctx->regs[inst->rtype.rd] = ctx->lo;
	}
	else if(inst->rtype.func == FUNC_MULT){  
		uint64_t temp = ctx->regs[inst->rtype.rs] * ctx->regs[inst->rtype.rt];
		ctx->lo = temp;
		temp>>32;
		ctx->hi = temp;
	}
	else if(inst->rtype.func == FUNC_MULTU){
		uint64_t temp = ctx->regs[inst->rtype.rs] * ctx->regs[inst->rtype.rt];
                ctx->lo = temp;
                temp>>32;
                ctx->hi = temp;
	}
	else if(inst->rtype.func == FUNC_DIV){
		 ctx->lo = ctx->regs[inst->rtype.rs] / ctx->regs[inst->rtype.rt];
		 ctx->hi = ctx->regs[inst->rtype.rs] % ctx->regs[inst->rtype.rt];
	} 
	else if(inst->rtype.func == FUNC_DIVU){
		 ctx->lo = ctx->regs[inst->rtype.rs] / ctx->regs[inst->rtype.rt];
                 ctx->hi = ctx->regs[inst->rtype.rs] % ctx->regs[inst->rtype.rt];
	}
	else if(inst->rtype.func == FUNC_ADD){
		ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] + ctx->regs[inst->rtype.rt];
	} 
	else if(inst->rtype.func == FUNC_ADDU){
		 ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] + ctx->regs[inst->rtype.rt];
	}
	else if(inst->rtype.func == FUNC_SUB){ 
		 ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] - ctx->regs[inst->rtype.rt];
	}
	else if(inst->rtype.func == FUNC_SUBU){
		 ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] - ctx->regs[inst->rtype.rt];
	}
	else if(inst->rtype.func == FUNC_AND){
		 ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] & ctx->regs[inst->rtype.rt];
	}
	else if(inst->rtype.func = FUNC_OR){ 
		ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] | ctx->regs[inst->rtype.rt];
		//printf("	result of or is: %x\n", ctx->regs[inst->rtype.rd]);
	}
	else if(inst->rtype.func == FUNC_XOR){
		 ctx->regs[inst->rtype.rd] = ctx->regs[inst->rtype.rs] ^ ctx->regs[inst->rtype.rt];	
	}
	else if(inst->rtype.func == FUNC_SLT){ 
		if(ctx->regs[inst->rtype.rs] < ctx->regs[inst->rtype.rt])
			ctx->regs[inst->rtype.rd] = 1;
		else
			ctx->regs[inst->rtype.rd] = 0;
	}
	else if(inst->rtype.func == FUNC_SLTU){
		if(ctx->regs[inst->rtype.rs] < ctx->regs[inst->rtype.rt])
                        ctx->regs[inst->rtype.rd] = 1;
                else
                        ctx->regs[inst->rtype.rd] = 0;
	}
	//printf("	rs: %x\n\trt: %x\n\trd: %x\n\tfunct: %x\n\tshamt: %x\n", inst->rtype.rs, inst->rtype.rt, inst->rtype.rd, inst->rtype.func, inst->rtype.shamt);
	return 1;
}

int SimulateItypeInstruction(union mips_instruction* inst, struct virtual_mem_region* memory, struct context* ctx, FILE* output){
	enum opcodes opcode = OP_ADDIU;
	if(inst->itype.opcode == OP_ADDI){ 
		 ctx->regs[inst->itype.rt] =  ctx->regs[inst->itype.rs] + (inst->itype.imm << 1); 
	}
	else if(inst->itype.opcode == OP_ADDIU){ // if its an addiu instruction
		//printf("	setting regester [%x] to [%x]\n", inst->itype.rt,((short) inst->itype.imm) + ctx->regs[inst->itype.rs]);
		ctx->regs[inst->itype.rt] = ctx->regs[inst->itype.rs] +(short) inst->itype.imm ;		
	}
	else if(inst->itype.opcode == OP_LUI){
		ctx->regs[inst->itype.rt] = inst->itype.imm << 16;
		//printf("	call to lui with rt [%x], rs [%x], and imm [%x]\n", inst->itype.rt, inst->itype.rs, inst->itype.imm << 16);
	}
	else if(inst->itype.opcode == OP_SLTI){ // TODO: throw exception if theres overflow
		if(ctx->regs[inst->itype.rs] < (inst->itype.imm << 1))
			ctx->regs[inst->itype.rt] = 1;
		else
			ctx->regs[inst->itype.rt] = 0;
	}
	else if(inst->itype.opcode == OP_SLTIU){
		if(ctx->regs[inst->itype.rs] < (inst->itype.imm << 1))
                        ctx->regs[inst->itype.rt] = 1;
                else
                        ctx->regs[inst->itype.rt] = 0;
	}
	else if(inst->itype.opcode == OP_ANDI){
		ctx->regs[inst->itype.rt] = ctx->regs[inst->itype.rs] & (inst->itype.imm << 1);
	}
	else if(inst->itype.opcode == OP_ORI){
		 ctx->regs[inst->itype.rt] = ctx->regs[inst->itype.rs] | (inst->itype.imm << 1);
	}
	else if(inst->itype.opcode == OP_XORI){
		 ctx->regs[inst->itype.rt] = ctx->regs[inst->itype.rs] ^ (inst->itype.imm << 1);
	}
	else if(inst->itype.opcode == OP_LW){ // if its a lw instruction
		uint32_t wordAsciiz = FetchWordFromVirtualMemory(ctx->regs[inst->itype.rs] + (short)inst->itype.imm, memory, output);	
		ctx->regs[inst->itype.rt] = wordAsciiz;
		//printf("	loaded [%08x] from address [%x] into regester [%08x], rs is [%x]\n", wordAsciiz, memAddress, inst->itype.rt, ctx->regs[inst->itype.rs]);
	}
	else if(inst->itype.opcode == OP_LB){
                uint32_t wordAsciiz = FetchWordFromVirtualMemory(ctx->regs[inst->itype.rs] + (short)inst->itype.imm, memory, output);
		uint8_t byte = (uint8_t)(wordAsciiz>>0);
		wordAsciiz = (uint32_t)(byte<<24);
		ctx->regs[inst->itype.rt] = wordAsciiz;
	}
	else if(inst->itype.opcode == OP_SW){
		//printf("	storing [%x] at address [%x], imm is [%x]\n", ctx->regs[inst->itype.rt], ctx->regs[inst->itype.rs] + inst->itype.imm, inst->itype.imm);
		StoreWordToVirtualMemory(ctx->regs[inst->itype.rs] + inst->itype.imm, ctx->regs[inst->itype.rt], memory, output);
	}
	else if(inst->itype.opcode == OP_SB){
		StoreWordToVirtualMemory(ctx->regs[inst->itype.rs] + inst->itype.imm, ctx->regs[inst->itype.rt], memory, output);
	}

	return 1;
}

int SimulateSyscall(uint32_t callnum, struct virtual_mem_region* memory, struct context* ctx, FILE* output)
{
	enum regids regid = REGID_ZERO;
	uint32_t v0_val = ctx->regs[2]; //access the value stored in $v0
	if(v0_val == 1){ // print int
		printf("%d\n", ctx->regs[REGID_A0]);	
	}
	else if(v0_val == 2){ // print float
		printf("%d\n", ctx->regs[REGID_A0]);
	}
	else if(v0_val == 3){ // print double
		printf("%d\n", ctx->regs[REGID_A0]);
        }
	else if(v0_val == 4){// print string
		uint32_t wordAsciiz = FetchWordFromVirtualMemory(ctx->regs[REGID_A0], memory, output);
		char* string = (char*)&wordAsciiz;
		int linefeedp = 1;
		int x;
		int offset = 0;
		while(linefeedp == 1){ // loop through the memory block, printing 4 bytes of the string at a time (oh kill em)
			wordAsciiz = FetchWordFromVirtualMemory(ctx->regs[REGID_A0] + offset, memory, output);
			string = (char*)&wordAsciiz;
			printf("%s", string);
			for(x = 0; x < 4; x++)
				if(string[x] == '\n')
					linefeedp = 0;
			offset += 4;
		}
	}
	else if(v0_val == 5){ // read int
		scanf("%d", &ctx->regs[2]);
        }
	else if(v0_val == 7){ // read double
		double inputD;
		scanf("%lf", &inputD);
		ctx->regs[2] = (uint32_t)inputD;
        }
	else if(v0_val == 8){ // read string
		unsigned char word[1000]; 
		scanf("%s", word);
		ctx->regs[REGID_A0] = (uint32_t)word;
		ctx->regs[REGID_A0 + 1] = (uint32_t)strlen(word);
        }
	else if(v0_val == 9){ // memory allocation

        }
	else if(v0_val == 10){//call to terminate the program
		return 0;
	}
	else if(v0_val == 11){ // print character
		printf("%c\n", ctx->regs[REGID_A0]);
        }
	else if(v0_val == 12){ // read character
		char inputC;
		scanf("%c", & inputC);
		ctx->regs[2] = (uint32_t)inputC;
        }
	else{
		fprintf(output, "ERROR: invalid system call argument of [%x]\n", ctx->regs[2]);
		exit(1);
	}

	return 1;
}

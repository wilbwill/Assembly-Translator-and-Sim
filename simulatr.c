#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000

typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);
int convertNum(int num);
int getOpcode(int opNum);
void getFieldsR(int currentOp, int *fields);
void getFieldsI(int currentOp, int *fields);
void getFieldsJ(int currentOp, int *fields);

int main(int argc, char *argv[]) {
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }

    /* read in the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL;
            state.numMemory++) {

        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }
    printf("%s\n", "");

    //Set all registers to 0
    for (int i = 0; i < NUMREGS; ++i) {
    	state.reg[i] = 0;
    }
    //Start the simulation
    state.pc = 0;
    int currentOp = state.mem[state.pc];
    int op = getOpcode(currentOp);
    int fields[3];
    int count = 0;
    while(op != 6) {
    	//print the current state
    	printState(&state);
    	++count;


    	//add
    	if(op == 0) {
    		//fill the fields
    		getFieldsR(currentOp, fields);
    		int regA = fields[0];
    		int regB = fields[1];
    		int destReg = fields[2];

    		//add contents of regA and regB then put it in destReg
    		state.reg[destReg] = state.reg[regA] + state.reg[regB];
    	}
    	//nor
    	else if(op == 1) {
    		getFieldsR(currentOp, fields);
    		int regA = fields[0];
    		int regB = fields[1];
    		int destReg = fields[2];

    		//or regA and regB then take the complememnt and store in destReg
    		state.reg[destReg] = ~(state.reg[regA] | state.reg[regB]);
    	}
    	//lw
    	else if(op == 2) {
    		//fill the fields
    		getFieldsI(currentOp, fields);
    		int regA = fields[0];
    		int regB = fields[1];
    		int offset = fields[2];

    		//set the mem address, which is offset + contents of regA
    		//then load memAdress in regB
    		int memAddress = offset + state.reg[regA];
    		state.reg[regB] = state.mem[memAddress];
    	}
    	//sw
    	else if(op == 3) {
    		//fill the fields
    		getFieldsI(currentOp, fields);
    		int regA = fields[0];
    		int regB = fields[1];
    		int offset = fields[2];

    		//calculate memAdress
    		int memAddress = offset + state.reg[regA];

    		//store regB into memAdress
    		state.mem[memAddress] = state.reg[regB];
    	}
    	//beq
    	else if(op == 4) {
    		//fill fields
    		getFieldsI(currentOp, fields);
    		int regA = fields[0];
    		int regB = fields[1];
    		int offset = fields[2];

    		// check if contents of regA == contents regB and then branch accordingly
    		if(state.reg[regA] == state.reg[regB]) {
    			//Dont add 1, because ++pc happens later
    			state.pc = state.pc + offset;
    			
    		}
    		printf("%s\n", "Beq done");
    	}
    	//jalr
    	else if(op == 5) {
    		getFieldsJ(currentOp, fields);
    		int regA = fields[0];
    		int regB = fields[1];

    		//store pc+1 in regB
    		state.reg[regB] = state.pc + 1;

    		//branch to address in regA
    		state.pc = state.reg[regA];
    	}
    	//noop
    	else if(op == 7) {

    	}
    	else {
    		printf("%s\n", "Invalid opcode");
    		exit(1);
    	}

    	//Get next opcode and increment pc
    	if(op != 5) state.pc++;
    	currentOp = state.mem[state.pc];
    	op = getOpcode(currentOp);

    	if(state.pc > state.numMemory) {
    		printf("%s\n", "PC out of file scope");
    		exit(1);
    	}

    }
    //print and halt machine
    ++count;
	printState(&state);
	printf("%s\n", "machine halted");
	printf("%s ", "total of");
	printf("%d ", count);
	printf("%s\n", "instructions executed");
	printf("%s\n", "final state of machine: ");


    ++state.pc;
    printState(&state);

    return(0);
}

int getOpcode(int opNum) {
	return opNum >> 22;
}

void getFieldsR(int currentOp, int *fields) {
	//& with 7 that is bitshifted will give us just the field we need
	fields[0] = (currentOp & (7 << 19)) >> 19;
	fields[1] = (currentOp & (7 << 16)) >> 16;
	fields[2] = currentOp & 7;

	if(fields[0] < 0 || fields[1] < 0 || fields[2] < 0) {
		printf("%s\n", "Negative register");
		exit(1);
	}
	else if(fields[0] > 7 || fields[1] > 7 || fields[2] > 7) {
		printf("%s\n", "Only eight registers");
		exit(1);
	}
}

void getFieldsI(int currentOp, int *fields) {
	//& with 7 that is bitshifted will give us just the field we need
	fields[0] = (currentOp & (7 << 19)) >> 19;
	fields[1] = (currentOp & (7 << 16)) >> 16;
	fields[2] = currentOp & 65535;

	//convert the number to a 32-bit number
	fields[2] = convertNum(fields[2]);

	if(fields[0] < 0 || fields[1] < 0) {
		printf("%s\n", "Negative register");
		exit(1);
	}
	else if(fields[0] > 7 || fields[1] > 7) {
		printf("%s\n", "Only eight registers");
		exit(1);
	}
}

void getFieldsJ(int currentOp, int *fields) {
	//& with 7 that is bitshifted will give us just the field we need
	fields[0] = (currentOp & (7 << 19)) >> 19;
	fields[1] = (currentOp & (7 << 16)) >> 16;

	if(fields[0] < 0 || fields[1] < 0) {
		printf("%s\n", "Negative register");
		exit(1);
	}
	else if(fields[0] > 7 || fields[1] > 7) {
		printf("%s\n", "Only eight registers");
		exit(1);
	}
}

void
printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i=0; i<statePtr->numMemory; i++) {
        printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
    for (i=0; i<NUMREGS; i++) {
        printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}


int convertNum(int num) {
    /* convert a 16-bit number into a 32-bit Linux integer */
    if (num & (1<<15) ) {
        num -= (1<<16);
    }
    return(num);
}

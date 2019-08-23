#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000

struct Labels {
	int address;
	char innerLabel[7];
};

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);

int
main(int argc, char *argv[]) {

    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
            arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

    if (argc != 3) {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
            argv[0]);
        exit(1);
    }

    inFileString = argv[1];
    outFileString = argv[2];

    inFilePtr = fopen(inFileString, "r");
    if (inFilePtr == NULL) {
        printf("error in opening %s\n", inFileString);
        exit(1);
    }
    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }

    struct Labels labels[256];

    for(int i = 0; i < 256; ++i){
    	labels[i].address = -1;
    }


//TODO check length of labels, make sure they are exactly 6
    /*first read through to gather labels in label struct*/
	int line = 0;
	int index = 0;
    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ) {
        if(strcmp(label, "") != 0) {
        	if(strlen(label) > 6) {
        		printf("%s\n", "Error: label more than 6 characters");
        		exit(1);
        	}
        	strcpy(labels[index].innerLabel, label);
        	labels[index].address = line;
        	++line;
        	++index;
        }
        else ++line;
    }

    /*Check duplicates in labels*/
    char labCheck[7];
	int cmp = -2;
	unsigned labNum = 0;
	for(int i = 0; labels[i].address != -1; ++i) {
		strcpy(labCheck, labels[i].innerLabel);
		for(int j = i + 1; labels[j].address != -1; ++j) {
			cmp = strcmp(labCheck, labels[j].innerLabel);
			if(cmp == 0) ++labNum;
			if(labNum >= 1) {
				printf("%s\n", "Error: duplicate label");
				exit(1);
			} 
		}
		labNum = 0;
	}

	/*rewind file pointer*/   
    rewind(inFilePtr);

    /*find the correct opcode for each line and do the
      appropriate operations*/
    int bitString = 0;
    int lineNum = 0;
    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ) {
    	bitString = 0;
    	if (!strcmp(opcode, "add")) {
    		if(atoi(arg0) > 7 || atoi(arg1) > 7 || atoi(arg2) > 7) {
    			printf("%s\n", "Error: Invalid register");
    		}
    		if(atoi(arg0) < 0 || atoi(arg1) < 0 || atoi(arg2) < 0) {
    			printf("%s\n", "Error: Invalid register");
    		}
        	bitString |= (atoi(arg0) << 19);
        	bitString |= (atoi(arg1) << 16);
        	bitString |= atoi(arg2);
   		} 
   		else if (!strcmp(opcode, "nor")) {
   			if(atoi(arg0) > 7 || atoi(arg1) > 7 || atoi(arg2) > 7) {
    			printf("%s\n", "Error: Invalid register");
    		}
    		if(atoi(arg0) < 0 || atoi(arg1) < 0 || atoi(arg2) < 0) {
    			printf("%s\n", "Error: Invalid register");
    		}
        	bitString |= 0b001 << 22;
        	bitString |= atoi(arg0) << 19;
        	bitString |= atoi(arg1) << 16;
        	bitString |= atoi(arg2);
   		} 
   		else if (!strcmp(opcode, "lw")) {
   			if(atoi(arg0) > 7 || atoi(arg1) > 7) {
    			printf("%s\n", "Error: Invalid register");
    		}
    		if(atoi(arg0) < 0 || atoi(arg1) < 0) {
    			printf("%s\n", "Error: Invalid register");
    		}
        	bitString |= 0b010 << 22;
   		} 
   		else if (!strcmp(opcode, "sw")) {
   			if(atoi(arg0) > 7 || atoi(arg1) > 7) {
    			printf("%s\n", "Error: Invalid register");
    		}
    		if(atoi(arg0) < 0 || atoi(arg1) < 0) {
    			printf("%s\n", "Error: Invalid register");
    		}
        	bitString |= 0b011 << 22;
   		} 
   		else if (!strcmp(opcode, "beq")) {
   			if(atoi(arg0) > 7 || atoi(arg1) > 7) {
    			printf("%s\n", "Error: Invalid register");
    		}
    		if(atoi(arg0) < 0 || atoi(arg1) < 0) {
    			printf("%s\n", "Error: Invalid register");
    		}
        	bitString |= 0b100 << 22;
   		} 
   		else if (!strcmp(opcode, "jalr")) {
   			if(atoi(arg0) > 7 || atoi(arg1) > 7) {
    			printf("%s\n", "Error: Invalid register");
    		}
    		if(atoi(arg0) < 0 || atoi(arg1) < 0) {
    			printf("%s\n", "Error: Invalid register");
    		}
        	bitString |= 0b101 << 22;
        	bitString |= atoi(arg0) << 19;
        	bitString |= atoi(arg1) << 16;
   		} 
   		else if (!strcmp(opcode, "halt")) {
        	bitString |= 0b110 << 22;
   		} 
   		else if (!strcmp(opcode, "noop")) {
        	bitString |= 0b111 << 22;
   		}
   		else if (!strcmp(opcode, ".fill")) {
   			
   		}
   		else {
   			printf("%s\n", "Error: Undefined opcode");
   			exit(1);
   		}


   		//check beq arg2
   		if(!strcmp(opcode, "beq")) {
        	bitString |= atoi(arg0) << 19;
        	bitString |= atoi(arg1) << 16;

        	//TODO check range of twos complement
   			if(isNumber(arg2)) {
   				if(atoi(arg2) < 0) {
   					int mask = 65535;
   					int offset = atoi(arg2);
   					if(offset < -32768 || offset > 32767) {
   						printf("%s\n", "Error: offset out of bounds");
   						exit(1);
   					}
   					offset &= mask;
   					bitString |= offset;
   				}
   				else bitString |= atoi(arg2);
   			}
   			else {
   				//TODO find address of label
   				int neededAddress = -1;
   				for(int i = 0; labels[i].address != -1; ++i) {
   					if(strcmp(labels[i].innerLabel, arg2) == 0) {
   						neededAddress = labels[i].address;
   						break;
   					}
   				}
   				if(neededAddress == -1) {
   					printf("%s\n", "Error: undefined label");
   					exit(1);
   				}

  				int mask = 65535;
   				int offset = neededAddress - lineNum - 1;
   				if(offset < -32768 || offset > 32767) {
   					printf("%s\n", "Error: offset out of bounds");
   					exit(1);
   				}
   				offset &= mask;
   				bitString |= offset;
   			}
   		}


   		//finsih for lw and sw
   		if(!strcmp(opcode, "lw") || !strcmp(opcode, "sw")) {
        	bitString |= atoi(arg0) << 19;
        	bitString |= atoi(arg1) << 16;

        	//TODO check range of twos complement
   			if(isNumber(arg2)) {
   				if(atoi(arg2) < 0) {
   					int mask = 65535;
   					int offset = atoi(arg2);
   					if(offset < -32768 || offset > 32767) {
   						printf("%s\n", "Error: offset out of bounds");
   						exit(1);
   					}
   					offset &= mask;
   					bitString |= offset;
   				}
   				else bitString |= atoi(arg2);
   			}
   			else {
   				//TODO find address of label
   				int neededAddress = -1;
   				for(int i = 0; labels[i].address != -1; ++i) {
   					if(strcmp(labels[i].innerLabel, arg2) == 0) {
   						neededAddress = labels[i].address;
   						break;
   					}
   				}
   				if(neededAddress == -1) {
   					printf("%s\n", "Error: undefined label");
   					exit(1);
   				}

  				int mask = 65535;
   				int offset = neededAddress;
   				if(offset < -32768 || offset > 32767) {
   					printf("%s\n", "Error: offset out of bounds");
   					exit(1);
   				}
   				offset &= mask;
   				bitString |= offset;
   			}
   		}

   		if(!strcmp(opcode, ".fill")) {
   			if(isNumber(arg0)) {
   				bitString |= atoi(arg0);
   			}
   			else {
   				int neededAddress = -1;
   				for(int i = 0; labels[i].address != -1; ++i) {
   					if(strcmp(labels[i].innerLabel, arg0) == 0) {

   						bitString |= labels[i].address;
   						break;
   					}
   				}

   			}
   		}

   		fprintf(outFilePtr, "%d\n", bitString);
   		++lineNum;
    }

    return(0);
}


/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int
readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
        char *arg1, char *arg2)
{
    char line[MAXLINELENGTH];
    char *ptr = line;

    /* delete prior values */
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    /* read the line from the assembly-language file */
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
        /* reached end of file */
        return(0);
    }

    /* check for line too long (by looking for a \n) */
    if (strchr(line, '\n') == NULL) {
        /* line too long */
        printf("error: line too long\n");
        exit(1);
    }

    /* is there a label? */
    ptr = line;
    if (sscanf(ptr, "%[^\t\n\r ]", label)) {
        /* successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }

    /*
     * Parse the rest of the line.  Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
    sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
        opcode, arg0, arg1, arg2);
    return(1);
}

int isNumber(char *string) {
    /* return 1 if string is a number */
    int i;
    return( (sscanf(string, "%d", &i)) == 1);
}

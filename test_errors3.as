        sw      0       -1       five    load reg1 with 5 (symbolic address)
        sw      1       2       3       load reg2 with -1 (numeric address)
start   add     -1       2       7       decrement reg1
        jalr     0       -1              goto end of program when reg1==0
        beq     0       -2       done   go back to the beginning of the loop
        nor		1		2		-3
        lw		-1		0		0
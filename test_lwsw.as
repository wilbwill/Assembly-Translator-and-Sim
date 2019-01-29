        sw      0       1       five    load reg1 with 5 (symbolic address)
        sw      1       2       3       load reg2 with -1 (numeric address)
start   beq     1       2       start       decrement reg1
        jalr    0       1              goto end of program when reg1==0
        lw     	0       0       start   go back to the beginning of the loop
        sw		1		2		start
five	sw		1		6		five
six		lw		2		4		six
neg		beq		7		7		-2
		sw		0		3		-3

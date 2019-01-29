begin   sw      0       1       five    load reg1 with 5 (symbolic address)
        sw      1       2       begin       load reg2 with -1 (numeric address)
start   lw     	1       2       2       decrement reg1
        jalr    0       1              goto end of program when reg1==0
        beq     0       0       done   go back to the beginning of the loop
        nor		1		2		3
done    beq		4		6		end
		lw		0		0		0
		sw		0		0		0
end		beq		0		0		0

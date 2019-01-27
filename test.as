        sw      0       1       five    load reg1 with 5 (symbolic address)
        sw      1       2       3       load reg2 with -1 (numeric address)
start   add     1       2       1       decrement reg1
        jalr     0       1              goto end of program when reg1==0
        beq     0       0       done   go back to the beginning of the loop
        nor		1		2		3
done    halt                            end of program
five    .fill   5
neg1    .fill   -1
stAddr  .fill   start                   will contain the address of start (2)

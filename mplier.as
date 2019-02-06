		lw		0		2		mcand		Load mcand into register 2
		lw		0		3		mplier		Load mplier into register 3
		lw		0		0		mask		Load the mask into register 0
start	nor		3		3		5			nor mplier with itself to create nand gate
		nor		0		0		6			nor mask with itself to create nand gate
		nor		5		6		5			nand gate, gives us least sig bit of mplier
		lw		0		6		zero		reset register 6, for beq comp
		beq		5		6		shift		if nand gave 0, dont add mcand to answer
		add		2		1		1			else add answer register and mcand
shift	add		0		0		0			left shift mask one bit
		add		2		2		2			left shift mcand one bit
		add		5		7		7			update exit condition
		beq		3		7		done		if reg 7 now equals mplier, we are done
		beq		0		0		start		else go back to start		
done	halt								
mcand	.fill	32766
mplier	.fill	10383
mask	.fill	1
zero	.fill	0

save	lw			0		1		count
big		lw			0		2		dec
start	add			1		2		1
		sw			0		1		0
		sw			0		2		1
		lw			0		5		count
		beq			0		1		done
		beq			0		0		start
done	halt
dec		.fill		-1
count	.fill		3

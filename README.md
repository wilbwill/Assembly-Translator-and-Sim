# AssemblyTranslatorSimulator
Program that translates assembly code to machine code.
It has another program that can simulate machine code in a register system.

Above I have many files, most of which are simply tests, which are denoted by the files with the .as or .mc extension.

Translator.c has takes in an assembly code file and outputs a machine code file.
Simulatr.c will parse through the bits in the mc file to get the operation code and which registers it needs to use or change.

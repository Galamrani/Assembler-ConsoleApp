# Assembler-Console-App
Assembler as console app program written in C.  this is a project in programming lab class, at the university.
  
  
## About the program
The Assembler is a Software that converts an assembly language code to machine code, it takes basic Computer commands and converts them into Binary Code that Computer's Processor can use to perform its Basic Operations.  
  in this project, we were given a set of assembly instructions, our goal is to build an assembler and convert instructions to a machine code file, entry file, external file and error file if necessary. (the machina code file is in binary code represented as hexadecimal code)


## How the program works
the program is divided into two transitions each of them has different goals.  

#### the first transition  
1. check for errors in the input file and if necessary create an error file with info about the error.  
  if an error occurred the second transition will not occur.
2. build a symbol table, a symbol table is a data structure built like a table.  
  the main goal is to store info about labels in the input file, like addras, values, etc, this info will help us in the second transition.
      
      
#### the second transition
1. build the code segment and data segments using the symbol table and the input file commands.  
  the code segment is a data structure built like a table, is goal is to store the binary code for each command that is identified as a code command.  
      the data segment is a data structure built like a table is goal is to store the binary code for each command that identified as a data command.  
            
2. building the output files using the data segment, code segment, and symbol table.  
  there are 3 output files the first is ps.ob (machine code file) file that contains the binary code of the input file as hexadecimal code.  
    the second is ps.ent (entry file) file that contains all the entry labels and their address.  
      the third is ps.ext (external file)  file that contains all the external labels and their address.

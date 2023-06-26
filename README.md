# Assembler-Console-App
The "Assembler-Console-App" is a console application written in C that emulates the functionality of an assembler. Designed as a project for a programming lab class at the university, this application allows users to input low-level assembly language instructions and receive corresponding machine code as output. By mimicking the behavior of an assembler, this project aims to provide a hands-on learning experience for understanding the translation process from assembly language to machine code. The app's intuitive console interface makes it easy for students and enthusiasts to practice and explore the intricacies of assembly programming in a controlled environment. 
     <sub>
  for a better understanding of the program use this link : &emsp;[instructions for the project](https://github.com/Galamrani/Assembler-Console-App/blob/main/booklet.download.pdf)
 </sub> 
  
## About the program
The Assembler is a Software that converts an assembly language code to machine code, it takes basic Computer commands and converts them into Binary Code that Computer's Processor can use to perform its Basic Operations.  
  in this project, we were given a set of assembly instructions, our goal is to build an assembler that creates files from ps.as file (input file)  
    to ps.ob (machine code ), ps.ent (entry file), ps.ext (external file) and error file if necessary. 


## How the program works
the program is divided into two transitions each of them taking care of different parts of the program workflow

#### the first transition  
1. check for errors in the input file and if necessary create an error file with info about the error.  
  if an error occurred the second transition will not occur.
2. build a symbol table, the symbol table main goal is to store info about labels in the input file, like addras, values, etc, this info will help us in the second transition.
      
      
#### the second transition
1. build the code segment and data segments using the symbol table and the input file commands.  
  the code segment goal is to store the binary code for each command that is identified as a code command.  
      the data segment goal is to store the binary code for each command that identified as a data command.  
            
2. building the output files using the data segment, code segment, and symbol table.  
  there are 3 output files the first is ps.ob (machine code file) file that contains the binary code of the input file as hexadecimal code.  
    the second is ps.ent (entry file) file that contains all the entry labels and their address.  
      the third is ps.ext (external file)  file that contains all the external labels and their address.

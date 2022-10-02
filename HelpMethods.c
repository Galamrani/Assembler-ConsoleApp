#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include "Prog.h"


/* 
	Methods and Tables for the First Transition and the Second Transicion 
*/



const char* CodeOrData[NUMBER_OF_ATTRIBUTES] =
{ "External","Data","Code" };


const char* ErrorTable[NUMBER_OF_ERRORS] =
{ "unknown command", "parameters out of range", "label already exist", "too many parentheses",
  "missing parentheses", "jmp or call parameter is not label", "stop instruction not valid" };


const char* AllInstructionCommands[NUMBER_OF_INSTRUCTION_COMMANDES] =
{ "add","sub","and","or","nor","move","mvhi","mvlo","addi","subi","andi","ori","nori","bne",
"beq","blt","bgt","lb","sb","lw","sw","lh","sh","jmp","la","call","stop" };


/* main commands */
const char* allRCommands[NUMBER_OF_R_COMMANDES] = { "add","sub","and","or","nor","move","mvhi","mvlo" };
const char* allICommands[NUMBER_OF_I_COMMANDES] = { "addi","subi","andi","ori","nori","bne","beq","blt","bgt","lb","sb","lw","sw","lh","sh" };
const char* allJCommands[NUMBER_OF_J_COMMANDES] = { "jmp","la","call","stop" };


/* sub commands */
const char* R_Commands_arithmetic[] = { "add","sub","and","or","nor" };
const char* R_Commands_copying[] = { "move","mvhi","mvlo" };
const char* I_Commands_arithmetic[] = { "addi","subi","andi","ori","nori" };
const char* I_Commands_branching[] = { "bne","beq","blt","bgt" };
const char* I_Commands_memory[] = { "lb","sb","lw","sw","lh","sh" };


/* Guide Commands */
const char* AllGuideCommands[NUMBER_OF_GUIDE_COMMANDES] =
{ ".dh",".dw",".db",".asciz",".entry",".extern" };



void SkeepSpace(char* line, int* index)
{
	while (isspace(line[*index]))
	{
		*index = *index + 1;
	}
}

void GetLabel(char* line, int* index)
{
	char nameLabel[MAX_LABEL_LENGTH];
	char* p = line + (*index);
	int i = 0;

	while (*p != ':')
	{
		nameLabel[i] = *p;
		p++;
		i++;
		(*index)++;
	}
	nameLabel[i] = '\0';
	(*index)++;

	AddToSymbolTable(nameLabel);

}

int GetParameterA(char* line, int* index)
{
	char Parameter[MAX_PARAMETER_LENGTH];
	char* p = line + (*index);
	int i = 0;


	while (!isspace(*p) && *p != ',' && *p != '\n')
	{
		Parameter[i] = *p;
		p++;
		i++;

		if (*p == '\n')
		{
			return;
		}

		(*index)++;
	}

	Parameter[i] = '\0';

	return atoi(Parameter);

}

int GetParameterB(char* line, int* index)
{
	char Parameter[MAX_NUMBER_LENGTH];
	char* p;
	int i = 0;

	SkeepSpace(line, index);
	p = line + (*index);

	if (*p == ',')
	{
		p++;
		*(index) = *(index)+1;
	}

	while (!isspace(*p) && *p != ',' && *p != '\n')
	{
		if (*p == '$')
		{
			p++;
		}
		else
		{
			Parameter[i] = *p;
			p++;
			i++;
		}

		*(index) = *(index)+1;
	}

	Parameter[i] = '\0';

	return atoi(Parameter);

}

char* GetWord(char* line, int* index)
{
	char Word[MAX_WORD_LENGTH];
	char* p;
	int i = 0;

	SkeepSpace(line, index);
	p = line + (*index);

	if (*p == ',')
	{
		p++;
		(*index)++;
	}

	SkeepSpace(line, index);
	p = line + (*index);

	while (!isspace(*p) && *p != '\n')
	{
		Word[i] = *p;
		p++;
		i++;
		(*index)++;
	}
	Word[i] = '\0';

	return Word;

}

int CountParameters(char* line, int* index)
{
	SkeepSpace(line, index);

	char* p = line + (*index);
	int counter = 0;



	if (*p == '\"')
	{
		p++;
		while (*p != '\"')
		{
			counter++;
			p++;
		}
		return counter + 1;
	}

	while (!isspace(*p) && *p != '\n')
	{
		GetParameterA(line, index);
		(*index)++;
		counter++;
		p = line + (*index);
	}

	return counter;

}



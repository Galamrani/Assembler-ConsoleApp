#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include "Prog.h"



/*
	main goals of the file:

	1.	go line by line in the input file and find all the syntax errors
		and if necessary create an error file with the errors names and lines

	2.	build a Symbol table and fill it , as much as possible 

*/



/* Global variables */

struct Label *SymbolTable[MAX_SYMBOL_TABLE_SIZE];
int IC = 100;
int DC = 0;
int ErrorFileCreated = 0;



/* First Transition Main Function */

void FirstTransition(FILE* fp)
{
	char line[MAX_LINE_lENGTH];
	int LineCount = 1;

	while (fgets(line, MAX_LINE_lENGTH, fp) != NULL)
	{

		int index = 0;

		SkeepSpace(line, &index);
		TryCatchLineError(line, index, &LineCount);

		if (FirstWordIsLabel(line))
		{
			GetLabel(line, &index);
			AnalyzeCommandAfterLabel(line, &index);
		}
		else if (FirstWordIsExternal(line))
		{
			GetExternalLabel(line, &index);
		}
		else if (FirstWordIsGuideCommand(line))
		{
			GetGuideCommand(line, &index);
		}
		else if (FirstWordIsInstruction(line))
		{
			UpDateIC();
		}

	}

	UpdateSymbolTable();
}



/* Bool Functions */

int FirstWordIsGuideCommand(char* line)
{
	int index = 0;

	SkeepSpace(line, &index);

	char *Word = GetWord(line, &index);

	for (int i = 0; i < NUMBER_OF_GUIDE_COMMANDES - 2; i++)
	{
		if (!strcmp(AllGuideCommands[i], Word))
		{
			return 1;
		}
	}

	return 0;
}


int FirstWordIsInstruction(char* line)
{
	int index = 0;

	SkeepSpace(line, &index);

	char* Word = GetWord(line, &index);

	for (int i = 0; i < NUMBER_OF_INSTRUCTION_COMMANDES; i++)
	{
		if (!strcmp(AllInstructionCommands[i], Word))
		{
			return 1;
		}
	}

	return 0;
}


int FirstWordIsExternal(char* line)
{
	int index = 0;

	SkeepSpace(line, &index);

	char* word = GetWord(line, &index);
	
	if (!strcmp(word, AllGuideCommands[5])) 
	{ 
		return 1; 
	}

	return 0;
}


int FirstWordIsEntry(char* line)
{
	int index = 0;

	SkeepSpace(line, &index);

	char* word = GetWord(line, &index);

	if (!strcmp(word, AllGuideCommands[4]))
	{
		return 1;
	}

	return 0;
}


int FirstWordIsLabel(char* line)
{
	char* p = line ;
	while ((*p != ':') && !(isspace(*p)))
	{
		p = p + 1;
	}

	if (*p == ':') { return 1; }
	else { return 0; }
}


int IsCommentLine(char* line)
{
	int i = 0;
	while (isspace(line[i]))
	{
		i++;
	}

	if (line[i] == ';')
	{
		return 1;
	}

	return 0;
}


int IsEmptyLine(char* line)
{
	int i = 0;
	while (line[i] != '\0')
	{
		if (!isspace(line[i]) || line[i] != '\n')
		{
			return 0;
		}
		i++;
	}

	return 1;

}



/* Auxiliary functions */

void UpdateSymbolTable()
{

	int isDate;
	int i = 0;

	while (SymbolTable[i] != NULL)
	{
		isDate = SymbolTable[i]->attribute;
		if (isDate == 2)
		{
			SymbolTable[i]->value = SymbolTable[i]->value + IC;
		}
		i++;
	}

}


void ChangeLastLabelValue(int isInstruction)
{
	int i = 0;

	while (SymbolTable[i] != NULL)
	{
		i++;
	}

	if (i != 0)
	{
		i = i - 1;
	}


	if (isInstruction)
	{
		SymbolTable[i]->value = IC;
		SymbolTable[i]->attribute = 1;

	}
	else
	{
		SymbolTable[i]->value = DC;
		SymbolTable[i]->attribute = 2;

	}

}


void AnalyzeCommandAfterLabel(char* line, int* index)
{
	int numberOfParmeters = 0;
	char* Word;

	SkeepSpace(line, index);
	Word = GetWord(line, index);

	for (int i = 0; i < NUMBER_OF_GUIDE_COMMANDES - 2; i++)
	{
		if (!strcmp(AllGuideCommands[i], Word))
		{
			// Guide command  -> change DC
			ChangeLastLabelValue(0);
			numberOfParmeters = CountParameters(line, index);
			UpDateDC(AllGuideCommands[i], numberOfParmeters);
			return;
		}
	}

	// Instrction command -> change IC
	ChangeLastLabelValue(1);
	UpDateIC();

}


void GetGuideCommand(char* line, int* index)
{
	int numberOfParmeters = 0;
	char* Word;

	SkeepSpace(line, index);
	Word = GetWord(line, index);

	for (int i = 0; i < NUMBER_OF_GUIDE_COMMANDES - 2; i++)
	{
		if (!strcmp(AllGuideCommands[i], Word))
		{
			numberOfParmeters = CountParameters(line, index);
			UpDateDC(AllGuideCommands[i], numberOfParmeters);
			return;
		}
	}
}


void AddToSymbolTable(char* labelName)
{
	int i = 0;

	while (SymbolTable[i] != NULL)
	{
		i++;
	}

	struct Label *p = (struct Label *)calloc(1, sizeof(struct Label));
	if (p == NULL) 
	{ 
		return; 
	}

	strcpy(&p->name, labelName);
	p->attribute = 0;
	p->entry = 0;
	p->value = 0;
	
	SymbolTable[i] = p;

}


void GetExternalLabel(char* line, int* index)
{
	char* p;
	char Label[MAX_LABEL_LENGTH];
	int i = 0;

	SkeepSpace(line, index);
	p = line + (*index);

	while (!isspace(*p))
	{
		p++;
		(*index)++;
	}

	SkeepSpace(line, index);
	p = line + (*index);

	while (!isspace(*p) && *p != '\n')
	{
		Label[i++] = *p;
		p++;
		(*index)++;
	}

	Label[i] = '\0';

	AddToSymbolTable(Label);

}


void UpDateIC()
{
	IC = IC + 4;
}


void UpDateDC(char* word, int numberOfParmeters)
{

	int byte = 0;
	int isAsciz = 0;

	if (!strcmp(word, AllGuideCommands[0]))
	{
		byte = 2;
	}
	if (!strcmp(word, AllGuideCommands[1]))
	{
		byte = 4;
	}
	if (!strcmp(word, AllGuideCommands[2]))
	{
		byte = 1;
	}
	if (!strcmp(word, AllGuideCommands[3]))
	{
		byte = 1;
		isAsciz = 1;
	}

	DC = DC + byte * numberOfParmeters;
}



/* Error handler functions */

void TryCatchLineError(char* line, int index, int* lineCount)
{

	if (FirstWordIsLabel(line))
	{
		TryCatchLabelExistError(line, lineCount);
		TryCatchAfterLabelError(line, lineCount);
	}
	else if (FirstWordIsExternal(line))
	{
		TryCatchExternLabelError(line, lineCount);
	}
	else if (FirstWordIsGuideCommand(line))
	{
		TryCatchGuideCommandError(line, &index, lineCount);
	}
	else if (FirstWordIsInstruction(line))
	{
		TryCatchInstructionError(line, &index, lineCount);
	}
	else if (IsCommentLine(line) || IsEmptyLine(line) || FirstWordIsEntry(line))
	{
		;
	}
	else
	{
		PrintToErrorFile(*lineCount, 0);
	}

	(*lineCount) = (*lineCount) + 1;
}


void PrintToErrorFile(int lineNumber, int errorNumber)
{
	ErrorFileCreated = 1;

	FILE* fp = fopen("ErrorFile.text", "a");

	fprintf(fp, "line: %d \t Error: %s \n", lineNumber, ErrorTable[errorNumber]);

	fclose(fp);
}


void TryCatchLabelExistError(char* line, int* lineCount)
{

	int i = 0;
	int j = 0;
	char nameLabel[MAX_LABEL_LENGTH];

	while (*(line + i) != ':')
	{
		nameLabel[i] = *(line + i);
		i++;
	}
	nameLabel[i] = '\0';

	while (SymbolTable[j] != NULL)
	{
		if (!strcmp(SymbolTable[j]->name, nameLabel))
		{
			PrintToErrorFile(*lineCount, 2);
		}
		j++;
	}

}


void TryCatchAfterLabelError(char* line, int* lineCount)
{
	int i = 0;

	while (*(line + i) != ':')
	{
		i++;
	}
	i++;

	SkeepSpace(line, &i);

	if (FirstWordIsInstruction(line + i))
	{
		TryCatchInstructionError(line, &i, lineCount);
	}
	else if (FirstWordIsGuideCommand(line + i))
	{
		TryCatchGuideCommandError(line, &i, lineCount);
	}
	else
	{
		PrintToErrorFile(*lineCount, 0);
	}
}  


void TryCatchExternLabelError(char* line, int* lineCount)
{
	int i = 0;
	int j = 0;
	char nameLabel[MAX_LABEL_LENGTH];

	while (!isspace(*(line + i)))
	{
		i++;
	}

	SkeepSpace(line + i, &i);

	while (isspace(*(line + i)) || *(line + i) == '\n' || *(line + i) == '\0')
	{
		nameLabel[i] = *(line + i);
		i++;
	}
	nameLabel[i] = '\0';

	while (SymbolTable[j] != NULL)
	{
		if (!strcmp(SymbolTable[j]->name, nameLabel))
		{
			PrintToErrorFile(*lineCount, 2);
		}
		j++;
	}
}


void TryCatchInstructionError(char* line, int* index, int* lineCount)
{
	int i;
	int j = *index;
	char* word = GetWord(line, &j);

	for (i = 0; i < NUMBER_OF_R_COMMANDES; i++)
	{
		if (!strcmp(word, allRCommands[i]))
		{
			TryCatchRInstructionError(line, index, lineCount);
			return;
		}
	}
	for (i = 0; i < NUMBER_OF_J_COMMANDES; i++)
	{
		if (!strcmp(word, allJCommands[i]))
		{
			TryCatchJInstructionError(line, index, lineCount, word);
			return;
		}
	}
	for (i = 0; i < NUMBER_OF_I_COMMANDES; i++)
	{
		if (!strcmp(word, allICommands[i]))
		{
			TryCatchIInstructionError(line, index, lineCount);
			return;
		}
	}
}


void TryCatchGuideCommandError(char* line, int* index, int* lineCount)
{
	char* word = GetWord(line, index);
	int i, state = -1;
	int j = *index;
	for (i = 0; i < NUMBER_OF_GUIDE_COMMANDES - 3; i++)
	{
		if (!strcmp(word, AllGuideCommands[i]))
		{
			// parmeter in range
			return;
		}
	}

	SkeepSpace(line, &j);

	while ((*(line + j) != '\0') && (*(line + j) != '\n'))
	{
		if (*(line + j) == '\"' && state == -1) 
		{ 
			state = 0;
		}
		else if (*(line + j) == '\"' && state == 0)
		{
			state = 1;
		}
		else if (*(line + j) == '\"' && state == 1)
		{
			PrintToErrorFile(*lineCount, 3);
		}

		j++;
	}

	if (state != 1)
	{
		PrintToErrorFile(*lineCount, 4);
	}
}


void TryCatchRInstructionError(char* line, int* index, int* lineCount)
{
	int parameter;
	int i = *index;
	while ((* (line + i) != '\0') && (*(line + i) != '\n'))
	{
		parameter = GetParameterB(line, &i);
		if (parameter < 0 || parameter > 32)
		{
			PrintToErrorFile(*lineCount, 1);
		}
	}
}


void TryCatchIInstructionError(char* line, int* index, int* lineCount)
{
	int parameter;
	GetWord(line, index);

	for (int i = 0; i < 3; i++)
	{
		parameter = GetParameterB(line, index);
		if ((i != 1) && (parameter < 0 || parameter > 32))
		{
			PrintToErrorFile(*lineCount, 1);
		}
	}

	return;
}


void TryCatchJInstructionError(char* line, int* index, int* lineCount, char* word)
{
	int parmeter;

	SkeepSpace(line, index);
	while (!isspace(*(line + *index)))
	{
		*index = *index + 1;
	}
	SkeepSpace(line, index);

	if (!strcmp(word, allJCommands[0])) // jmp
	{
		if (isalpha(*(line + *index)))
		{
			parmeter = GetParameterB(line, index);
			if (parmeter < 0 || parmeter > 32)
			{
				PrintToErrorFile(*lineCount, 1);
			}
		}

	}
	else if (!strcmp(word, allJCommands[3])) // stop
	{
		if (!( *(line + *index) == '\0' || *(line + *index) == '\n'))
		{
			PrintToErrorFile(*lineCount, 6);
		}
	}
	else
	{
		if (!isalpha(*(line + *index))) // la, call
		{
			PrintToErrorFile(*lineCount, 5);
		}
	}
	
	return;
}







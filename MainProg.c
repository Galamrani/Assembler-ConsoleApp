#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include "Prog.h"



int main()
{

	FILE* fp;
	ClearOutputFiles();
	fp = fopen("ps.as", "r");
	TryCatchFileNotFound(fp);

	FirstTransition(fp);


	if (!ErrorFileCreated)
	{
		fseek(fp, 0, SEEK_SET);
		SecondTransition(fp);
		CreateOutputFiles();
	}

	FreeMemory();
	fclose(fp);

	return 0;
}


void TryCatchFileNotFound(FILE* fp)
{
	if (fp == NULL)
	{
		printf("File Not Found Error");
		exit(1);
	}
}


void FreeMemory()
{
	int i = 0;

	for (i = 0; i < 26; i++)
	{
		free(InstructionTable[i]);
	}
	i = 0;
	while (SymbolTable[i] != NULL)
	{
		free(SymbolTable[i]);

		i++;
	}
	i = 0;
	while (DataSegment[i] != NULL)
	{
		free(DataSegment[i]);

		i++;
	}
	i = 0;
	while (CodeSegment[i] != NULL)
	{
		free(CodeSegment[i]);

		i++;
	}

}


void ClearOutputFiles()
{
	FILE* fp;

	fp = fopen("ps.ob", "w");
	TryCatchFileNotFound(fp);
	fclose(fp);

	fp = fopen("ps.ent", "w");
	TryCatchFileNotFound(fp);
	fclose(fp);

	fp = fopen("ps.ext", "w");
	TryCatchFileNotFound(fp);
	fclose(fp);

	fp = fopen("ErrorFile.text", "w");
	TryCatchFileNotFound(fp);
	fclose(fp);
}






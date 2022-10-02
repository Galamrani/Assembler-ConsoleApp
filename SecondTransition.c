#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include "Math.h"
#include "Prog.h"


/*
	main goals of the file:

	1.	go line by line in the input file and finish to fill the Symbol table 
		
	2.	go line by line in the input file and build the Code segment and Data segment

	3. build the output files using the  Code segment , Data segment , Symbol table
		
*/



/* Global variables */

int ICF = 100;
int IDF = 0;
struct Instruction* InstructionTable[MAX_INSTRUCTION_TABLE_SIZE];
struct Data* DataSegment[MAX_DATA_SEGMENT];
struct Code* CodeSegment[MAX_CODE_SEGMENT];



/* Second Transition Main Function */

void SecondTransition(FILE* fp)
{

	char line[MAX_LINE_lENGTH];

	InitializeInstructionTable();

	while (fgets(line, MAX_LINE_lENGTH, fp) != NULL)
	{

		int index = 0;

		if (FirstWordIsLabel(line))
		{
			SkeepLabel(line, &index);
		}
		else if (FirstWordIsEntry(line))
		{
			ChangeEntryLabel(line, &index);
			continue;
		}
		else if (FirstWordIsExternal(line) || IsCommentLine(line) || IsEmptyLine(line))
		{
			continue;
		}

		SkeepSpace(line, &index);

		if (FirstWordIsInstruction(line + index))
		{
			AnalyseInstruction(line, &index);
			UpDateICF();
		}
		else if (FirstWordIsGuideCommand(line + index))
		{
			AnalyseGuideCommand(line, &index);
		}

	}

}



/* Guide command Functions */

void AnalyseGuideCommand(char* line, int* index)
{

	int i = 0;
	char* command = GetWord(line, index);

	if (!strcmp(command, AllGuideCommands[0]))
	{
		//.dh
		AnalyseGuideCommandBH(line, index);
	}
	else if (!strcmp(command, AllGuideCommands[1]))
	{
		//.dw
		AnalyseGuideCommandBW(line, index);
	}
	else if (!strcmp(command, AllGuideCommands[2]))
	{
		//.db
		AnalyseGuideCommandBD(line, index);
	}
	else if (!strcmp(command, AllGuideCommands[3]))
	{
		//.asciz
		AnalyseGuideCommandASCIZ(line, index);
	}

}


void AnalyseGuideCommandBH(char* line, int* index)
{
	int parameter;

	while (*(line + *index) != '\0' && *(line + *index) != '\n')
	{
		SkeepSpace(line, index);

		parameter = GetParameterB(line, index);

		AddToDataSegment(parameter, 16);

		IDF = IDF + 2;

		(*index) = (*index) + 1;
	}
}


void AnalyseGuideCommandBD(char* line, int* index)
{
	int parameter;

	while (*(line + *index) != '\0' && *(line + *index) != '\n')
	{
		SkeepSpace(line, index);

		parameter = GetParameterB(line, index);

		AddToDataSegment(parameter, 8);

		IDF = IDF + 1;

		(*index) = (*index) + 1;
	}
}


void AnalyseGuideCommandBW(char* line, int* index)
{
	int parameter;

	while (*(line + *index) != '\0' && *(line + *index) != '\n')
	{
		SkeepSpace(line, index);

		parameter = GetParameterB(line, index);

		AddToDataSegment(parameter, 32);

		IDF = IDF + 4;

		(*index) = (*index) + 1;
	}
}


void AnalyseGuideCommandASCIZ(char* line, int* index)
{
	char Letter;
	int aascii;

	while (*(line + *index) != '\0' && *(line + *index) != '\"' && *(line + *index) != '\n')
	{
		Letter = GetChar(line, index);

		AddToDataSegment(Letter, 8);

		(*index) = (*index) + 1;

		IDF = IDF + 1;

		SkeepSpace(line, index);
	}

	AddToDataSegment(0, 8);

	IDF = IDF + 1;
}


void AddToDataSegment(int parameter, int bitsNumber)
{
	int i = 0;
	char* binaryArray = ConvertToBinary(parameter, bitsNumber);

	while (DataSegment[i] != NULL)
	{
		i++;
	}

	DataSegment[i] = (struct Code*)calloc(1, sizeof(struct Code));
	DataSegment[i]->command_in_binary = binaryArray;

}



/* R command Functions */

void AnalyseRCommand(char* command, char* line, int* index)
{
	for (int i = 0; i < 5; i++)
	{
		if (!strcmp(command, R_Commands_arithmetic[i]))
		{
			ArithmeticRCommand(command, line, index);
			return;
		}
	}

	CopyingRCommand(command, line, index);
}


void ArithmeticRCommand(char* command, char* line, int* index)
{

	struct Instruction* p = InstructionTableLookUp(command);
	int rs = GetParameterB(line, index);
	int rt = GetParameterB(line, index);
	int rd = GetParameterB(line, index);
	int opCode = p->opCode;
	int funct = p->funct;

	AddRToCodeSegment(opCode, rs, rt, rd, funct);

}


void CopyingRCommand(char* command, char* line, int* index)
{
	struct Instruction* p = InstructionTableLookUp(command);
	int rt = GetParameterB(line, index);
	int rs = GetParameterB(line, index);
	int rd = 0;
	int opCode = p->opCode;
	int funct = p->funct;

	AddRToCodeSegment(opCode, rs, rt, rd, funct);
}


void AddRToCodeSegment(int _opCode, int _rs, int _rt, int _rd, int _funct)
{
	int i = 0;

	char* opCode = ConvertToBinary(_opCode, 6);
	char* rs = ConvertToBinary(_rs, 5);
	char* rt = ConvertToBinary(_rd, 5);
	char* rd = ConvertToBinary(_rt, 5);
	char* funct = ConvertToBinary(_funct, 5);

	char* binaryArry = GetRBinaryArry(opCode, rs, rt, rd, funct);

	struct Code* p = (struct Code*)calloc(1, sizeof(struct Code));
	if (p == NULL)
	{
		return;
	}

	p->command_in_binary = binaryArry;
	p->address = ICF;

	while (CodeSegment[i] != NULL)
	{
		i++;
	}

	CodeSegment[i] = p;

}


char* GetRBinaryArry(char* opCode, char* rs, char* rt, char* rd, char* funct)
{
	int i;
	int j = 0;

	char* binaryArry = (char*)malloc(sizeof(char) * (33));

	for (j = 0, i = 0; i < 6; i++)
	{
		binaryArry[i] = *(opCode + j++);
	}
	for (j = 0, i = 6; i < 11; i++)
	{
		binaryArry[i] = *(rs + j++);
	}
	for (j = 0, i = 11; i < 16; i++)
	{
		binaryArry[i] = *(rt + j++);
	}
	for (j = 0, i = 16; i < 21; i++)
	{
		binaryArry[i] = *(rd + j++);
	}
	for (j = 0, i = 21; i < 26; i++)
	{
		binaryArry[i] = *(funct + j++);
	}
	for (j = 0, i = 26; i < 32; i++)
	{
		binaryArry[i] = '0';
	}

	binaryArry[32] = '\0';

	return binaryArry;
}



/* I command Functions */

void AnalyseICommand(char* command, char* line, int* index)
{
	int i;
	for (i = 0; i < 5; i++)
	{
		if (!strcmp(command, I_Commands_arithmetic[i]))
		{
			ArithmeticICommand(command, line, index);
			return;
		}
	}
	for (i = 0; i < 4; i++)
	{
		if (!strcmp(command, I_Commands_branching[i]))
		{
			BranchingICommand(command, line, index);
			return;
		}
	}

	CopyingRCommand(command, line, index);
}


void ArithmeticICommand(char* command, char* line, int* index)
{
	struct Instruction* p = InstructionTableLookUp(command);
	int rs = GetParameterB(line, index);
	int immed = GetParameterB(line, index);
	int rt = GetParameterB(line, index);
	int opCode = p->opCode;

	AddIToCodeSegment(opCode, rs, rt, immed);
}


void BranchingICommand(char* command, char* line, int* index)
{
	int rs = GetParameterB(line, index);
	int rt = GetParameterB(line, index);
	char* labelName = GetWord(line, index);

	struct Instruction* p = InstructionTableLookUp(command);
	struct Label* label = SymbolTableLookUp(labelName);

	int immed = (label->value) - ICF;
	int opCode = p->opCode;

	AddIToCodeSegment(opCode, rs, rt, immed);
}


void MemoryICommand(char* command, char* line, int* index)
{
	struct Instruction* p = InstructionTableLookUp(command);
	int rs = GetParameterB(line, index);
	int immed = GetParameterB(line, index);
	int rt = GetParameterB(line, index);
	int opCode = p->opCode;

	AddIToCodeSegment(opCode, rs, rt, immed);
}


void AddIToCodeSegment(int _opCode, int _rs, int _rt, int _immed)
{
	int i = 0;

	char* opCode = ConvertToBinary(_opCode, 6);
	char* rs = ConvertToBinary(_rs, 5);
	char* rt = ConvertToBinary(_immed, 16);
	char* rd = ConvertToBinary(_rt, 5);


	char* binaryArry = GetIBinaryArry(opCode, rs, rt, rd);

	struct Code* p = (struct Code*)calloc(1, sizeof(struct Code));
	if (p == NULL)
	{
		return;
	}

	p->command_in_binary = binaryArry;
	p->address = ICF;

	while (CodeSegment[i] != NULL)
	{
		i++;
	}

	CodeSegment[i] = p;

}


char* GetIBinaryArry(char* opCode, char* rs, char* rt, char* immed)
{
	int i;
	int j = 0;

	char* binaryArry = (char*)malloc(sizeof(char) * (33));

	for (j = 0, i = 0; i < 6; i++)
	{
		binaryArry[i] = *(opCode + j++);
	}
	for (j = 0, i = 6; i < 11; i++)
	{
		binaryArry[i] = *(rs + j++);
	}
	for (j = 0, i = 11; i < 16; i++)
	{
		binaryArry[i] = *(rt + j++);
	}
	for (j = 0, i = 16; i < 32; i++)
	{
		binaryArry[i] = *(immed + j++);
	}
	
	binaryArry[32] = '\0';

	return binaryArry;
}



/* J command Functions */

void AnalyseJCommand(char* command, char* line, int* index)
{
	for (int i = 0; i < 3; i++)
	{
		if (!strcmp(command, allJCommands[i]))
		{
			JCommand(command, line, index);
			return;
		}
	}

	StopJCommand(command, line, index);

}


void JCommand(char* command, char* line, int* index)
{
	int address;
	int reg;
	char* labelName;
	struct Label* label;

	struct Instruction* p = InstructionTableLookUp(command);

	SkeepSpace(line, index);

	if (line[*index] == '$')
	{
		 address = GetParameterB(line, index);
		 reg = 1;
	}
	else 
	{
		labelName = GetWord(line, index);
		label = SymbolTableLookUp(labelName);
		address = label->value;
		reg = 0;
		if (!label->attribute)
		{
			AppendToEXTfile(label->name, ICF);
		}
	}

	int opCode = p->opCode;

	AddJToCodeSegment(opCode, reg, address);
}


void StopJCommand(char* command, char* line, int* index)
{
	struct Instruction* p = InstructionTableLookUp(command);
	int reg = 0;
	int address = 0;
	int opCode = p->opCode;

	AddJToCodeSegment(opCode, reg, address);
}


void AddJToCodeSegment(int _opCode, int _reg, int _address)
{
	int i = 0;

	char* opCode = ConvertToBinary(_opCode, 6);
	char* reg = ConvertToBinary(_reg, 1);
	char* address = ConvertToBinary(_address, 25);


	char* binaryArry = GetJBinaryArry(opCode, reg, address);

	struct Code* p = (struct Code*)calloc(1, sizeof(struct Code));
	if (p == NULL)
	{
		return;
	}

	p->command_in_binary = binaryArry;
	p->address = ICF;

	while (CodeSegment[i] != NULL)
	{
		i++;
	}

	CodeSegment[i] = p;

}


char* GetJBinaryArry(char* opCode, char* reg, char* address)
{
	int i;
	int j = 0;

	char* binaryArry = (char*)malloc(sizeof(char) * (33));

	for (j = 0, i = 0; i < 6; i++)
	{
		binaryArry[i] = *(opCode + j++);
	}
	for (j = 0, i = 6; i < 7; i++)
	{
		binaryArry[i] = *(reg + j++);
	}
	for (j = 0, i = 7; i < 32; i++)
	{
		binaryArry[i] = *(address + j++);
	}

	binaryArry[32] = '\0';

	return binaryArry;
}



/* Output functions */

void CreateOutputFiles()
{
	CreateOBfile();

	CreateENTfile();
}


void CreateOBfile()
{
	FILE* fp;
	fp = fopen("ps.ob", "a");
	TryCatchFileNotFound(fp);

	fprintf(fp, "\t\t%d\t%d\t\t \n", ICF - 100, IDF);

	AppendCodeSegment(fp);

	AppendDataSegment(fp);




	fclose(fp);
}


void CreateENTfile()
{
	int i = 0;
	FILE* fp;
	fp = fopen("ps.ent", "a");
	TryCatchFileNotFound(fp);

	while (SymbolTable[i] != NULL)
	{
		if (SymbolTable[i]->entry)
		{
			fprintf(fp,"%s \t %d \n", SymbolTable[i]->name, SymbolTable[i]->value);
		}

		i++;
	}

	fclose(fp);
}


void AppendToEXTfile(char* labelName, int address)
{
	int i = 0;
	FILE* fp;
	fp = fopen("ps.ext", "a");
	TryCatchFileNotFound(fp);


	fprintf(fp, "%s \t %d \n", labelName, address);


	fclose(fp);
}


void AppendCodeSegment(FILE* fp)
{
	int i = 0;
	int j, k;
	int firstHex, secondHex, thirdHex, fourthHex;
	char firstByte[8], secondByte[8], thirdByte[8], fourthByte[8];
	char* binaryArray;

	while (CodeSegment[i] != NULL)
	{
		binaryArray = CodeSegment[i]->command_in_binary;

		for (k = 0, j = 0; j < 8; j++)
		{
			firstByte[k++] = *(binaryArray + j);
		}
		for (k = 0, j = 8; j < 16; j++)
		{
			secondByte[k++] = *(binaryArray + j);
		}
		for (k = 0, j = 16; j < 24; j++)
		{
			thirdByte[k++] = *(binaryArray + j);
		}
		for (k = 0, j = 24; j < 32; j++)
		{
			fourthByte[k++] = *(binaryArray + j);
		}

		firstHex = ConvertToDec(firstByte);
		secondHex = ConvertToDec(secondByte);
		thirdHex = ConvertToDec(thirdByte);
		fourthHex = ConvertToDec(fourthByte);

		fprintf(fp, "%d:\t%02X\t%02X\t%02X\t%02X\n"
			,CodeSegment[i]->address, fourthHex, thirdHex, secondHex, firstHex);

		i++;
	}

}


void AppendDataSegment(FILE* fp)
{
	char* dataBinaryArray = GetDataBinaryArray();
	int arraySize = GetArraySize(dataBinaryArray);
	char byteArray[9];
	int hexNumber;
	int i = 0, j = 0;
	int address;

	while (CodeSegment[i] != NULL)
	{
		i++;
	}

	address = ((CodeSegment[i - 1]->address)) + 4;

	for (i = 0; i < arraySize; i++)
	{

		byteArray[j++] = dataBinaryArray[i];

		if (i % 32 == 0)
		{
			if (i == 0)
			{
				fprintf(fp, "%d:\t", address);
			}
			else
			{
				fprintf(fp, "\n%d:\t", address);
			}

			address = address + 4;
		}

		if (i % 8 == 0 && i != 0)
		{
			byteArray[8] = '\0';

			hexNumber = ConvertToDec(byteArray);

			fprintf(fp, "%02X\t", hexNumber);

			j = 0;

			ResetArray(byteArray);

		}
	
	}

	byteArray[j] = '\0';

}



/* Auxiliary functions */

void ChangeEntryLabel(char* line, int* index)
{
	char* labelName;
	struct Label* label;
	char* p = line + (*index);

	while (!isspace(*p))
	{
		p++;
		(*index)++;
	}
	(*index)++;

	SkeepSpace(line, index);

	labelName = GetWord(line, index);

	label = SymbolTableLookUp(labelName);

	label->entry = 1;
}


struct Instruction* InitializeComponent(char* _name, char* _type, int _funct, int _opCode)
{

	struct Instruction* p = (struct Instruction*)calloc(1, sizeof(struct Instruction));

	if (p == NULL)
	{
		return NULL;
	}

	strcpy(&p->name, _name);
	strcpy(&p->type, _type);
	p->funct = _funct;
	p->opCode = _opCode;

	return p;
}


void AnalyseInstruction(char* line, int* index)
{

	int i = 0;
	char* command = GetWord(line, index);

	for (i = 0; i < NUMBER_OF_R_COMMANDES; i++)
	{
		if (!strcmp(allRCommands[i], command))
		{
			// R anlyezer
			AnalyseRCommand(command, line, index);
			return;
		}

	}
	for (i = 0; i < NUMBER_OF_J_COMMANDES; i++)
	{
		if (!strcmp(allJCommands[i], command))
		{
			// J anlyezer
			AnalyseJCommand(command, line, index);
			return;
		}
	}
	for (i = 0; i < NUMBER_OF_I_COMMANDES; i++)
	{
		if (!strcmp(allRCommands[i], command))
		{
			// I anlyezer
			AnalyseICommand(command, line, index);
			return;
		}
	}
}


void InitializeInstructionTable()
{
	int i = 0;

	InstructionTable[i++] = InitializeComponent("add", "R", 1, 0);
	InstructionTable[i++] = InitializeComponent("sub", "R", 2, 0);
	InstructionTable[i++] = InitializeComponent("and", "R", 3, 0);
	InstructionTable[i++] = InitializeComponent("or", "R", 4, 0);
	InstructionTable[i++] = InitializeComponent("nor", "R", 5, 0);
	InstructionTable[i++] = InitializeComponent("move", "R", 1, 1);
	InstructionTable[i++] = InitializeComponent("mvhi", "R", 2, 1);
	InstructionTable[i++] = InitializeComponent("mvlo", "R", 3, 1);

	InstructionTable[i++] = InitializeComponent("addi", "I", -1, 10);
	InstructionTable[i++] = InitializeComponent("subi", "I", -1, 11);
	InstructionTable[i++] = InitializeComponent("andi", "I", -1, 12);
	InstructionTable[i++] = InitializeComponent("ori", "I", -1, 13);
	InstructionTable[i++] = InitializeComponent("nori", "I", -1, 14);
	InstructionTable[i++] = InitializeComponent("bne", "I", -1, 15);
	InstructionTable[i++] = InitializeComponent("beq", "I", -1, 16);
	InstructionTable[i++] = InitializeComponent("blt", "I", -1, 17);
	InstructionTable[i++] = InitializeComponent("bgt", "I", -1, 18);
	InstructionTable[i++] = InitializeComponent("lb", "I", -1, 19);
	InstructionTable[i++] = InitializeComponent("sb", "I", -1, 20);
	InstructionTable[i++] = InitializeComponent("lw", "I", -1, 21);
	InstructionTable[i++] = InitializeComponent("sw", "I", -1, 22);
	InstructionTable[i++] = InitializeComponent("lh", "I", -1, 23);
	InstructionTable[i++] = InitializeComponent("sh", "I", -1, 24);


	InstructionTable[i++] = InitializeComponent("jmp", "J", -1, 30);
	InstructionTable[i++] = InitializeComponent("la", "J", -1, 31);
	InstructionTable[i++] = InitializeComponent("call", "J", -1, 32);
	InstructionTable[i++] = InitializeComponent("stop", "J", -1, 63);

}


void SkeepLabel(char* line, int* index)
{
	char* p = line + (*index);
	int i = 0;

	while (*p != ':')
	{
		p++;
		i++;
		(*index)++;
	}
	(*index)++;

}


void UpDateICF()
{
	ICF = ICF + 4;
}


char* ConvertToBinary(int number, int bitsNumber)
{
	char* binary = (char*)malloc(sizeof(char) * (bitsNumber + 1));
	int i, c, k, j = 0;

	if (bitsNumber == 32)
	{
		for (c = 31; c >= 0; c--)
		{
			k = number >> c;

			if (k & 1)
			{
				binary[j++] = '1';
			}				
			else
			{
				binary[j++] = '0';
			}
		}

		binary[j] = '\0';

		return binary;
	}

	for (i = (1 << bitsNumber - 1); i > 0; i = i / 2) {
		binary[j++] = (number & i) ? '1' : '0';
	}
	binary[j] = '\0';

	return binary;
}


struct Instruction* InstructionTableLookUp(char* command)
{
	for (int i = 0; i < MAX_INSTRUCTION_TABLE_SIZE; i++)
	{
		if (!strcmp(&InstructionTable[i]->name, command))
		{
			return InstructionTable[i];
		}
	}
}


struct Label* SymbolTableLookUp(char* label)
{
	for (int i = 0; i < MAX_SYMBOL_TABLE_SIZE; i++)
	{
		if (!strcmp(&SymbolTable[i]->name, label))
		{
			return SymbolTable[i];
		}
	}
}


char GetChar(char* line, int* index)
{
	char* p = line + (*index);
	int i = 0;

	while (isspace(*p) || *p == '\n' || *p == '\"')
	{
		(*index) = (*index) + 1;
		p++;
	}

	return *p;
}


int ConvertToDec(char* binaryArray)
{
	int x = (int)(*(binaryArray + 0)) == 48 ? 0 : 1;
	int y = (int)(*(binaryArray + 1)) == 48 ? 0 : 1;
	int z = (int)(*(binaryArray + 2)) == 48 ? 0 : 1;
	int w = (int)(*(binaryArray + 3)) == 48 ? 0 : 1;
	int e = (int)(*(binaryArray + 4)) == 48 ? 0 : 1;
	int q = (int)(*(binaryArray + 5)) == 48 ? 0 : 1;
	int r = (int)(*(binaryArray + 6)) == 48 ? 0 : 1;
	int t = (int)(*(binaryArray + 7)) == 48 ? 0 : 1;


	int hexNum = pow(2, 7) * x + pow(2, 6) * y + pow(2, 5) * z +
		pow(2, 4) * w + pow(2, 3) * e + pow(2, 2) * q +
			pow(2, 1) * r + pow(2, 0) * t;

	
	return hexNum;
}


char* GetDataBinaryArray()
{
	int n = 0, k = 0;
	int i, j;
	char* dataBinaryArray = (char*)calloc(IDF * 8 + 1 ,sizeof(char));

	while (DataSegment[n] != NULL)
	{
		j = GetArraySize((DataSegment[n]->command_in_binary));

		for (i = 0; i < j; i++)
		{
			if (DataSegment[n]->command_in_binary[i] == '\0')
			{
				continue;
			}

			dataBinaryArray[k++] = DataSegment[n]->command_in_binary[i];			
		}

		n++;
	}

	dataBinaryArray[k] = '\0';

	return dataBinaryArray;
}


void ResetArray(char array[])
{
	int arraySize = sizeof(array);

	for (int i = 0; i < arraySize; i++)
	{
		array[i] = '\0';
	}

}


int GetArraySize(char* array)
{
	int i = 0;
	int counter = 0;
	while (array[i++] != '\0')
	{
		counter++;
	}

	return counter;
}





























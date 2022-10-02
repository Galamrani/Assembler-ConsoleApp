#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>



#define MAX_LINE_lENGTH 80 
#define MAX_LABEL_LENGTH 10
#define MAX_WORD_LENGTH 15
#define MAX_NUMBER_LENGTH 15


#define MAX_SYMBOL_TABLE_SIZE 30
#define MAX_INSTRUCTION_TABLE_SIZE 27
#define NUMBER_OF_GUIDE_COMMANDES 6

#define MAX_PARAMETER_LENGTH 100
#define NUMBER_OF_ATTRIBUTES 3
#define NUMBER_OF_INSTRUCTION_COMMANDES 27

#define NUMBER_OF_R_COMMANDES 8
#define NUMBER_OF_I_COMMANDES 15
#define NUMBER_OF_J_COMMANDES 4

#define MAX_CODE_SEGMENT 50
#define MAX_DATA_SEGMENT 50
#define NUMBER_OF_ERRORS 7

const char* allRCommands[NUMBER_OF_R_COMMANDES];
const char* allICommands[NUMBER_OF_I_COMMANDES];
const char* allJCommands[NUMBER_OF_J_COMMANDES];
extern const char* AllGuideCommands[NUMBER_OF_GUIDE_COMMANDES];
extern const char* CodeOrData[NUMBER_OF_ATTRIBUTES];
extern const char* ErrorTable[NUMBER_OF_ERRORS];

extern const char* R_Commands_arithmetic[];
extern const char* R_Commands_copying[];
extern const char* I_Commands_arithmetic[];
extern const char* I_Commands_branching[];
extern const char* I_Commands_memory[];



extern const char* AllInstructionCommands[NUMBER_OF_INSTRUCTION_COMMANDES];
extern struct Label* SymbolTable[MAX_SYMBOL_TABLE_SIZE];
extern struct Instruction* InstructionTable[MAX_INSTRUCTION_TABLE_SIZE];
extern struct Data* DataSegment[MAX_DATA_SEGMENT];
extern struct Code* CodeSegment[MAX_CODE_SEGMENT];
extern ErrorFileCreated;



struct Label {

	char name[MAX_LABEL_LENGTH];
	int value; // value 
	int attribute; //  external = 0, code = 1, data = 2
	int entry; // entry or not

};
struct Instruction {

	char name[MAX_WORD_LENGTH];
	char type;
	int funct;
	int opCode;
};

/* Data Segment */
struct Data {
	char* command_in_binary;
};

/* Code Segment  */
struct Code {

	char* command_in_binary;
	int address;
};



/* MainProg */
void FreeMemory();
void TryCatchFileNotFound(FILE* );
void ClearOutputFiles();

/* First Transition  */
void UpdateSymbolTable();
void TryCatchLineError(char*, int, int*);
void AnalyzeCommandAfterLabel(char*, int*);
void ChangeLastLabelValue(int);
void FirstTransition(FILE*);
void GetLabel(char *, int*);
void AddToSymbolTable(char*);
char* GetWord(char*, int*);
void SkeepSpace(char*, int*);
void UpDateDC(char*, int);
void UpDateIC();
int GetParameterA(char*, int*);
void GetExternalLabel(char*, int*);
void GetGuideCommand(char*, int*);
int CountParameters(char*, int*);
int FirstWordIsExternal(char*);
int FirstWordIsInstruction(char*);
int FirstWordIsLabel(char*);
int FirstWordIsGuideCommand(char*);
int FirstWordIsEntry(char*);

void PrintToErrorFile(int, int);
void TryCatchLineError(char*, int, int*);
void TryCatchLabelExistError(char*, int*);
void TryCatchAfterLabelError(char*, int*);
void TryCatchExternLabelError(char*, int*);
void TryCatchInstructionError(char*, int*, int*);
void TryCatchGuideCommandError(char*, int*, int*);
void TryCatchRInstructionError(char*, int*, int*);
void TryCatchIInstructionError(char*, int*, int*);
void TryCatchJInstructionError(char*, int*, int*, char*);


/* Second Transition  */
void SkeepLabel(char*, int*);
void UpDateICF();
void AnalyseInstruction(char*, int*);
void InitializeInstructionTable();
struct Instruction* InitializeComponent(char*, char*, int, int);
void SkeepLabel(char*, int*);
struct Instruction* InitializeComponent(char*, char*, int, int);
char* ConvertToBinary(int, int);
int GetParameterB(char*, int*);
struct Instruction* InstructionTableLookUp(char*);
struct Label* SymbolTableLookUp(char*);
char GetChar(char*, int*);
int ConvertToDec(char*);
void CreateOutputFiles();
void CreateOBfile();
void CreateENTfile();
void AppendToEXTfile(char*, int);
void AppendCodeSegment(FILE*);
void AppendDataSegment(FILE*);
char* GetDataBinaryArray();
void ResetArray(char[]);
int GetArraySize(char*);

void AnalyseRCommand(char*, char*, int*);
void ArithmeticRCommand(char*, char*, int*);
void CopyingRCommand(char*, char*, int*);
void AnalyseICommand(char*, char*, int*);
void ArithmeticICommand(char*, char*, int*);
void BranchingICommand(char*, char*, int*);
void MemoryICommand(char*, char*, int*);
void AnalyseJCommand(char*, char*, int*);
void JCommand(char*, char*, int*);
void StopJCommand(char*, char*, int*);
void ChangeEntryLabel(char*, int*);

void AddRToCodeSegment(int, int, int, int, int);
void AddIToCodeSegment(int, int, int, int);
void AddJToCodeSegment(int, int, int);
char* GetRBinaryArry(char*, char*, char*, char*, char*);
char* GetIBinaryArry(char*, char*, char*, char*);
char* GetJBinaryArry(char*, char*, char*);

void AnalyseGuideCommand(char*, int*);
void AnalyseGuideCommandBH(char*, int*);
void AnalyseGuideCommandBD(char*, int*);
void AnalyseGuideCommandBW(char*, int*);
void AnalyseGuideCommandASCIZ(char*, int*);
void AddToDataSegment(int, int);











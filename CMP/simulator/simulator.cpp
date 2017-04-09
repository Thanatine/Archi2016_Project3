#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "instruction.h"

using namespace std;

Instruction instruction;

int main(int argc, char* argv[])
{
    //file variable
    FILE *iimage, *dimage;
    int iSize, dSize;
    unsigned int *iInstruction, *dInstruction;
	size_t iresult, dresult;
    
    //initiates
	
    instruction.cycle = 0;
    instruction.PC = 0;
	for(int i = 0 ; i < 32 ; i++)	instruction.reg[i] = 0;
    
    if(argc == 1){
        instruction.cmp.sizeiMemory = 64; //64
        instruction.cmp.sizedMemory = 32; //32
        instruction.cmp.pageSizeiMemory = 8; //8
        instruction.cmp.pageSizedMemory = 16; //16
        instruction.cmp.sizeiCache = 16; //16
        instruction.cmp.blockSizeiCache = 4; //4
        instruction.cmp.setiCache = 4; //4
        instruction.cmp.sizedCache = 16; //16
        instruction.cmp.blockSizedCache = 4; //4
        instruction.cmp.setdCache = 1; //1
    }
    else{
        instruction.cmp.sizeiMemory = atoi(argv[1]); //64
        instruction.cmp.sizedMemory = atoi(argv[2]); //32
        instruction.cmp.pageSizeiMemory = atoi(argv[3]); //8
        instruction.cmp.pageSizedMemory = atoi(argv[4]); //16
        instruction.cmp.sizeiCache = atoi(argv[5]); //16
        instruction.cmp.blockSizeiCache = atoi(argv[6]); //4
        instruction.cmp.setiCache = atoi(argv[7]); //4
        instruction.cmp.sizedCache = atoi(argv[8]); //16
        instruction.cmp.blockSizedCache = atoi(argv[9]); //4
        instruction.cmp.setdCache = atoi(argv[10]); //1
    }
    
    
    for(int i = 0 ; i < 256 ; i++)	{instruction.cmp.dDisk[i] = 0;  instruction.iMemory[i] = 0;}
    for(int i = 0 ; i < 256 ; i++)  {instruction.cmp.iDisk[i] = 0;  instruction.dMemory[i] = 0;}
    for(int i = 0 ; i < 1024 ; i++) instruction.cmp.iTLB[i].valid = false;
    for(int i = 0 ; i < 1024 ; i++) instruction.cmp.dTLB[i].valid = false;
    for(int i = 0 ; i < 1024 ; i++) instruction.cmp.iPTE[i].valid = false;
    for(int i = 0 ; i < 1024 ; i++) instruction.cmp.dPTE[i].valid = false;
    for(int i = 0 ; i < 1024 ; i++) for(int j = 0 ; j < 1024 ; j++) instruction.cmp.iCache[i][j].valid = false;
    for(int i = 0 ; i < 1024 ; i++) for(int j = 0 ; j < 1024 ; j++) instruction.cmp.dCache[i][j].valid = false;
    for(int i = 0 ; i < 1024 ; i++) instruction.cmp.iMemory[i].valid = false;
    for(int i = 0 ; i < 1024 ; i++) instruction.cmp.dMemory[i].valid = false;
    instruction.cmp.lengthiPTE = 1024 / instruction.cmp.pageSizeiMemory; //128
    instruction.cmp.lengthdPTE = 1024 / instruction.cmp.pageSizedMemory;
    instruction.cmp.lengthiTLB = instruction.cmp.lengthiPTE / 4; //32
    instruction.cmp.lengthdTLB = instruction.cmp.lengthdPTE / 4;
    instruction.cmp.lengthiCache = instruction.cmp.sizeiCache / instruction.cmp.setiCache / instruction.cmp.blockSizeiCache;
    instruction.cmp.lengthdCache = instruction.cmp.sizedCache / instruction.cmp.setdCache / instruction.cmp.blockSizedCache;
    instruction.cmp.lengthiMemory = instruction.cmp.sizeiMemory / instruction.cmp.pageSizeiMemory; //8
    instruction.cmp.lengthdMemory = instruction.cmp.sizedMemory / instruction.cmp.pageSizedMemory;
    
    //readfile
    iimage = fopen ("iimage.bin" ,"rb" );
    dimage = fopen ("dimage.bin", "rb");

    fseek(iimage, 0, SEEK_END);
    fseek(dimage, 0, SEEK_END);
    iSize = ftell(iimage);
    /*if(iSize > 1032){
        iSize = 1032;
    }*/
    dSize = ftell(dimage);
    /*if(dSize > 1032){
        dSize = 1032;
    }*/
    rewind (iimage);
    rewind (dimage);

    iInstruction = (unsigned int*) malloc (sizeof(unsigned int) * iSize);
    dInstruction = (unsigned int*) malloc (sizeof(unsigned int) * dSize);

    iresult = fread (iInstruction, sizeof(unsigned int), iSize, iimage);
    dresult = fread (dInstruction, sizeof(unsigned int), dSize, dimage);
	
	instruction.cmp.report = fopen("report.rpt", "w");
	instruction.snapshot = fopen("snapshot.rpt", "w");

	//data access
	//iDisk
    for(int i = 0 ; i < iInstruction[1] + 2; i++){
		iInstruction[i] = instruction.changeEndian(iInstruction[i]);
		//instruction.cmp.iDisk[i] = iInstruction[i];
        instruction.iMemory[i] = iInstruction[i];
	}
    
    //dDisk
	for(int i = 0 ; i < dInstruction[1] + 2 ; i++){
		dInstruction[i] = instruction.changeEndian(dInstruction[i]);
	}
	
	for(int i = 2 ; i < 2 + dInstruction[1] ; i++){
		//instruction.cmp.dDisk[i - 2] = dInstruction[i];
        instruction.dMemory[i - 2] = dInstruction[i];
	}
    instruction.reg[29] = dInstruction[0];
	
	//simulation starts
	instruction.PC = instruction.iMemory[0];
    
	while(instruction.haltsignal == false){
        
        //printf("cycle = %d\n", instruction.cycle);
        
        fprintf(instruction.snapshot, "cycle %d\n", instruction.cycle);
        
        instruction.cmp.instructionMemoryMining(instruction.PC, instruction.cycle);
        
        if(instruction.PC < iInstruction[0]){
            instruction.decode(0);
        }
        else{
            instruction.decode(instruction.iMemory[(instruction.PC - iInstruction[0]) / 4 + 2]);
        }
        
        instruction.cycle++;

    }
    
    instruction.cmp.printCMP();

	return 0;
}



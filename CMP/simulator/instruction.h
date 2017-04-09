#include <iostream>
#include <stdio.h>
#include <stdlib.h>
//R type
#define RTYPE 0x00 
#define ADD 0x20
#define ADDU 0x21
#define SUB 0x22
#define AND 0x24
#define OR 0x25
#define XOR 0x26
#define NOR 0x27
#define NAND 0x28
#define SLT 0x2A
#define SLL 0x00
#define SRL 0x02
#define SRA 0x03
#define JR 0x08

//I type
#define ADDI 0x08
#define ADDIU 0x09
#define LW 0x23
#define LH 0x21
#define LHU 0x25
#define LB 0x20
#define LBU 0x24
#define SW 0x2B
#define SH 0x29
#define SB 0x28
#define LUI 0x0F
#define ANDI 0x0C
#define ORI 0x0D
#define NORI 0x0E
#define SLTI 0x0A
#define BEQ 0x04
#define BNE 0x05
#define BGTZ 0x07

//J type
#define J 0x02
#define JAL 0x03

//Specialized Instruction
#define HALT 0x3F

using namespace std;

struct TranslationLookAsideBuffer
{
    int VPN;
    int PPN; //physical page number
    int recentlyUsedCycle;
    bool valid;
    
};

struct PageTableEntry
{
    int PPN;
    bool valid;
    
};

struct Cache
{
    int tag;
    int MRU;
    bool valid;
    
};

struct Memory
{
    int recentlyUsedCycle;
    bool valid;
    
};


class CacheMemoryPagetable
{
public:
    unsigned int iDisk[256], dDisk[256];
    TranslationLookAsideBuffer iTLB[1024], dTLB[1024];
    PageTableEntry iPTE[1024], dPTE[1024];
    Cache iCache[1024][1024], dCache[1024][1024];
    Memory iMemory[1024], dMemory[1024];
    FILE *report;
    
    int sizeiMemory, sizedMemory;
    int pageSizeiMemory, pageSizedMemory;
    int sizeiCache, sizedCache;
    int blockSizeiCache, blockSizedCache;
    int setiCache, setdCache;
    int lengthiTLB, lengthiPTE, lengthiCache, lengthiMemory;
    int lengthdTLB, lengthdPTE, lengthdCache, lengthdMemory;
    
    int hitsiCache, missesiCache;
    int hitsdCache, missesdCache;
    int hitsiTLB, missesiTLB;
    int hitsdTLB, missesdTLB;
    int hitsiPTE, missesiPTE;
    int hitsdPTE, missesdPTE;
    
    void instructionMemoryMining(unsigned int VA, int cycle);
    void dataMemoryMining(unsigned int VA, int cycle);
    void printCMP();
    
};

class Instruction
{
public:
	unsigned int reg[32];
    unsigned int iMemory[256], dMemory[256];
	unsigned int PC;
    int cycle;
	FILE *snapshot;
	bool haltsignal;
    CacheMemoryPagetable cmp;
		
	unsigned int changeEndian(unsigned int hex);
	void decode(unsigned int input);
    
	void printreg();
};

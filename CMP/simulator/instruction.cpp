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

#include "instruction.h"

unsigned int Instruction::changeEndian(unsigned int hex)
{
	return (hex <<24 & 0xff000000) | (hex << 8 & 0x00ff0000) | (hex >> 8 & 0x0000ff00) | (hex >> 24 & 0x000000ff);
}

void Instruction::decode(unsigned int input)
{
	int pc_flag = 0;
    
    unsigned int rt = input << 11 >> 27, rs = input << 6 >> 27, rd = input << 16 >> 27, shamt = input << 21 >> 27;
    unsigned int cUnsigned = input << 16 >> 16, cSigned = ((int)(input << 16) >> 16);
    
    reg[0] = 0;
    printreg();
    
	switch(input >> 26){
		//R type
		case RTYPE:
			switch(input << 26 >> 26){
				case ADD:
					reg[rd] = reg[rs] + reg[rt];
                    break;
				
                case ADDU:
					reg[rd] = reg[rs] + reg[rt];
					break;
				
                case SUB:
					reg[rd] = reg[rs] - reg[rt];
					break;
				
                case AND:
					reg[rd] = reg[rs] & reg[rt];
					break;
				
                case OR:
					reg[rd] = reg[rs] | reg[rt];
					break;
				
                case XOR:
					reg[rd] = reg[rs] ^ reg[rt];
					break;
				
                case NOR:
					reg[rd] = ~(reg[rs] | reg[rt]);
					break;
				
                case NAND:
					reg[rd] = ~(reg[rs] & reg[rt]);
					break;
				
                case SLT:
                    if((reg[rs] >> 31) == 0){
                        if((reg[rt] >> 31) == 0)    reg[rd] = reg[rs] < reg[rt];
                        else    reg[rd] = reg[rs] > reg[rt];
                    }
                    else{
                        if((reg[rt] >> 31) == 0)    reg[rd] = reg[rs] > reg[rt];
                        else    reg[rd] = reg[rs] < reg[rt];
                    }
                    break;
				
                case SLL:
					reg[rd] = (reg[rt] << (shamt));
					break;
				
                case SRL:
					reg[rd] = (reg[rt] >> (shamt));
					break; 
				
                case SRA:
                    reg[rd] = ((int)reg[rt] >> (shamt));
					break;
				
                case JR:
					PC = reg[rs];
					pc_flag = 1;
					break;
				
                default:
					break;
			} 
			break;
		//I tyoe
		case ADDI:
			reg[rt] = reg[rs] + cSigned;
			break;
		
        case ADDIU:
			reg[rt] = reg[rs] + cSigned;
			break;
		
        case LW:
            cmp.dataMemoryMining(reg[rs] + cSigned, cycle);
            
			reg[rt] = dMemory[(reg[rs] + cSigned) / 4];
            break;
		
        case LH:
            cmp.dataMemoryMining(reg[rs] + cSigned, cycle);
            
			if((reg[rs] + cSigned) % 4 == 0)
				reg[rt] = (int)dMemory[(reg[rs] + cSigned) / 4] >> 16;
			
			else
				reg[rt] = (int)(dMemory[(reg[rs] + cSigned) / 4] << 16) >> 16;
			
			break;
		
        case LHU:
            
            cmp.dataMemoryMining(reg[rs] + cSigned, cycle);
            
			if((reg[rs] + cSigned) % 4 == 0)
				reg[rt] = dMemory[(reg[rs] + cSigned) / 4] >> 16;
			
            else
				reg[rt] = dMemory[(reg[rs] + cSigned) / 4] << 16 >> 16;
            
			break;
		
        case LB:
            
            cmp.dataMemoryMining(reg[rs] + cSigned, cycle);
            
			if((reg[rs] + cSigned) % 4 == 0)
				reg[rt] = (int)dMemory[(reg[rs] + cSigned) / 4] >> 24;
			
            else if((reg[rs] + cSigned) % 4 == 1)
				reg[rt] = (int)(dMemory[(reg[rs] + cSigned) / 4] << 8) >> 24;
			
            else if((reg[rs] + cSigned) % 4 == 2)
				reg[rt] = (int)(dMemory[(reg[rs] + cSigned) / 4] << 16) >> 24;
			
			else
				reg[rt] = (int)(dMemory[(reg[rs] + cSigned) / 4] << 24) >> 24;
			
			break;
		
        case LBU:
            
            cmp.dataMemoryMining(reg[rs] + cSigned, cycle);
            
			if((reg[rs] + cSigned) % 4 == 0)
				reg[rt] = dMemory[(reg[rs] + cSigned) / 4 ] >> 24;
			
            else if((reg[rs] + cSigned) % 4 == 1)
				reg[rt] = dMemory[(reg[rs] + cSigned) / 4 ] << 8 >> 24;
			
            else if((reg[rs] + cSigned) % 4 == 2)
				reg[rt] = dMemory[(reg[rs] + cSigned) / 4 ] << 16 >> 24;
            
            else
                reg[rt] = dMemory[(reg[rs] + cSigned) / 4 ] << 24 >> 24;
        
			break;
		
        case SW:;
            
            cmp.dataMemoryMining(reg[rs] + cSigned, cycle);
            
			dMemory[(reg[rs] + cSigned) / 4] = reg[rt];
            
			break;
		
        case SH:
            cmp.dataMemoryMining(reg[rs] + cSigned, cycle);
            
            if((reg[rs] + cSigned) % 4 == 0)
                dMemory[(reg[rs] + cSigned) / 4] = (dMemory[(reg[rs] + cSigned) / 4] & 0x0000FFFF) + ((reg[rt] & 0x0000FFFF) << 16);
            
            else
                dMemory[(reg[rs] + cSigned) / 4] = (dMemory[(reg[rs] + cSigned) / 4] & 0xFFFF0000) + (reg[rt] & 0x0000FFFF);
            
			break;
		
        case SB:
            cmp.dataMemoryMining(reg[rs] + cSigned, cycle);
            
            if((reg[rs] + cSigned) % 4 == 0)
                dMemory[(reg[rs] + cSigned) / 4 ] = (dMemory[(reg[rs] + cSigned) / 4] & 0x00FFFFFF) + ((reg[rt] & 0x000000FF) << 24);
            
            else if((reg[rs] + cSigned) % 4 == 1)
                dMemory[(reg[rs] + cSigned) / 4 ] = (dMemory[(reg[rs] + cSigned) / 4] & 0xFF00FFFF) + ((reg[rt] & 0x000000FF) << 16);
            
            else if((reg[rs] + cSigned) % 4 == 2)
                dMemory[(reg[rs] + cSigned) / 4 ] = (dMemory[(reg[rs] + cSigned) / 4] & 0xFFFF00FF) + ((reg[rt] & 0x000000FF) << 8);
            
            else
                dMemory[(reg[rs] + cSigned) / 4 ] = (dMemory[(reg[rs] + cSigned) / 4] & 0xFFFFFF00) + (reg[rt] & 0x000000FF);

			break;
		
        case LUI:
			reg[rt] = (input << 16);
			break;
		
        case ANDI:
			reg[rt] = reg[rs] & cUnsigned;
			break;
		
        case ORI:
			reg[rt] = reg[rs] | cUnsigned;
			break;
		
        case NORI:
			reg[rt] = ~(reg[rs] | cUnsigned);
			break;
		
        case SLTI:
            if((reg[rs] >> 31) == 0){
                if(cSigned >> 31 == 0)  reg[rt] = reg[rs] < cSigned;
                else    reg[rt] = reg[rs] > cSigned;
            }
            else{
                if((cSigned >> 31) == 0)    reg[rt] = reg[rs] > cSigned;
                else    reg[rt] = reg[rs] < cSigned;
            }
			break;
		
        case BEQ:
			if(reg[rs] == reg[rt]){
				PC = PC + 4 + 4 * cSigned;
				pc_flag = 1;
			}
			break;
		
        case BNE:
			if(reg[rs] != reg[rt]){
				PC = PC + 4 + 4 * cSigned;
				pc_flag = 1;
			}
			break;
		
        case BGTZ:
			if((int)reg[rs] > 0){
				PC = PC + 4 + 4 * cSigned;
				pc_flag = 1;
			}
			break;
		
        //J type
		case J:
			PC = ((PC + 4) >> 28) | 4 * (input << 6 >> 6);
			pc_flag = 1;
			break;
		
        case JAL:
			reg[31] = PC + 4;
			PC = ((PC + 4) >> 28) | 4 * (input << 6 >> 6);
			pc_flag = 1;
			break;
		
            //Specialized:
		case HALT:
			haltsignal = true;
			return;		
			break;
		
        default:
			break;
				
	}

	if(!pc_flag)	PC = PC + 4;

	return;
}


void Instruction::printreg()
{
	for(int i = 0 ; i < 32 ; i++){
		fprintf(snapshot, "$%02d: 0x%08X\n", i, reg[i]);
	}
	fprintf(snapshot, "PC: 0x%08X\n\n\n", PC);

	return;
}

void CacheMemoryPagetable::instructionMemoryMining(unsigned int VA, int cycle)
{
    bool hitSignalTLB = false;
    for(int i = 0 ; i < lengthiTLB ; i++){
        //TLB hit
        if(iTLB[i].VPN == VA / pageSizeiMemory && iTLB[i].valid == true){
            iTLB[i].recentlyUsedCycle = cycle;
            hitSignalTLB = true;
            hitsiTLB++;
            break;
        }
    }
    
    //TLB miss
    int leastCycleUsedIndexTLB = 0;
    
    //PTE hit
    if(iPTE[VA / pageSizeiMemory].valid == true && hitSignalTLB == false){
        
        missesiTLB++;
        hitsiPTE++;

        for(int i = 0 ; i < lengthiTLB ; i++){
            if(iTLB[i].valid == false){
                leastCycleUsedIndexTLB = i;
                break;
            }
            if(iTLB[i].recentlyUsedCycle < iTLB[leastCycleUsedIndexTLB].recentlyUsedCycle && iTLB[i].valid == true){
                leastCycleUsedIndexTLB = i;
            }
            
        }
        
        iTLB[leastCycleUsedIndexTLB].recentlyUsedCycle = cycle;
        iTLB[leastCycleUsedIndexTLB].PPN = iPTE[VA / pageSizeiMemory].PPN;
        iTLB[leastCycleUsedIndexTLB].VPN = VA / pageSizeiMemory;
        iTLB[leastCycleUsedIndexTLB].valid = true;

        iMemory[iPTE[VA / pageSizeiMemory].PPN].recentlyUsedCycle = cycle;
        
    }
    //PTE miss
    else if(hitSignalTLB == false){
        
        missesiTLB++;
        missesiPTE++;
        
        bool replaceSignalMEM = true;
        for(int i = 0 ; i < lengthiMemory ; i++){
            if(iMemory[i].valid == false){
                replaceSignalMEM = false;
                break;
            }
        }
        
        //MEM should replace
        if(replaceSignalMEM == true){
            
            //printf("check\n");
            
            //replace iMemory by LRU, Memory indexed by PPN
            int leastCycleUsedIndexMEM = 0;
            for(int i = 0 ; i < lengthiMemory ; i++){
                if(iMemory[i].recentlyUsedCycle < iMemory[leastCycleUsedIndexMEM].recentlyUsedCycle && iMemory[i].valid == true){
                    //PPN
                    leastCycleUsedIndexMEM = i;
                }
            }
            
            iMemory[leastCycleUsedIndexMEM].recentlyUsedCycle = cycle;
            iMemory[leastCycleUsedIndexMEM].valid = true;
            
            //set replaced page to invalid in PTE
            for(int i = 0 ; i < lengthiPTE ; i++){
                if(iPTE[i].PPN == leastCycleUsedIndexMEM && iPTE[i].valid == true){
                    //將在PTE中被取代的page的valid設成0
                    iPTE[i].valid = false;
                    
                    //set replaced page to invalid in TLB
                    for(int j = 0 ; j < lengthiTLB ; j++){
                        if(iTLB[j].VPN == i){
                            iTLB[j].valid = false;
                        }
                    }
                    
                    /*int offset = VA % pageSizeiMemory;
                    int PA = iPTE[i].PPN * pageSizeiMemory + offset;
                    int indexCache = (PA / blockSizeiCache) % lengthiCache;
                    int tag = (PA / blockSizeiCache) / lengthiCache;*/

                    //printf("=> which tag: %d\n", tag);
                    
                    //set replaced page to invalid in Cache
                    for(int m = 0 ; m < lengthiCache ; m++){
                        for(int n = 0 ; n < setiCache ; n++){
                            if( (iCache[m][n].tag * lengthiCache * blockSizeiCache
                                 + m * blockSizeiCache)
                               / pageSizeiMemory == iPTE[i].PPN && iCache[m][n].valid == true){
                                iCache[m][n].valid = false;
                            }
                        }
                    }
                    
                }
            }
            
            //update PTE
            iPTE[VA / pageSizeiMemory].valid = true;
            iPTE[VA / pageSizeiMemory].PPN = leastCycleUsedIndexMEM;
            
            //update TLB
            leastCycleUsedIndexTLB = 0;
            for(int i = 0 ; i < lengthiTLB ; i++){
                if(iTLB[i].valid == false){
                    leastCycleUsedIndexTLB = i;
                    break;
                }
                if(iTLB[i].recentlyUsedCycle < iTLB[leastCycleUsedIndexTLB].recentlyUsedCycle && iTLB[i].valid == true){
                    leastCycleUsedIndexTLB = i;
                }
                
            }
            
            iTLB[leastCycleUsedIndexTLB].valid = true;
            iTLB[leastCycleUsedIndexTLB].recentlyUsedCycle = cycle;
            iTLB[leastCycleUsedIndexTLB].VPN = VA / pageSizeiMemory;
            iTLB[leastCycleUsedIndexTLB].PPN = iPTE[VA / pageSizeiMemory].PPN;
            
        }
        
        //MEM no need to replace
        else{
            int leastUsedIndexMEM = 0;
            for(int i = 0 ; i < lengthiMemory ; i++){
                if(iMemory[i].valid == false){
                    iMemory[i].valid = true;
                    iMemory[i].recentlyUsedCycle = cycle;
                    leastUsedIndexMEM = i;
                    break;
                }
            }
            
            //update PTE
            iPTE[VA / pageSizeiMemory].valid = true;
            iPTE[VA / pageSizeiMemory].PPN = leastUsedIndexMEM;
            
            //update TLB
            leastCycleUsedIndexTLB = 0;
            for(int i = 0 ; i < lengthiTLB ; i++){
                if(iTLB[i].valid == false){
                    leastCycleUsedIndexTLB = i;
                    break;
                }
                
                if(iTLB[i].recentlyUsedCycle < iTLB[leastCycleUsedIndexTLB].recentlyUsedCycle && iTLB[i].valid == true){
                    leastCycleUsedIndexTLB = i;
                }
                
            }
            
            iTLB[leastCycleUsedIndexTLB].valid = true;
            iTLB[leastCycleUsedIndexTLB].recentlyUsedCycle = cycle;
            iTLB[leastCycleUsedIndexTLB].VPN = VA / pageSizeiMemory;
            iTLB[leastCycleUsedIndexTLB].PPN = leastUsedIndexMEM;
            
        }
        
    }
    
    /*printf("VA: 0x%8X, cycle: %d, TLB hits: %d, TLB misses: %d, PTE hits: %d, PTE misses: %d\n", VA, cycle, hitsiTLB, missesiTLB, hitsiPTE, missesiPTE);
   
    printf("TLB: \n");
    for(int i = 0 ; i < lengthiTLB ; i++){
        if(iTLB[i].valid == true)
            printf("-> PPN: %d, VPN: %d, LRU: %d\n", iTLB[i].PPN, iTLB[i].VPN * pageSizeiMemory, iTLB[i].recentlyUsedCycle);
        else    break;
    }
    
    printf("PTE: \n");
    for(int i = 0 ; i < lengthiPTE ; i++){
        if(iPTE[i].valid == true)
            printf("-> VPN: %d, PPN: %d\n", i * pageSizeiMemory, iPTE[i].PPN);
        else    continue;
    }*/
    
    
    int offset = VA % pageSizeiMemory;
    int PA = iPTE[VA / pageSizeiMemory].PPN * pageSizeiMemory + offset;
    int indexCache = (PA / blockSizeiCache) % lengthiCache;
    int tag = (PA / blockSizeiCache) / lengthiCache;
    
    /*printf("iCache - cycle: %d, hits: %d, misses: %d, VA: %X, tag = %d\n", cycle, hitsiCache, missesiCache, VA, tag);
     for(int i = 0 ; i < lengthiCache ; i++){
         for(int j = 0 ; j < setiCache ; j++){
             if(iCache[i][j].valid == true){
                 printf("-> [%d][%d], MRU = %d, tag = %d\n", i, j, iCache[i][j].MRU, iCache[i][j].tag);
             }
         }
     }*/
    
    for(int j = 0 ; j < setiCache ; j++){
        //Cache hit
        if(iCache[indexCache][j].tag == tag && iCache[indexCache][j].valid == true){
            hitsiCache++;
            iCache[indexCache][j].MRU = 1;
            
            //flip detect
            bool flipSignalCache = true;
            for(int k = 0 ; k < setiCache ; k++){
                if(iCache[indexCache][k].MRU == 0 || iCache[indexCache][k].valid == false){
                    flipSignalCache = false;
                    break;
                }
            }
            
            if(setiCache == 1){
                iCache[indexCache][0].MRU = 0;
            }
            else if(flipSignalCache == true){
                for(int k = 0 ; k < setiCache ; k++){
                    if(k != j){
                        iCache[indexCache][k].MRU = 0;
                    }
                }
            }
            
            return;
            
        }
    }

    
    //Cache miss
    missesiCache++;
    
    //find any invalid block
    for(int j = 0 ; j < setiCache ; j++){
        if(iCache[indexCache][j].valid == false){
            iCache[indexCache][j].tag = tag;
            iCache[indexCache][j].valid = true;
            iCache[indexCache][j].MRU = 1;
            
            //flip detect
            bool flipSignalCache = true;
            for(int k = 0 ; k < setiCache ; k++){
                if(iCache[indexCache][k].MRU == 0 || iCache[indexCache][k].valid == false){
                    flipSignalCache = false;
                    break;
                }
            }
            
            if(setiCache == 1){
                iCache[indexCache][0].MRU = 0;
            }
            else if(flipSignalCache == true){
                for(int k = 0 ; k < setiCache ; k++){
                    if(k != j){
                        iCache[indexCache][k].MRU = 0;
                    }
                }
            }
            
            return;
            
        }
    }
    
    
    //no invalid block -> implement bits-pseudo LRU
    for(int j = 0 ;j < setiCache ; j++){
        if(iCache[indexCache][j].MRU == 0){
            iCache[indexCache][j].tag = tag;
            iCache[indexCache][j].valid = true;
            iCache[indexCache][j].MRU = 1;
            
            bool flipSignalCache = true;
            for(int k = 0 ; k < setiCache ; k++){
                if(iCache[indexCache][k].MRU == 0 || iCache[indexCache][k].valid == false){
                    flipSignalCache = false;
                    break;
                }
            }
            
            if(setiCache == 1){
                iCache[indexCache][0].MRU = 0;
            }
            else if(flipSignalCache == true){
                for(int k = 0 ; k < setiCache ; k++){
                    if(k != j){
                        iCache[indexCache][k].MRU = 0;
                    }
                }
            }
            
            return;
        }
        
    }
    
    return;
    
}

void CacheMemoryPagetable::dataMemoryMining(unsigned int VA, int cycle)
{
    bool hitSignalTLB = false;
    for(int i = 0 ; i < lengthdTLB ; i++){
        //TLB hit
        if(dTLB[i].VPN == VA / pageSizedMemory && dTLB[i].valid == true){
            dTLB[i].recentlyUsedCycle = cycle;
            hitSignalTLB = true;
            hitsdTLB++;
            break;
        }
    }
    
    //TLB miss
    int leastCycleUsedIndexTLB = 0;
    
    //PTE hit
    if(dPTE[VA / pageSizedMemory].valid == true && hitSignalTLB == false){
        
        missesdTLB++;
        hitsdPTE++;
        
        for(int i = 0 ; i < lengthdTLB ; i++){
            if(dTLB[i].valid == false){
                leastCycleUsedIndexTLB = i;
                break;
            }
            if(dTLB[i].recentlyUsedCycle < dTLB[leastCycleUsedIndexTLB].recentlyUsedCycle && dTLB[i].valid == true){
                leastCycleUsedIndexTLB = i;
            }
            
        }
        
        dTLB[leastCycleUsedIndexTLB].recentlyUsedCycle = cycle;
        dTLB[leastCycleUsedIndexTLB].PPN = dPTE[VA / pageSizedMemory].PPN;
        dTLB[leastCycleUsedIndexTLB].VPN = VA / pageSizedMemory;
        dTLB[leastCycleUsedIndexTLB].valid = true;
        
        dMemory[dPTE[VA / pageSizedMemory].PPN].recentlyUsedCycle = cycle;
        
    }
    //PTE miss
    else if(hitSignalTLB == false){
        
        missesdTLB++;
        missesdPTE++;
        
        bool replaceSignalMEM = true;
        for(int i = 0 ; i < lengthdMemory ; i++){
            if(dMemory[i].valid == false){
                replaceSignalMEM = false;
                break;
            }
        }
        
        //MEM should replace
        if(replaceSignalMEM == true){
            
            //printf("check\n");
            
            //replace dMemory by LRU, Memory indexed by PPN
            int leastCycleUsedIndexMEM = 0;
            for(int i = 0 ; i < lengthdMemory ; i++){
                if(dMemory[i].recentlyUsedCycle < dMemory[leastCycleUsedIndexMEM].recentlyUsedCycle && dMemory[i].valid == true){
                    //PPN
                    leastCycleUsedIndexMEM = i;
                }
            }
            
            dMemory[leastCycleUsedIndexMEM].recentlyUsedCycle = cycle;
            dMemory[leastCycleUsedIndexMEM].valid = true;
            
            //set replaced page to invalid in PTE
            for(int i = 0 ; i < lengthdPTE ; i++){
                if(dPTE[i].PPN == leastCycleUsedIndexMEM && dPTE[i].valid == true){
                    //將在PTE中被取代的page的valid設成0
                    dPTE[i].valid = false;
                    
                    //set replaced page to invalid in TLB
                    for(int j = 0 ; j < lengthdTLB ; j++){
                        if(dTLB[j].VPN == i){
                            dTLB[j].valid = false;
                        }
                    }
                    
                    /*int offset = VA % pageSizedMemory;
                     int PA = dPTE[i].PPN * pageSizedMemory + offset;
                     int indexCache = (PA / blockSizedCache) % lengthdCache;
                     int tag = (PA / blockSizedCache) / lengthdCache;*/
                    
                    //printf("=> which tag: %d\n", tag);
                    
                    //set replaced page to invalid in Cache
                    for(int m = 0 ; m < lengthdCache ; m++){
                        for(int n = 0 ; n < setdCache ; n++){
                            if( (dCache[m][n].tag * lengthdCache * blockSizedCache
                                 + m * blockSizedCache)
                               / pageSizedMemory == dPTE[i].PPN && dCache[m][n].valid == true){
                                dCache[m][n].valid = false;
                            }
                        }
                    }
                    
                }
            }
            
            //update PTE
            dPTE[VA / pageSizedMemory].valid = true;
            dPTE[VA / pageSizedMemory].PPN = leastCycleUsedIndexMEM;
            
            //update TLB
            leastCycleUsedIndexTLB = 0;
            for(int i = 0 ; i < lengthdTLB ; i++){
                if(dTLB[i].valid == false){
                    leastCycleUsedIndexTLB = i;
                    break;
                }
                if(dTLB[i].recentlyUsedCycle < dTLB[leastCycleUsedIndexTLB].recentlyUsedCycle && dTLB[i].valid == true){
                    leastCycleUsedIndexTLB = i;
                }
                
            }
            
            dTLB[leastCycleUsedIndexTLB].valid = true;
            dTLB[leastCycleUsedIndexTLB].recentlyUsedCycle = cycle;
            dTLB[leastCycleUsedIndexTLB].VPN = VA / pageSizedMemory;
            dTLB[leastCycleUsedIndexTLB].PPN = dPTE[VA / pageSizedMemory].PPN;
            
        }
        
        //MEM no need to replace
        else{
            int leastUsedIndexMEM = 0;
            for(int i = 0 ; i < lengthdMemory ; i++){
                if(dMemory[i].valid == false){
                    dMemory[i].valid = true;
                    dMemory[i].recentlyUsedCycle = cycle;
                    leastUsedIndexMEM = i;
                    break;
                }
            }
            
            //update PTE
            dPTE[VA / pageSizedMemory].valid = true;
            dPTE[VA / pageSizedMemory].PPN = leastUsedIndexMEM;
            
            //update TLB
            leastCycleUsedIndexTLB = 0;
            for(int i = 0 ; i < lengthdTLB ; i++){
                if(dTLB[i].valid == false){
                    leastCycleUsedIndexTLB = i;
                    break;
                }
                
                if(dTLB[i].recentlyUsedCycle < dTLB[leastCycleUsedIndexTLB].recentlyUsedCycle && dTLB[i].valid == true){
                    leastCycleUsedIndexTLB = i;
                }
                
            }
            
            dTLB[leastCycleUsedIndexTLB].valid = true;
            dTLB[leastCycleUsedIndexTLB].recentlyUsedCycle = cycle;
            dTLB[leastCycleUsedIndexTLB].VPN = VA / pageSizedMemory;
            dTLB[leastCycleUsedIndexTLB].PPN = leastUsedIndexMEM;
            
        }
        
    }
    
    /*printf("VA: 0x%8X, cycle: %d, TLB hits: %d, TLB misses: %d, PTE hits: %d, PTE misses: %d\n", VA, cycle, hitsdTLB, missesdTLB, hitsdPTE, missesdPTE);
     
     printf("TLB: \n");
     for(int i = 0 ; i < lengthdTLB ; i++){
     if(dTLB[i].valid == true)
     printf("-> PPN: %d, VPN: %d, LRU: %d\n", dTLB[i].PPN, dTLB[i].VPN * pageSizedMemory, dTLB[i].recentlyUsedCycle);
     else    break;
     }
     
     printf("PTE: \n");
     for(int i = 0 ; i < lengthdPTE ; i++){
     if(dPTE[i].valid == true)
     printf("-> VPN: %d, PPN: %d\n", i * pageSizedMemory, dPTE[i].PPN);
     else    continue;
     }*/
    
    
    int offset = VA % pageSizedMemory;
    int PA = dPTE[VA / pageSizedMemory].PPN * pageSizedMemory + offset;
    int indexCache = (PA / blockSizedCache) % lengthdCache;
    int tag = (PA / blockSizedCache) / lengthdCache;
    
    /*printf("dCache - cycle: %d, hits: %d, misses: %d, VA: %X, tag = %d\n", cycle, hitsdCache, missesdCache, VA, tag);
     for(int i = 0 ; i < lengthdCache ; i++){
     for(int j = 0 ; j < setdCache ; j++){
     if(dCache[i][j].valid == true){
     printf("-> [%d][%d], MRU = %d, tag = %d\n", i, j, dCache[i][j].MRU, dCache[i][j].tag);
     }
     }
     }*/
    
    for(int j = 0 ; j < setdCache ; j++){
        //Cache hit
        if(dCache[indexCache][j].tag == tag && dCache[indexCache][j].valid == true){
            hitsdCache++;
            dCache[indexCache][j].MRU = 1;
            
            //flip detect
            bool flipSignalCache = true;
            for(int k = 0 ; k < setdCache ; k++){
                if(dCache[indexCache][k].MRU == 0 || dCache[indexCache][k].valid == false){
                    flipSignalCache = false;
                    break;
                }
            }
            
            if(setdCache == 1){
                dCache[indexCache][0].MRU = 0;
            }
            else if(flipSignalCache == true){
                for(int k = 0 ; k < setdCache ; k++){
                    if(k != j){
                        dCache[indexCache][k].MRU = 0;
                    }
                }
            }
            
            return;
            
        }
    }
    
    
    //Cache miss
    missesdCache++;
    
    //find any invalid block
    for(int j = 0 ; j < setdCache ; j++){
        if(dCache[indexCache][j].valid == false){
            dCache[indexCache][j].tag = tag;
            dCache[indexCache][j].valid = true;
            dCache[indexCache][j].MRU = 1;
            
            //flip detect
            bool flipSignalCache = true;
            for(int k = 0 ; k < setdCache ; k++){
                if(dCache[indexCache][k].MRU == 0 || dCache[indexCache][k].valid == false){
                    flipSignalCache = false;
                    break;
                }
            }
            
            if(setdCache == 1){
                dCache[indexCache][0].MRU = 0;
            }
            else if(flipSignalCache == true){
                for(int k = 0 ; k < setdCache ; k++){
                    if(k != j){
                        dCache[indexCache][k].MRU = 0;
                    }
                }
            }
            
            return;
            
        }
    }
    
    
    //no invalid block -> implement bits-pseudo LRU
    for(int j = 0 ;j < setdCache ; j++){
        if(dCache[indexCache][j].MRU == 0){
            dCache[indexCache][j].tag = tag;
            dCache[indexCache][j].valid = true;
            dCache[indexCache][j].MRU = 1;
            
            bool flipSignalCache = true;
            for(int k = 0 ; k < setdCache ; k++){
                if(dCache[indexCache][k].MRU == 0 || dCache[indexCache][k].valid == false){
                    flipSignalCache = false;
                    break;
                }
            }
            
            if(setdCache == 1){
                dCache[indexCache][0].MRU = 0;
            }
            else if(flipSignalCache == true){
                for(int k = 0 ; k < setdCache ; k++){
                    if(k != j){
                        dCache[indexCache][k].MRU = 0;
                    }
                }
            }
            
            return;
        }
        
    }
    
    return;
    
}

void CacheMemoryPagetable::printCMP()
{
    fprintf(report, "ICache :\n# hits: %d\n# misses: %d\n\n", hitsiCache, missesiCache);
    fprintf(report, "DCache :\n# hits: %d\n# misses: %d\n\n", hitsdCache, missesdCache);
    fprintf(report, "ITLB :\n# hits: %d\n# misses: %d\n\n", hitsiTLB, missesiTLB);
    fprintf(report, "DTLB :\n# hits: %d\n# misses: %d\n\n", hitsdTLB, missesdTLB);
    fprintf(report, "IPageTable :\n# hits: %d\n# misses: %d\n\n", hitsiPTE, missesiPTE);
    fprintf(report, "DPageTable :\n# hits: %d\n# misses: %d\n\n", hitsdPTE, missesdPTE);
    
    return;
    
}

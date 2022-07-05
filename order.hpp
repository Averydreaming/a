#ifndef ORDER_HPP
#define ORDER_HPP
#include <bits/stdc++.h>
using namespace std;
enum Order_Name{
    LUI,AUIPC,ADD,SUB,SLL,SLT,SLTU,XOR,SRL,SRA,OR,AND,
	ADDI,SLTI,SLTIU,XORI,ORI,ANDI,SLLI,SRLI,SRAI,JAL,JALR,
	LB,LH,LW,LBU,LHU,
    SB,SH,SW,
	BEQ,BNE,BLT,BGE,BLTU,BGEU,
	END
};
class Order{
	public:
    Order_Name o=END;
    unsigned rd,rs1,rs2,imm=0;
	Order(){};
    Order(Order_Name _o,unsigned _rd,unsigned _rs1,unsigned _rs2,unsigned _imm):o(_o),rd(_rd),rs1(_rs1),rs2(_rs2),imm(_imm){};
};
int k;
// 0 clac 1 load 2 store 3 branch
unsigned judge_order(Order_Name x) {
    if (x==LUI||x==AUIPC||x==ADD||x==SUB||x==SLL||x==SLT||x==SLTU||x==XOR||x==SRL||x==SRA||x==OR||x==AND||x==ADDI||x==SLTI||x==SLTIU||x==XORI||x==ORI||x==ANDI||x==SLLI||x==SRLI||x==SRAI) return 0; 
    if (x==LB||x==LH||x==LW||x==LBU||x==LHU) return 1;
    if (x==SB||x==SH||x==SW) return 2;
    if (x==BEQ||x==BNE||x==BLT||x==BGE||x==BLTU||x==BGEU||x==JAL||x==JALR) return 3;
	return 4;
}
Order Instruction_Decode(unsigned x) {
    k++;
   // cout<<"!!"<<x<<endl;
    Order a;//cout<<"imm"<<a.imm<<endl;
	if (x==0x0ff00513) { a.o=END; return a;}
    int op=x&127;
    int op1=(x>>12)&7;
    int op2=(x>>25)&127;
    a.rd=(x>>7)&31;
	a.rs1=(x>>15)&31;
	a.rs2=(x>>20)&31;
    
   // if (k==175) cout<<op<<' '<<op1<<' '<<op2<<endl;
    //if (x==14026291) cout<<op<<' '<<op1<<' '<<op2<<endl;
    if (op==0b0000011)   //Il 
    {   
		
        switch (op1) {
        case 0b000: a.o=LB; break;
        case 0b001: a.o=LH; break;
        case 0b010: a.o=LW; break;
        //case 0b011: a.o=LD;
        case 0b100: a.o=LBU; break;
        case 0b101: a.o=LHU; break;
        //case 0b110: a.o=LWU;
        }
        a.imm=(x>>20);
        if(a.imm>>11)a.imm|=0xfffff000;
    }//
    if (op==0b0010011)   //Io1
    {
        switch (op1) {
        case 0b000: a.o=ADDI; break;
        case 0b001: a.o=SLLI; break;
        case 0b010: a.o=SLTI; break;
        case 0b011: a.o=SLTIU; break;
        case 0b100: a.o=XORI; break;
        case 0b101: if (op2==0) a.o=SRLI; else a.o=SRAI; break;
        case 0b110: a.o=ORI; break;
        case 0b111: a.o=ANDI; break;
        }
        a.imm=(x>>20);
        if ((op1!=0b101 && op1!=0b001)&& (a.imm>>11))a.imm|=0xfffff000;
    }//
    /*if (op==0b0011011)   //Io2
    {
        case 0b000: a.o=ADDIW;
        case 0b001: a.o=SLLIW;
        case 0b101: if (op2==0b0000000) a.o=SRLIW; else a.o=SRALW;
    }*/
   // if (op==0B1100111)   { a.o=ADDIW;}
    //if (op==0B1110011)   //Io { if (x>>) a.o=SRLIW; else a.o=SRALW; }
     if (op==0b1100011)   {
        switch (op1) {
        case 0b000: a.o=BEQ; break;
        case 0b001: a.o=BNE; break;
        case 0b100: a.o=BLT; break;
        case 0b101: a.o=BGE; break;
        case 0b110: a.o=BLTU; break;
        case 0b111: a.o=BGEU; break;
        }
        a.imm=(((x>>7)&0x1)<<11) | (((x>>8)&0xf)<<1) | (((x>>25)&0x3f)<<5)  | (((x>>31)&1)<<12);
        if(a.imm>>12)a.imm|=0xffffe000;
    }

    if (op==0b0110011)   //R1
    {
        switch (op1) {
        case 0b000: if(op2==0) a.o=ADD; else a.o=SUB; break;
        case 0b001: a.o=SLL; break;
        case 0b010: a.o=SLT; break;
        case 0b011: a.o=SLTU; break;
        case 0b100: a.o=XOR;  break;
        case 0b101: if (op2==0b0000000) a.o=SRL; else a.o=SRA; break;
        case 0b110: a.o=OR; break;
        case 0b111: a.o=AND;  break;
        }
        //if (x==14026291) cout<<a.imm<<endl;
    }
    if (op==0b0010111)   { a.o=AUIPC;a.imm=(x>>12)<<12;}
    if (op==0b0110111)   { a.o=LUI; a.imm=(x>>12)<<12;}
    if (op==0b1101111)   
    { 
        a.o=JAL;	
        a.imm=(((x>>12)&0xff)<<12) | (((x>>20)&0x1)<<11) | (((x>>21)&0x3ff)<<1)  | (((x>>31)&1)<<20);
        if(a.imm>>20)a.imm|=0xfff00000;
    }
    if (op==0b1100111)   { a.o=JALR; a.imm=(x>>20);	if(a.imm>>11)a.imm|=0xfffff000;}//
    if (op==0b0100011)   {
		//cout<<"!!!"<<op1<<endl;
        switch (op1) {
        case 0b000: a.o=SB; break;
        case 0b001: a.o=SH; break;
        case 0b010: a.o=SW; break;
        }
        a.imm=((x>>25)<<5) | ((x>>7)&31);
        if(a.imm>>11)a.imm|=0xfffff000;
    }
    //cout<<x<<' '<<a.imm<<endl;
	return a;
}


#endif
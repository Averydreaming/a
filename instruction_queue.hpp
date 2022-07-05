#ifndef INSTRUCTION_QUEUE_HPP
#define INSTRUCTION_QUEUE_HPP
#include<bits/stdc++.h>
#include"order.hpp"
#include"container.hpp"
unsigned get_pc_Num(unsigned x) {
  return (unsigned)mem[x]+((unsigned)mem[x+1]<<8)+((unsigned)mem[x+2]<<16)+((unsigned)mem[x+3]<<24);
}
bool BranchJudge(int x){
	if(BHT[x].l[0]==0)return 0;
	return 1;
}

void Instruction_Fetch(){
	if(IQ.size==32)return;
    unsigned x1=get_pc_Num(pc);
	Order order=Instruction_Decode(x1);
	if(order.o==END){next_fle=1;return;}
	Insturction_Queue_node x;
	x.inst=x1,x.o=order.o,x.pc=pc;
	if(judge_order(order.o)==3 && order.o!=JAL && order.o!=JALR){ x.jumppc=pc+order.imm; if(BranchJudge(x.inst&0xfff))x.isjump=1; else x.isjump=0;}
	next_IQ.rear=(IQ.rear+1)%32;
	next_IQ.l[next_IQ.rear]=x;
	next_IQ.size++;
	if(judge_order(order.o)!=3){ next_pc=pc+4; return;}
	if(judge_order(order.o)==3){
		if(order.o==JAL) {next_pc=pc+order.imm; return;}
		if(order.o==JALR) {next_pc=pc+4; return;}
		if(BranchJudge(x.inst&0xfff))next_pc=pc+order.imm;else next_pc=pc+4;
	}
}

void Instruction_Send(){
	if(IQ.size==0 || ROB.size==32)return;
	Insturction_Queue_node x=IQ.l[IQ.front];
	next_IQ.front=(IQ.front+1)%32;
	next_IQ.size--;
	Order order=Instruction_Decode(x.inst);
	if(judge_order(x.o)==1||judge_order(x.o)==2){ //进入ROB SLB
		if(SLB.size==32)return;
		next_SLB.rear=(SLB.rear+1)%32,next_SLB.size++;
		next_ROB.rear=(ROB.rear+1)%32,next_ROB.size++;
		next_ROB.l[next_ROB.rear].pc=x.pc;
		next_ROB.l[next_ROB.rear].inst=x.inst;
		next_ROB.l[next_ROB.rear].o=x.o;
		next_ROB.l[next_ROB.rear].dest=order.rd;
		next_ROB.l[next_ROB.rear].ready=0;
		if(reg[order.rs1].busy){
			if(next_ROB.l[reg[order.rs1].reorder].ready)next_SLB.l[next_SLB.rear].vj=next_ROB.l[reg[order.rs1].reorder].value,next_SLB.l[next_SLB.rear].qj=-1;
			else next_SLB.l[next_SLB.rear].qj=reg[order.rs1].reorder;
		}else next_SLB.l[next_SLB.rear].vj=reg[order.rs1].reg,next_SLB.l[next_SLB.rear].qj=-1;

		if(judge_order(x.o)==2){
			if(reg[order.rs2].busy){
				if(next_ROB.l[reg[order.rs2].reorder].ready)next_SLB.l[next_SLB.rear].vk=next_ROB.l[reg[order.rs2].reorder].value,next_SLB.l[next_SLB.rear].qk=-1;
				else next_SLB.l[next_SLB.rear].qk=reg[order.rs2].reorder;
			}   else next_SLB.l[next_SLB.rear].vk=reg[order.rs2].reg,next_SLB.l[next_SLB.rear].qk=-1;
		}
		else next_SLB.l[next_SLB.rear].qk=-1;
		next_SLB.l[next_SLB.rear].inst=x.inst;
		next_SLB.l[next_SLB.rear].o=x.o;
		next_SLB.l[next_SLB.rear].pc=x.pc;
		next_SLB.l[next_SLB.rear].A=order.imm;
		next_SLB.l[next_SLB.rear].reorder=next_ROB.rear;
		if(judge_order(x.o)==2)next_SLB.l[next_SLB.rear].ready=0;
		
		if(judge_order(x.o)!=2){next_reg[order.rd].reorder=next_ROB.rear,next_reg[order.rd].busy=1;}
		return;
	}
	if(judge_order(x.o)==0||judge_order(x.o)==3){  
		int pos=-1;
		for(int i=0;i<32;i++) if(!RS.l[i].busy){pos=i;break;}
		if(pos==-1)return;
		int b=(ROB.rear+1)%32;
		next_ROB.rear=b,next_ROB.size++;
		next_ROB.l[b].inst=x.inst;
		next_ROB.l[b].o=x.o;
		next_ROB.l[b].pc=x.pc;
		next_ROB.l[b].jumppc=x.jumppc;
		next_ROB.l[b].isjump=x.isjump;
		next_ROB.l[b].dest=order.rd;
		next_ROB.l[b].ready=0;

		if( (x.inst&0x7f)!=0x37&&(x.inst&0x7f)!=0x17 && (x.inst&0x7f)!=0x6f ){
			if(reg[order.rs1].busy){
				unsigned h=reg[order.rs1].reorder;
				if(next_ROB.l[h].ready){
					next_RS.l[pos].vj=next_ROB.l[h].value,next_RS.l[pos].qj=-1;
				}
				else next_RS.l[pos].qj=h;
			}
			else next_RS.l[pos].vj=reg[order.rs1].reg,next_RS.l[pos].qj=-1;
		}
		else next_RS.l[pos].qj=-1;

		if( (x.inst&0x7f)==0x33 || (x.inst&0x7f)==0x63){
			if(reg[order.rs2].busy){
				unsigned h=reg[order.rs2].reorder;
				if(next_ROB.l[h].ready)next_RS.l[pos].vk=next_ROB.l[h].value,next_RS.l[pos].qk=-1;
				else next_RS.l[pos].qk=h;
			}
			else next_RS.l[pos].vk=reg[order.rs2].reg,next_RS.l[pos].qk=-1;
		}
		else next_RS.l[pos].qk=-1;
	    next_RS.l[pos].o=x.o;
		next_RS.l[pos].pc=x.pc;
		next_RS.l[pos].jumppc=x.jumppc;
		next_RS.l[pos].A=order.imm;
		next_RS.l[pos].ROB_pos=b;
		next_RS.l[pos].busy=1;


		if((x.inst&0x7f)!=0x63){next_reg[order.rd].reorder=b,next_reg[order.rd].busy=1;}
	}
}
#endif 
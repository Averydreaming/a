#ifndef REORDERBUFFER_HPP
#define REORDERBUFFER_HPP
#include<bits/stdc++.h>

#include"order.hpp"
#include"container.hpp"
using namespace std;
/*
class ReorderBuffer_node{
puROB.frontlic:
	Order_Name o;
	unsigned inst,dest,value;
	int pc,jumppc;
	ROB.frontool isjump;
	ROB.frontool ready;
};
class ReorderBuffer{
puROB.frontlic:
	ReorderBuffer_node l[32];
	int front=1,rear=0;
	int size=0;
}ROB,next_ROB;
ready表示已经运算完 等待commit
inst-标志这条原始指令信息
Dest-目的寄存器
*/
void deal_ReorderBuffer(){
	if(!ROB.size)return;
	if(judge_order(ROB.l[ROB.front].o)<2){//Load or calc
		if(!ROB.l[ROB.front].ready)return;
		next_ROB.size--,next_ROB.front=(ROB.front+1)%32;
		int rd=ROB.l[ROB.front].dest;
		next_reg[rd].reg=ROB.l[ROB.front].value;
		if(reg[rd].busy&&reg[rd].reorder==ROB.front)next_reg[rd].busy=0;
		return;
	}

	if(judge_order(ROB.l[ROB.front].o)==2){
		if(!ROB.l[ROB.front].ready){
			for(int i=0;i<32;i++){
				if(SLB.l[i].reorder==ROB.front){
					next_SLB.l[i].ready=1;
				}
			}
			return;
		}
		next_ROB.size--,next_ROB.front=(ROB.front+1)%32;
		return;
	}
	
	if(judge_order(ROB.l[ROB.front].o)==3){
		if(!ROB.l[ROB.front].ready)return;
		next_ROB.size--,next_ROB.front=(ROB.front+1)%32;
		if(ROB.l[ROB.front].o==JAL){
			int rd=ROB.l[ROB.front].dest;
			next_reg[rd].reg=ROB.l[ROB.front].value;
			if(reg[rd].busy&&reg[rd].reorder==ROB.front)next_reg[rd].busy=0;
		}
		else {
			if( (ROB.l[ROB.front].value^ROB.l[ROB.front].isjump)==1 || ROB.l[ROB.front].o==JALR){
				int x=ROB.l[ROB.front].dest&0xfff;
				if(BHT[x].l[0]==0&&BHT[x].l[1]==0)next_BHT[x].l[0]=0,next_BHT[x].l[1]=1;
				if(BHT[x].l[0]==0&&BHT[x].l[1]==1)next_BHT[x].l[0]=1,next_BHT[x].l[1]=0;
				if(BHT[x].l[0]==1&&BHT[x].l[1]==0)next_BHT[x].l[0]=0,next_BHT[x].l[1]=1;
				if(BHT[x].l[0]==1&&BHT[x].l[1]==1)next_BHT[x].l[0]=1,next_BHT[x].l[1]=0;
				update_BHT_id=x;
				if(ROB.l[ROB.front].value)next_pc=ROB.l[ROB.front].jumppc;
				else next_pc=ROB.l[ROB.front].pc+4;
				flc=1;
				if(ROB.l[ROB.front].o==JALR){
					int rd=ROB.l[ROB.front].dest;
					next_reg[rd].reg=ROB.l[ROB.front].value;
					if(reg[rd].busy&&reg[rd].reorder==ROB.front)next_reg[rd].busy=0;
				}
				return;
			}
			else {
				int x=ROB.l[ROB.front].dest&0xfff;
				if(BHT[x].l[0]==0&&BHT[x].l[1]==0)next_BHT[x].l[0]=0,next_BHT[x].l[1]=0;
				if(BHT[x].l[0]==0&&BHT[x].l[1]==1)next_BHT[x].l[0]=0,next_BHT[x].l[1]=0;
				if(BHT[x].l[0]==1&&BHT[x].l[1]==0)next_BHT[x].l[0]=1,next_BHT[x].l[1]=1;
				if(BHT[x].l[0]==1&&BHT[x].l[1]==1)next_BHT[x].l[0]=1,next_BHT[x].l[1]=1;
				update_BHT_id=x;
				return;
			}
		}
	}
}
#endif 
#include <bits/stdc++.h>
#include"order.hpp"
#include"container.hpp"
#include"reorderbuffer.hpp"
#include"calc_execution.hpp"
#include"reservationstation.hpp"
#include"storeload_execution.hpp"
#include"storeloadbuffer.hpp"
#include"instruction_queue.hpp"
using namespace std;
int get_num(char c) { if ((c<='9') && (c>='0')) return c-'0'; else return c-'A'+10; }
void init_read() {
    char s[15];
    unsigned num=0;
    while (~scanf("%s",s)) {
        if (s[0]=='@') {
            for (int i=1;i<=8;i++)
            num=(num<<4)+(get_num(s[i]));
           // cout<<"1"<<' '<<num<<endl;
        } else {
            mem[num]=(get_num(s[0])<<4)|(get_num(s[1]));
            num++;
           //cout<<"2"<<' '<<mem[num]<<endl;
        }
    }
}
/*
reg
IQ
ROB
SLB
RS
BHT
fl
*/
void deal_Clear(){
	for(int i=0;i<32;i++)next_reg[i].busy=0;//reg
	next_IQ.front=1,next_IQ.rear=0,next_IQ.size=0;//IQ
	next_ROB.front=1,next_ROB.rear=0,next_ROB.size=0;
	for(int i=0;i<32;i++)next_ROB.l[i].ready=0;//ROB
	next_SLB.front=1,next_SLB.rear=0,next_SLB.size=0;Sclock=0;//SLB
	for(int i=0;i<32;i++)next_RS.l[i].busy=0;//RS
	next_fle=0;
}

void deal_Copy(){
	pc=next_pc;
	for(int i=0;i<32;i++)reg[i]=next_reg[i]; reg[0].reg=0,reg[0].busy=0;
	IQ=next_IQ;
	ROB=next_ROB;
	SLB=next_SLB;
	RS=next_RS;
	BHT[update_BHT_id]=next_BHT[update_BHT_id];
	fle=next_fle;
	flc=0;
}
bool judge_end() {
	return (fle&&IQ.size==0&&ROB.size==0);
}
int main(){
   // freopen("1.data","r",stdin);
	init_read();
	while(1){
		Clock++;
		//deal_Instruction_Queue();
		Instruction_Fetch();
		deal_ReorderBuffer();
		deal_ReservationStation();
		deal_StoreLoadBuffer();
		Instruction_Send();
		if(flc) deal_Clear();
		deal_Copy();
		if(judge_end()) break;
	}
	printf("%u\n",reg[10].reg&255u);
	return 0;
}

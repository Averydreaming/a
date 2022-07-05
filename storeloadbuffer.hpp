#ifndef STORELOADBUFFER_HPP
#define STORELOADBUFFER_HPP
#include"order.hpp"
#include"container.hpp"
#include"storeload_execution.hpp"
using namespace std;
void deal_StoreLoadBuffer(){
	if(Sclock==0) {
		if(!SLB.size)return;
		if(judge_order(SLB.l[SLB.front].o)==1){ if(SLB.l[SLB.front].qj==-1){ Sclock=3;} }
		if(judge_order(SLB.l[SLB.front].o)==2){ if(SLB.l[SLB.front].qj==-1&&SLB.l[SLB.front].qk==-1&&SLB.l[SLB.front].ready) Sclock=3;}
		return;
	}
	Sclock--;
	if(Sclock==0){
			if(judge_order(SLB.l[SLB.front].o)==1){
				
				unsigned value; storeload_execution(SLB.l[SLB.front],value);
				int q=SLB.l[SLB.front].reorder;
				//SLB
				next_SLB.size--;
				next_SLB.front=(SLB.front+1)%32;
				for(int j=0;j<32;j++){
					if(SLB.l[j].qj==q)next_SLB.l[j].qj=-1,next_SLB.l[j].vj=value;
					if(SLB.l[j].qk==q)next_SLB.l[j].qk=-1,next_SLB.l[j].vk=value;
				}
				//ROB
				next_ROB.l[q].value=value;
				next_ROB.l[q].ready=1;

				// RS
				for(int j=0;j<32;j++){
					if(RS.l[j].busy){
						if(RS.l[j].qj==q)next_RS.l[j].qj=-1,next_RS.l[j].vj=value;
						if(RS.l[j].qk==q)next_RS.l[j].qk=-1,next_RS.l[j].vk=value;
					}
				}

			}
			else {
				next_ROB.l[SLB.l[SLB.front].reorder].ready=1;
                unsigned value; storeload_execution(SLB.l[SLB.front],value);
				next_SLB.size--,next_SLB.front=(SLB.front+1)%32;
			}
	}
	return;
}
	
#endif
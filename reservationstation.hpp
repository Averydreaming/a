#ifndef RESERVATIONSTATION_HPP
#define RESERVATIONSTATION_HPP
#include<bits/stdc++.h>

#include"order.hpp"
#include"container.hpp"
#include"calc_execution.hpp"
using namespace std;
/*
/*
class ReservationStation_node{
public:
	
	Order_Name o;
	bool busy;
	unsigned vj,vk,qj,qk,A,ROB_pos;
	unsigned pc,jumppc;
};
class ReservationStation{
public:
	ReservationStation_node l[32];
}RS,next_RS;
o 操作类型
busy表示代表的指令是否已经处理
qj=-1 表示 vj已经存在
qk=-1 表示 vk已经存在
ROB_pos 该指令在ROB中位置
A - 立即数

用于解决计算操作的乱序问题 数据早
update 
IQ
SLB
ROB
RS
reg

*/
//done
void deal_ReservationStation(){
	for(int i=0;i<32;i++){
		if(RS.l[i].busy&&RS.l[i].qj==-1&&RS.l[i].qk==-1){
			next_RS.l[i].busy=0;
			int q=RS.l[i].ROB_pos;

			unsigned value,jumppc;
			calc_execution(RS.l[i],value,jumppc);
			//ROB
			
			next_ROB.l[q].value=value;
			next_ROB.l[q].ready=1;
			if(RS.l[i].o==JALR)next_ROB.l[q].jumppc=jumppc;
			
			//RS
			for(int j=0;j<32;j++){
				if(RS.l[j].busy){
					if(RS.l[j].qj==q) next_RS.l[j].qj=-1,next_RS.l[j].vj=value;
					if(RS.l[j].qk==q) next_RS.l[j].qk=-1,next_RS.l[j].vk=value;
				}
			}

			// SLB
			for(int j=0;j<32;j++){
				if(SLB.l[j].qj==q)next_SLB.l[j].qj=-1,next_SLB.l[j].vj=value;
				if(SLB.l[j].qk==q)next_SLB.l[j].qk=-1,next_SLB.l[j].vk=value;
			}
			break;
		}
	}
}
#endif 
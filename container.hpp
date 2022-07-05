#ifndef CONTAINER_HPP
#define CONTAINER_HPP
#include"order.hpp"
/*
内存
reg

IQ
SLB
ROB
RS

*/
int Clock=0,OKnum=0;
int predictTot=0,predictSuccess=0;
unsigned next_pc,pc;
bool flc=0;
bool fle,next_fle;

unsigned char mem[500000];
class Register{
public:
	unsigned reg,reorder;//reorder 当前这个时刻该寄存器的值应该是哪条指令的结果
	//reg 表示当前时刻要被计算出 必须之前计算出什么
	bool busy;/*是否已经被使用（被使用的定义就是在ROB中还未被commit） busy=1表示还在使用中 */
}reg[32],next_reg[32];


class Insturction_Queue_node{
public:
	unsigned inst;
	Order_Name o;
	unsigned pc,jumppc;
	bool isjump;
};
class Insturction_Queue{
public:
	Insturction_Queue_node l[32];
	int front=1,rear=0;
	int size=0;
}IQ,next_IQ;


class ReorderBuffer_node{
public:
	Order_Name o;
	unsigned inst,dest,value;
	int pc,jumppc;
	bool isjump;
	bool ready;
};
class ReorderBuffer{
public:
	ReorderBuffer_node l[32];
	int front=1,rear=0;
	int size=0;
}ROB,next_ROB;


class StoreLoadBuffer_node{
public:
	Order_Name o;
	unsigned inst,pc,vj,vk,qj,qk,A,reorder;
	bool ready;
};
class StoreLoadBuffer{
public:
	StoreLoadBuffer_node l[32];
	int front=1,rear=0;
	int size=0;
}SLB,next_SLB;
unsigned Sclock=0;


class ReservationStation_node{
public:
	Order_Name o;
	unsigned vj,vk,qj,qk,A,ROB_pos;
	unsigned pc,jumppc;
	bool busy;
};
class ReservationStation{
public:
	ReservationStation_node l[32];
}RS,next_RS;



class Branch_History_Table{
public:
	bool l[2];
}BHT[1<<12],next_BHT[1<<12];
int update_BHT_id;
#endif
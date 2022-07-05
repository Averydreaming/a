#ifndef CALC_EXECUTION_HPP
#define CALC_EXECUTION_HPP
void calc_execution(ReservationStation_node x,unsigned &value,unsigned &jumppc){
	switch (x.o)
	{
	case LUI:value=x.A;break;
	case AUIPC:value=x.pc+x.A;break;
	case ADD:value=x.vj+x.vk;break;
	case SUB:value=x.vj-x.vk;break;
	case SLL:value=x.vj<<(x.vk&31);break;
	case SLT:value=((int)x.vj<(int)x.vk);break;
	case SLTU:value=(x.vj<x.vk);break;
	case XOR:value=x.vj^x.vk;break;
	case SRL:value=x.vj>>(x.vk&31);break;
	case SRA:value=(int)x.vj>>(x.vk&31);break;
	case OR:value=x.vj|x.vk;break;
	case AND:value=x.vj&x.vk;break;
	case JALR:jumppc=(x.vj+x.A)&(~1);value=x.pc+4;break;
	case ADDI:value=x.vj+x.A;break;
	case SLTI:value=((int)x.vj<(int)x.A);break;
	case SLTIU:value=(x.vj<x.A);break;
	case XORI:value=x.vj^x.A;break;
	case ORI:value=x.vj|x.A;break;
	case ANDI:value=x.vj&x.A;break;
	case SLLI:value=x.vj<<x.A;break;
	case SRLI:value=x.vj>>x.A;break;
	case SRAI:value=(int)x.vj>>x.A;break;
	case JAL:value=x.pc+4;break;
	case BEQ:value=(x.vj==x.vk);break;
	case BNE:value=(x.vj!=x.vk);break;
	case BLT:value=((int)x.vj<(int)x.vk);break;
	case BGE:value=((int)x.vj>=(int)x.vk);break;
	case BLTU:value=(x.vj<x.vk);break;
	case BGEU:value=(x.vj>=x.vk);break;
	}
}

#endif
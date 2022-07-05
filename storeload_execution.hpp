#ifndef STORELOAD_EXECUTION_HPP
#define STORELOAD_EXECUTION_HPP
//done 
void storeload_execution(StoreLoadBuffer_node x,unsigned int &value)
{
    int t=x.vj+x.A;
    switch (x.o){
    case LB:value=(char)mem[t];break;
	case LH:value=(short)((unsigned short)mem[t]+((unsigned short)mem[t+1]<<8));break;
	case LW:value=(int)((unsigned int)mem[t]+((unsigned int)mem[t+1]<<8)+((unsigned int)mem[t+2]<<16)+((unsigned int)mem[t+3]<<24));break;
	case LBU:value=mem[t];break;
	case LHU:value=(unsigned short)mem[t]+((unsigned short)mem[t+1]<<8);break;
    case SB:mem[t]=x.vk&255;break;
	case SH:mem[t]=x.vk&255,mem[t+1]=(x.vk>>8)&255;break;
	case SW:mem[t]=x.vk&255,mem[t+1]=(x.vk>>8)&255,mem[t+2]=(x.vk>>16)&255,mem[t+3]=(x.vk>>24)&255;break;
    }
}
#endif 
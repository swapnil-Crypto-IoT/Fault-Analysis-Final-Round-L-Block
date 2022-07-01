#include<stdio.h>

unsigned long long int s8[16] = {8,7,14,5,15,13,0,6,11,12,9,10,2,4,1,3};
unsigned long long int s9[16] = {11,2,15,0,7,2,9,13,4,8,1,12,14,10,3,6};

int main()
{
   unsigned long long int a = 0x1023456789abcdef,b=0x1023;
   unsigned long long int t,temp,result;
   unsigned int i;
   t=0;
   
   for(i=0;i<5;i++)
   {
   	t=a;
   	a<<= 29;
   	b=b<<(29);	
   	b|= t>>(64-29);
   	t=(b&0xffffffff0000)>>16;
   	a|=t;
   	t=a;
   	a&=0x00ffffffffffffff;
	a|=(s8[(t&0xf000000000000000)>>60] <<60);
	a|=(s9[(t&0x0f00000000000000)>>56] <<56);
	t = ((t>>30)&0x1f)^(i);
	printf("\n %016llx this is t\n",t);
	a &= 0xfffffff83fffffff;
	a|=t<<30;
	b&=0xffff;
   	printf("\n %016llx %04llx\n",a,b);
   }
    
   result = ((a&0xffff)<<16)|(b&0x0000ffff);
   
   printf("\n %08llx",result);
}  



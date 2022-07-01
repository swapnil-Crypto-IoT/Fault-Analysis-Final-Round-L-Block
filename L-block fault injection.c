#include<stdio.h>

/*-----------------------ALL SBOX FROM 0-9--------------------------*/
unsigned long int s0[16] = {14,9,15,0,13,4,10,11,1,2,8,3,7,6,12,5};
unsigned long int s1[16] = {4,11,14,9,15,13,0,10,7,12,5,6,2,8,1,3};
unsigned long int s2[16] = {1,14,7,12,15,13,0,6,11,5,9,3,2,4,8,10};
unsigned long int s3[16] = {7,6,8,11,0,15,3,14,9,10,12,13,5,2,4,1};
unsigned long int s4[16] = {14,5,15,0,7,2,12,13,1,8,4,9,11,10,6,3};
unsigned long int s5[16] = {2,13,11,12,15,14,0,9,7,10,6,3,1,8,4,5};
unsigned long int s6[16] = {11,9,4,14,0,15,10,13,6,12,5,7,3,8,1,2};
unsigned long int s7[16] = {13,10,15,0,14,4,9,11,2,1,8,3,7,5,12,6};
unsigned long long int s8[16] = {8,7,14,5,15,13,0,6,11,12,9,10,2,4,1,3};
unsigned long long int s9[16] = {11,5,15,0,7,2,9,13,4,8,1,12,14,10,3,6};
/*-------------------------------------------------------------------*/
unsigned long int r[33];

/*-----------------------LEFT CIRCULAR SHIFT BY 8 FUNCTION-----------*/
unsigned long int LCS_8(unsigned long int a)
{
	
	a=((a<<8)^a>>(32-8));
	return(a&0xffffffff);
}

/*-----------------------SBOX SUBSTITUTION--------------------------*/
unsigned long int sbox(unsigned long int a)
{
	unsigned int i,t;
	unsigned long int result=0;
	for (i=0;i<8;i++)
	{
		if(i==0)
		result |= s0[(a>>(i*4))&0xf]<<(i*4);
		
		if(i==1)
		result |= s1[(a>>(i*4))&0xf]<<(i*4);
		
		if(i==2)
		result |= s2[(a>>(i*4))&0xf]<<(i*4);
		
		if(i==3)
		result |= s3[(a>>(i*4))&0xf]<<(i*4);
		
		if(i==4)
		result |= s4[(a>>(i*4))&0xf]<<(i*4);
		
		if(i==5)
		result |= s5[(a>>(i*4))&0xf]<<(i*4);
		
		if(i==6)
		result |= s6[(a>>(i*4))&0xf]<<(i*4);
		
		if(i==7)
		result |= s7[(a>>(i*4))&0xf]<<(i*4);
	}
	return (result);
}

/*-----------------------PLAYER DIFFUSION--------------------------*/
unsigned long int Pbox(unsigned long int a)
{
	unsigned long int t1=0;
	
	t1 |= (a>>4)&0x000f000f;
	t1 |= (a<<4)&0xf000f000;
	t1 |= (a>>8)&0x00f000f0;
	t1 |= (a<<8)&0x0f000f00;
	return(t1);
}

/*----------------------KEY SCHEDULING ALGO--------------------------*/
unsigned long int *key_data(unsigned long long int a, unsigned long long int b)
{
   unsigned long long int t=0;
   unsigned int i;
   	
   	for (i=1;i<33;i++)
   	{
	 		t=a;
		   	a<<= 29;
		   	b=b<<(29);	
		   	b|= t>>(64-29);
		   	a|=(b&0xffffffff0000)>>16;
		   	t=a;
		   	a&=0x00fffff83fffffff;
			a|=(s9[(t&0xf000000000000000)>>60] <<60);
			a|=(s8[(t&0x0f00000000000000)>>56] <<56);
			t = ((t>>30)&0x1f)^(i);
			a|=t<<30;
			b&=0xffff;
		   	r[i] = (a&0xffffffff00000000)>>32;
		   	
		   //	printf("\n r[%d] %lx\n",i,r[i]);
	
	}
   	return(r);	
}


/*-----------------------MAIN FUNCTION-------------------------*/
int main()
{
	unsigned long int xl=0x0,xr=0x0,t1,x;
	unsigned long long int a = 0x0,b=0x0;
	unsigned int r_c,i;
	
	unsigned long int data1,data2;
	unsigned long int shifter[8]={0xfffffff0,0xffffff0f,0xfffff0ff,0xffff0fff,0xfff0ffff,0xff0fffff,
								  0xf0ffffff,0x0fffffff};
	printf("\n ***64 bit plaintext*** \n\n %08lx %08lx\n",xl,xr);
	
	printf("\n ***80 bit key*** \n\n %016llx %04llx\n",a,b);
	
	key_data(a,b);
	for(i=0;i<8;i++)
	{
		xl=0x0,xr=0x0;
		a = 0x0,b=0x0;
		r[0]= (a&0xffffffff00000000)>>32;
		for (r_c=0;r_c<32;r_c++)
		{
			xr = LCS_8(xr);
			x=xl;
			x^=r[r_c];
	        x=sbox(x);
			x = Pbox(x);
			xr ^= x;
			//printf("\n the round %02d output is : %08lx %08lx",r_c+1,xl,xr);
			if(r_c!=31)
			{
			t1=xl;
			xl=xr;
			xr=t1;
			}
		}
		
		data1 = xl;
		//printf("\n the data without fault injection is %08lx\n",data1);
		
	/*-------------Injecting byte fault---------------*/
	
	 //printf("\n\n--- Injecting byte fault ---\n\n");
		xl=0x0,xr=0x0,t1,x;
		a = 0x0,b=0x0;
		
		r[0]= (a&0xffffffff00000000)>>32;
		for (r_c=0;r_c<32;r_c++)
		{
			xr = LCS_8(xr);
			x=xl;
			x^=r[r_c];
			if(r_c==28)
			//printf("\n shifted value is %08lx\n",shifter[i]);
			//---------------
			x = x&shifter[i];
			//--------------
	        x=sbox(x);
			x = Pbox(x);
			xr ^= x;
			//printf("\n the round %02d output is : %08lx %08lx",r_c+1,xl,xr);
			if(r_c!=31)
			{
			t1=xl;
			xl=xr;
			xr=t1;
			}
		}	
		data2 = xl;
		//printf("\n the data with fault injection is %08lx\n",data1);
		
		printf("\n %d) fault %08lx xor %08lx ---> %08lx \n",i+1,data1,data2,data1^data2);	
	}
}

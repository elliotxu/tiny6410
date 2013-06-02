#include "nand_8k.h"

#define MEM_SYS_CFG     (*((volatile unsigned long *)0x7E00F120))
#define NFCONF          (*((volatile unsigned long *)0x70200000))
#define NFCONT          (*((volatile unsigned long *)0x70200004))
#define NFCMMD          (*((volatile unsigned long *)0x70200008))
#define NFADDR          (*((volatile unsigned long *)0x7020000C))
#define NFDATA          (*((volatile unsigned char *)0x70200010))
#define NFSTAT          (*((volatile unsigned long *)0x70200028))

#define NAND_DISABLE_CE()	(NFCONT |= (1 << 1))
#define NAND_ENABLE_CE()	(NFCONT &= ~(1 << 1))
#define NF_TRANSRnB()		do { while(!(NFSTAT & (1 << 0))); } while(0)

#define NAND_CMD_RESET		0xff
#define NAND_CMD_READID		0x90
#define NAND_CMD_READ0		0
#define NAND_CMD_READSTART	0x30

void nand_select(void)
{
	NFCONT &= ~(1<<1);
}

void nand_deselect(void)
{
	NFCONT |= (1<<1);
}


void nand_cmd(u8 cmd)
{
	NFCMMD = cmd;
}

void nand_addr(u8 addr)
{
	NFADDR = addr;
}

u8 nand_get_data(void)
{
	return NFDATA;
}

void nand_send_data(u8 data)
{
	NFDATA = data;
}

void wait_ready(void)
{
	while ((NFSTAT & 0x1) == 0);
}

void nand_reset(void)
{
	/* 选中 */
	nand_select();
	
	/* 发出0xff命令 */
	nand_cmd(0xff);

	/* 等待就绪 */
	wait_ready();
	
	/* 取消选中 */
	nand_deselect();
}


void nand_init(void)
{
	/* 让xm0csn2用作nand flash 的片选引脚 */
	MEM_SYS_CFG &= ~(1<<1);

	/* 设置时间参数:hclk = 7.5ns */
#define TACLS     0
#define TWRPH0    3//3
#define TWRPH1    1//1
	NFCONF &= ~((1<<30) | (7<<12) | (7<<8) | (7<<4));	//对相应位进行清零
	NFCONF |= ((TACLS<<12) | (TWRPH0<<8) | (TWRPH1<<4));

	/* 使能nand flash controller */
	NFCONT |= 1;

	nand_reset();
}


//8192
void nand_send_addr(u32 addr)
{
#if 0	
	u32 page   = addr / (8*1024);
	u32 colunm = addr & ((8*1024) - 1);

	/* 这两个地址表示从页内哪里开始 */
	nand_addr(colunm & 0xff);
	nand_addr((colunm >> 8) & 0xff);

	/* 下面三个地址表示哪一页 */
	nand_addr(page & 0xff);
	nand_addr((page >> 8) & 0xff);
	nand_addr((page >> 16) & 0xff);
#else
	//这个很容易理解错误，因为要屏蔽掉ECC的那些存储空间
	nand_addr(addr & 0xff);         /* a0~a7 */
	nand_addr((addr >> 8) & 0x1f);   /* 程序的角度: a8~a12 */

	nand_addr((addr >> 13) & 0xff); /* 程序的角度: a13~a20 */
	nand_addr((addr >> 21) & 0xff); /* 程序的角度: a21~a28 */
	nand_addr((addr >> 29) & 0x07); /* 程序的角度: a29~a31 */
	
#endif
}


/*
get NAND flash detail info 
*/
void nand_info(void)
{
	s32 i;
	u8 Buf[6];
	nand_select();
	nand_cmd(0x90);
	nand_send_addr(0x00);
	wait_ready();
	for(i=0;i<6;i++)
	{
		Buf[i]=nand_get_data();
	}
	printf("NAND info\r\n");
	//Maker code
	printf("Maker code:\t0x%02X\r\n",Buf[0]);
	//Device code
	printf("Device code:\t0x%02X\r\n",Buf[1]);
	//internal chip number
	i=Buf[2]&0x03;
	switch(i)
	{
		case 0:i=1;
			break;
		case 1:i=2;
			break;
		case 2:i=4;
			break;
		case 3:i=8;
			break;
	}
	printf("internal chip number:%d\r\n",i);
	//cell type
	i=(Buf[2]&(0x3<<2))>>2;
	switch(i)
	{
		case 0:i=2;
			break;
		case 1:i=4;
			break;
		case 2:i=8;
			break;
		case 3:i=16;
			break;
	}
	printf("cell type:%d\r\n",i);
	//Programmed Pages
	i=(Buf[2]&(0x3<<4))>>4;
	switch(i)
	{
		case 0:i=1;
			break;
		case 1:i=2;
			break;
		case 2:i=4;
			break;
		case 3:i=8;
			break;
	}
	printf("Programmed Pages:%d \r\n",i);
	//Interleave Operation
	printf("Interleave Operation:");
	i=Buf[2]&(0x1<<6);
	if(i>0) printf("Support\r\n");
	else 	printf("Not Support\r\n");
	// Cache Operation
	i=Buf[2]&(0x1<<7);
	printf("Cache Operation:");
	if(i>0) printf("Support\r\n");
	else 	printf("Not Support\r\n");
	//page size
	i=Buf[3]&(0x03);
	printf("page size:");
	switch(i)
	{
		case 0:i=2;
			printf("%d KB\r\n",i);
			break;
		case 1:i=4;
			printf("%d KB\r\n",i);
			break;
		case 2:i=8;
			printf("%d KB\r\n",i);
			break;
		case 3:
			printf("unknow\r\n");
			break;
	}
	//block size
	printf("block size:");
	i=Buf[3]&(0x01<<7);
	if(i>0) 
	{
		printf("unknow\r\n");
	}
	else
	{
		i=(Buf[3]&(0x03<<4))>>4;
		switch(i)
		{
			case 0:	printf("128KB\r\n");
				break;
			case 1:printf("256KB\r\n");
				break;
			case 2:printf("512KB\r\n");
				break;
			case 3:printf("1MB\r\n");
				break;
				default :printf("err\r\n");
		}
	}
	//Redundant Area Size
	printf("Redundant Area Size:");
	i=Buf[3]&(0x01<<6);
	if(i)
	{
		i=Buf[3]&(0x03<<2);
		if(i==0)printf("436B\r\n");
		else printf("unknow\r\n");
	}
	else
	{	
		i=(Buf[3]&(0x03<<2))>>2;
		switch(i)
		{
			case 0:	printf("unknow\r\n");
				break;
			case 1:printf("128B\r\n");
				break;
			case 2:printf("218B\r\n");
				break;
			case 3:printf("400B\r\n");
				break;
		}
	
	}
	//plane number
	printf("plane number:");
	i=(Buf[4]&(0x03<<3))>>3;
	switch(i)
	{
		case 0:	printf("1\r\n");
			break;
		case 1:printf("2\r\n");
			break;
		case 2:printf("4\r\n");
			break;
		case 3:printf("8\r\n");
			break;
	}
	//ECC level
	printf("ECC level:");
	i=(Buf[4]&(0x07<<4))>>4;
	switch(i)
	{
		case 0:	printf("1bit/512B\r\n");
			break;
		case 1:printf("2bit/512B\r\n");
			break;
		case 2:printf("4bit/512B\r\n");
			break;
		case 3:printf("8bit/512B\r\n");
			break;
		case 4:printf("16bit/512B\r\n");
			break;
		case 5:printf("24bit/1KB\r\n");
			break;
		case 6:
		case 7:
		printf("unknow\r\n");
		break;
	}
	//Device Version
	printf("Device Version:");
	i=Buf[4]&(0x07);
	if(i==0) printf("50 nm\r\n");
	else if(i==1) printf("40 nm\r\n");
	else if(i==2) printf("30 nm\r\n");
	else printf("unknow\r\n");
	//EDO
	printf("EDO:");
	i=Buf[4]&(0x01<<6);
	if(i>0) printf("Support\r\n");
	else 	printf("Not Support\r\n");
	//Interface
	printf("Interface:");
	i=Buf[4]&(0x01<<7);
	if(i>0) printf("DDR\r\n");
	else 	printf("SDR\r\n");
}

void nand_read(u32 nand_start, u32 ddr_start, u32 len)
{
	u32 addr = nand_start;
	s32 i, count = 0;
	u8 *dest = (u8 *)ddr_start;
	
	/* 选中芯片 */
	nand_select();

	while (count < len)
	{
		/* 发出命令0x00 */
		nand_cmd(0x00);

		/* 发出地址 */
		nand_send_addr(addr);

		/* 发出命令0x30 */
		nand_cmd(0x30);

		/* 等待就绪 */
		wait_ready();

		/* 连续读数据 */
		for (i = 0; i < (8*1024) && count < len; i++)
		{
			dest[count++] = nand_get_data();
		}

		addr += (8*1024);			
	}

	/* 取消片选 */
	nand_deselect();

}
void nand_write_page(u8 *buf, u32 PageAddr)
{
	u32 i;
	
	NAND_ENABLE_CE();  // 选中nand
	nand_cmd(0x80);
	/* Write Address */
	NFADDR = 0;
	NFADDR = 0;

	NFADDR = (PageAddr) & 0xff;
	NFADDR = (PageAddr >> 8) & 0xff;
	NFADDR = (PageAddr >> 16) & 0xff;
	
	for(i=0;i<(8*1024);i++)
	{
		nand_send_data(buf[i]);
	}
	nand_cmd(0x10);
	wait_ready();
}
void nand_read_page (u8 *buf, u32 PageAddr)
{
	s32 i;
//	int page_size = 512;
//	if (large_block==2)
//	    page_size = 4096;
	s32 page_size = (8*1024);
	
	NAND_ENABLE_CE();  // 选中nand

	NFCMMD = 0;

	/* Write Address */
	NFADDR = 0;
	NFADDR = 0;

	NFADDR = (PageAddr) & 0xff;
	NFADDR = (PageAddr >> 8) & 0xff;
	NFADDR = (PageAddr >> 16) & 0xff;

	NFCMMD = 0x30;

	//NF_TRANSRnB();
	wait_ready();
	
	for(i=0; i < page_size; i++) 
	{
	    *buf++ = NFDATA;
	}

	NAND_DISABLE_CE();

}

void nand_erase_block(u32 BlockAddr)
{
	//s32 page = addr / (8*1024);
	
	nand_select();
	nand_cmd(0x60);
	//需要输入page地址，但是page是起作用的，只有block起作用
//	nand_addr(page & 0xff);
//	nand_addr((page >> 8) & 0xff);
//	nand_addr((page >> 16) & 0xff);
//Block地址A21~A32
nand_addr((BlockAddr&0x01)<<7);// Block bit0
nand_addr((BlockAddr>>1)&0xff);// Block bit1~8
nand_addr((BlockAddr>>9)&0x0f);// Block最低位

	nand_cmd(0xd0);
	wait_ready();

	nand_deselect();
}

void nand_write(u32 nand_start, u8 * buf, u32 len)
{
	u8 temp;
	u32 count = 0;
	u32 addr  = nand_start;
	u32 i = nand_start % (8*1024);
	
	nand_select();
	while (count < len)
	{
		nand_cmd(0x80);
		nand_send_addr(addr);

		for (; i < (8*1024) && count < len; i++)
		{
			nand_send_data(buf[count++]);
			addr++;
		}

		nand_cmd(0x10);
		wait_ready();			
		i = 0;			
	}

	nand_deselect();

}

int copy2ddr(u32 nand_start, u32 ddr_start, u32 len)
{
	u8 *dest = (u8 *)ddr_start;
	u32 free_page = 0;
	s32 i;

	if(len > 8192)
	{
		//free_page = ((len - 8192)+4096) >> 12;
		free_page=(len-8*1024)/(8*1024)+1;	//计算除了前面8K还有多少page需要copy
	}
	/* 初始化nand flash controller */
//	nand_init();
	
	/* 读nand flash */
	/* 拷贝前面的8K ，注意这里虽然每次读出8K数据，但是dest+=2048其实只保留了2K而已其余的会被
	下一次拷贝数据时覆盖掉*/
	for (i = 0; i < 4; i++, dest+=2048)
	{
		nand_read_page(dest, i);
	}

	/* 拷贝8K以后的数据 */
	for (i = 4; i < (free_page+4); i++, dest+=8192) 
	{
		nand_read_page(dest, i);
	}
	
	return 0;
}


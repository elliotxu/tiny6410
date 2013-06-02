#include "basetype.h"
#include <stdio.h>
#include <nand_8k.h>
#include "uart.h"
#include "lib/printf.h"
#include "lib/string.h"

void nand_read_test(void)
{

	int i;
	char buf[1024*8];
	unsigned int addr;
	unsigned int size;
	
	printf("enter the start address: ");
	scanf("%s", buf);
	addr = strtoul(buf, NULL, 0);
	printf("read addr = 0x%x\n\r", addr);

	printf("enter the size: ");
	scanf("%s", buf);
	size = strtoul(buf, NULL, 0);

	if (size > (1024*8))
	{
		printf("the max size is 100\n\r");
		size = (1024*8);
	}

	nand_read(addr, (unsigned int)buf, size);

	printf("datas: \n\r");
	for (i = 0; i < size; i++)
	{
		printf("%02x ", buf[i]);
		if ((i+1) % 16 == 0)
		{
			printf("\n\r");
		}
	}
	printf("\n\r");
}

void nand_erase_test(void)
{
	char buf[100];
	unsigned long addr;
	
	printf("enter the start address: ");
	scanf("%s", buf);
	addr = strtoul(buf, NULL, 0);
	printf("erase addr = 0x%x\n\r", addr);

	nand_erase_block(addr);
}

void nand_write_test(void)
{
	char buf[100];
	unsigned long addr;
	unsigned long size;
	
	printf("enter the start address: ");
	scanf("%s", buf);
	addr = strtoul(buf, NULL, 0);

	printf("enter the string: ");
	scanf("%s", buf);

	size = strlen(buf) + 1;

	nand_write(addr, (unsigned char *)buf, size);
}


void update_program(void)
{
	u8 *buf = (u8 *)0x52000000;
	u32 len = 0;
	s32 have_begin = 0;
	s32 nodata_time = 0;
	u32 erase_addr;
	u32 blockNum;
	char c;
	u32 i;
	u32 temp;
	u32 index=0;

	/* 读串口获得数据 */
	printf("\n\ruse V2.2.exe/gtkterm to send file\n\r", len);
	while (1)
	{
		if (getc_nowait(&buf[len]) == 0)
		{
			have_begin = 1;
			nodata_time = 0;
			len++;
		}
		else
		{
			if (have_begin)
			{
				nodata_time++;
			}			
		}

		if (nodata_time == 1000)
		{
			break;
		}
	}
	printf("have get %d bytes data\n\r", len);
	printf("the first 16 bytes data: \n\r");
	for (i = 0; i < 16; i++)
	{
		printf("%02x ", buf[i]);
	}
	printf("\n\r");

	printf("Press Y to program the flash: \n\r");

	c = getc();
	
	if (c == 'y' || c == 'Y')
	{	
		//test
		/*for(i=0;i<len;i++)
		{
			if((i%16)==0)printf("\r\n");
			printf("%02X ",buf[i]);
		}*/
		/* 烧写到nand flash block 0 */
		blockNum=len>>20;//因为从0地址开始所以len也就是最后的地址
		for (erase_addr = 0; erase_addr < blockNum; erase_addr ++)
		{
			nand_erase_block(erase_addr);
		}
		printf("len=%d\r\n",len);
		//由于6410的特点，自动拷贝8k数据到SRAM是从page0~page3的前2k数据组成的。
		//判断有多少个2k数据
		if((len<=(2*1024))) //len<=2K
		{
		printf("page0\r\n");
			nand_write((0*8*1024), buf, len);							//写到page0
		}
		else if((len>(2*1024)) && (len<=(4*1024)))// 2K<len<=4K
		{
		printf("page0~1\r\n");
			nand_write((0*8*1024), buf, (2*1024));				//写到page0
			nand_write((1*8*1024), &buf[2*1024], len-(2*1024));	//写到page1
		}
		else if((len>(4*1024)) && (len<=(6*1024)))// 4K<len<=6K
		{
		printf("page0~2\r\n");
			nand_write((0*1*8*1024), buf, (2*1024));			//写到page0
			nand_write((1*8*1024), &buf[2*1024], (2*1024));		//写到page1
			nand_write((2*8*1024), &buf[4*1024], len-(4*1024));	//写到page2
		}
		else if((len>(6*1024)) && (len<=(8*1024)))// 6K<len<=8K
		{
		printf("page0~3\r\n");
			nand_write((0*8*1024), buf, (2*1024));				//写到page0
			nand_write((1*8*1024), &buf[2*1024], (2*1024));		//写到page1
			nand_write((2*8*1024), &buf[4*1024], (2*1024));		//写到page2
			nand_write((3*8*1024), &buf[6*1024], len-(6*1024));	//写到page3
		}
		else if(len>(8*1024))// len>8K
		{
		printf("page0~3...\r\n");
			nand_write((0*8*1024), buf, (2*1024));				//写到page0
			nand_write((1*8*1024), &buf[2*1024], (2*1024));		//写到page1
			nand_write((2*8*1024), &buf[4*1024], (2*1024));		//写到page2
			nand_write((3*8*1024), &buf[6*1024], (2*1024));		//写到page3

			nand_write((4*8*1024), &buf[8*1024], len-(8*1024));

		}

		//nand_write(0, buf, len);
		
		printf("update program successful\n\r");
	}
	else
	{
		printf("Cancel program!\n\r");
	}
}
void run_program(void)
{
	unsigned char *buf = (unsigned char *)0x52000000;
	unsigned long len = 0;
	int have_begin = 0;
	int nodata_time = 0;
	void (*theProgram)(void);

	/* 读串口获得数据 */
	printf("\n\ruse gtkterm to send file\n\r", len);
	while (1)
	{
		if (getc_nowait(&buf[len]) == 0)
		{
			have_begin = 1;
			nodata_time = 0;
			len++;
		}
		else
		{
			if (have_begin)
			{
				nodata_time++;
			}			
		}

		if (nodata_time == 1000)
		{
			break;
		}
	}
	printf("have get %d bytes data\n\r", len);

	printf("jump to 0x52000000 to run it\n\r");
	
	theProgram = (void (*)(void))0x52000000;

	theProgram();		
}
u8 IdBuf[6];
u8 PageBuf[8*1024];
int main(void)
{
	char c;
	u32 i;
	u32 addr;
	u32 len;
	
	init_uart();
	printf("can update program with serial port\n\r");

	while (1)
	{
		printf("[w] write the nand flash\n\r");
		printf("[r] read the nand flash\n\r");
		printf("[e] erase the nand flash\n\r");
		printf("[g] get file, and write to nand flash 0 block\n\r");
		printf("[x] get file to ddr(0x52000000), run it\n\r");
		printf("[i] get nand flash ID\r\n");
		printf("[P] read nand page \r\n");
		printf("[P] write page \r\n");
		printf("[n] write nand test \r\n");
		do 
		{
			c = getc();
			if (c == '\n' || c == '\r')
			{
				printf("\n\r");
			}
			else
			{
				printf("%c\r\n",c);
			}
		} while (c == '\n' || c == '\r');
		
		switch (c)
		{
			case 'w':
			case 'W':
			{
				nand_write_test();
				break;
			}

			case 'r':
			case 'R':
			{
				nand_read_test();
				break;
			}

			case 'e':
			case 'E':
			{
				//nand_erase_test();
				printf("enter erase block addr: ");
				scanf("%d", &addr);
				printf("\r\n");
				printf("erase block addr = %d\n\r", addr);
				nand_erase_block(addr);
				break;
			}

			case 'g':
			case 'G':
			{
				update_program();
				break;
			}

			case 'x':
			case 'X':
			{
				run_program();
				break;
			}
			case 'i':
			case 'I':
			{
				nand_info();
				break;
			}
			case 'p':
			{
				printf("enter read page address: ");
				scanf("%d", &addr);
				printf("\r\n");
				printf("read page addr = %d\n\r", addr);
				
				nand_read_page(PageBuf,addr);
				for(i=0;i<sizeof(PageBuf);i++)
				{
					if(i%16==0)printf("\r\n");
					printf("%02X ",PageBuf[i]);				
				}
				printf("\r\n");
				
				break;
			}
			case 'P':
			{
				printf("enter write page address: ");
				scanf("%d", &addr);
				printf("\r\n");

				printf("write page addr = 0x%x\n\r", addr);
				for(i=0;i<(8*1024);i++) PageBuf[i]=i&0xff;	
				nand_write_page(PageBuf,addr);

				break;
			}
			case 'n':
			{
				printf("enter write nand start addr: ");
				scanf("%d", &addr);
				printf("\r\n");
				
				printf("enter write nand len: ");
				scanf("%d", &len);
				printf("\r\n");
				printf("enter write nand start addr:%d\r\n",addr);
				printf("enter write nand len:%d\r\n",len);
				nand_write(addr,0,len);
				break;
			}
		}
	}
	
	return 0;
}


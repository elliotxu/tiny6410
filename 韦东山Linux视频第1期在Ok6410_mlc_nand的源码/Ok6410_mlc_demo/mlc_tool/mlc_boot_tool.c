#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
	FILE		*fp;    /* 定义一个文件指针 */
	unsigned char		*Buf, *a;
	int		BufLen;
	int		nbytes, fileLen, curPosition;
	unsigned int	checksum, count;
	int		i;

	if (argc != 3) /* 如果参数个数错误，打印帮助信息 */
	{
		/* 应用工具时,格式必须是 ./mlc_boot_tool.exe old.bin new.bin */
		printf("Usage: mkbl1 <source file> <destination file>\n");
		return -1;
	}

	fp = fopen(argv[1], "rb"); /* 以读二进制的方式打开没有头部信号的old.bin文件 */
	if( fp == NULL)
	{
		printf("source file open error\n");
		return -1;
	}

	fseek(fp, 0L, SEEK_END); /* 让文件位置指针指向文件末尾，便于下行的统计大小的操作 */
	fileLen = ftell(fp); /* 用于得到文件位置指针当前位置相对于文件首的偏移字节数,即文件大小*/

	BufLen = fileLen + 8*1024;
	Buf = (char *)malloc(BufLen); /* 动态分配一段24k的内存空间 */
	if (!Buf) /* 分配失败，将返回0 */
	{
		printf("Alloc buffer failed!\n");
		return -1;
	}

	memset(Buf, 0x00, BufLen); /* 将上面分配的空间清零 */

	if(fileLen < 2048)
	{
		printf("source file too small !! do not need the tool !!\n");
		free(Buf); /* 释放内存 */
		return -1;
	}
	
	fseek(fp, 0L, SEEK_SET); /* 让文件位置指针指向文件开始 */	
	nbytes = fread(Buf, 1, 2048, fp); /* 将编译生成的old.bin文件拷贝前2K数据到到buf中 */
	if ( nbytes != 2048 ) /* 返回值等于拷贝的元素的个数 */
	{
		printf("0 page. source file read error\n"); /* 如果个数和实际的不想等，则失败 */
		free(Buf); /* 释放内存 */
		fclose(fp); /* 关闭文件 */
		return -1;
	}

	curPosition = ftell(fp);  /* 获得当前文件指针的位置 */
	if((fileLen - curPosition) < 2048)
	{
		if((fileLen - curPosition) > 0)
		{
			nbytes = fread(Buf + 4096, 1, (fileLen - curPosition), fp);
			if ( nbytes != (fileLen - curPosition) ) /* 返回值等于拷贝的元素的个数 */
			{
				printf("1 page. source file read error\n"); /* 如果个数和实际的不想等，则失败 */
				free(Buf); /* 释放内存 */
				fclose(fp); /* 关闭文件 */
				return -1;
			}
		}
	}
	else
	{
		nbytes = fread(Buf + 4096, 1, 2048, fp);
		if ( nbytes != 2048 ) /* 返回值等于拷贝的元素的个数 */
		{
			printf("1 page. source file read error\n"); /* 如果个数和实际的不想等，则失败 */
			free(Buf); /* 释放内存 */
			fclose(fp); /* 关闭文件 */
			return -1;
		}
	}

	curPosition = ftell(fp);  /* 获得当前文件指针的位置 */
	if((fileLen - curPosition) < 2048)
	{
		if((fileLen - curPosition) > 0)
		{
			nbytes = fread(Buf + 8192, 1, (fileLen - curPosition), fp);
			if ( nbytes != (fileLen - curPosition) ) /* 返回值等于拷贝的元素的个数 */
			{
				printf("2 page. source file read error\n"); /* 如果个数和实际的不想等，则失败 */
				free(Buf); /* 释放内存 */
				fclose(fp); /* 关闭文件 */
				return -1;
			}
		}
	}
	else
	{
		nbytes = fread(Buf + 8192, 1, 2048, fp);
		if ( nbytes != 2048 ) /* 返回值等于拷贝的元素的个数 */
		{
			printf("2 page. source file read error\n"); /* 如果个数和实际的不想等，则失败 */
			free(Buf); /* 释放内存 */
			fclose(fp); /* 关闭文件 */
			return -1;
		}
	}

	curPosition = ftell(fp);  /* 获得当前文件指针的位置 */
	if((fileLen - curPosition) < 2048)
	{
		if((fileLen - curPosition) > 0)
		{
			nbytes = fread(Buf + 12288, 1, (fileLen - curPosition), fp);
			if ( nbytes != (fileLen - curPosition) ) /* 返回值等于拷贝的元素的个数 */
			{
				printf("3 page. source file read error\n"); /* 如果个数和实际的不想等，则失败 */
				free(Buf); /* 释放内存 */
				fclose(fp); /* 关闭文件 */
				return -1;
			}
		}
	}
	else
	{
		nbytes = fread(Buf + 12288, 1, 2048, fp);
		if ( nbytes != 2048 ) /* 返回值等于拷贝的元素的个数 */
		{
			printf("3 page. source file read error\n"); /* 如果个数和实际的不想等，则失败 */
			free(Buf); /* 释放内存 */
			fclose(fp); /* 关闭文件 */
			return -1;
		}
	}


	curPosition = ftell(fp);  /* 获得当前文件指针的位置 */
	if((fileLen - curPosition) <= 0)
	{
	}
	else
	{
		nbytes = fread(Buf + 16384, 1, fileLen - 8192, fp);
		if ( nbytes != fileLen - 8192 ) /* 返回值等于拷贝的元素的个数 */
		{
			printf("4 free. source file read error\n"); /* 如果个数和实际的不想等，则失败 */
			free(Buf); /* 释放内存 */
			fclose(fp); /* 关闭文件 */
			return -1;
		}
	}
	fclose(fp); /* 关闭文件 */
	
	/* 到这里为止，把新的bin的数据已经构造完成，现在要把这些数据写入到新的bin文件中 */
	fp = fopen(argv[2], "wb"); /* 以二进制写的方式创建一个新的二进制文件 */
	if (fp == NULL)
	{
		printf("destination file open error\n");
		free(Buf); /* 释放内存 */
		return -1;
	}

	a = Buf; /* 指向内存的首地址 */
	nbytes = fwrite( a, 1, BufLen, fp); /* 把buf中的数据写入新创建的bin文件 */

	if ( nbytes != BufLen ) /* 返回值等于写入的元素的个数 */
	{
		printf("destination file write error\n");
		free(Buf); /* 释放内存 */
		fclose(fp);/* 关闭文件 */
		return -1;
	}

	free(Buf); /* 释放内存 */
	fclose(fp);/* 关闭文件 */

	return 0;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
	FILE		*fp;    /* ����һ���ļ�ָ�� */
	unsigned char		*Buf, *a;
	int		BufLen;
	int		nbytes, fileLen, curPosition;
	unsigned int	checksum, count;
	int		i;

	if (argc != 3) /* ��������������󣬴�ӡ������Ϣ */
	{
		/* Ӧ�ù���ʱ,��ʽ������ ./mlc_boot_tool.exe old.bin new.bin */
		printf("Usage: mkbl1 <source file> <destination file>\n");
		return -1;
	}

	fp = fopen(argv[1], "rb"); /* �Զ������Ƶķ�ʽ��û��ͷ���źŵ�old.bin�ļ� */
	if( fp == NULL)
	{
		printf("source file open error\n");
		return -1;
	}

	fseek(fp, 0L, SEEK_END); /* ���ļ�λ��ָ��ָ���ļ�ĩβ���������е�ͳ�ƴ�С�Ĳ��� */
	fileLen = ftell(fp); /* ���ڵõ��ļ�λ��ָ�뵱ǰλ��������ļ��׵�ƫ���ֽ���,���ļ���С*/

	BufLen = fileLen + 8*1024;
	Buf = (char *)malloc(BufLen); /* ��̬����һ��24k���ڴ�ռ� */
	if (!Buf) /* ����ʧ�ܣ�������0 */
	{
		printf("Alloc buffer failed!\n");
		return -1;
	}

	memset(Buf, 0x00, BufLen); /* ���������Ŀռ����� */

	if(fileLen < 2048)
	{
		printf("source file too small !! do not need the tool !!\n");
		free(Buf); /* �ͷ��ڴ� */
		return -1;
	}
	
	fseek(fp, 0L, SEEK_SET); /* ���ļ�λ��ָ��ָ���ļ���ʼ */	
	nbytes = fread(Buf, 1, 2048, fp); /* ���������ɵ�old.bin�ļ�����ǰ2K���ݵ���buf�� */
	if ( nbytes != 2048 ) /* ����ֵ���ڿ�����Ԫ�صĸ��� */
	{
		printf("0 page. source file read error\n"); /* ���������ʵ�ʵĲ���ȣ���ʧ�� */
		free(Buf); /* �ͷ��ڴ� */
		fclose(fp); /* �ر��ļ� */
		return -1;
	}

	curPosition = ftell(fp);  /* ��õ�ǰ�ļ�ָ���λ�� */
	if((fileLen - curPosition) < 2048)
	{
		if((fileLen - curPosition) > 0)
		{
			nbytes = fread(Buf + 4096, 1, (fileLen - curPosition), fp);
			if ( nbytes != (fileLen - curPosition) ) /* ����ֵ���ڿ�����Ԫ�صĸ��� */
			{
				printf("1 page. source file read error\n"); /* ���������ʵ�ʵĲ���ȣ���ʧ�� */
				free(Buf); /* �ͷ��ڴ� */
				fclose(fp); /* �ر��ļ� */
				return -1;
			}
		}
	}
	else
	{
		nbytes = fread(Buf + 4096, 1, 2048, fp);
		if ( nbytes != 2048 ) /* ����ֵ���ڿ�����Ԫ�صĸ��� */
		{
			printf("1 page. source file read error\n"); /* ���������ʵ�ʵĲ���ȣ���ʧ�� */
			free(Buf); /* �ͷ��ڴ� */
			fclose(fp); /* �ر��ļ� */
			return -1;
		}
	}

	curPosition = ftell(fp);  /* ��õ�ǰ�ļ�ָ���λ�� */
	if((fileLen - curPosition) < 2048)
	{
		if((fileLen - curPosition) > 0)
		{
			nbytes = fread(Buf + 8192, 1, (fileLen - curPosition), fp);
			if ( nbytes != (fileLen - curPosition) ) /* ����ֵ���ڿ�����Ԫ�صĸ��� */
			{
				printf("2 page. source file read error\n"); /* ���������ʵ�ʵĲ���ȣ���ʧ�� */
				free(Buf); /* �ͷ��ڴ� */
				fclose(fp); /* �ر��ļ� */
				return -1;
			}
		}
	}
	else
	{
		nbytes = fread(Buf + 8192, 1, 2048, fp);
		if ( nbytes != 2048 ) /* ����ֵ���ڿ�����Ԫ�صĸ��� */
		{
			printf("2 page. source file read error\n"); /* ���������ʵ�ʵĲ���ȣ���ʧ�� */
			free(Buf); /* �ͷ��ڴ� */
			fclose(fp); /* �ر��ļ� */
			return -1;
		}
	}

	curPosition = ftell(fp);  /* ��õ�ǰ�ļ�ָ���λ�� */
	if((fileLen - curPosition) < 2048)
	{
		if((fileLen - curPosition) > 0)
		{
			nbytes = fread(Buf + 12288, 1, (fileLen - curPosition), fp);
			if ( nbytes != (fileLen - curPosition) ) /* ����ֵ���ڿ�����Ԫ�صĸ��� */
			{
				printf("3 page. source file read error\n"); /* ���������ʵ�ʵĲ���ȣ���ʧ�� */
				free(Buf); /* �ͷ��ڴ� */
				fclose(fp); /* �ر��ļ� */
				return -1;
			}
		}
	}
	else
	{
		nbytes = fread(Buf + 12288, 1, 2048, fp);
		if ( nbytes != 2048 ) /* ����ֵ���ڿ�����Ԫ�صĸ��� */
		{
			printf("3 page. source file read error\n"); /* ���������ʵ�ʵĲ���ȣ���ʧ�� */
			free(Buf); /* �ͷ��ڴ� */
			fclose(fp); /* �ر��ļ� */
			return -1;
		}
	}


	curPosition = ftell(fp);  /* ��õ�ǰ�ļ�ָ���λ�� */
	if((fileLen - curPosition) <= 0)
	{
	}
	else
	{
		nbytes = fread(Buf + 16384, 1, fileLen - 8192, fp);
		if ( nbytes != fileLen - 8192 ) /* ����ֵ���ڿ�����Ԫ�صĸ��� */
		{
			printf("4 free. source file read error\n"); /* ���������ʵ�ʵĲ���ȣ���ʧ�� */
			free(Buf); /* �ͷ��ڴ� */
			fclose(fp); /* �ر��ļ� */
			return -1;
		}
	}
	fclose(fp); /* �ر��ļ� */
	
	/* ������Ϊֹ�����µ�bin�������Ѿ�������ɣ�����Ҫ����Щ����д�뵽�µ�bin�ļ��� */
	fp = fopen(argv[2], "wb"); /* �Զ�����д�ķ�ʽ����һ���µĶ������ļ� */
	if (fp == NULL)
	{
		printf("destination file open error\n");
		free(Buf); /* �ͷ��ڴ� */
		return -1;
	}

	a = Buf; /* ָ���ڴ���׵�ַ */
	nbytes = fwrite( a, 1, BufLen, fp); /* ��buf�е�����д���´�����bin�ļ� */

	if ( nbytes != BufLen ) /* ����ֵ����д���Ԫ�صĸ��� */
	{
		printf("destination file write error\n");
		free(Buf); /* �ͷ��ڴ� */
		fclose(fp);/* �ر��ļ� */
		return -1;
	}

	free(Buf); /* �ͷ��ڴ� */
	fclose(fp);/* �ر��ļ� */

	return 0;
}

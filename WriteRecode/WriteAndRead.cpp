#include "stdafx.h"
#include <stdlib.h>


// C语言读文件
void MyReadFile(const char *charFilePath)
{
	FILE *pfile = fopen(charFilePath, "rb");//打开文件，返回文件操作符
	char *pread;
	size_t result;
	if (pfile)  //打开文件一定要判断是否成功
	{
		fseek(pfile, 0, SEEK_END);   //将文件内部的指针指向文件末尾
		long lsize = ftell(pfile);   //获取文件长度，（得到文件位置指针当前位置相对于文件首的偏移字节数）
		rewind(pfile);               //将文件内部的指针重新指向一个流的开头
		pread = (char *)malloc(lsize * sizeof(char) + 1);//申请内存空间，lsize*sizeof(char)是为了更严谨，16位上char占一个字符，其他机器上可能变化

		//用malloc申请的内存是没有初始值的，如果不赋值会导致写入的时候找不到结束标志符而出现内存比实际申请值大，写入数据后面跟随乱码的情况
		memset(pread, 0, lsize * sizeof(char) + 1);//将内存空间都赋值为‘\0’

		result = fread(pread, 1, lsize, pfile);//将pfile中内容读入pread指向内存中
	}
	fclose(pfile); //关掉文件操作符，和句柄一样，有open就一定有close
	free(pread);   //释放内存
	pread = NULL;  //指针不再使用，一定要“删除”，防止产生野指针
}

//C语言写文件
void MyWriteFile(const char *charFilePath)
{
	FILE *fp;
	fp = fopen(charFilePath, "wb+");
	char str[20] = "hello world";
	fwrite(str, 20, 1, fp);

	fclose(fp);
}




//MFC 读文件
void MyReadFile2()
{
// 	CFile file(_T("aaa.png"), CFile::modeRead);
// 	char* pBuf;
// 	DWORD dwFileLen;
// 	dwFileLen = file.GetLength();
// 	pBuf = new char[dwFileLen + 1];
// 	pBuf[dwFileLen] = 0;
// 	file.Read(pBuf, dwFileLen);
// 	file.Close();
}
//MFC写文件
void MyWriteFile2(const char *charFilePath)
{
// 	CFile file2;
// 
// 	file2.Open("aaa.txt", CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate, NULL);
// 
// 	file2.Write(pBuf, (dwFileLen));
// 
// 	//Write( const void* lpBuf, UINT nCount )  lpBuf是写入数据的Buf指针，nCount是Buf里需要写入文件的字节数
// 
// 	file2.Close();

}


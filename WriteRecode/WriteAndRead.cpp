#include "stdafx.h"
#include <stdlib.h>


// C���Զ��ļ�
void MyReadFile(const char *charFilePath)
{
	FILE *pfile = fopen(charFilePath, "rb");//���ļ��������ļ�������
	char *pread;
	size_t result;
	if (pfile)  //���ļ�һ��Ҫ�ж��Ƿ�ɹ�
	{
		fseek(pfile, 0, SEEK_END);   //���ļ��ڲ���ָ��ָ���ļ�ĩβ
		long lsize = ftell(pfile);   //��ȡ�ļ����ȣ����õ��ļ�λ��ָ�뵱ǰλ��������ļ��׵�ƫ���ֽ�����
		rewind(pfile);               //���ļ��ڲ���ָ������ָ��һ�����Ŀ�ͷ
		pread = (char *)malloc(lsize * sizeof(char) + 1);//�����ڴ�ռ䣬lsize*sizeof(char)��Ϊ�˸��Ͻ���16λ��charռһ���ַ������������Ͽ��ܱ仯

		//��malloc������ڴ���û�г�ʼֵ�ģ��������ֵ�ᵼ��д���ʱ���Ҳ���������־���������ڴ��ʵ������ֵ��д�����ݺ��������������
		memset(pread, 0, lsize * sizeof(char) + 1);//���ڴ�ռ䶼��ֵΪ��\0��

		result = fread(pread, 1, lsize, pfile);//��pfile�����ݶ���preadָ���ڴ���
	}
	fclose(pfile); //�ص��ļ����������;��һ������open��һ����close
	free(pread);   //�ͷ��ڴ�
	pread = NULL;  //ָ�벻��ʹ�ã�һ��Ҫ��ɾ��������ֹ����Ұָ��
}

//C����д�ļ�
void MyWriteFile(const char *charFilePath)
{
	FILE *fp;
	fp = fopen(charFilePath, "wb+");
	char str[20] = "hello world";
	fwrite(str, 20, 1, fp);

	fclose(fp);
}




//MFC ���ļ�
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
//MFCд�ļ�
void MyWriteFile2(const char *charFilePath)
{
// 	CFile file2;
// 
// 	file2.Open("aaa.txt", CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate, NULL);
// 
// 	file2.Write(pBuf, (dwFileLen));
// 
// 	//Write( const void* lpBuf, UINT nCount )  lpBuf��д�����ݵ�Bufָ�룬nCount��Buf����Ҫд���ļ����ֽ���
// 
// 	file2.Close();

}


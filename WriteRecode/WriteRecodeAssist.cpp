#include "stdafx.h"
#include "WriteRecodeAssist.h"
#include <atlstr.h>
#include <io.h>
#include <string>    
#include <direct.h>
using namespace std;

#include <dbghelp.h>
#pragma comment(lib, "Dbghelp.lib")

#define  MAX_WRITELOG_BUFFER		 10000


/*
*  ��    �ܣ�����β��\r\n
*  ���������str   ������ַ���
*  ����ֵ  ����                                      
*/
void *fun_r_n(char * const str) 
{
	int i,j;
	for (i = 0; str[i];) 
	{
		if (str[i]=='\n'||str[i]=='\r' || str[i]=='\t' || str[i]==' ') 
		{
			for (j=i; str[j]; j++) 
			{
				str[j]=str[j+1];
			}
		}
		else i++;
	}
	return str;
}


/*
*  ��    �ܣ���ȡ�ı�ָ���е��ı�
*  ��    ����1.filename   �ļ�·��
*			 2.iHang	  ��ȡ�е�����
             3.StrLine    ָ���е�����
*  �� �� ֵ��BOOL                                      
*/
BOOL ReadSpeacialLine(char *filename, int iHang, char *StrLine) 
{ 
	FILE *fp; 
	int WhichLine = iHang;             //ָ��Ҫ��ȡ��һ��
	int CurrentIndex=0;             //��ǰ��ȡ����
	if((fp = fopen(filename,"r")) == NULL) //�ж��ļ��Ƿ���ڼ��ɶ�
	{ 
		OutputDebugMessageA("��ȡ�ı�ָ���е��ı� �򿪾��ʧ��!!!");
		return FALSE; 
	} 

	while (0 == feof(fp)) 
	{ 
		if (CurrentIndex == WhichLine)
		{
			fgets(StrLine,1024,fp);  //��ȡһ��

			fclose(fp);

			return TRUE;
		}
		fgets(StrLine,1024,fp);  //��ȡһ��,����λ����һ��
		CurrentIndex++;
	} 
	fclose(fp);                     //�ر��ļ�
	return FALSE; 
}
/*
* ��    �ܣ����������־��Ϣ      
* �����������ʽ������Ϣ       
* ����ֵ  ����
* �޸����ݣ�  
*/
void ErrorMessage(char* chForm, ...)
{
	char chBuffer[MAX_WRITELOG_BUFFER] = {0};

	if(0 >Safe_sprintf(chBuffer,MAX_WRITELOG_BUFFER,"[WriteRecode][%05d]: ",GetCurrentThreadId()))
	{
		ERRTO_LOG_RETURNNOVAL("WriteRecode","ErroMessage:XXX::Safe_sprintf")
	}

	int len = Safe_strlen(chBuffer);
	if (len < 0)
	{
		ERRTO_LOG_RETURNNOVAL("WriteRecode","ErroMessage:XXX::Safe_strlen")
	}

	va_list argptr;
	int cnt;
	va_start(argptr, chForm);
	cnt = Safe_vsprintf(chBuffer+len,MAX_WRITELOG_BUFFER-len,chForm, argptr);
	va_end(argptr);

	if (cnt < 0)
	{
		ERRTO_LOG_RETURNNOVAL("WriteRecode","ErroMessage:XXX::Safe_vsprintf")
	}

	ErrTo_log("ErrorMessage",chBuffer);
}

/*
* ��    �ܣ����������־��Ϣ
* ��	������ʽ������Ϣ
* �� �� ֵ����
*/
void ErrorMessageW(WCHAR* chForm, ...)
{
	WCHAR chBuffer[MAX_WRITELOG_BUFFER] = {0};

	if(0 >Safe_swprintf(chBuffer,MAX_WRITELOG_BUFFER,L"[WriteRecode][%05d]: ",GetCurrentThreadId()))
	{
		ERRTO_LOG_RETURNNOVAL("WriteRecode","ErroMessage:XXX::Safe_sprintf")
	}

	int len = Safe_wcslen(chBuffer);
	if (len < 0)
	{
		ERRTO_LOG_RETURNNOVAL("WriteRecode","ErroMessage:XXX::Safe_strlen")
	}

	va_list argptr;
	int cnt;
	va_start(argptr, chForm);
	cnt = Safe_vswprintf(chBuffer+len,MAX_WRITELOG_BUFFER-len,chForm, argptr);
	va_end(argptr);

	if (cnt < 0)
	{
		ERRTO_LOG_RETURNNOVAL("WriteRecode","ErroMessage:XXX::Safe_vsprintf")
	}

	ErrTo_logW(L"ErrorMessage",chBuffer);
}

/* 
* ��    �ܣ������־��Ϣ      
* �����������ʽ������Ϣ       
* ���������
* ����ֵ  ��                                        
*/
void OutputDebugMessageA(char* chForm, ...)
{
	char chBuffer[MAX_WRITELOG_BUFFER] = {0};

	if(0 >Safe_sprintf(chBuffer,MAX_WRITELOG_BUFFER,"[WriteRecode][%05d]: ",GetCurrentThreadId()))
	{
		ERRTO_LOG_RETURNNOVAL("WriteRecode","OutputDebugMessage:XXX::Safe_sprintf")
	}

	int len = Safe_strlen(chBuffer);
	if (len < 0)
	{
		ERRTO_LOG_RETURNNOVAL("WriteRecode","OutputDebugMessage:XXX::Safe_strlen")
	}

	va_list argptr;
	int cnt;
	va_start(argptr, chForm);
	cnt = Safe_vsprintf(chBuffer+len,MAX_WRITELOG_BUFFER-len,chForm, argptr);
	va_end(argptr);

	if (cnt < 0)
	{
		ERRTO_LOG_RETURNNOVAL("WriteRecode","OutputDebugMessage:XXX::Safe_vsprintf")
	}

	ErrTo_log("WriteRecode",chBuffer);
}

/* 
* ��    �ܣ������־��Ϣ      
* �����������ʽ������Ϣ       
* ���������
* ����ֵ  ��                                        
*/
void OutputDebugMessage(wchar_t* chForm, ...)
{
	wchar_t chBufferW[MAX_WRITELOG_BUFFER] = {0};
	if(0 > Safe_swprintf(chBufferW,MAX_WRITELOG_BUFFER,L"[WriteRecode][%05d]: ",GetCurrentThreadId()))
	{
		ERRTO_LOG_RETURNNOVAL("WriteRecode","OutputDebugMessageW:XXX::Safe_wcscpy")
	}
	int len = Safe_wcslen(chBufferW);

	if (len < 0)
	{
		ERRTO_LOG_RETURNNOVAL("WriteRecode","OutputDebugMessageW:XXX::Safe_strlen")
	}

	va_list argptr;
	int cnt;
	va_start(argptr, chForm);
	cnt = Safe_vswprintf(chBufferW+len,MAX_WRITELOG_BUFFER-len,chForm, argptr);
	va_end(argptr);

	if (cnt < 0)
	{
		ERRTO_LOG_RETURNNOVAL("WriteRecode","OutputDebugMessageW:XXX::Safe_vsprintf")
	}

	ErrTo_logW(L"WriteRecode",chBufferW);
}


/* 
* ��    �ܣ������־��Ϣ      
* �����������ʽ������Ϣ       
* ���������
* ����ֵ  ��                                        
*/
void OutputDebugMessageW(TCHAR* chForm, ...)
{
	TCHAR chBuffer[1000] = {0};
	if (NULL == Safe_wcscpy(chBuffer,1000,_T("[WriteRecode]: ")))
	{
		ERRTO_LOG_RETURNNOVAL("WriteRecode","OutputDebugMessage_1");
	}

	va_list argptr;
	
	int iLen = Safe_wcslen(_T("[WriteRecode]: "));
	if (iLen < 0)
	{
		ERRTO_LOG_RETURNNOVAL("WriteRecode","Safe_wcslen");
	}
	va_start(argptr, chForm);
	int cnt = _vstprintf(chBuffer + iLen,chForm, argptr);
	va_end(argptr);

	WCHAR chModule[512] = {0};
	if (NULL == Safe_wcscpy(chModule,512,_T("WriteRecode")))
	{
		ERRTO_LOG_RETURNNOVAL("WriteRecode","Safe_wcscpy");
	}
	ErrTo_logW(chModule, (wchar_t*)chBuffer);
}

/*
* ��    �ܣ������ļ��е�·���ж��ļ������Ƿ�Ϊ��
* ���������szPath     �ļ���·��
* ����ֵ  ��TRUE:EMPTY  FALSE:NO EMPTY                                       
*/
BOOL IsFolderEmpty(WCHAR* szPath)  
{  
	WCHAR szFind[MAX_PATH] = {0};  
	WIN32_FIND_DATA findFileData;  
	BOOL bRet = TRUE;  

	char chszPath[MAX_PATH] = {0};
	if (0>Safe_WtoA(szPath, chszPath, MAX_PATH))
	{
		ERRTO_LOG_RETURN("WriteRecode","IsFolderEmpty_0",FALSE);
	}

	int nlen = Safe_strlen(chszPath);
	if (0 > nlen)
	{
		OutputDebugMessageA("IsFolderEmpty �����ַ���ָ�����");
		return FALSE;
	}
	else if (nlen >0)
	{
		if (nlen <MAX_PATH)
		{
			if (NULL == Safe_wcscpy(szFind,MAX_PATH,szPath))
			{
				ERRTO_LOG_RETURN("WriteRecode","IsFolderEmpty_1",FALSE);
			}
		}
		else
		{
			OutputDebugMessageA("IsFolderEmpty szPath ������Χ");
			return FALSE;
		}
	}
	else if (nlen == 0)
	{
		OutputDebugMessageA("IsFolderEmpty �����ַ������ȵ���0");
	}

	int nlenPah = Safe_strlen(chszPath);
	if (0 > nlenPah)
	{
		OutputDebugMessageA("IsFolderEmpty �����ַ���ָ�����");
		return NULL;
	}
	else if (nlenPah >0)
	{
		if (nlenPah + 4<MAX_PATH)
		{
			if (NULL == Safe_wcscat(szFind,MAX_PATH,L"\\*.*"))     //����һ��Ҫָ��ͨ�������Ȼ�����ȡ�����ļ���Ŀ¼ 
			{
				ERRTO_LOG_RETURN("WriteRecode","IsFolderEmpty_3",FALSE);
			}
		}
		else
		{
			OutputDebugMessageA("IsFolderEmpty szPath �������ӷ�Χ");
			return FALSE;
		}
	}
	else if (nlenPah == 0)
	{
		OutputDebugMessageA("IsFolderEmpty �����ַ������ȵ���0");
	}

	HANDLE hFind = ::FindFirstFileW(szFind, &findFileData);  
	if (INVALID_HANDLE_VALUE == hFind)  
	{  
		return FALSE;  
	}  
	while (bRet)  
	{  
		//һ���ҵ�'.'��".."����Ϊ��  
		if (findFileData.cFileName[0] != L'.')  
		{  
			::FindClose(hFind);  
			return TRUE;  
		}  
		bRet = ::FindNextFileW(hFind, &findFileData);
	}  
	::FindClose(hFind);  
	return FALSE;  
}

/*
* ��    �ܣ��ж��ļ������Ƿ���ָ���ļ�
* ���������szPath      
* ����ֵ  �������ļ�����                                      
*/
WCHAR IsFolderHave(WCHAR* szPath, WCHAR *wchFilePath)  
{  
	WCHAR szFind[MAX_PATH] = {0};  
	WIN32_FIND_DATA findFileData;  
	BOOL bRet = TRUE;  

	char chszPath[MAX_PATH] = {0};
	if (0>Safe_WtoA(szPath, chszPath, MAX_PATH))
	{
		ERRTO_LOG_RETURN("WriteRecode","IsFolderHave_1",NULL);
	}

	int iLen = Safe_strlen(chszPath);
	if (0 >= iLen)
	{
		OutputDebugMessageA("��ȡ���ش洢��url.txt�ļ���·������ : %d....",iLen);
		return NULL;
	}

	if ((iLen <MAX_PATH) && (iLen >0))
	{
		if (NULL == Safe_wcscpy(szFind,MAX_PATH,szPath))
		{
			ERRTO_LOG_RETURN("WriteRecode","IsFolderHave_2",NULL);
		}
	} 
	else
	{
		OutputDebugMessageA("IsFolderHave szPath ����ճ����Χ");
		return NULL;
	}

	/*int iLen = Safe_strlen(chszPath);
	if (0 >= iLen)
	{
	OutputDebugMessageA("��ȡ���ش洢��url.txt�ļ���·������ : %d....",iLen);
	return NULL;
	}*/

	if ((iLen +4 < MAX_PATH) && (iLen >0))
	{
		if (NULL == Safe_wcscat(szFind,MAX_PATH,L"\\*.*"))     //����һ��Ҫָ��ͨ�������Ȼ�����ȡ�����ļ���Ŀ¼  
		{
			ERRTO_LOG_RETURN("WriteRecode","IsFolderHave_4",NULL);
		}
	} 
	else
	{
		OutputDebugMessageA("IsFolderHave szPath �������ӷ�Χ");
		return NULL;
	}

	HANDLE hFind = ::FindFirstFileW(szFind, &findFileData);  
	if (INVALID_HANDLE_VALUE == hFind)  
	{   
		return NULL;
	}  
	while (bRet)  
	{  
		//һ���ҵ�'.'��".."����Ϊ��  
		if (findFileData.cFileName[0] != L'.')  
		{  
			::FindClose(hFind);
			if (NULL == Safe_memcpy(wchFilePath, MAX_PATH, (const void*)findFileData.cFileName, MAX_PATH))
			{
				OutputDebugMessageA("IsFolderHave����wchFilePath ��������");
				return NULL;
			}
			return findFileData.cFileName[0];  
		}  
		bRet = ::FindNextFileW(hFind, &findFileData);
	}  
	::FindClose(hFind); 
	return findFileData.cFileName[0];  
}


/*
* ��    �ܣ��ж��Ƿ����ļ���
* ���������pDir      
* ����ֵ  ��TRUE:�ɹ�ö�٣�FALSE��ʧ�ܣ�                                     
*/
BOOL IsDirectory(const char *pDir)
{
	char szCurPath[512];
	ZeroMemory(szCurPath, 512);

	int nlen = Safe_strlen(pDir);
	if (0 > nlen)
	{
		OutputDebugMessageA("IsDirectory �����ַ���ָ�����");
		return NULL;
	}
	else if (nlen >0)
	{
		if (nlen + 4 <512)
		{
			if ( 0 >Safe_sprintf(szCurPath, 512, "%s//*", pDir))
			{
				ERRTO_LOG_RETURN("WriteRecode", "IsDirectory_1", FALSE);
			}
		} 
		else
		{
			OutputDebugMessageA("IsDirectory pDir Safe_strlen more than 512");
			return FALSE;
		}
	}
	else if (nlen == 0)
	{
		OutputDebugMessageA("IsDirectory �����ַ������ȵ���0");
	}

	WIN32_FIND_DATAA FindFileData;		
	ZeroMemory(&FindFileData, sizeof(WIN32_FIND_DATAA));

	HANDLE hFile = FindFirstFileA(szCurPath, &FindFileData); /**< find first file by given path. */

	if( hFile == INVALID_HANDLE_VALUE )	
	{
		FindClose(hFile);
		return FALSE; /** ��������ҵ���һ���ļ�����ôû��Ŀ¼ */
	}
	else
	{	
		FindClose(hFile);
		return TRUE;
	}
}


/*
* ��    �ܣ�ɾ��ָ���ļ���
* ���������DirName      
* ����ֵ  ��TRUE���ɹ�����true��FALSE��ʧ�ܷ���false                                         
*/
BOOL DeleteDirectory(const char * DirName,BOOL bDeleteDir)
{
	//	CFileFind tempFind;		//����һ��CFileFind�����������������
	int iLen = Safe_strlen(DirName);
	if (iLen <= 0)
	{
		OutputDebugMessageA("DeleteDirectory ɾ��·��Ϊ��");
		return FALSE;
	}
	char szCurPath[MAX_PATH];		//���ڶ���������ʽ
	if (iLen +4<MAX_PATH)
	{
		_snprintf(szCurPath, MAX_PATH, "%s//*.*", DirName);	//ƥ���ʽΪ*.*,����Ŀ¼�µ������ļ�
	} 
	else
	{
		OutputDebugMessageA("DeleteDirectory DirName�����˸�ʽ����Χ");
		return FALSE;
	}

	WIN32_FIND_DATAA FindFileData;		
	ZeroMemory(&FindFileData, sizeof(WIN32_FIND_DATAA));
	HANDLE hFile = NULL;

	int nlen = Safe_strlen(szCurPath);
	if (0 > nlen)
	{
		OutputDebugMessageA("DeleteDirectory �����ַ���ָ�����");
		return NULL;
	}
	else if (nlen >0)
	{
		if (nlen >0)
		{
			hFile = FindFirstFileA(szCurPath, &FindFileData);
		} 
		else
		{
			OutputDebugMessageA("DeleteDirectory hFile Ϊ��");
			return FALSE;
		}
	}
	else if (nlen == 0)
	{
		OutputDebugMessageA("DeleteDirectory �����ַ������ȵ���0");
	}


	BOOL IsFinded = TRUE;
	while(IsFinded) 
	{
		IsFinded = FindNextFileA(hFile, &FindFileData);	//�ݹ������������ļ�
		int err1 = 0;
		int iFind1 = 0;
		int err2 = 0;
		int iFind2 = 0;
		iFind1 = Safe_strcmp(FindFileData.cFileName, ".",err1);
		iFind2 = Safe_strcmp(FindFileData.cFileName, "..",err2);
		//if( strcmp(FindFileData.cFileName, ".") && strcmp(FindFileData.cFileName, "..") ) //�������"." ".."Ŀ¼
		if ((err1 != 0) || (err2 != 0))
		{
			ERRTO_LOG_NORETURN("WriteRecode","Safe_strcmp");
		}
		if ( iFind1 !=0 && iFind2 != 0)
		{
			string strFileName = "";
			strFileName = strFileName + DirName + "//" + FindFileData.cFileName;
			string strTemp;
			strTemp = strFileName;
			if( IsDirectory(strFileName.c_str()) ) //�����Ŀ¼����ݹ�ص���
			{	
				DeleteDirectory(strTemp.c_str());
			}
			else
			{
				DeleteFileA(strTemp.c_str());
			}
		}
	}
	FindClose(hFile);

	if (bDeleteDir)
	{
		BOOL bRet = RemoveDirectoryA(DirName);
		if( bRet == 0 ) //ɾ��Ŀ¼
		{
			return FALSE;
		}
	}
	
	return TRUE;
}


/*
* ��    �ܣ�����ָ��·���µ��ļ���
* ���������1.filepath   //�ļ���·��
*			2.
* ����ֵ  ������true�ɹ���falseʧ��                                    
*/
BOOL CleanupFolder(tstring filepath, bool isConnect)
{
	BOOL bRet = false;
	bool bHave = IsFolderEmpty((WCHAR*)filepath.c_str());
	if ( bHave)
	{
		WCHAR wchFilePath[MAX_PATH] = {0};
		IsFolderHave((WCHAR*)filepath.c_str(),wchFilePath);
		char chDel[MAX_PATH] = {0};
		char chDel2[MAX_PATH] = {0};
		if (0>Safe_WtoA((WCHAR*)filepath.c_str(), chDel,MAX_PATH))
		{
			ERRTO_LOG_NORETURN("WriteRecode","CleanupFolder_1")
				return false;
		}
		if (0>Safe_WtoA(wchFilePath, chDel2,MAX_PATH))
		{
			ERRTO_LOG_NORETURN("WriteRecode","CleanupFolder_2")
				return false;
		}

		if (isConnect)
		{
			if (NULL == Safe_strcat(chDel, MAX_PATH, "\\"))
			{
				ERRTO_LOG_NORETURN("WriteRecode", "WndProc_13005_45_3");
			}
			if (NULL == Safe_strcat(chDel, MAX_PATH, chDel2))
			{
				ERRTO_LOG_NORETURN("WriteRecode","WndProc_13005_45_4");
			}
		}

		bRet = DeleteDirectory(chDel);
		bHave = false;
	}
	return bRet;
}


/*
* ��    �ܣ�ȥ���ַ��������пո�
* ���������1.pStr     //���˵��ַ���
* ����ֵ  ��
*/
void delspace(char * pStr)
{
	int i,j=0;
	for ( i = 0;pStr[i]!='\0';i ++ ) 
	{
		if(pStr[i] != ' ')
		{
			pStr[j++] = pStr[i];
		}
	}
	pStr[j] = '\0';
}

/*
* ��    �ܣ������ļ����µ������ļ���������
* ���������1.path     ���˵��ַ���
*			2.files    ���ض�̬����
* ����ֵ  ��
* �޸����ݣ�                                  
*/
BOOL GetFileAllNames(string dir, vector<string> &vFile)
{
	_finddata_t fileDir;
	intptr_t lfDir;

	dir+="\\*.*";
	if((lfDir = _findfirst(dir.c_str(),&fileDir)) == -1l)
	{
		return FALSE;
	}
	else
	{
		do
		{
			int ret1 = 0,ret2 = 0;
			int err1 = 0,err2 = 0;
			ret1 = Safe_strcmp(fileDir.name,".",err1);
			ret2 = Safe_strcmp(fileDir.name,"..",err2);
			if (err1 !=0 || err2 != 0)
			{
				return FALSE;
			}
			if (ret1 == 0 || ret2 == 0)
			{
				;
			}
			else
			{
				string str(fileDir.name);
				vFile.push_back(str);
			}

		}while( _findnext( lfDir, &fileDir ) == 0 );

	}
	_findclose(lfDir);

	return TRUE;
}


/*
* ��    �ܣ�����ǿ�Ƴ�ʼ�����ļ�
* ��    ����szPath      
*           filePath   ���ص��ļ�����
* ����ֵ  �������ļ�����                                     
*/
BOOL RetFileName(WCHAR *szPath, CHAR *filePath)
{
	WCHAR szFind[MAX_PATH] = {0};  
	WIN32_FIND_DATA findFileData;  
	BOOL bRet = TRUE;  

	char chszPath[MAX_PATH] = {0};
	if (0>Safe_WtoA(szPath, chszPath, MAX_PATH))
	{
		ERRTO_LOG_RETURN("WriteRecode","RetFileName_1",FALSE);
	}

	int iLen = Safe_strlen(chszPath);
	if (iLen <= 0)
	{
		OutputDebugMessage(_T("��ȡ�ļ��쳣"));
		return FALSE;
	}

	if (iLen <MAX_PATH)
	{
		if (NULL == Safe_wcscpy(szFind,MAX_PATH,szPath))
		{
			ERRTO_LOG_RETURN("WriteRecode","RetFileName_2",FALSE);
		}
	} 
	else
	{
		OutputDebugMessageA("RetFileName szPath ����ճ����Χ");
		return FALSE;
	}

	if (iLen + 4<MAX_PATH)
	{
		if (NULL == Safe_wcscat(szFind,MAX_PATH,L"\\*.*"))    //����һ��Ҫָ��ͨ�������Ȼ�����ȡ�����ļ���Ŀ¼
		{
			ERRTO_LOG_RETURN("WriteRecode","RetFileName_4",FALSE);
		}
	} 
	else
	{
		OutputDebugMessageA("RetFileName szPath �������ӷ�Χ");
		return FALSE;
	}

	HANDLE hFind = ::FindFirstFileW(szFind, &findFileData);  
	if (INVALID_HANDLE_VALUE == hFind)  
	{   
		return FALSE;
	}  
	while (bRet)
	{  
		//һ���ҵ�'.'��".."����Ϊ��  
		if (findFileData.cFileName[0] != L'.')  
		{  
			::FindClose(hFind);
			//WCHAR cwFileName[260] = {0};
			if (NULL == Safe_memcpy(filePath, MAX_PATH, findFileData.cFileName, MAX_PATH))
			{
				OutputDebugMessageA("RetFileName����filePath �����쳣");
				return FALSE;
			}

			return bRet;
		}  
		bRet = ::FindNextFileW(hFind, &findFileData);
	}  
	::FindClose(hFind); 
	if (NULL == Safe_memcpy(filePath, MAX_PATH, findFileData.cFileName, MAX_PATH))
	{
		OutputDebugMessageA("RetFileName����filePath �����쳣");
		return FALSE;
	}
	return bRet;
}


/*
* ��    �ܣ����ɱ���dmp�ļ�      
* ���������ϵͳ      
* ���������ϵͳ
* ����ֵ  ��ִ�н��                                         
*/
LONG WINAPI MyUnhandledFilter(struct _EXCEPTION_POINTERS *lpExceptionInfo)
{
	LONG ret = EXCEPTION_EXECUTE_HANDLER;

	TCHAR szFileName[MAX_PATH] = {0};
	TCHAR   chPath1[MAX_PATH] = {0};   
	GetModuleFileName(NULL,chPath1,MAX_PATH);  
	tstring strPath = chPath1;
	size_t len1 = strPath.rfind('\\');
	strPath = strPath.substr(0,len1+1);

	SYSTEMTIME st;
	::GetLocalTime(&st);
	if ( 0 >Safe_swprintf(szFileName, MAX_PATH, _T("%sWriteRecode_%04d-%02d-%02d-%02d-%02d-%02d-%02d-%02d.dmp"),strPath.c_str(), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, rand()%100))
	{
		ERRTO_LOG_RETURN("WriteRecode","MyUnhandledFilter_1",0);
	}

	HANDLE hFile = ::CreateFile(szFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if (hFile != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION ExInfo;

		ExInfo.ThreadId = ::GetCurrentThreadId();
		ExInfo.ExceptionPointers = lpExceptionInfo;
		ExInfo.ClientPointers = false;

		// write the dump
		BOOL bOK = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL );

		if (bOK)
		{
			OutputDebugMessageA("Create Dump File Success!_2\n");
		}
		else
		{
			OutputDebugMessageA("MiniDumpWriteDump Failed: %d_3\n", GetLastError());
		}

		::CloseHandle(hFile);
	}
	else
	{
		OutputDebugMessage(_T("Create File %s Failed %d_4\n"), szFileName, GetLastError());
	}
	return ret;
}


/*
* ���ܣ���Դ�ַ����в����Ӵ��������Ӵ��״γ��ֵ����ֽ�λ�á�
* ������ 
*      __in const char *pcSrc      Դ�ַ���
*	   __in int iSrcLen			   Դ�ַ�������
*	   __in const char *pcSub	   ���ַ���
*	   __in int iSubLen			   ���ַ�������
*����ֵ�� -1:δ�ҵ�      ����:�Ӵ���λ��
*/

int FindSubString(__in const char *pcSrc, __in int iSrcLen, __in const char *pcSub, __in int iSubLen)
{
	if ( (pcSrc==NULL) || (pcSub==NULL) || (iSrcLen<=0) || (iSubLen<=0)  )
		return -1;

	int iPos=0;
	for(;;)
	{  
		while( iPos ++ < iSrcLen  )//�ȶԣ��������ֽ���ȵ�λ��       
		{
			if(*pcSrc++ == *pcSub)
				break;
		}
		if ( (iPos-1) > iSrcLen-iSubLen  )
			return -1;

		bool bFinded=true;
		for (int i=1; i<iSubLen; i++)
		{
			if( *(pcSrc+i-1) != *(pcSub+i) )
			{
				bFinded=false;
				break;	
			}
		}
		if (bFinded)
			return iPos-1;
	}
}


/*
* �������ƣ�MyDeleteFile
* ��    �ܣ�path���ļ�·��      
* �����������       
* ����ֵ  ��0��ɾ���ɹ���-1��ɾ��ʧ��                                            
*/
int MyDeleteFile(const TCHAR *path)
{
	if (path == NULL)
	{
		OutputDebugMessageA("ɾ���ļ�·��Ϊ��");
		return -2;
	}
	if(0 == _taccess(path,0))//����ļ�����:�ļ�Ϊֻ���޷�ɾ��
	{
		//ȥ���ļ�ֻ������
		SetFileAttributes(path,0);
		if(DeleteFile(path))//ɾ���ɹ�
		{
			return 0;
		}
		else//�޷�ɾ��:�ļ�ֻ������Ȩ��ִ��ɾ��
		{
			return -1;
		}
	}
	else//�ļ�������
	{
		return -1;
	}
}

/*
* �������ƣ�MyDeleteFile
* ��    �ܣ�ɾ��ָ���ļ�
* ���������path���ļ�·��           
* ����ֵ  ��0��ɾ���ɹ���-1��ɾ��ʧ��   
* �޸����ݣ�                                           
*/
int MyDeleteFileA(const char *path)
{
	if (path == NULL)
	{
		OutputDebugMessageA("ɾ���ļ�·��Ϊ��");
		return -2;
	}

	if(0 == access(path,0))//����ļ�����:�ļ�Ϊֻ���޷�ɾ��
	{
		//ȥ���ļ�ֻ������
		SetFileAttributesA(path,0);
		if(DeleteFileA(path))//ɾ���ɹ�
		{
			return 0;
		}
		else//�޷�ɾ��:�ļ�ֻ������Ȩ��ִ��ɾ��
		{
			return -1;
		}
	}
	else//�ļ�������
	{
		return -1;
	}
}

void MyMakeDir(const wchar_t * chPathW)
{
	if (0 == _taccess(chPathW,0))
	{
		return;
	}
	int nRet = _tmkdir(chPathW);
	if (nRet != 0)
	{

		OutputDebugMessage(L"Make dir:%s failed,GetLastError = %d",chPathW,GetLastError());
	}
}

void MyMakeDirA(const char * chPathA)
{
	if (0 == access(chPathA,0))
	{
		return;
	}
	int nRet = _mkdir(chPathA);
	if (nRet != 0)
	{

		OutputDebugMessageA("Make dir:%s failed,GetLastError = %d",chPathA,GetLastError());
	}
}



/*
* �������ƣ�MySafeWtoA
* �������ܣ����ֽ�ת��Ϊ���ֽ�
* ��    ����pBufW  ���ֽ�
*			pBufA  ���ֽ�
*			len	   pBufA�ĳ���
* �� �� ֵ��
*/
BOOL MySafeWtoA(wchar_t *pBufW,char *pBufA,int len)
{
	if (NULL == Safe_memset(pBufA,len,0,len))
	{
		ERRTO_LOG_RETURN("WriteRecode","Safe_AtoW",FALSE);
	}
	int ret = Safe_WtoA(pBufW,pBufA,len);
	if (ret < 0)
	{
		ERRTO_LOG_RETURN("WriteRecode","MySafeWtoA ERROR",FALSE);
	}
	else
	{
		return TRUE;
	}

	//return Safe_WtoA(pBufW,pBufA,len);
}

/*
* �������ƣ�MySafeAtoW
* �������ܣ����ֽ�ת��Ϊ���ֽ�
* ��    ����pBufW  ���ֽ�
*			pBufA  ���ֽ�
*			len	   pBufW�ĳ���
* �� �� ֵ��
*/
BOOL MySafeAtoW(const char*pBufA,wchar_t *pBufW,int len)
{
	if (NULL == Safe_memset(pBufW,len,0,len))
	{
		ERRTO_LOG_RETURN("WriteRecode","Safe_AtoW",FALSE);
	}
	int ret = Safe_AtoW(pBufA,pBufW,len);
	if (ret < 0)
	{
		ERRTO_LOG_RETURN("WriteRecode","MySafeAtoW ERROR",FALSE);
	}
	else
	{
		return TRUE;
	}
}

/*
* �������ƣ�PlGetGuid
* ��    �ܣ�����guid����ʽ���ط�      
* �����������       
* �����������
* ����ֵ  ����ʽ������ַ���                                            
*/
string PlGetGuid()
{
	string strid;
	GUID guid;
	char buf[1024] = {0};
	if (S_OK == ::CoCreateGuid(&guid))
	{
		if (0 > Safe_sprintf(buf, 1024, "%08X%04X%04x%02X%02X%02X%02X%02X%02X%02X%02X"
			, guid.Data1, guid.Data2, guid.Data3, guid.Data4[0], guid.Data4[1]
		, guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]
		, guid.Data4[6], guid.Data4[7]))
		{
			ERRTO_LOG_RETURN("WriteRecode", "PlGetGuid", NULL)
		}
	}
	strid = buf;
	return strid;
}


/*
* �������ƣ�replace_all
* ��    �ܣ��滻�ַ�		
* ��    ����str   �ַ���
            old_value   Ŀ���ַ���
			new_value   Դ�ַ���
* ����ֵ  ��                                   
/************************************************************************/
string&   replace_all(string& str,const string&old_value,const string&  new_value)     
{     
	while(true)   
	{     
		string::size_type   pos(0);     
		if( (pos=str.find(old_value))!=string::npos )    
		{
			str.replace(pos,old_value.length(),new_value); 
		}
		else
		{
			break;     
		}
	}     
	return   str;     
}    


/*
* �������ƣ�GetFileDataByName
* ��    �ܣ�ͨ���ļ�����ȡ�ļ������ݺ��ļ�����
* ���������pchFileName���ļ����� 
			 ulNameLeng���ļ������ȣ�  
			 ppchData���ļ����ݣ�  
			 lDataLength���ļ�����		
* �� �� ֵ��
*/
BOOL GetFileDataByName(IN const CHAR *pchFileName, IN ULONG ulNameLeng,  OUT CHAR **ppchData, OUT LONG &lDataLength)
{
	/* ������Ч���ж� */
	if ((NULL == pchFileName) || (0 == ulNameLeng))
	{
		return FALSE;
	}

	/* ��ָ�����ļ� */
	FILE *fp = fopen(pchFileName, "rb");
	if (NULL == fp) 
	{
		return FALSE;
	}

	/* �����ļ���С�������Ӧ���ڴ�ռ� */
	fseek(fp, 0, SEEK_SET);
	fseek(fp, 0, SEEK_END);
	LONG lFileSize = ftell(fp);

	fseek(fp, 0, SEEK_SET);
	char* pCmdData = Safe_new<char>(lFileSize + 1);
	if (NULL == pCmdData)
	{
		fclose(fp);
		fp = NULL;
		return FALSE;
	}

	if (NULL == Safe_memset(pCmdData, lFileSize + 1, 0, lFileSize + 1))
	{
		if (NULL != fp)
		{
			fclose(fp);
			fp = NULL;
		}
		Safe_delete(pCmdData);
		return FALSE;
	}

	/* ��ȡ�ļ��е����� */
	fread(pCmdData, lFileSize, 1, fp);
	fclose(fp);
	fp = NULL;


	*ppchData = pCmdData;
	lDataLength = lFileSize;

	return TRUE;
}


/*
* ��  �ܣ���ȡע���char����ֵ
* ��  ����key  ������
*		  str  ֵ
*		  nStrLength char�ĳ���
* ����ֵ��
*/
void GetRegInfoStr(char* key,char* str,int nStrLength)
{
	LONG lResult;
	HKEY hKey;
	lResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE,"SOFTWARE\\FetchView\\GooDataBridge",0,KEY_READ,&hKey);//��key
	if (ERROR_SUCCESS == lResult)
	{
		DWORD dwRegType = REG_SZ;
		DWORD dwValueSize = nStrLength;
		LONG lRet;
		lRet = RegQueryValueExA(hKey,key,0,&dwRegType,(LPBYTE)str,&dwValueSize);
		RegCloseKey(hKey);
	}
}

/*
* ��  �ܣ���ȡע���DWORD����ֵ
* ��  ����key  ������
*		  val  ֵ
* ����ֵ��
*/

void GetRegInfoDWORD(char*key, int& val)
{
	LONG lResult;
	HKEY hKey;
	lResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE,"SOFTWARE\\FetchView\\GooDataBridge",0,KEY_READ,&hKey);//��key
	if (ERROR_SUCCESS == lResult)
	{
		DWORD dwRegType = REG_DWORD;
		DWORD dwValueSize = 0;
		LONG lRet;
		if(ERROR_SUCCESS != RegQueryValueExA(hKey, key, NULL, &dwRegType, NULL, &dwValueSize)) 
		{
			RegCloseKey(hKey);
			return;
		}

		if (0 == dwValueSize)
		{
			RegCloseKey(hKey);
			return;
		}

		BYTE* pByData = NULL;
		pByData = Safe_new<BYTE>(dwValueSize);
		if(NULL == pByData)
		{
			RegCloseKey(hKey);
			return;
		}


		lRet=RegQueryValueExA(hKey,key,0,&dwRegType,pByData,&dwValueSize);
		if (lRet != ERROR_SUCCESS)
		{
			Safe_delete<BYTE>(pByData);
			pByData = NULL;
			RegCloseKey(hKey);
			return;
		}

		if (dwRegType == REG_DWORD)
		{
			val = *(int*)pByData;
		}

		Safe_delete<BYTE>(pByData);
		RegCloseKey(hKey);
	}	
}

/*
* ��  �ܣ�����ע���ָ������ֵ
* ��  ����key  ������
*		  val  ֵ
* ����ֵ��
*/
void SetRegInfoDWORD(char* key, int val)
{
	DWORD dwResult;
	HKEY hKey;
	LONG lStatus;

	lStatus = RegCreateKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\FetchView\\GooDataBridge", 0, NULL, REG_OPTION_NON_VOLATILE, 
		KEY_WRITE, NULL, &hKey, &dwResult);

	if (ERROR_SUCCESS != lStatus)
	{
		return;
	}

	lStatus = RegSetValueExA(hKey, key, 0, REG_DWORD, (const BYTE*)&val, sizeof(val));

	RegCloseKey(hKey);
}


/*
* ��   �ܣ� UTF8תGBK
* ���������utf8  UTF8����
* ����ֵ �� ת���ĳ���UTF8����                                     
*/
string UTFToGBK(const char* utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len+1];
	memset(wstr, 0, len+1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);

	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len+1];
	memset(str, 0, len+1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if(wstr) delete[] wstr;
	return str;
}

/*
* ��   �ܣ�GBKתUTF8
* ���������strGBK��GBK����
* ����ֵ ��ת���ĳ���UTF8����                                     
*/
string GBKToUTF8(const string& strGBK) 
{ 
	string strOutUTF8 = ""; 
	wchar_t* pszBuf = NULL;
	int nlen = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0); 
	pszBuf = new WCHAR[nlen]; 
	MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, pszBuf, nlen); 

	nlen = WideCharToMultiByte(CP_UTF8, 0, pszBuf, -1, NULL, 0, NULL, NULL); 
	char * utf8 = new char[nlen]; 
	WideCharToMultiByte(CP_UTF8, 0, pszBuf, -1, utf8, nlen, NULL, NULL); 
	strOutUTF8 = utf8; 
	delete[]pszBuf; 
	pszBuf = NULL; 
	delete[]utf8; 
	utf8 = NULL; 
	return strOutUTF8; 
}

/*
* ��  �ܣ����ֽ�ת��Ϊ���ֽ�
* ��  ����str   ���ֽ�
* ����ֵ�����ֽ�
*/
string UnicodeToANSI( const wstring& str )
{
	char*     pElementText;
	int    iTextLen;
	// wide char to multi char
	iTextLen = WideCharToMultiByte( CP_ACP,0,str.c_str(),-1,NULL,0,NULL,NULL );
	pElementText = new char[iTextLen + 1];
	memset( ( void* )pElementText, 0, sizeof( char ) * ( iTextLen + 1 ) );

	::WideCharToMultiByte( CP_ACP,0,str.c_str(),-1,pElementText,iTextLen,NULL,NULL );
	string strText;
	strText = pElementText;
	delete[] pElementText;
	return strText;
}

/*
* ��  �ܣ����ֽ�ת��Ϊ���ֽ�
* ��  ����str  ���ֽ��ַ���
* ����ֵ�����ֽ��ַ���
*/
wstring ANSIToUnicode( const string& str )
{
	int len = 0;
	len = str.length();
	int unicodeLen = ::MultiByteToWideChar( CP_ACP,0,str.c_str(),-1,NULL,0 );
	wchar_t * pUnicode;
	pUnicode = new wchar_t[unicodeLen+1];
	memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t));
	::MultiByteToWideChar( CP_ACP,0,str.c_str(),-1,(LPWSTR)pUnicode,unicodeLen );
	wstring rt;
	rt = ( wchar_t* )pUnicode;
	delete pUnicode;
	return rt;
}

/*
* ��  �ܣ���wstringת��Ϊstring
* ��  ����str   wstring 
* ����ֵ��string
*/
string wstringToString(const wstring & wstr)
{
	LPCWSTR pwszSrc = wstr.c_str();
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
	if (nLen == 0)
		return string("");
	char *pszDst = new char[nLen];
	if (!pszDst)
		return string("");
	WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
	string str(pszDst);
	delete[] pszDst;
	pszDst = NULL;
	return str;
}

/*
* ��  �ܣ���stringת��Ϊwstring 
* ��  ����str   string
* ����ֵ��wstring
*/
wstring stringToWstring(const string &str)
{
	LPCSTR pszSrc = str.c_str();
	int nLen = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, NULL, 0);
	if (nLen == 0)
		return wstring(L"");
	wchar_t * pwszDst = new wchar_t[nLen];
	if (!pwszDst)
		return wstring(L"");
	MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, pwszDst, nLen);
	std::wstring wstr(pwszDst);
	delete[] pwszDst;
	pwszDst = NULL;
	return wstr;
}
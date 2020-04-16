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
*  功    能：掉结尾的\r\n
*  输入参数：str   传入的字符串
*  返回值  ：无                                      
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
*  功    能：读取文本指定行的文本
*  参    数：1.filename   文件路径
*			 2.iHang	  读取行的索引
             3.StrLine    指定行的内容
*  返 回 值：BOOL                                      
*/
BOOL ReadSpeacialLine(char *filename, int iHang, char *StrLine) 
{ 
	FILE *fp; 
	int WhichLine = iHang;             //指定要读取哪一行
	int CurrentIndex=0;             //当前读取的行
	if((fp = fopen(filename,"r")) == NULL) //判断文件是否存在及可读
	{ 
		OutputDebugMessageA("读取文本指定行的文本 打开句柄失败!!!");
		return FALSE; 
	} 

	while (0 == feof(fp)) 
	{ 
		if (CurrentIndex == WhichLine)
		{
			fgets(StrLine,1024,fp);  //读取一行

			fclose(fp);

			return TRUE;
		}
		fgets(StrLine,1024,fp);  //读取一行,并定位到下一行
		CurrentIndex++;
	} 
	fclose(fp);                     //关闭文件
	return FALSE; 
}
/*
* 功    能：输出错误日志信息      
* 输入参数：格式化的消息       
* 返回值  ：无
* 修改内容：  
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
* 功    能：输出错误日志信息
* 参	数：格式化的信息
* 返 回 值：无
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
* 功    能：输出日志信息      
* 输入参数：格式化的消息       
* 输出参数：
* 返回值  ：                                        
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
* 功    能：输出日志信息      
* 输入参数：格式化的消息       
* 输出参数：
* 返回值  ：                                        
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
* 功    能：输出日志信息      
* 输入参数：格式化的消息       
* 输出参数：
* 返回值  ：                                        
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
* 功    能：根据文件夹的路径判断文件夹内是否为空
* 输入参数：szPath     文件夹路径
* 返回值  ：TRUE:EMPTY  FALSE:NO EMPTY                                       
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
		OutputDebugMessageA("IsFolderEmpty 输入字符串指针错误");
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
			OutputDebugMessageA("IsFolderEmpty szPath 超出范围");
			return FALSE;
		}
	}
	else if (nlen == 0)
	{
		OutputDebugMessageA("IsFolderEmpty 输入字符串长度等于0");
	}

	int nlenPah = Safe_strlen(chszPath);
	if (0 > nlenPah)
	{
		OutputDebugMessageA("IsFolderEmpty 输入字符串指针错误");
		return NULL;
	}
	else if (nlenPah >0)
	{
		if (nlenPah + 4<MAX_PATH)
		{
			if (NULL == Safe_wcscat(szFind,MAX_PATH,L"\\*.*"))     //这里一定要指明通配符，不然不会读取所有文件和目录 
			{
				ERRTO_LOG_RETURN("WriteRecode","IsFolderEmpty_3",FALSE);
			}
		}
		else
		{
			OutputDebugMessageA("IsFolderEmpty szPath 超出连接范围");
			return FALSE;
		}
	}
	else if (nlenPah == 0)
	{
		OutputDebugMessageA("IsFolderEmpty 输入字符串长度等于0");
	}

	HANDLE hFind = ::FindFirstFileW(szFind, &findFileData);  
	if (INVALID_HANDLE_VALUE == hFind)  
	{  
		return FALSE;  
	}  
	while (bRet)  
	{  
		//一旦找到'.'或".."，则不为空  
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
* 功    能：判断文件夹下是否有指定文件
* 输入参数：szPath      
* 返回值  ：返回文件名字                                      
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
		OutputDebugMessageA("读取本地存储的url.txt文件夹路径长度 : %d....",iLen);
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
		OutputDebugMessageA("IsFolderHave szPath 超出粘贴范围");
		return NULL;
	}

	/*int iLen = Safe_strlen(chszPath);
	if (0 >= iLen)
	{
	OutputDebugMessageA("读取本地存储的url.txt文件夹路径长度 : %d....",iLen);
	return NULL;
	}*/

	if ((iLen +4 < MAX_PATH) && (iLen >0))
	{
		if (NULL == Safe_wcscat(szFind,MAX_PATH,L"\\*.*"))     //这里一定要指明通配符，不然不会读取所有文件和目录  
		{
			ERRTO_LOG_RETURN("WriteRecode","IsFolderHave_4",NULL);
		}
	} 
	else
	{
		OutputDebugMessageA("IsFolderHave szPath 超出连接范围");
		return NULL;
	}

	HANDLE hFind = ::FindFirstFileW(szFind, &findFileData);  
	if (INVALID_HANDLE_VALUE == hFind)  
	{   
		return NULL;
	}  
	while (bRet)  
	{  
		//一旦找到'.'或".."，则不为空  
		if (findFileData.cFileName[0] != L'.')  
		{  
			::FindClose(hFind);
			if (NULL == Safe_memcpy(wchFilePath, MAX_PATH, (const void*)findFileData.cFileName, MAX_PATH))
			{
				OutputDebugMessageA("IsFolderHave复制wchFilePath 出现问题");
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
* 功    能：判断是否是文件夹
* 输入参数：pDir      
* 返回值  ：TRUE:成功枚举，FALSE：失败：                                     
*/
BOOL IsDirectory(const char *pDir)
{
	char szCurPath[512];
	ZeroMemory(szCurPath, 512);

	int nlen = Safe_strlen(pDir);
	if (0 > nlen)
	{
		OutputDebugMessageA("IsDirectory 输入字符串指针错误");
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
		OutputDebugMessageA("IsDirectory 输入字符串长度等于0");
	}

	WIN32_FIND_DATAA FindFileData;		
	ZeroMemory(&FindFileData, sizeof(WIN32_FIND_DATAA));

	HANDLE hFile = FindFirstFileA(szCurPath, &FindFileData); /**< find first file by given path. */

	if( hFile == INVALID_HANDLE_VALUE )	
	{
		FindClose(hFile);
		return FALSE; /** 如果不能找到第一个文件，那么没有目录 */
	}
	else
	{	
		FindClose(hFile);
		return TRUE;
	}
}


/*
* 功    能：删除指定文件夹
* 输入参数：DirName      
* 返回值  ：TRUE：成功返回true，FALSE：失败返回false                                         
*/
BOOL DeleteDirectory(const char * DirName,BOOL bDeleteDir)
{
	//	CFileFind tempFind;		//声明一个CFileFind类变量，以用来搜索
	int iLen = Safe_strlen(DirName);
	if (iLen <= 0)
	{
		OutputDebugMessageA("DeleteDirectory 删除路径为空");
		return FALSE;
	}
	char szCurPath[MAX_PATH];		//用于定义搜索格式
	if (iLen +4<MAX_PATH)
	{
		_snprintf(szCurPath, MAX_PATH, "%s//*.*", DirName);	//匹配格式为*.*,即该目录下的所有文件
	} 
	else
	{
		OutputDebugMessageA("DeleteDirectory DirName超出了格式化范围");
		return FALSE;
	}

	WIN32_FIND_DATAA FindFileData;		
	ZeroMemory(&FindFileData, sizeof(WIN32_FIND_DATAA));
	HANDLE hFile = NULL;

	int nlen = Safe_strlen(szCurPath);
	if (0 > nlen)
	{
		OutputDebugMessageA("DeleteDirectory 输入字符串指针错误");
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
			OutputDebugMessageA("DeleteDirectory hFile 为空");
			return FALSE;
		}
	}
	else if (nlen == 0)
	{
		OutputDebugMessageA("DeleteDirectory 输入字符串长度等于0");
	}


	BOOL IsFinded = TRUE;
	while(IsFinded) 
	{
		IsFinded = FindNextFileA(hFile, &FindFileData);	//递归搜索其他的文件
		int err1 = 0;
		int iFind1 = 0;
		int err2 = 0;
		int iFind2 = 0;
		iFind1 = Safe_strcmp(FindFileData.cFileName, ".",err1);
		iFind2 = Safe_strcmp(FindFileData.cFileName, "..",err2);
		//if( strcmp(FindFileData.cFileName, ".") && strcmp(FindFileData.cFileName, "..") ) //如果不是"." ".."目录
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
			if( IsDirectory(strFileName.c_str()) ) //如果是目录，则递归地调用
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
		if( bRet == 0 ) //删除目录
		{
			return FALSE;
		}
	}
	
	return TRUE;
}


/*
* 功    能：清理指定路径下的文件夹
* 输入参数：1.filepath   //文件夹路径
*			2.
* 返回值  ：返回true成功，false失败                                    
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
* 功    能：去掉字符串中所有空格
* 输入参数：1.pStr     //过滤的字符串
* 返回值  ：
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
* 功    能：返回文件夹下的所有文件名称数组
* 输入参数：1.path     过滤的字符串
*			2.files    返回动态数组
* 返回值  ：
* 修改内容：                                  
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
* 功    能：返回强制初始化的文件
* 参    数：szPath      
*           filePath   返回的文件名字
* 返回值  ：返回文件名字                                     
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
		OutputDebugMessage(_T("读取文件异常"));
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
		OutputDebugMessageA("RetFileName szPath 超出粘贴范围");
		return FALSE;
	}

	if (iLen + 4<MAX_PATH)
	{
		if (NULL == Safe_wcscat(szFind,MAX_PATH,L"\\*.*"))    //这里一定要指明通配符，不然不会读取所有文件和目录
		{
			ERRTO_LOG_RETURN("WriteRecode","RetFileName_4",FALSE);
		}
	} 
	else
	{
		OutputDebugMessageA("RetFileName szPath 超出连接范围");
		return FALSE;
	}

	HANDLE hFind = ::FindFirstFileW(szFind, &findFileData);  
	if (INVALID_HANDLE_VALUE == hFind)  
	{   
		return FALSE;
	}  
	while (bRet)
	{  
		//一旦找到'.'或".."，则不为空  
		if (findFileData.cFileName[0] != L'.')  
		{  
			::FindClose(hFind);
			//WCHAR cwFileName[260] = {0};
			if (NULL == Safe_memcpy(filePath, MAX_PATH, findFileData.cFileName, MAX_PATH))
			{
				OutputDebugMessageA("RetFileName复制filePath 出现异常");
				return FALSE;
			}

			return bRet;
		}  
		bRet = ::FindNextFileW(hFind, &findFileData);
	}  
	::FindClose(hFind); 
	if (NULL == Safe_memcpy(filePath, MAX_PATH, findFileData.cFileName, MAX_PATH))
	{
		OutputDebugMessageA("RetFileName复制filePath 出现异常");
		return FALSE;
	}
	return bRet;
}


/*
* 功    能：生成崩溃dmp文件      
* 输入参数：系统      
* 输出参数：系统
* 返回值  ：执行结果                                         
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
* 功能：从源字符串中查找子串，返回子串首次出现的首字节位置。
* 参数： 
*      __in const char *pcSrc      源字符串
*	   __in int iSrcLen			   源字符串长度
*	   __in const char *pcSub	   子字符串
*	   __in int iSubLen			   子字符串长度
*返回值： -1:未找到      正数:子串的位置
*/

int FindSubString(__in const char *pcSrc, __in int iSrcLen, __in const char *pcSub, __in int iSubLen)
{
	if ( (pcSrc==NULL) || (pcSub==NULL) || (iSrcLen<=0) || (iSubLen<=0)  )
		return -1;

	int iPos=0;
	for(;;)
	{  
		while( iPos ++ < iSrcLen  )//比对，查找首字节相等的位置       
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
* 函数名称：MyDeleteFile
* 功    能：path：文件路径      
* 输入参数：无       
* 返回值  ：0：删除成功，-1：删除失败                                            
*/
int MyDeleteFile(const TCHAR *path)
{
	if (path == NULL)
	{
		OutputDebugMessageA("删除文件路径为空");
		return -2;
	}
	if(0 == _taccess(path,0))//如果文件存在:文件为只读无法删除
	{
		//去掉文件只读属性
		SetFileAttributes(path,0);
		if(DeleteFile(path))//删除成功
		{
			return 0;
		}
		else//无法删除:文件只读或无权限执行删除
		{
			return -1;
		}
	}
	else//文件不存在
	{
		return -1;
	}
}

/*
* 函数名称：MyDeleteFile
* 功    能：删除指定文件
* 输入参数：path：文件路径           
* 返回值  ：0：删除成功，-1：删除失败   
* 修改内容：                                           
*/
int MyDeleteFileA(const char *path)
{
	if (path == NULL)
	{
		OutputDebugMessageA("删除文件路径为空");
		return -2;
	}

	if(0 == access(path,0))//如果文件存在:文件为只读无法删除
	{
		//去掉文件只读属性
		SetFileAttributesA(path,0);
		if(DeleteFileA(path))//删除成功
		{
			return 0;
		}
		else//无法删除:文件只读或无权限执行删除
		{
			return -1;
		}
	}
	else//文件不存在
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
* 函数名称：MySafeWtoA
* 函数功能：宽字节转换为多字节
* 参    数：pBufW  宽字节
*			pBufA  多字节
*			len	   pBufA的长度
* 返 回 值：
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
* 函数名称：MySafeAtoW
* 函数功能：宽字节转换为多字节
* 参    数：pBufW  宽字节
*			pBufA  多字节
*			len	   pBufW的长度
* 返 回 值：
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
* 函数名称：PlGetGuid
* 功    能：生成guid，格式化回返      
* 输入参数：无       
* 输出参数：无
* 返回值  ：格式化后的字符串                                            
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
* 函数名称：replace_all
* 功    能：替换字符		
* 参    数：str   字符串
            old_value   目标字符串
			new_value   源字符串
* 返回值  ：                                   
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
* 函数名称：GetFileDataByName
* 功    能：通过文件名获取文件的内容和文件长度
* 输入参数：pchFileName：文件名； 
			 ulNameLeng：文件名长度；  
			 ppchData：文件内容；  
			 lDataLength：文件长度		
* 返 回 值：
*/
BOOL GetFileDataByName(IN const CHAR *pchFileName, IN ULONG ulNameLeng,  OUT CHAR **ppchData, OUT LONG &lDataLength)
{
	/* 参数有效性判断 */
	if ((NULL == pchFileName) || (0 == ulNameLeng))
	{
		return FALSE;
	}

	/* 打开指定的文件 */
	FILE *fp = fopen(pchFileName, "rb");
	if (NULL == fp) 
	{
		return FALSE;
	}

	/* 计算文件大小，分配对应的内存空间 */
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

	/* 读取文件中的数据 */
	fread(pCmdData, lFileSize, 1, fp);
	fclose(fp);
	fp = NULL;


	*ppchData = pCmdData;
	lDataLength = lFileSize;

	return TRUE;
}


/*
* 功  能：获取注册表char类型值
* 参  数：key  键名称
*		  str  值
*		  nStrLength char的长度
* 返回值：
*/
void GetRegInfoStr(char* key,char* str,int nStrLength)
{
	LONG lResult;
	HKEY hKey;
	lResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE,"SOFTWARE\\FetchView\\GooDataBridge",0,KEY_READ,&hKey);//打开key
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
* 功  能：获取注册表DWORD类型值
* 参  数：key  键名称
*		  val  值
* 返回值：
*/

void GetRegInfoDWORD(char*key, int& val)
{
	LONG lResult;
	HKEY hKey;
	lResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE,"SOFTWARE\\FetchView\\GooDataBridge",0,KEY_READ,&hKey);//打开key
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
* 功  能：设置注册表指定键的值
* 参  数：key  键名称
*		  val  值
* 返回值：
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
* 功   能： UTF8转GBK
* 输入参数：utf8  UTF8数据
* 返回值 ： 转换的出的UTF8数据                                     
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
* 功   能：GBK转UTF8
* 输入参数：strGBK：GBK数据
* 返回值 ：转换的出的UTF8数据                                     
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
* 功  能：多字节转换为宽字节
* 参  数：str   宽字节
* 返回值：多字节
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
* 功  能：多字节转换为宽字节
* 参  数：str  多字节字符串
* 返回值：宽字节字符串
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
* 功  能：将wstring转换为string
* 参  数：str   wstring 
* 返回值：string
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
* 功  能：将string转换为wstring 
* 参  数：str   string
* 返回值：wstring
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
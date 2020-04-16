/*
* 文件名称：WriteRecodeAssist.h 
* 文件标识：写日志辅助函数
*/

#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include "gagutil.h"
using namespace std;


#ifdef UNICODE
#define tstring wstring
#else
#define tstring string
#endif

//打印日志
void ErrorMessage(char* chForm, ...);
void ErrorMessageW(WCHAR* chForm, ...);
void OutputDebugMessageA(char* chForm, ...);
void OutputDebugMessage(TCHAR* chForm, ...);
void OutputDebugMessageW(TCHAR* chForm, ...);


//文件与文件夹相关操作
void *fun_r_n(char * const str);
BOOL ReadSpeacialLine(char *filename, int iHang, char *StrLine);
BOOL IsFolderEmpty(WCHAR* szPath); 
WCHAR IsFolderHave(WCHAR* szPath, WCHAR *wchFilePath);  
BOOL IsDirectory(const char *pDir);
BOOL DeleteDirectory(const char * DirName,BOOL bDeleteDir = TRUE);
BOOL CleanupFolder(tstring filepath, bool isConnect);
void delspace(char * pStr);
BOOL GetFileAllNames(string dir, vector<string> &vFile);
BOOL RetFileName(WCHAR *szPath, CHAR *filePath);
BOOL GetFileDataByName(IN const CHAR *pchFileName, IN ULONG ulNameLeng, OUT CHAR **ppchData, OUT LONG &lDataLength);

//删除、创建文件
int MyDeleteFileA(const char *path);
int MyDeleteFile(const TCHAR *path);
void MyMakeDir(const wchar_t * chPathW);
void MyMakeDirA(const char * chPathA);



//注册表操作
void GetRegInfoStr(char* key,char* str,int nStrLength);
void GetRegInfoDWORD(char* key,int& val);
void SetRegInfoDWORD(char* key, int val);


//生成奔溃dmp文件
LONG WINAPI MyUnhandledFilter(struct _EXCEPTION_POINTERS *lpExceptionInfo);


//字符串操作
int FindSubString(__in const char *pcSrc, __in int iSrcLen, __in const char *pcSub, __in int iSubLen);
string&   replace_all(string&   str,const   string&   old_value,const   string&   new_value);


string  UTFToGBK(const char* utf8);
string  GBKToUTF8(const string& strGBK);
string  UnicodeToANSI( const wstring& str);
wstring ANSIToUnicode( const string& str);
string  wstringToString(const wstring & wstr);
wstring stringToWstring(const string &str);
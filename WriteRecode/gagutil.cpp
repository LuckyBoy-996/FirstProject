#include "stdafx.h"
#include "gagutil.h"

#ifdef WIN32

HGLOBAL Safe_GlobalAlloc ( UINT uFlags,SIZE_T dwBytes, unsigned int maxUpLevel)
{
	if (dwBytes <= 0 || dwBytes > maxUpLevel)
	{
		return NULL;
	}
	HGLOBAL a;
#ifdef WIN32
	__try
#else
	try
#endif
	{
		a=GlobalAlloc(uFlags, dwBytes);
	}
#ifdef WIN32
	__except( EXCEPTION_EXECUTE_HANDLER )
	{
		return NULL;
	}
#else
	catch(...)
	{
		return NULL;
	}
#endif
	return a;
}
void Safe_GlobalFree( HGLOBAL hMem)
{
	if (hMem)
	{
		GlobalFree(hMem);
		hMem=NULL;
	}
}

HLOCAL Safe_LocalAlloc ( UINT uFlags,SIZE_T dwBytes, unsigned int maxUpLevel)
{
	if (dwBytes <= 0 || dwBytes > maxUpLevel)
	{
		return NULL;
	}
	HLOCAL a;
#ifdef WIN32
	__try
#else
	try
#endif
	{
		a=LocalAlloc(uFlags, dwBytes);
	}
#ifdef WIN32
	__except( EXCEPTION_EXECUTE_HANDLER )
	{
		return NULL;
	}
#else
	catch(...)
	{
		return NULL;
	}
#endif
	return a;
}
void Safe_LocalFree( HLOCAL hMem)
{
	if (hMem)
	{
		LocalFree(hMem);
		hMem=NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
/*功能:计算字符长度
参数:	_Str:字符串指针
返回值： -1：指针错误，其他：字符串长度
*/
//////////////////////////////////////////////////////////////////////////

int Safe_wcslen(const wchar_t * _Str)
{
	if (_Str == NULL||IsInValidReadPtr(_Str,sizeof(wchar_t)))
	{
		return -1;
	}
	return (int)wcslen(_Str);
}
const wchar_t* Safe_wcsstr(const wchar_t *s1,const wchar_t *s2,int& err)
{
	err=0;
	if (s1 == NULL||s2 == NULL||IsInValidReadPtr(s1,sizeof(wchar_t)) || IsInValidReadPtr(s2,sizeof(wchar_t)))
	{
		err=1;
		return NULL;
	}
	return wcsstr(s1,s2);
}

//////////////////////////////////////////////////////////////////////////
/*功能:字符串拷贝
参数:	dst:目的字符串buf指针
countof_Dst：可容纳wchar 的个数 wchar_t wBuf[1024]; countof_Dst = 1024
src：源字符串buf指针

返回值： NULL：指针错误，其他：dst地址
*/
//////////////////////////////////////////////////////////////////////////
wchar_t *  Safe_wcscpy(wchar_t * dst, int countof_Dst, const wchar_t * src)
{
	int n = Safe_wcslen(src);

	if (n < 0 || n >= countof_Dst)
	{
		return NULL;
	}

	if (dst == NULL||IsInValidWritePtr(dst, countof_Dst) || IsInValidReadPtr(src, n))
	{
		return NULL;
	}

	return wcscpy(dst, src);
}

//////////////////////////////////////////////////////////////////////////
/*功能:字符串拷贝
参数:	dst:目的字符串buf指针
countof_Dst：可容纳wchar 的个数 wchar_t wBuf[1024]; countof_Dst = 1024
src：源字符串buf指针
count：拷贝的长度

返回值： NULL：指针错误，其他：dst地址
*/
//////////////////////////////////////////////////////////////////////////

wchar_t * Safe_wcsncpy(wchar_t * dst, unsigned int countof_Dst, const wchar_t * source, int count)
{
	if (count < 0 )
	{
		return NULL;
	}
	int n = Safe_wcslen(source);
	if (n < 0 || n < count || count >= countof_Dst)
	{
		return NULL;
	}

	if (dst == NULL|| IsInValidWritePtr(dst,countof_Dst) || IsInValidReadPtr(source,count))
	{
		return NULL;
	}

	return wcsncpy(dst,source,count);
}

//////////////////////////////////////////////////////////////////////////
/*功能:字符串链接
参数:	dst:目的字符串buf指针
countof_Dst：可容纳wchar 的个数 wchar_t wBuf[1024]; countof_Dst = 1024
src：源字符串buf指针

返回值： NULL：指针错误，其他：dst地址
*/
//////////////////////////////////////////////////////////////////////////

wchar_t*  Safe_wcscat(wchar_t * dst, int countof_Dst, const wchar_t * src)
{
	if (IsInValidWritePtr(dst,countof_Dst) || IsInValidReadPtr(src,2))
	{
		return NULL;
	}
	int ndst = Safe_wcslen(dst);
	if (ndst < 0)
	{
		return NULL;
	}
	int n = Safe_wcslen(src);
	if (n < 0 || n >= countof_Dst-ndst)
	{
		return NULL;
	}

	return wcscat(dst,src);
}

//////////////////////////////////////////////////////////////////////////
/*功能:字符串链接
参数:	dst:目的字符串buf指针
countof_Dst：可容纳wchar 的个数 wchar_t wBuf[1024]; countof_Dst = 1024
src：源字符串buf指针
count：需要链接的长度
返回值： NULL：指针错误，其他：dst地址
*/
//////////////////////////////////////////////////////////////////////////
wchar_t *  Safe_wcsncat(wchar_t * dst, int countof_Dst, const wchar_t * src, int count)
{
	int ndst = Safe_wcslen(dst);
	if (count < 0 || ndst < 0)
	{
		return NULL;
	}
	int n = Safe_wcslen(src);

	if (n < 0 || n < count || count >= countof_Dst-ndst)
	{
		return NULL;
	}
	if (IsInValidWritePtr(dst, countof_Dst) || IsInValidReadPtr(src,count))
	{
		return NULL;
	}


	return wcsncat(dst,src,count);
}

//////////////////////////////////////////////////////////////////////////
/*功能:字符串比较
参数:	src：源字符串buf指针
dst:目的字符串buf指针
err：返回函数调用的错误值 0：正确， 其他：错误
返回值： =0：相同，<0:src<dst ,>0: src>dst
*/
//////////////////////////////////////////////////////////////////////////
int  Safe_wcscmp (const wchar_t * src, const wchar_t * dst, int& err)
{
	err = 0;
	if (src == NULL || dst == NULL||
		IsInValidReadPtr(dst,Safe_wcslen(dst)) ||
		IsInValidReadPtr(src, Safe_wcslen(src)))
	{
		err = 1;
		return -2;
	}
	return wcscmp(src,dst);
}

//////////////////////////////////////////////////////////////////////////
/*功能:字符串比较
参数:	src：源字符串buf指针
dst:目的字符串buf指针
count:需要比较的长度，字符个数
err：返回函数调用的错误值 0：正确， 其他：错误
返回值： =0：相同，<0:src<dst ,>0: src>dst
*/
//////////////////////////////////////////////////////////////////////////
int  Safe_wcsncmp(const wchar_t *src, const wchar_t *dst, unsigned int count, int& err)
{
	err = 0;
	int n1 = Safe_wcslen(src);
	int n2 = Safe_wcslen(dst);
	if (n1 < 0 || n2 < 0 || n1 < count || n2 < count)
	{
		err = 1;
		return -2;
	}

	err=0;
	if (IsInValidReadPtr(dst, n2) || IsInValidReadPtr(src, n1))
	{
		err = 1;
		return -2;
	}


	return wcsncmp(src,dst,count);
}



/************************************************************************/
/* 函数名称：Safe_WtoA
/* 功    能：宽字节转多字节        
/* 输入参数：ptrW：宽字节数据
/* 输出参数：ptrA：多字节数据
/* 输入参数：nLenA：多字节数据长度
/* 返回值  ：数据长度   
/* 修改内容：  
/*     1.  创建                                        
/************************************************************************/
int  WINAPI Safe_WtoA(IN WCHAR *ptrW,OUT char *ptrA,IN int nLenA)
{
	if (IsInValidReadPtr(ptrW,Safe_wcslen(ptrW)) || IsInValidWritePtr(ptrA,Safe_strlen(ptrA)))
	{  
		return -1;  
	} 
	int len = WideCharToMultiByte( CP_ACP,0,ptrW,-1,NULL,0,NULL,NULL );
	if (len > nLenA)
	{
		return -2;  
	}
	WideCharToMultiByte( CP_ACP,0,ptrW,-1,ptrA,len,NULL,NULL );
	return len;
}

/************************************************************************/
/* 函数名称：Safe_AtoW
/* 功    能：多字节转宽字节    
/* 输入参数：ptrA：多字节数据
/* 输出参数：ptrW：宽字节数据
/* 输入参数：nLenW：宽字节数据长度
/* 返回值  ：数据长度   
/* 修改内容：  
/*     1.  创建                                        
/************************************************************************/
int  WINAPI Safe_AtoW(IN const char *ptrA,OUT WCHAR *ptrW,IN int nLenW)
{
	if (IsInValidReadPtr(ptrA,Safe_strlen(ptrA)) || IsInValidWritePtr(ptrW,Safe_wcslen(ptrW)) )
	{  
		return -1;  
	} 
	int len = MultiByteToWideChar( CP_ACP,0,ptrA,-1,NULL,0 );
	if (len > nLenW)
	{
		return -2;  
	}
	MultiByteToWideChar( CP_ACP,0,ptrA,-1,ptrW,len );
	return len;
}
//////////////////////////////////////////////////////////////////////////
/*功能:字符串格式化
参数:	buffer：源字符串buf指针
buf_size:源字符串buf大小
format:格式化
argptr:参数
返回值： 字符串长度
*/
//////////////////////////////////////////////////////////////////////////
int Safe_vswprintf(wchar_t *buffer, int buf_size,const wchar_t *format, va_list argptr)
{  
	if (buf_size < 1 || IsInValidWritePtr(buffer,buf_size))
	{  
		return -1;  
	}  

	int res = _vsnwprintf(buffer, buf_size-1, format, argptr);  
	if (res == -1) 
	{  
		buffer[0] = 0;  
		return -2;  
	} 
	buffer[res]= 0;
	return res;  
}


#endif


//////////////////////////////////////////////////////////////
/*功能:释放动态分配的内存，一维数组
参数:pT:要释放内存指针的地址
*/
//////////////////////////////////////////////////////////////////////////

char* Safe_malloc(int length, unsigned int maxUpLevel)
{
	if (length <= 0 || length > maxUpLevel)
	{
		return NULL;
	}
	char *pArray; 
#ifdef WIN32
	__try
#else
	try
#endif
	{
		pArray = (char *)malloc(length);
	}
#ifdef WIN32
	__except( EXCEPTION_EXECUTE_HANDLER )
	{
		return NULL;
	}
#else
	catch(...)
	{
		return NULL;
	}
#endif
	if (NULL == pArray)
	{
		return NULL;
	}
	memset(pArray, 0, length);
	return pArray;
}

//////////////////////////////////////////////////////////////////////////
/*功能:释放动态分配的内存，一维数组
参数:pT:要释放内存指针的地址
*/
//////////////////////////////////////////////////////////////////////////

void Safe_free(char *&pT)
{
	if(NULL != pT&&!IsInValidReadPtr(pT,sizeof(char)))
	{
		free(pT);
		pT = NULL;
	}
}




//////////////////////////////////////////////////////////////////////////
/*功能:清空内存
参数:	_Dst:需清空的内存
		sizeof_Dst：定义（申请）的内存大小
		_Val：初始值
		_Size：清空的长度
*/
//////////////////////////////////////////////////////////////////////////
void *  Safe_memset(void * _Dst, unsigned int sizeof_Dst, int _Val, int _Size)
{
	if (_Dst == NULL ||IsInValidWritePtr(_Dst,sizeof_Dst))
	{
		return NULL;
	}

	if ( _Size < 0 || sizeof_Dst < _Size)
	{
		return NULL;
	}

	return memset(_Dst, _Val, _Size);
}

//////////////////////////////////////////////////////////////////////////
/*功能:内存拷贝
参数:	dst:目的地址
		sizeof_Dst：目的内存大小
		src：源地址指针
		count：拷贝的长度
*/
//////////////////////////////////////////////////////////////////////////
void *  Safe_memcpy (void * dst, unsigned int sizeof_Dst, const void * src,  int count )
{
	if (dst == NULL || src == NULL ||IsInValidWritePtr(dst,sizeof_Dst) || IsInValidReadPtr(src, count))
	{
		return NULL;
	}
	if ( count < 0 || sizeof_Dst < count)
	{
		return NULL;
	}
	return memcpy (dst,src,count );
}

//////////////////////////////////////////////////////////////////////////
/*功能:内存比较
参数:	dst:目的地址
src：源地址指针
len：比较的长度
err：返回值 0：正确，其他错误
*/
//////////////////////////////////////////////////////////////////////////
int Safe_memcmp (const void *dst, const void *src, unsigned int len,int& err)
{
	err = 0;
	if (dst == NULL || src == NULL || IsInValidReadPtr(dst,len) || IsInValidReadPtr(src,len))
	{
		err = 1;
		return -2;
	}

	return memcmp (dst,src,len );
}

//////////////////////////////////////////////////////////////////////////
/*功能:计算字符长度
参数:	_Str:字符串指针
返回值： -1：指针错误，其他：字符串长度
*/
//////////////////////////////////////////////////////////////////////////
int  Safe_strlen(const char * _Str)
{
	if (_Str == NULL||IsInValidReadPtr(_Str,1))
	{
		return -1;
	}
	return (int)strlen(_Str);
}
//////////////////////////////////////////////////////////////////////////
/*功能:字符串比较
参数 s1 big strings s2 small strings
参数 err 返回引用 err=1错误 
*/
//////////////////////////////////////////////////////////////////////////
const char *  Safe_strstr(const char *s1,const char *s2,int& err)
{
	err=0;
	if (s1 == NULL||s2 == NULL||IsInValidReadPtr(s1,1) || IsInValidReadPtr(s2,1))
	{
		err=1;
		return NULL;
	}
	return strstr(s1,s2);
}
//////////////////////////////////////////////////////////////////////////
/*功能:字符串拷贝
参数:	dst:目的字符串buf指针
		sizeof_Dst：目的字符buf长度
		src：源字符串buf指针

返回值： NULL：指针错误，其他：dst地址
*/
//////////////////////////////////////////////////////////////////////////
char *  Safe_strcpy(char * dst, int sizeof_Dst, const char * src)
{
	int n = Safe_strlen(src);
	if (n < 0 || n >= sizeof_Dst)
	{
		return NULL;
	}

	if (dst == NULL||IsInValidWritePtr(dst, sizeof_Dst) || IsInValidReadPtr(src,n))
	{
		return NULL;
	}

	return strcpy(dst, src);
}

//////////////////////////////////////////////////////////////////////////
/*功能:字符串拷贝
参数:	dst:目的字符串buf指针
		sizeof_Dst：目的字符buf长度
		src：源字符串buf指针
		count：拷贝的长度

返回值： NULL：指针错误，其他：dst地址
*/
//////////////////////////////////////////////////////////////////////////
char * Safe_strncpy(char * dst, unsigned int sizeof_Dst, const char * source, int count)
{
	int n = Safe_strlen(source);
	if (n < 0 || n < count || count >= sizeof_Dst)
	{
		return NULL;
	}

	if ( dst == NULL||IsInValidWritePtr(dst, sizeof_Dst) || IsInValidReadPtr(source, count))
	{
		return NULL;
	}
	if (count < 0 )
	{
		return NULL;
	}

	return strncpy(dst, source, count);
}

//////////////////////////////////////////////////////////////////////////
/*功能:字符串链接
参数:	dst:目的字符串buf指针
		sizeof_Dst：目的字符buf长度
		src：源字符串buf指针

返回值： NULL：指针错误，其他：dst地址
*/
//////////////////////////////////////////////////////////////////////////
char *  Safe_strcat(char * dst, int sizeof_Dst, const char * src)
{
	int ndst = Safe_strlen(dst);
	if (ndst < 0)
	{
		return NULL;
	}
	int n = Safe_strlen(src);
	if (n < 0 || n >= sizeof_Dst-ndst)
	{
		return NULL;
	}

	if (IsInValidWritePtr(dst,ndst) || IsInValidReadPtr(src, n))
	{
		return NULL;
	}
	
	return strcat(dst,src);
}

//////////////////////////////////////////////////////////////////////////
/*功能:字符串链接
参数:	dst:目的字符串buf指针
	sizeof_Dst：目的字符buf长度
	src：源字符串buf指针
	count：需要链接的长度
返回值： NULL：指针错误，其他：dst地址
*/
//////////////////////////////////////////////////////////////////////////
char* Safe_strncat(char * dst, int sizeof_Dst, const char * src, int count)
{
	int ndst = Safe_strlen(dst);
	if (count < 0 || ndst < 0)
	{
		return NULL;
	}
	int n = Safe_strlen(src);

	if (n < 0 || n < count || count >= sizeof_Dst-ndst)
	{
		return NULL;
	}
	if (IsInValidWritePtr(dst, sizeof_Dst) || IsInValidReadPtr(src,count))
	{
		return NULL;
	}
	
	return strncat(dst,src,count);
}

//////////////////////////////////////////////////////////////////////////
/*功能:字符串比较
参数:	src：源字符串buf指针
		dst:目的字符串buf指针
		err：返回函数调用的错误值 0：正确， 其他：错误
返回值： =0：相同，<0:src<dst ,>0: src>dst
*/
//////////////////////////////////////////////////////////////////////////
int  Safe_strcmp (const char * src, const char * dst, int& err)
{
	err = 0;
	if (src == NULL || dst == NULL||IsInValidReadPtr(dst,1) || IsInValidReadPtr(src,1))
	{
		err = 1;
		return -2;
	}
	return strcmp(src,dst);
}

//////////////////////////////////////////////////////////////////////////
/*功能:字符串比较
参数:	src：源字符串buf指针
		dst:目的字符串buf指针
		count:需要比较的长度
		err：返回函数调用的错误值 0：正确， 其他：错误
返回值： =0：相同，<0:src<dst ,>0: src>dst
*/
//////////////////////////////////////////////////////////////////////////
int  Safe_strncmp(const char *src, const char *dst, unsigned int count, int& err)
{
	err = 0;
	int n1 = Safe_strlen(src);
	int n2 = Safe_strlen(dst);
	if (n1 < 0 || n2 < 0 || n1 < count || n2 < count)
	{
		err = 1;
		return -2;
	}

	err=0;
	if (IsInValidReadPtr(dst,n2) || IsInValidReadPtr(src,n1))
	{
		err = 1;
		return -2;
	}
	
	return strncmp(src,dst,count);
}

//////////////////////////////////////////////////////////////////////////
/*功能:字符串格式化
参数:	buffer：源字符串buf指针
buf_size:源字符串buf大小
format:格式化
argptr:参数
返回值： 字符串长度
*/
//////////////////////////////////////////////////////////////////////////
int Safe_vsprintf (char *buffer, int buf_size,const char *format, va_list argptr)
{  
	if (buf_size < 1 || IsInValidWritePtr(buffer, buf_size) )
	{  
		return -1;  
	}  

	int res = vsnprintf(buffer, buf_size-1, format, argptr);  
	if (res == -1) 
	{  
		buffer[0] = 0;  
		return -2;  
	} 
	buffer[res]= 0;
	return res;  
} 


#ifdef WIN32

bool  GetFinallylogPath(SYSTEMTIME& sm,char* moduleName,char* ApplogPath)
{

	if (!GetCurrentModulePath(ApplogPath, MAX_PATH))
	{
		return false;
	}
	CreateDirectoryA(ApplogPath,NULL);	
	char ttmp[1024] = "";
	if (0 > Safe_sprintf(ttmp,1024,"\\%04d-%02d-%02d",sm.wYear,sm.wMonth,sm.wDay))
	{
		return false;
	}

	if (!Safe_strcat(ApplogPath,1024,ttmp))
	{
		return false;
	}

	CreateDirectoryA(ApplogPath,NULL);	
	if (0 > Safe_sprintf(ttmp,1024,"\\%s_%04d-%02d-%02d.log",moduleName,sm.wYear,sm.wMonth,sm.wDay))
	{
		return false;
	}

	if (!Safe_strcat(ApplogPath,1024,ttmp))
	{
		return false;
	}
	return true;
}

CGaglog::CGaglog(const char *moduleName)
{
	m_fp=NULL;
	SetModuleName(moduleName);
}
void  CGaglog::SetModuleName(const char *moduleName)
{
	Safe_strcpy(m_moduleName,sizeof(m_moduleName),moduleName);
	if (Safe_strlen(m_moduleName)==0)
	{
		Safe_strcpy(m_moduleName,sizeof(m_moduleName),"NotSet");
	}
	if (m_fp)
	{
		CloseHandle(m_fp);
		m_fp=NULL;
	}
	SYSTEMTIME sm;
	GetLocalTime(&sm);
	Safe_sprintf(m_CurYYR,100,"%04d-%02d-%02d",sm.wYear,sm.wMonth,sm.wDay);
}

void CGaglog::WriteLog(const char* strlog)
{	
	OutputDebugStringA(strlog);	
	OutputDebugStringA("\n");	

	SYSTEMTIME sm;
	GetLocalTime(&sm);
	char   strTime[1024]=""; 
	if (0 > Safe_sprintf(strTime,1024,"[%04d-%02d-%02d %02d-%02d-%02d-%03d] ",sm.wYear,sm.wMonth,sm.wDay,sm.wHour,sm.wMinute,sm.wSecond,sm.wMilliseconds))
	{
		return;
	}

	char temp_CurYYR[100]="";
	Safe_sprintf(temp_CurYYR,100,"%04d-%02d-%02d",sm.wYear,sm.wMonth,sm.wDay);
	int err=0;
	bool bCloseFp=false;
	if (m_fp==NULL)
	{
		bCloseFp=true;
	}

	if (Safe_strcmp(m_CurYYR,temp_CurYYR,err)!=0)
	{
		Safe_sprintf(m_CurYYR,100,"%04d-%02d-%02d",sm.wYear,sm.wMonth,sm.wDay);
		bCloseFp=true;
		if (m_fp)
		{
			CloseHandle(m_fp);
			m_fp=NULL;
		}
	}
	char ApplogPath[1024] = "";
	if (bCloseFp)
	{
		if (GetFinallylogPath(sm,m_moduleName,ApplogPath)==false)
		{
			return;
		}
		m_fp=CreateFileA(ApplogPath,GENERIC_READ|GENERIC_WRITE,  FILE_SHARE_READ |FILE_SHARE_WRITE  /*FILE_SHARE_DELETE*/,NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
		SetFilePointer(m_fp, 0, NULL, FILE_END); 
	}
	if (m_fp)
	{
		DWORD nWrite=0;
		WriteFile(m_fp,strTime,Safe_strlen(strTime),&nWrite,NULL);
		WriteFile(m_fp,strlog,Safe_strlen(strlog),&nWrite,NULL);
		WriteFile(m_fp,"\r\n",Safe_strlen("\r\n"),&nWrite,NULL);
		//FlushFileBuffers(m_fp);
	}
}
CGaglog::~CGaglog()
{
	if (m_fp)
	{
		CloseHandle(m_fp);
		m_fp=NULL;
	}
}

#endif


void ErrTo_log(const char* moduleName,const char* Errlog)
{
#ifdef WIN32
	CGaglog a(moduleName);
	a.WriteLog(Errlog);
#else
	if (access("/root/parse_lib/debuglog",0)!=-1)
	{
		printf(moduleName);
		printf(Errlog);
		printf("\n");
	}
#endif
}

#ifdef WIN32
void ErrTo_logW(const wchar_t* WmoduleName,const wchar_t* WErrlog)
{
	char moduleName[MAX_PATH]={0};
	Safe_WtoA(const_cast<wchar_t*>(WmoduleName),moduleName,MAX_PATH);	
	int wLen=Safe_wcslen(WErrlog);
	char *ErrLog=Safe_new<char>(wLen*2+2);
	if (ErrLog)
	{
		Safe_WtoA(const_cast<wchar_t*>(WErrlog),ErrLog,wLen*2+2);
		ErrTo_log(moduleName,ErrLog);
		Safe_delete<char> (ErrLog);	
	}
}
#endif




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
/*����:�����ַ�����
����:	_Str:�ַ���ָ��
����ֵ�� -1��ָ������������ַ�������
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
/*����:�ַ�������
����:	dst:Ŀ���ַ���bufָ��
countof_Dst��������wchar �ĸ��� wchar_t wBuf[1024]; countof_Dst = 1024
src��Դ�ַ���bufָ��

����ֵ�� NULL��ָ�����������dst��ַ
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
/*����:�ַ�������
����:	dst:Ŀ���ַ���bufָ��
countof_Dst��������wchar �ĸ��� wchar_t wBuf[1024]; countof_Dst = 1024
src��Դ�ַ���bufָ��
count�������ĳ���

����ֵ�� NULL��ָ�����������dst��ַ
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
/*����:�ַ�������
����:	dst:Ŀ���ַ���bufָ��
countof_Dst��������wchar �ĸ��� wchar_t wBuf[1024]; countof_Dst = 1024
src��Դ�ַ���bufָ��

����ֵ�� NULL��ָ�����������dst��ַ
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
/*����:�ַ�������
����:	dst:Ŀ���ַ���bufָ��
countof_Dst��������wchar �ĸ��� wchar_t wBuf[1024]; countof_Dst = 1024
src��Դ�ַ���bufָ��
count����Ҫ���ӵĳ���
����ֵ�� NULL��ָ�����������dst��ַ
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
/*����:�ַ����Ƚ�
����:	src��Դ�ַ���bufָ��
dst:Ŀ���ַ���bufָ��
err�����غ������õĴ���ֵ 0����ȷ�� ����������
����ֵ�� =0����ͬ��<0:src<dst ,>0: src>dst
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
/*����:�ַ����Ƚ�
����:	src��Դ�ַ���bufָ��
dst:Ŀ���ַ���bufָ��
count:��Ҫ�Ƚϵĳ��ȣ��ַ�����
err�����غ������õĴ���ֵ 0����ȷ�� ����������
����ֵ�� =0����ͬ��<0:src<dst ,>0: src>dst
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
/* �������ƣ�Safe_WtoA
/* ��    �ܣ����ֽ�ת���ֽ�        
/* ���������ptrW�����ֽ�����
/* ���������ptrA�����ֽ�����
/* ���������nLenA�����ֽ����ݳ���
/* ����ֵ  �����ݳ���   
/* �޸����ݣ�  
/*     1.  ����                                        
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
/* �������ƣ�Safe_AtoW
/* ��    �ܣ����ֽ�ת���ֽ�    
/* ���������ptrA�����ֽ�����
/* ���������ptrW�����ֽ�����
/* ���������nLenW�����ֽ����ݳ���
/* ����ֵ  �����ݳ���   
/* �޸����ݣ�  
/*     1.  ����                                        
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
/*����:�ַ�����ʽ��
����:	buffer��Դ�ַ���bufָ��
buf_size:Դ�ַ���buf��С
format:��ʽ��
argptr:����
����ֵ�� �ַ�������
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
/*����:�ͷŶ�̬������ڴ棬һά����
����:pT:Ҫ�ͷ��ڴ�ָ��ĵ�ַ
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
/*����:�ͷŶ�̬������ڴ棬һά����
����:pT:Ҫ�ͷ��ڴ�ָ��ĵ�ַ
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
/*����:����ڴ�
����:	_Dst:����յ��ڴ�
		sizeof_Dst�����壨���룩���ڴ��С
		_Val����ʼֵ
		_Size����յĳ���
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
/*����:�ڴ濽��
����:	dst:Ŀ�ĵ�ַ
		sizeof_Dst��Ŀ���ڴ��С
		src��Դ��ַָ��
		count�������ĳ���
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
/*����:�ڴ�Ƚ�
����:	dst:Ŀ�ĵ�ַ
src��Դ��ַָ��
len���Ƚϵĳ���
err������ֵ 0����ȷ����������
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
/*����:�����ַ�����
����:	_Str:�ַ���ָ��
����ֵ�� -1��ָ������������ַ�������
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
/*����:�ַ����Ƚ�
���� s1 big strings s2 small strings
���� err �������� err=1���� 
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
/*����:�ַ�������
����:	dst:Ŀ���ַ���bufָ��
		sizeof_Dst��Ŀ���ַ�buf����
		src��Դ�ַ���bufָ��

����ֵ�� NULL��ָ�����������dst��ַ
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
/*����:�ַ�������
����:	dst:Ŀ���ַ���bufָ��
		sizeof_Dst��Ŀ���ַ�buf����
		src��Դ�ַ���bufָ��
		count�������ĳ���

����ֵ�� NULL��ָ�����������dst��ַ
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
/*����:�ַ�������
����:	dst:Ŀ���ַ���bufָ��
		sizeof_Dst��Ŀ���ַ�buf����
		src��Դ�ַ���bufָ��

����ֵ�� NULL��ָ�����������dst��ַ
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
/*����:�ַ�������
����:	dst:Ŀ���ַ���bufָ��
	sizeof_Dst��Ŀ���ַ�buf����
	src��Դ�ַ���bufָ��
	count����Ҫ���ӵĳ���
����ֵ�� NULL��ָ�����������dst��ַ
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
/*����:�ַ����Ƚ�
����:	src��Դ�ַ���bufָ��
		dst:Ŀ���ַ���bufָ��
		err�����غ������õĴ���ֵ 0����ȷ�� ����������
����ֵ�� =0����ͬ��<0:src<dst ,>0: src>dst
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
/*����:�ַ����Ƚ�
����:	src��Դ�ַ���bufָ��
		dst:Ŀ���ַ���bufָ��
		count:��Ҫ�Ƚϵĳ���
		err�����غ������õĴ���ֵ 0����ȷ�� ����������
����ֵ�� =0����ͬ��<0:src<dst ,>0: src>dst
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
/*����:�ַ�����ʽ��
����:	buffer��Դ�ַ���bufָ��
buf_size:Դ�ַ���buf��С
format:��ʽ��
argptr:����
����ֵ�� �ַ�������
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




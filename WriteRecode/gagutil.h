
#pragma once

#include<stdio.h>
#include<memory.h>
#include <stdlib.h>
#include <stdarg.h>

#ifndef MAX_MALLOC_SIZE
#define  MAX_MALLOC_SIZE     1024 * 1024 * 1024 //1G
#endif

#ifdef WIN32
#include <excpt.h>
#pragma warning (disable:4018)
#pragma warning (disable:4996)
#pragma warning (disable:4800)
#pragma warning (disable:4996)
#pragma warning (disable:4267)
#else
#include <unistd.h>
#endif
#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#ifdef WIN32
#include <Windows.h>
#endif



#ifdef WIN32

inline BOOL GetCurrentModulePath(__out char* pPathBuff, __in size_t bufOfSize)
{
	char tmp[MAX_PATH],driver[MAX_PATH],path[MAX_PATH],filename[MAX_PATH],ext[MAX_PATH];
	::GetModuleFileNameA(NULL,tmp,MAX_PATH);
	_splitpath(tmp,driver,path,filename,ext);
	sprintf_s(pPathBuff,bufOfSize,"%s%s",driver,path);
	return TRUE;
}
#endif

inline bool IsInValidReadPtr(const void*lp,unsigned int ucb)
{
#ifdef WIN32
	return IsBadReadPtr(lp,ucb);
#else
	return false;
#endif
}

inline bool IsInValidWritePtr(void *lp,unsigned int ucb)
{
#ifdef WIN32
	return IsBadWritePtr(lp,ucb);
#else
	return false;
#endif
}
//////////////////////////////////////////////////////////////////////////
/*����:�����ڴ棬һά����
 ����:   class T ģ������
 ����:length:Ҫ�����ڴ�Ĵ�С
 ����: maxUpLevel�����ڴ��С����
 ����:T*:�����ڴ�ָ��ĵ�ַ
*/
//////////////////////////////////////////////////////////////////////////
template<class T>
T* Safe_new(int length, unsigned int maxUpLevel=MAX_MALLOC_SIZE)
{
	if (length <= 0 || length * sizeof(T) > maxUpLevel)
	{
		return NULL;
	}
	T *pArray;
#ifdef WIN32
	__try
#else
	try
#endif
	{
		pArray = new T[length];
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
	memset(pArray, 0, length * sizeof(T));
	return pArray;
}
//////////////////////////////////////////////////////////////////////////
/*����:�ͷ��ڴ棬һά����
����:   class T ģ������
����:pT:Ҫ�ͷ��ڴ�ָ��ĵ�ַ
����:
*/
//////////////////////////////////////////////////////////////////////////
template<class T>
void Safe_delete(T *&pT)
{
	if(NULL != pT&&!IsInValidReadPtr(pT,sizeof(T)))
	{
		delete[] pT;
		pT = NULL;
	}
}


//////////////////////////////////////////////////////////////////////////
/*����:��̬�����ڴ棬��ά����
����:   class T ģ������
����:column:������
����:row:������
����: maxUpLevel�����ڴ��С����
*/
//////////////////////////////////////////////////////////////////////////
template<class T>
T** Safe_newArray(int column, int row,unsigned int maxUpLevel=MAX_MALLOC_SIZE)
{
	if (column <= 0|| row <= 0 || row * column * sizeof(T) > maxUpLevel)
	{
		return NULL;
	}
	T **pArray;
#ifdef WIN32
	__try
#else
	try
#endif
	{
		pArray= new T*[row];
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


#ifdef WIN32
	__try
#else
	try
#endif
	{
		pArray[0] = new T[row * column];
	}
#ifdef WIN32
	__except( EXCEPTION_EXECUTE_HANDLER )
	{
		Safe_delete<T*>(pArray);
		return NULL;
	}
#else
	catch(...)
	{
		Safe_delete<T*>(pArray);
		return NULL;
	}
#endif
	if (NULL == pArray[0])
	{
		Safe_delete<T*>(pArray);
		return NULL;
	}
	memset(pArray[0], 0, row * column * sizeof(T));
	for(int i = 1; i < row; i++)
	{
		pArray[i] = pArray[i - 1] + column;
	}
	return pArray;
}

//////////////////////////////////////////////////////////////////////////
template<class T>
void Safe_deleteArray(T **&pT)
{
	if (pT != NULL&&!IsInValidReadPtr(pT,sizeof(T)))
	{
		Safe_delete<T>(pT[0]);
		Safe_delete<T*>(pT);
	}
}
//////////////////////////////////////////////////////////////////////////
/*����:�ַ�����ʽ��
����:	buf��Դ�ַ���bufָ��
buf_size:Դ�ַ���buf��С
fmt:��ʽ��
...:����
����ֵ�� �ַ�������
*/
//////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/*����:�ַ�����ʽ��
����:	buf��Դ�ַ���bufָ��
buf_size:Դ�ַ���buf��С
fmt:��ʽ��
...:����
����ֵ�� �ַ�������
*/
//////////////////////////////////////////////////////////////////////////
inline int Safe_sprintf(char* buf, int buf_size, const char* fmt, ...)
{  
	if (IsInValidWritePtr(buf,sizeof(buf))||buf_size< 1)
	{  
		return -1;  
	}  
	va_list ap;  
	va_start(ap, fmt);  
	int res = vsnprintf(buf, buf_size, fmt, ap);  
	va_end(ap);  
	if (res == -1||res>=buf_size) 
	{  
		buf[0] = 0;  
		return -2;  
	} 
	buf[res]= 0;
	return res;  
} 





//////////////////////////////////////////////////////////////////////////
/*����:�����ڴ棬һά����
����:length:Ҫ�����ڴ�Ĵ�С
����: maxUpLevel�����ڴ��С����
����:char*:�����ڴ�ָ��ĵ�ַ
*/
//////////////////////////////////////////////////////////////////////////
char* Safe_malloc(int length, unsigned int maxUpLevel=MAX_MALLOC_SIZE);
//////////////////////////////////////////////////////////////////////////
/*����:�ͷ��ڴ棬һά����
����:pT:Ҫ�ͷ��ڴ�ָ��ĵ�ַ
����:
*/
//////////////////////////////////////////////////////////////////////////
 void Safe_free(char *&pT);

//////////////////////////////////////////////////////////////////////////
/*����:�ͷŶ�̬������ڴ�, ��ά����
����:   class T ģ������
����:pT:Ҫ�ͷ��ڴ�ָ��ĵ�ַ
*/
//////////////////////////////////////////////////////////////////////////
/*����:����ڴ�
����:	_Dst:����յ��ڴ�
sizeof_Dst�����壨���룩���ڴ��С
_Val����ʼֵ
_Size����յĳ���
*/
//////////////////////////////////////////////////////////////////////////
 void *  Safe_memset(void * _Dst, unsigned int sizeof_Dst, int _Val, int _Size);
//////////////////////////////////////////////////////////////////////////
/*����:�ڴ濽��
����:	dst:Ŀ�ĵ�ַ
sizeof_Dst��Ŀ���ڴ��С
src��Դ��ַָ��
count�������ĳ���
*/
//////////////////////////////////////////////////////////////////////////
 void *  Safe_memcpy (void * dst, unsigned int sizeof_Dst, const void * src,  int count );

//////////////////////////////////////////////////////////////////////////
/*����:�ڴ�Ƚ�
����:	dst:Ŀ�ĵ�ַ
src��Դ��ַָ��
len���Ƚϵĳ���
err������ֵ 0����ȷ����������
*/
//////////////////////////////////////////////////////////////////////////
 int  Safe_memcmp (const void *dst, const void *src, unsigned int len,int& err);

//////////////////////////////////////////////////////////////////////////
/*����:�����ַ�����
����:	_Str:�ַ���ָ��
����ֵ�� -1��ָ������������ַ�������
*/
//////////////////////////////////////////////////////////////////////////
 int  Safe_strlen(const char * _Str);
//////////////////////////////////////////////////////////////////////////
/*����:�ַ����Ƚ�
���� s1 big strings s2 small strings
���� err �������� err=1���� 
*/
//////////////////////////////////////////////////////////////////////////
 const char *  Safe_strstr(const char *s1,const char *s2,int& err);
//////////////////////////////////////////////////////////////////////////
/*����:�ַ�������
����:	dst:Ŀ���ַ���bufָ��
sizeof_Dst��Ŀ���ַ�buf����
src��Դ�ַ���bufָ��

����ֵ�� NULL��ָ�����������dst��ַ
*/
//////////////////////////////////////////////////////////////////////////
 char *  Safe_strcpy(char * dst, int sizeof_Dst, const char * src);
//////////////////////////////////////////////////////////////////////////
/*����:�ַ�������
����:	dst:Ŀ���ַ���bufָ��
sizeof_Dst��Ŀ���ַ�buf����
src��Դ�ַ���bufָ��
count�������ĳ���

����ֵ�� NULL��ָ�����������dst��ַ
*/
//////////////////////////////////////////////////////////////////////////
 char *  Safe_strncpy(char * dst, unsigned int sizeof_Dst, const char * source, int count);
//////////////////////////////////////////////////////////////////////////
/*����:�ַ�������
����:	dst:Ŀ���ַ���bufָ��
sizeof_Dst��Ŀ���ַ�buf����
src��Դ�ַ���bufָ��

����ֵ�� NULL��ָ�����������dst��ַ
*/
//////////////////////////////////////////////////////////////////////////
 char *  Safe_strcat(char * dst, int sizeof_Dst, const char * src);
//////////////////////////////////////////////////////////////////////////
/*����:�ַ�������
����:	dst:Ŀ���ַ���bufָ��
sizeof_Dst��Ŀ���ַ�buf����
src��Դ�ַ���bufָ��
count����Ҫ���ӵĳ���
����ֵ�� NULL��ָ�����������dst��ַ
*/
//////////////////////////////////////////////////////////////////////////
 char *  Safe_strncat(char * dst, int sizeof_Dst, const char * src, int count);
//////////////////////////////////////////////////////////////////////////
/*����:�ַ����Ƚ�
����:	src��Դ�ַ���bufָ��
dst:Ŀ���ַ���bufָ��
err�����غ������õĴ���ֵ 0����ȷ�� ����������
����ֵ�� =0����ͬ��<0:src<dst ,>0: src>dst
*/
//////////////////////////////////////////////////////////////////////////
 int  Safe_strcmp (const char * src, const char * dst, int& err);
//////////////////////////////////////////////////////////////////////////
/*����:�ַ����Ƚ�
����:	src��Դ�ַ���bufָ��
dst:Ŀ���ַ���bufָ��
count:��Ҫ�Ƚϵĳ���
err�����غ������õĴ���ֵ 0����ȷ�� ����������
����ֵ�� =0����ͬ��<0:src<dst ,>0: src>dst
*/
//////////////////////////////////////////////////////////////////////////
 int  Safe_strncmp(const char *src, const char *dst, unsigned int count, int& err);

#ifdef WIN32



//////////////////////////////////////////////////////////////////////////
/*����:ȫ�ֶ��������ڴ�
����:dwBytes:Ҫ�����ڴ�Ĵ�С
     uFlags����־
����: maxUpLevel�����ڴ��С����
����:���������óɹ�,�򷵻�һ���·�����ڴ����ľ����
     ����������ʧ��,�򷵻�NULL
*/
//////////////////////////////////////////////////////////////////////////
 HGLOBAL Safe_GlobalAlloc ( UINT uFlags,SIZE_T dwBytes, unsigned int maxUpLevel=MAX_MALLOC_SIZE);
//////////////////////////////////////////////////////////////////////////
/*����:�����ڴ��ͷ�
����:hMem:�ڴ����ľ��
*/
//////////////////////////////////////////////////////////////////////////
 void Safe_GlobalFree( HGLOBAL hMem);
//////////////////////////////////////////////////////////////////////////
/*����:���������ڴ�
����:dwBytes:Ҫ�����ڴ�Ĵ�С
uFlags����־
����: maxUpLevel�����ڴ��С����
����:���������óɹ�,�򷵻�һ���·�����ڴ����ľ����
����������ʧ��,�򷵻�NULL
*/
//////////////////////////////////////////////////////////////////////////
 HLOCAL Safe_LocalAlloc ( UINT uFlags,SIZE_T dwBytes, unsigned int maxUpLevel=MAX_MALLOC_SIZE);
/*����:�����ڴ��ͷ�
����:hMem:�ڴ����ľ��
*/
//////////////////////////////////////////////////////////////////////////
 void Safe_LocalFree( HLOCAL hMem);

//////////////////////////////////////////////////////////////////////////
/*����:�����ַ�����
����:	_Str:�ַ���ָ��
����ֵ�� -1��ָ������������ַ�������
*/
//////////////////////////////////////////////////////////////////////////
 int  Safe_wcslen(const wchar_t * _Str);
//////////////////////////////////////////////////////////////////////////
/*����:�ַ����Ƚ�
���� s1 big strings s2 small strings
���� err �������� err=1���� 
*/
//////////////////////////////////////////////////////////////////////////
 const wchar_t *  Safe_wcsstr(const wchar_t *s1,const wchar_t *s2,int& err);
//////////////////////////////////////////////////////////////////////////
/*����:�ַ�������
����:	dst:Ŀ���ַ���bufָ��
countof_Dst��������wchar �ĸ��� wchar_t wBuf[1024]; countof_Dst = 1024
src��Դ�ַ���bufָ��

����ֵ�� NULL��ָ�����������dst��ַ
*/
//////////////////////////////////////////////////////////////////////////
 wchar_t *  Safe_wcscpy(wchar_t * dst, int countof_Dst, const wchar_t * src);
//////////////////////////////////////////////////////////////////////////
/*����:�ַ�������
����:	dst:Ŀ���ַ���bufָ��
countof_Dst��������wchar �ĸ��� wchar_t wBuf[1024]; countof_Dst = 1024
src��Դ�ַ���bufָ��
count�������ĳ���

����ֵ�� NULL��ָ�����������dst��ַ
*/
//////////////////////////////////////////////////////////////////////////
 wchar_t * Safe_wcsncpy(wchar_t * dst, unsigned int countof_Dst, const wchar_t * source, int count);
//////////////////////////////////////////////////////////////////////////
/*����:�ַ�������
����:	dst:Ŀ���ַ���bufָ��
countof_Dst��������wchar �ĸ��� wchar_t wBuf[1024]; countof_Dst = 1024
src��Դ�ַ���bufָ��

����ֵ�� NULL��ָ�����������dst��ַ
*/
//////////////////////////////////////////////////////////////////////////
 wchar_t *  Safe_wcscat(wchar_t * dst, int countof_Dst, const wchar_t * src);
//////////////////////////////////////////////////////////////////////////
/*����:�ַ�������
����:	dst:Ŀ���ַ���bufָ��
countof_Dst��������wchar �ĸ��� wchar_t wBuf[1024]; countof_Dst = 1024
src��Դ�ַ���bufָ��
count����Ҫ���ӵĳ���
����ֵ�� NULL��ָ�����������dst��ַ
*/
//////////////////////////////////////////////////////////////////////////
 wchar_t *  Safe_wcsncat(wchar_t * dst, int countof_Dst, const wchar_t * src, int count);
//////////////////////////////////////////////////////////////////////////
/*����:�ַ����Ƚ�
����:	src��Դ�ַ���bufָ��
dst:Ŀ���ַ���bufָ��
err�����غ������õĴ���ֵ 0����ȷ�� ����������
����ֵ�� =0����ͬ��<0:src<dst ,>0: src>dst
*/
//////////////////////////////////////////////////////////////////////////
 int  Safe_wcscmp (const wchar_t * src, const wchar_t * dst, int& err);
//////////////////////////////////////////////////////////////////////////
/*����:�ַ����Ƚ�
����:	src��Դ�ַ���bufָ��
dst:Ŀ���ַ���bufָ��
count:��Ҫ�Ƚϵĳ��ȣ��ַ�����
err�����غ������õĴ���ֵ 0����ȷ�� ����������
����ֵ�� =0����ͬ��<0:src<dst ,>0: src>dst
*/
//////////////////////////////////////////////////////////////////////////
 int  Safe_wcsncmp(const wchar_t *src, const wchar_t *dst, unsigned int count, int& err);

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
 int  WINAPI Safe_AtoW(IN const char *ptrA,OUT WCHAR *ptrW,IN int nLenW);
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
 int  WINAPI Safe_WtoA(IN WCHAR *ptrW,OUT char *ptrA,IN int nLenA);

//////////////////////////////////////////////////////////////////////////
/*����:�ַ�����ʽ��
����:	buffer��Դ�ַ���bufָ��
buf_size:Դ�ַ���buf��С
format:��ʽ��
argptr:����
����ֵ�� �ַ�������
*/
//////////////////////////////////////////////////////////////////////////
 int Safe_vsprintf (char *buffer, int buf_size,const char *format, va_list argptr);

//////////////////////////////////////////////////////////////////////////
/*����:�ַ�����ʽ��
����:	buffer��Դ�ַ���bufָ��
buf_size:Դ�ַ���buf��С
format:��ʽ��
argptr:����
����ֵ�� �ַ�������
*/
//////////////////////////////////////////////////////////////////////////
 int Safe_vswprintf(wchar_t *buffer, int buf_size,const wchar_t *format, va_list argptr);

//////////////////////////////////////////////////////////////////////////
/*����:�ַ�����ʽ��
����:	buf��Դ�ַ���bufָ��
countof_buf:Դ�ַ���buf���ַ�����С
fmt:��ʽ��
...:����
����ֵ�� �ַ�������
*/




#endif


void ErrTo_log(const char* moduleName,const char* Errlog);

void ErrTo_logW(const wchar_t* moduleName,const wchar_t* Errlog);


#ifdef WIN32

class CGaglog
{	
public:
	CGaglog(const char *moduleName);
	void WriteLog(const char* strlog);
	~CGaglog();
private:
	void  SetModuleName(const char *moduleName);
	HANDLE m_fp;
	char m_moduleName[100];
	char m_CurYYR[100];
};
//////////////////////////////////////////////////////////////////////////
inline int Safe_swprintf(wchar_t* buf, int countof_buf, const wchar_t* fmt, ...)
{  
	if (countof_buf < 1 || IsInValidWritePtr(buf, countof_buf))
	{  
		return -1;  
	}  
	va_list ap;  
	va_start(ap, fmt);  
	int res = _vsnwprintf(buf, countof_buf, fmt, ap);  
	va_end(ap);  
	if (res == -1 || res >= countof_buf) 
	{  
		buf[0] = 0;  
		return -2;  
	}  
	return res;  
} 
#endif



#ifndef ERRTO_LOG_RETURN
#define ERRTO_LOG_RETURN(MODULENAME,FUNCNAME,RET_VAL)   \
{														\
	char Errlog[2048]="";						        \
	sprintf (Errlog,"[Memory_Err] File:%s Line:%d  FUNCNAME=%s", __FILE__,__LINE__,FUNCNAME);	\
	ErrTo_log(MODULENAME,Errlog);					    \
	return RET_VAL;}	
#endif

#ifndef ERRTO_LOG_RETURNNOVAL
#define ERRTO_LOG_RETURNNOVAL(MODULENAME,FUNCNAME)      \
{ 											            \
	char Errlog[2048]="";						        \
	sprintf (Errlog,"[Memory_Err] File:%s Line:%d  FUNCNAME=%s", __FILE__,__LINE__,FUNCNAME);	\
	ErrTo_log(MODULENAME,Errlog);					    \
	return;}
#endif

#ifndef ERRTO_LOG_NORETURN
#define ERRTO_LOG_NORETURN(MODULENAME,FUNCNAME)      \
{ 											            \
	char Errlog[2048]="";						        \
	sprintf (Errlog,"[Memory_Err] File:%s Line:%d  FUNCNAME=%s", __FILE__,__LINE__,FUNCNAME);	\
	ErrTo_log(MODULENAME,Errlog);					    \
}
#endif

 inline char* memstr(char* full_data, int full_data_len,const char* substr,int sublen)  
{  
	if (full_data == NULL || full_data_len <= 0 || substr == NULL) {  
		return NULL;  
	}  
	if (*substr == '\0') {  
		return NULL;  
	}  
	int i=0;  
	char* cur = full_data;  
	int last_possible = full_data_len - sublen + 1;  
	for (i = 0; i < last_possible; i++) {  
		if (*cur == *substr) {  
			//assert(full_data_len - i >= sublen);  
			if (memcmp(cur, substr, sublen) == 0) {  
				//found  
				return cur;  
			}  
		}  
		cur++;  
	}  
	return NULL;  
}  
//////////////////////////////////////////////////////////////////////////
/*����:�ж��ڴ洮findstr�Ƿ����ڴ洮membuf���Ӵ�
����:	membuf:  �������ڴ洮
        memLen:  �������ڴ洮�ĳ���
        findstr: Ҫ�����ڴ洮
		FindNum: Ҫ�����ڴ洮�ĳ���
		findPtr:����ҵ�,��ú�������findstr��membuf���״γ��ֵĵ�ַ;����,����NULL
����ֵ�� �� 
*/

 inline void Safe_Memstrstr( char* membuf,int memLen,const  char* findstr, int FindNum,char* &findPtr)
 {
 	findPtr=memstr(membuf, memLen,findstr,FindNum); 
 }




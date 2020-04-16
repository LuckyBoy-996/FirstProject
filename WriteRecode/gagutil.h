
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
/*功能:申请内存，一维数组
 参数:   class T 模板类型
 参数:length:要申请内存的大小
 参数: maxUpLevel申请内存大小上限
 返回:T*:申请内存指针的地址
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
/*功能:释放内存，一维数组
参数:   class T 模板类型
参数:pT:要释放内存指针的地址
返回:
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
/*功能:动态分配内存，二维数组
参数:   class T 模板类型
参数:column:数组列
参数:row:数组行
参数: maxUpLevel申请内存大小上限
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
/*功能:字符串格式化
参数:	buf：源字符串buf指针
buf_size:源字符串buf大小
fmt:格式化
...:参数
返回值： 字符串长度
*/
//////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/*功能:字符串格式化
参数:	buf：源字符串buf指针
buf_size:源字符串buf大小
fmt:格式化
...:参数
返回值： 字符串长度
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
/*功能:申请内存，一维数组
参数:length:要申请内存的大小
参数: maxUpLevel申请内存大小上限
返回:char*:申请内存指针的地址
*/
//////////////////////////////////////////////////////////////////////////
char* Safe_malloc(int length, unsigned int maxUpLevel=MAX_MALLOC_SIZE);
//////////////////////////////////////////////////////////////////////////
/*功能:释放内存，一维数组
参数:pT:要释放内存指针的地址
返回:
*/
//////////////////////////////////////////////////////////////////////////
 void Safe_free(char *&pT);

//////////////////////////////////////////////////////////////////////////
/*功能:释放动态分配的内存, 二维数组
参数:   class T 模板类型
参数:pT:要释放内存指针的地址
*/
//////////////////////////////////////////////////////////////////////////
/*功能:清空内存
参数:	_Dst:需清空的内存
sizeof_Dst：定义（申请）的内存大小
_Val：初始值
_Size：清空的长度
*/
//////////////////////////////////////////////////////////////////////////
 void *  Safe_memset(void * _Dst, unsigned int sizeof_Dst, int _Val, int _Size);
//////////////////////////////////////////////////////////////////////////
/*功能:内存拷贝
参数:	dst:目的地址
sizeof_Dst：目的内存大小
src：源地址指针
count：拷贝的长度
*/
//////////////////////////////////////////////////////////////////////////
 void *  Safe_memcpy (void * dst, unsigned int sizeof_Dst, const void * src,  int count );

//////////////////////////////////////////////////////////////////////////
/*功能:内存比较
参数:	dst:目的地址
src：源地址指针
len：比较的长度
err：返回值 0：正确，其他错误
*/
//////////////////////////////////////////////////////////////////////////
 int  Safe_memcmp (const void *dst, const void *src, unsigned int len,int& err);

//////////////////////////////////////////////////////////////////////////
/*功能:计算字符长度
参数:	_Str:字符串指针
返回值： -1：指针错误，其他：字符串长度
*/
//////////////////////////////////////////////////////////////////////////
 int  Safe_strlen(const char * _Str);
//////////////////////////////////////////////////////////////////////////
/*功能:字符串比较
参数 s1 big strings s2 small strings
参数 err 返回引用 err=1错误 
*/
//////////////////////////////////////////////////////////////////////////
 const char *  Safe_strstr(const char *s1,const char *s2,int& err);
//////////////////////////////////////////////////////////////////////////
/*功能:字符串拷贝
参数:	dst:目的字符串buf指针
sizeof_Dst：目的字符buf长度
src：源字符串buf指针

返回值： NULL：指针错误，其他：dst地址
*/
//////////////////////////////////////////////////////////////////////////
 char *  Safe_strcpy(char * dst, int sizeof_Dst, const char * src);
//////////////////////////////////////////////////////////////////////////
/*功能:字符串拷贝
参数:	dst:目的字符串buf指针
sizeof_Dst：目的字符buf长度
src：源字符串buf指针
count：拷贝的长度

返回值： NULL：指针错误，其他：dst地址
*/
//////////////////////////////////////////////////////////////////////////
 char *  Safe_strncpy(char * dst, unsigned int sizeof_Dst, const char * source, int count);
//////////////////////////////////////////////////////////////////////////
/*功能:字符串链接
参数:	dst:目的字符串buf指针
sizeof_Dst：目的字符buf长度
src：源字符串buf指针

返回值： NULL：指针错误，其他：dst地址
*/
//////////////////////////////////////////////////////////////////////////
 char *  Safe_strcat(char * dst, int sizeof_Dst, const char * src);
//////////////////////////////////////////////////////////////////////////
/*功能:字符串链接
参数:	dst:目的字符串buf指针
sizeof_Dst：目的字符buf长度
src：源字符串buf指针
count：需要链接的长度
返回值： NULL：指针错误，其他：dst地址
*/
//////////////////////////////////////////////////////////////////////////
 char *  Safe_strncat(char * dst, int sizeof_Dst, const char * src, int count);
//////////////////////////////////////////////////////////////////////////
/*功能:字符串比较
参数:	src：源字符串buf指针
dst:目的字符串buf指针
err：返回函数调用的错误值 0：正确， 其他：错误
返回值： =0：相同，<0:src<dst ,>0: src>dst
*/
//////////////////////////////////////////////////////////////////////////
 int  Safe_strcmp (const char * src, const char * dst, int& err);
//////////////////////////////////////////////////////////////////////////
/*功能:字符串比较
参数:	src：源字符串buf指针
dst:目的字符串buf指针
count:需要比较的长度
err：返回函数调用的错误值 0：正确， 其他：错误
返回值： =0：相同，<0:src<dst ,>0: src>dst
*/
//////////////////////////////////////////////////////////////////////////
 int  Safe_strncmp(const char *src, const char *dst, unsigned int count, int& err);

#ifdef WIN32



//////////////////////////////////////////////////////////////////////////
/*功能:全局堆上申请内存
参数:dwBytes:要申请内存的大小
     uFlags：标志
参数: maxUpLevel申请内存大小上限
返回:若函数调用成功,则返回一个新分配的内存对象的句柄。
     若函数调用失败,则返回NULL
*/
//////////////////////////////////////////////////////////////////////////
 HGLOBAL Safe_GlobalAlloc ( UINT uFlags,SIZE_T dwBytes, unsigned int maxUpLevel=MAX_MALLOC_SIZE);
//////////////////////////////////////////////////////////////////////////
/*功能:堆上内存释放
参数:hMem:内存对象的句柄
*/
//////////////////////////////////////////////////////////////////////////
 void Safe_GlobalFree( HGLOBAL hMem);
//////////////////////////////////////////////////////////////////////////
/*功能:堆上申请内存
参数:dwBytes:要申请内存的大小
uFlags：标志
参数: maxUpLevel申请内存大小上限
返回:若函数调用成功,则返回一个新分配的内存对象的句柄。
若函数调用失败,则返回NULL
*/
//////////////////////////////////////////////////////////////////////////
 HLOCAL Safe_LocalAlloc ( UINT uFlags,SIZE_T dwBytes, unsigned int maxUpLevel=MAX_MALLOC_SIZE);
/*功能:堆上内存释放
参数:hMem:内存对象的句柄
*/
//////////////////////////////////////////////////////////////////////////
 void Safe_LocalFree( HLOCAL hMem);

//////////////////////////////////////////////////////////////////////////
/*功能:计算字符长度
参数:	_Str:字符串指针
返回值： -1：指针错误，其他：字符串长度
*/
//////////////////////////////////////////////////////////////////////////
 int  Safe_wcslen(const wchar_t * _Str);
//////////////////////////////////////////////////////////////////////////
/*功能:字符串比较
参数 s1 big strings s2 small strings
参数 err 返回引用 err=1错误 
*/
//////////////////////////////////////////////////////////////////////////
 const wchar_t *  Safe_wcsstr(const wchar_t *s1,const wchar_t *s2,int& err);
//////////////////////////////////////////////////////////////////////////
/*功能:字符串拷贝
参数:	dst:目的字符串buf指针
countof_Dst：可容纳wchar 的个数 wchar_t wBuf[1024]; countof_Dst = 1024
src：源字符串buf指针

返回值： NULL：指针错误，其他：dst地址
*/
//////////////////////////////////////////////////////////////////////////
 wchar_t *  Safe_wcscpy(wchar_t * dst, int countof_Dst, const wchar_t * src);
//////////////////////////////////////////////////////////////////////////
/*功能:字符串拷贝
参数:	dst:目的字符串buf指针
countof_Dst：可容纳wchar 的个数 wchar_t wBuf[1024]; countof_Dst = 1024
src：源字符串buf指针
count：拷贝的长度

返回值： NULL：指针错误，其他：dst地址
*/
//////////////////////////////////////////////////////////////////////////
 wchar_t * Safe_wcsncpy(wchar_t * dst, unsigned int countof_Dst, const wchar_t * source, int count);
//////////////////////////////////////////////////////////////////////////
/*功能:字符串链接
参数:	dst:目的字符串buf指针
countof_Dst：可容纳wchar 的个数 wchar_t wBuf[1024]; countof_Dst = 1024
src：源字符串buf指针

返回值： NULL：指针错误，其他：dst地址
*/
//////////////////////////////////////////////////////////////////////////
 wchar_t *  Safe_wcscat(wchar_t * dst, int countof_Dst, const wchar_t * src);
//////////////////////////////////////////////////////////////////////////
/*功能:字符串链接
参数:	dst:目的字符串buf指针
countof_Dst：可容纳wchar 的个数 wchar_t wBuf[1024]; countof_Dst = 1024
src：源字符串buf指针
count：需要链接的长度
返回值： NULL：指针错误，其他：dst地址
*/
//////////////////////////////////////////////////////////////////////////
 wchar_t *  Safe_wcsncat(wchar_t * dst, int countof_Dst, const wchar_t * src, int count);
//////////////////////////////////////////////////////////////////////////
/*功能:字符串比较
参数:	src：源字符串buf指针
dst:目的字符串buf指针
err：返回函数调用的错误值 0：正确， 其他：错误
返回值： =0：相同，<0:src<dst ,>0: src>dst
*/
//////////////////////////////////////////////////////////////////////////
 int  Safe_wcscmp (const wchar_t * src, const wchar_t * dst, int& err);
//////////////////////////////////////////////////////////////////////////
/*功能:字符串比较
参数:	src：源字符串buf指针
dst:目的字符串buf指针
count:需要比较的长度，字符个数
err：返回函数调用的错误值 0：正确， 其他：错误
返回值： =0：相同，<0:src<dst ,>0: src>dst
*/
//////////////////////////////////////////////////////////////////////////
 int  Safe_wcsncmp(const wchar_t *src, const wchar_t *dst, unsigned int count, int& err);

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
 int  WINAPI Safe_AtoW(IN const char *ptrA,OUT WCHAR *ptrW,IN int nLenW);
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
 int  WINAPI Safe_WtoA(IN WCHAR *ptrW,OUT char *ptrA,IN int nLenA);

//////////////////////////////////////////////////////////////////////////
/*功能:字符串格式化
参数:	buffer：源字符串buf指针
buf_size:源字符串buf大小
format:格式化
argptr:参数
返回值： 字符串长度
*/
//////////////////////////////////////////////////////////////////////////
 int Safe_vsprintf (char *buffer, int buf_size,const char *format, va_list argptr);

//////////////////////////////////////////////////////////////////////////
/*功能:字符串格式化
参数:	buffer：源字符串buf指针
buf_size:源字符串buf大小
format:格式化
argptr:参数
返回值： 字符串长度
*/
//////////////////////////////////////////////////////////////////////////
 int Safe_vswprintf(wchar_t *buffer, int buf_size,const wchar_t *format, va_list argptr);

//////////////////////////////////////////////////////////////////////////
/*功能:字符串格式化
参数:	buf：源字符串buf指针
countof_buf:源字符串buf的字符数大小
fmt:格式化
...:参数
返回值： 字符串长度
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
/*功能:判断内存串findstr是否是内存串membuf的子串
参数:	membuf:  被查找内存串
        memLen:  被查找内存串的长度
        findstr: 要查找内存串
		FindNum: 要查找内存串的长度
		findPtr:如果找到,则该函数返回findstr在membuf中首次出现的地址;否则,返回NULL
返回值： 无 
*/

 inline void Safe_Memstrstr( char* membuf,int memLen,const  char* findstr, int FindNum,char* &findPtr)
 {
 	findPtr=memstr(membuf, memLen,findstr,FindNum); 
 }




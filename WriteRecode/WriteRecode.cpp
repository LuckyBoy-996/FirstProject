#include "stdafx.h"
#include <iostream>
#include "WriteRecodeAssist.h"
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	//test2分支，第一次修改
	cout<<"写日志例子程序"<<endl;
	OutputDebugMessage(L"写日志列子 aaaaaaaaaaaa");
	ErrorMessage("写日志列子    aaaaaaaaaaaa");
	getchar();
	return 0;
}


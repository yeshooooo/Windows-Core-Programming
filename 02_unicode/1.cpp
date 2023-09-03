#include <Windows.h>
#include <stdio.h>
#include <strsafe.h>
int main()
{
/*	const char* str = "hello world!";*/
	const char* str = "你好世界"; // 此时，其实也是2个字节表示的汉字，汉字的首位是1，ascii的首位字符是0，使用起来较麻烦
	// 核心编程书中 typedef unsigned short wchar_t,高版本中直接是unicode字符了
	wchar_t b; // 2个字节
	TCHAR s;
	const TCHAR *str1 = TEXT("杨大毛");
	printf("%d\n",sizeof(s));
	wprintf(L"%S\n", str1);
	printf("%S\n", str1);

	TCHAR mem[1000];
	// StringCchPrintf 把字符串打印到内存中了，而printf是打印到控制台
	StringCchPrintf(mem, 1000, L"%d,%s", 100, L"This is test");
	printf("%d,%s", 100, "This is test!");
	

}
#include <Windows.h>
#include <stdio.h>
#include <strsafe.h>
int main()
{
/*	const char* str = "hello world!";*/
	const char* str = "�������"; // ��ʱ����ʵҲ��2���ֽڱ�ʾ�ĺ��֣����ֵ���λ��1��ascii����λ�ַ���0��ʹ���������鷳
	// ���ı������ typedef unsigned short wchar_t,�߰汾��ֱ����unicode�ַ���
	wchar_t b; // 2���ֽ�
	TCHAR s;
	const TCHAR *str1 = TEXT("���ë");
	printf("%d\n",sizeof(s));
	wprintf(L"%S\n", str1);
	printf("%S\n", str1);

	TCHAR mem[1000];
	// StringCchPrintf ���ַ�����ӡ���ڴ����ˣ���printf�Ǵ�ӡ������̨
	StringCchPrintf(mem, 1000, L"%d,%s", 100, L"This is test");
	printf("%d,%s", 100, "This is test!");
	

}
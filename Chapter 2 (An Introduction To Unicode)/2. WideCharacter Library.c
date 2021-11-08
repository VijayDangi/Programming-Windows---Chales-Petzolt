#include <windows.h>
#include<wchar.h>

int main()
{
	char str[] = "Hello";
	wchar_t str1[] = L"Hello";

	int length = strlen(str);
	printf("normal string size using strlen() : %d\n", length); //length = 5

	length = strlen(str1); //warning C4133: 'function': incompatible types - from 'wchar_t [6]' to 'const char *'
	printf("wide character string size using strlen() : %d\n", length); //length = 1, why?

	// Hello :- 0x0048 0x0065 0x006C 0x006C 0x006F  0x0021  (16-bit for 1 character
	// Stored in Intel processor as : 48 00 65 00 6C 00 6C 00 6F 00 21 00
	// strlen read 1-byte at a time, first it reads 48 then it read 00 which is the end of string character.

	//wcslen() :- wide-character string length version.

	length = wcslen(str1);
	printf("wide character string size using wcslen() : %d\n", length); //length = 5

	// wcslen() declared in both string.h and wchar.h

	printf("printf(str) :- %s\n", str);
	printf("printf(str1) :- %s\n", str1);

	wprintf(L"wprintf(str) :- %s\n", str);
	wprintf(L"wprintf(str) :- %hs\n", str);
	wprintf(L"wprintf(str1) :- %s\n", str1);
}

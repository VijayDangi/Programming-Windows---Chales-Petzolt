#include <windows.h>
#include <wchar.h>

int main()
{
/*	wchar_t str[] = "VIJAY"; // error C2053: 'str': wide string mismatch*/
/*	char str1[] = L"VIJAY";  // error C2053: 'str': wide string mismatch*/

	char str2[] = "VIJAY";
	wchar_t str3[] = L"VIJAY"; //L for  long

	wchar_t c = L'A';

	printf("%c\n", str2[1]);
	printf("%c\n", str3[1]);
	printf("%c\n", c);
	return 0;
}

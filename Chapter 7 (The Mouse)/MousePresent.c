#include <windows.h>
#include <stdio.h>

int main()
{
	printf("Is Mouse Present : %s\n", GetSystemMetrics(SM_MOUSEPRESENT) ? "YES" : "NO");
	printf("Number of button(s) on mouse : %d\n", GetSystemMetrics(SM_CMOUSEBUTTONS));
	
	return(0);
}

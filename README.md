
______________________ ANSI VERSION ______________________

**compile**
cl.exe /EHsc /c <filename>.c    ===> <filename>.obj

______________________ UNICODE VERSION ______________________
 
**compile**
cl.exe /EHsc /c /D UNICODE <filename>.c    ===> <filename>.obj


**if resource file**\n
rc.exe <Resource_filename>.rc	===> <Resource_filename>.res


 **link**
link.exe <filename>.obj <Resource_filename>.res user32.lib gdi32.lib kernel32.lib   ===> <filename>.exe

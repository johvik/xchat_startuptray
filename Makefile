default:
	gcc -Wall -Os -DWIN32 -c startuptray.c
	dllwrap --def startuptray.def -dllname startuptray.dll startuptray.o

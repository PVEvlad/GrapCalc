cls
fasm extra.asm bin/extra.o
gcc token.c -c -o bin/token.o
gcc help.c  -c -o bin/help.o
gcc graph.c  -c -o bin/graph.o
gcc main.c  -c -o bin/main.o
gcc bin/token.o bin/help.o bin/graph.o bin/main.o bin/extra.o bin/gdi32.dll bin/comctl32.dll -o bin/main1.exe
bin\main1
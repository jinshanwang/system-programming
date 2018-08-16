all: part1 part2 part3

part1:smsh.h smsh2.c executeMulComm.c splitline.c
	gcc -o smsh2 smsh2.c splitline.c executeMulComm.c
part2:smsh.h smsh2.c executeMulComm.c splitline.c
		gcc -o smsh3 smsh2.c splitline.c executeMulComm.c
part3:smsh.h smsh2.c executeMulComm.c splitline.c
	gcc -o smsh4 smsh2.c splitline.c executeMulComm.c
clean:
	rm smsh2 smsh3 smsh4

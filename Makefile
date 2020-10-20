CC := gcc
CFLAGS := -lssl -lcrypto
DEPS = SysEng.h


syseng: SysEng.c functions.c
		$(CC) -o syseng SysEng.c functions.c $(CFLAGS)
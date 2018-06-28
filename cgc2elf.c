#define _GNU_SOURCE
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <err.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define	C_IDENT "\177ELF\x01\x01\x01\x00\x00\x00\x00\x00\x00\x00\x00"

int
main(int argc, char *argv[])
{
	int i, j;
    FILE *f, *fw;
	int ret = 0;
    int unused __attribute__((unused));
    char *buffer = NULL;
    char *fname;
    char name[300];
    long length;

	if (argc < 2) {
		fprintf(stderr, "files...\n");
		return (1);
	}

	for (i = 1; i < argc; i++) {
        memset(name, 0, strlen(name));
        snprintf(name, strlen(argv[i])+1, "%s", argv[i]);

        f = fopen(name, "r+");

        if (!f) {
            fprintf(stderr, "couldn't open %s\n",name);
            return 1;
        }

        fseek(f, 0L, SEEK_END);
        length = ftell(f);
        rewind(f);
        buffer = malloc(length);
        if (buffer) {
            unused = fread(buffer, 1, length, f);
            fclose(f);
        }

        for (j = 0; j < 15; j++) buffer[j] = C_IDENT[j];

        fname = strncat(name, "_elf", 5);
        fw = fopen(fname, "wb");

        fwrite(buffer, 1, length, fw);

        free(buffer);
        buffer=NULL;
        fclose(fw);
	}
	return (0);
}

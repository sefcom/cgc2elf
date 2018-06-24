#define _GNU_SOURCE
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <err.h>
#include <unistd.h>
#include <string.h>

#define	C_IDENT "\177ELF\x01\x01\x01\x00\x00\x00\x00\x00\x00\x00\x00"

int
main(int argc, char *argv[])
{
	int i, j;
    FILE *f;
	int ret = 0;
    int unused __attribute__((unused));
    char *buffer = 0;
    long length;

	if (argc < 2) {
		fprintf(stderr, "files...\n");
		return (1);
	}

	for (i = 1; i < argc; i++) {
		f = fopen(argv[i], "r+");

        fseek(f, 0L, SEEK_END);
        length = ftell(f);
        rewind(f);
        buffer = malloc(length);
        if (buffer) {
            unused = fread(buffer, 1, length, f);
            fclose(f);
        }

        for (j = 0; j < 15; j++) buffer[j] = C_IDENT[j];

        char *fname = strncat(argv[i], "_elf", 5);
        FILE *fw = fopen(fname, "wb");

        fwrite(buffer, 1, length, fw);
        fclose(fw);
	}
	return (0);
}

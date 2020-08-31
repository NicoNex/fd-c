#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#include "args.h"

const char C_ZERO = '\0';
const char C_NLN = '\n';

#ifdef __WIN32
	const char DEF_SEP = '\\';
#else
	const char DEF_SEP = '/';
#endif

struct arg arg;
char separator = DEF_SEP;

// Returns 1 if a file or directory exists otherwhise 0.
static inline int exists(const char *path) {
	return access(path, F_OK) == 0;
}

// Returns 1 if a file type is a directory otherwhise 0.
static inline int is_dir(const unsigned char type) {
	return type == DT_DIR;
}

// Recursively walks into a directory tree.
int walk_dir(const char *path) {
	struct dirent **namelist;
	int n = scandir(path, &namelist, NULL, alphasort);
	if (n == -1) {
		perror("scandir returned -1");
		return 1;
	}

	// TODO: make it match the names.
	for (int i = 2; i < n; i++) {
		const char *fname = namelist[i]->d_name;
		int len = strlen(path) + strlen(fname) + 2;
		char fpath[len];

		snprintf(fpath, len, "%s%c%s", path, separator, fname);
		// TODO: check if fname matches with the pattern and print it if so.

		printf("%s%c", fpath, C_NLN); // debug
		if (is_dir(namelist[i]->d_type)) {
			walk_dir(fpath);
		}
		free(namelist[i]);
	}
	return 0;
}

// Prints the usage and exits.
void usage(void) {
	puts("TODO: put usage here.");
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		// TODO: write a nice usage message.
		puts("Too few arguments");
		return 1;
	}

	arg = parse_arg(argc, argv, usage);

	if (exists(arg.path)) {
		walk_dir(arg.path);
	}

	return 0;
}

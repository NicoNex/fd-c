#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <regex.h>
#include <sys/stat.h>

#include "args.h"

const char C_ZERO = '\0';
const char C_NLN = '\n';

struct arg arg;
char separator;
regex_t re;

// Returns 1 if a file or directory exists otherwhise 0.
static inline int exists(const char *path) {
	return access(path, F_OK) == 0;
}

// Returns 1 if a file type is a directory otherwhise 0.
static inline int is_dir(const unsigned char type) {
	return type == DT_DIR;
}

// Returns 1 if a file name matches the regex.
static inline int matches(const char *str) {
	regmatch_t pmatch[1];
	return regexec(&re, str, 1, pmatch, 0) == 0;
}

static inline void replace(const char *in, const char *out, char t, char r) {
	
}

static inline void replace_chr(char *str, const char t, const char c) {
	if (*str != '\0') {
		if (*str == t) {
			*str = c;
		}
		replace_chr(++str, t, c);
	}
}

static inline void print_file(const char *path, const char *name) {
	int len = strlen(path) + strlen(name) + 1;
	char tmp[len];
	char end_ln = arg.use_zero ? '\0' : '\n';
	
	strncpy(tmp, path, len);
	replace_chr(tmp, '/', arg.path_sep);
	printf("%s%c", tmp, end_ln);
}


// Recursively walks into a directory tree.
int walk_dir(const char *path) {
	struct dirent **namelist;
	int n = scandir(path, &namelist, NULL, alphasort);
	if (n == -1) {
		char errmsg[256];
		snprintf(errmsg, 256, "scandir: unable to read directory %s.", path);
		perror(errmsg);
		exit(EXIT_FAILURE);
	}

	for (int i = 2; i < n; i++) {
		const char *fname = namelist[i]->d_name;

		if (matches(fname)) {
			print_file(path, fname);
		}
		
		if (is_dir(namelist[i]->d_type)) {
			int len = strlen(path) + strlen(fname) + 2;
			char fpath[len];
			
			snprintf(fpath, len, "%s%c%s", path, DEF_SEP, fname);
			walk_dir(fpath);
		}
		free(namelist[i]);
	}
	return 0;
}

// Prints the usage and exits.
void usage(void) {
	puts("fd - Find all files mathing a pattern.\n\
Fd recursively finds all the files whose names match a pattern provided in input.\n\
Usage:\n\
    fd [options] pattern [path]\n\
\n\
Flags:\n\
    -g    Use shell file name pattern instead of regex.\n\
    -i    Ignore case.\n\
    -0    Separate search results by the null character (instead of newlines). Useful for piping results to 'xargs'.\n\
\n\
Options:\n\
    -s    Specify the path separator to use (e.g. fd -s=/ pattern).");
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		usage();
		exit(EXIT_FAILURE);
	}

	arg = parse_arg(argc, argv, usage);
	separator = arg.path_sep;
	int cflags = arg.ignore_case ? REG_EXTENDED|REG_ICASE : REG_EXTENDED;
	regcomp(&re, arg.pattern, cflags);

	if (exists(arg.path)) {
		walk_dir(arg.path);
	}

	regfree(&re);
	return 0;
}

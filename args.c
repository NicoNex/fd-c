#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "args.h"


// Returns 1 if s1 and s2 are equal otherwhise 0.
static inline int are_equal(const char *s1, const char *s2) {
	return strcmp(s1, s2) == 0;
}

static void parse_flag(struct arg *a, const char *flag, void (*usage)()) {
	if (are_equal(flag, "-g")) {
		a->use_glob = 1;
	} else if (are_equal(flag, "-0")) {
		a->use_zero = 1;
	} else if (are_equal(flag, "-i")) {
		a->ignore_case = 1;
	} else if (strstr(flag, "-s=") != NULL) {
		if (strlen(flag) != 4) {
			usage();
			exit(EXIT_FAILURE);
		}
		a->path_sep = flag[3];
	} else {
		usage();
		exit(EXIT_FAILURE);
	}
}

struct arg parse_arg(int argc, char *argv[], void (*usage)()) {
	if (argc < 2) {
		usage();
		exit(EXIT_FAILURE);
	}

	struct arg ret = {0};
	ret.path_sep = DEF_SEP;
	int n = 0;

	for (int i = 1; i < argc; i++) {
		char *arg = argv[i];

		switch (arg[0]) {
		case '-':
			parse_flag(&ret, arg, usage);
			break;
		default:
			if (n == 0) {
				ret.pattern = arg;
			} else if (n == 1) {
				int len = strlen(arg);
				if (arg[len-1] == '/') {
					arg[len-1] = '\0';
				}
				ret.path = arg;
			} else {
				usage();
				exit(EXIT_FAILURE);
			}
			n++;
			break;
		}
	}

	if (ret.path == NULL) {
		ret.path = malloc(2);
		strncpy(ret.path, ".", 2);
	}

	return ret;
}

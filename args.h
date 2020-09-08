#ifndef ARGS_H_
#define ARGS_H_

#ifdef __WIN32
	#define DEF_SEP '\\'
#else
	#define DEF_SEP '/'
#endif

struct arg {
	int use_glob;
	int use_zero;
	int ignore_case;
	char *pattern;
	char *path;
	char path_sep;
};

struct arg parse_arg(int argc, char *argv[], void (*usage)());

#endif
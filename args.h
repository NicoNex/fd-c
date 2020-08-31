#ifndef ARGS_H_
#define ARGS_H_

struct arg {
	int use_glob;
	int use_zero;
	char *pattern;
	char *path;
};

struct arg parse_arg(int argc, char *argv[], void (*usage)());

#endif
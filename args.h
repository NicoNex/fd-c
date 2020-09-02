#ifndef ARGS_H_
#define ARGS_H_

struct arg {
	int use_glob;
	int use_zero;
	int ignore_case;
	char *pattern;
	char *path;
	char separator;
};

struct arg parse_arg(int argc, char *argv[], void (*usage)());

#endif
#ifndef SHELL_H
#define SHELL_H

enum SHELL_TYPE
{
	INT,
	FLOAT,
	CHAR,
	STRING
};

struct shell_list
{
	struct shell_list* next;
	char* name;
	void* data;
	enum SHELL_TYPE type;
};
typdef struct shell_list shell_list_t;

void shell_execute(char** argv, int argc);

#endif



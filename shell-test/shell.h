#ifndef SHELL_H
#define SHELL_H

enum SHELL_TYPE
{
	INT,
	FLOAT,
	CHAR,
	STRING,
};

struct shell_list
{
	struct shell_list* next;
	char* name;
	void* data;
	enum SHELL_TYPE type;
};
typedef struct shell_list shell_list_t;

extern shell_list_t* shell_variables = 0;

typedef void (*SHELL_COMMAND_CALLBACK)(char**, int);

struct shell_command
{
	const char* command;
	int min_args;
	SHELL_COMMAND_CALLBACK callback;
};

void shell_execute(char** argv, int argc);


#endif



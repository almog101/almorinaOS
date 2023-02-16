#ifndef SHELshell_listL_H
#define SHELL_H

enum SHELL_TYPE
{
	SHELL_TYPE_INT,
	SHELL_TYPE_FLOAT,
	SHELL_TYPE_STRING,
};

typedef struct 
{
	struct shell_list* next;
	char* name;
	void* data;
	enum SHELL_TYPE type;
}shell_list_t;

typedef void (*SHELL_COMMAND_CALLBACK)(char**, int);

struct shell_command
{
	const char* command;
	int min_args;
	SHELL_COMMAND_CALLBACK callback;
};

void print_greetings();
void shell_main();

#endif

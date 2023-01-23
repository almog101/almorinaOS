#ifndef SHELL_H
#define SHELL_H

enum SHELL_TYPE
{
	SHELL_TYPE_INT,
	SHELL_TYPE_STRING,
};

struct shell_list
{
	struct shell_list* next;
	char* name;
	void* data;
	enum SHELL_TYPE type;
};
typedef struct shell_list shell_list_t;

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

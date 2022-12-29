#include <stdio.h>
#include "string.h"
#include <stdint.h>
#include "shell.h"

void strip_spaces(char* s)
{
	const char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while (*s++ = *d++);
}

int isop(char c)
{
	return ((c == '-') || (c == '+') || (c == '/') || (c == '*'));
}

int eval(const char* expression)
{
	char exp[100] ={0};
	strcpy(exp, expression);
	strip_spaces(exp);

	int nums[100] = {0};
	char ops[100] = {0};
	int count = 0;
	
	// parse expression to operators and numbers
	char* p = exp;
	for (int i = 0; i < sizeof(exp) && exp[i]; i++)
	{
		if (isop(exp[i]))
		{
			ops[count] = exp[i];
			exp[i] =0;
			nums[count++ ] = atoi(p);
			p = exp+i+1;
		}
	}
	nums[count++] = atoi(p);


	while (count > 0)
	{
		int index = 0;
		for (int i = 1; i<count-1; i++)
			if ((ops[index] == '-' || ops[index] == '+') && (ops[i] == '*' || ops[i] == '/'))
				index = i;


		int n;
		switch (ops[index]) {
		case '-':
			n = nums[index] - nums[index+1];
			break;
		case '+':
			n = nums[index] + nums[index+1];
			break;
		case '/':
			if (nums[index+1] == 0)
			{
				printf("Error: can't devide by 0!\n");
				return 0;
			}

			n = nums[index] / nums[index+1];
			break;
		case '*':
			n = nums[index] * nums[index+1];
			break;
		}

		for (int i = index+1; i<count; i++)
		{
			nums[i] = nums[i+1];
			ops[i-1] = ops[i];
		}

		nums[index] = n;

		count--;
	}

	return *nums;
}


/* this function splits the command into its arguments 
 * and returns the number of them */
int shell_parse(const char* line, char*** argv)
{
	int size = strlen(line);

	char** args = 0;
	uint8_t argc = 0;

	char* start = line;
	char* end = line;

	do
	{
		// find the end of the argumnet
		end = strchr(start, ' ');
		if (end == 0)
			end = line + size;

		// copy the argument into new string
		char* arg = malloc(end-start+1);
		memcpy(arg, start, end-start);
		arg[end-start] = 0;
		
		// add the argument to args list
		args = realloc(args, sizeof(char*) * (++argc));
		args[argc-1] = arg;
		
		start = end+1;
	} while(end < line + size - 1);

	*argv = args;
	return argc;
}

void echo(char** argv, int argc)
{
	for (int i =1; i<argc; i++)
	{
		for (char* p_arg = argv[i]; *p_arg != 0; p_arg++)
		{
			if (*p_arg == '$')
			{
				// TODO: print_var(p_arg);
				break;
			}
			putchar(*p_arg);
		}
		putchar(' ');
	}
	putchar('\n');
}

void set(char** argv, int argc)
{
	shell_list_t* node = malloc(sizeof(shell_list_t));
	
	node->name = malloc(strlen(argv[1]));
	strcpy(node->name, argv[1]);
	node->data = malloc(strlen(argv[2]));
	strcpy(node->data, argv[2]);
	node->type = STRING;

    if(shell_variables == NULL) {
    	shell_variables = node;
      	return;
    }

	shell_list_t* curr = shell_variables;
  	while(curr->next != NULL)
    	curr = curr->next;
  	curr->next = node;
}

void vars(char** argv, int argc)
{
	shell_list_t* curr = shell_variables;
  	while(curr != NULL)
	{
		printf("$%s = '%s'\n", curr->name, curr->data);
		curr = curr->next;
	}
}

void help(char** argv, int argc);

struct shell_command shell_callback[] = {
	// COMMAND	MIN ARGS	CALLBACK FUNC
	{"echo", 	2, 			echo},
	{"set", 	3, 			set},
	{"help", 	1, 			help},
	{"vars",		1,			vars}
};

void help(char** argv, int argc)
{
	for (int i = 0; i<sizeof(shell_callback)/sizeof(struct shell_command); i++)
		printf("%s ", shell_callback[i].command);
	putchar('\n');
}

void shell_execute(char** argv, int argc)
{
	if (argc < 1)
		return;

	for (int i = 0; i<sizeof(shell_callback)/sizeof(struct shell_command); i++)
	{
		if (strcmp(argv[0], shell_callback[i].command) != 0)
			continue;

		if (argc < shell_callback[i].min_args)
		{
			printf("Not enough arguments for '%s' command!\n", shell_callback[i].command);
			return;
		}

		shell_callback[i].callback(argv, argc);
		return;
	}

	printf("invalid command!\n");
}

void main()
{
	char line[100] = {0};
	
	do 
	{
		printf("command?> ");
		fgets(line, sizeof(line), stdin);

		for (char* i = line; *i != 0; i++)
			if (*i == '\n') *i =0;

		if (strcmp(line, "exit") == 0)
			break;

		char** args;
		int argc=shell_parse(line, &args);

		shell_execute(args, argc);

		//cleanup
		for (int i = 0; i<argc; i ++)
			free(args[i]);
		free(args);

	} while(1);

}

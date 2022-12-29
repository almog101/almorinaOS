#include <stdio.h>
#include "string.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
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

bool isop(char c)
{
	return ((c == '-') || (c == '+') || (c == '/') || (c == '*'));
}

bool isdigit(char c)
{
	return (c>='0') && (c<='9');
}

bool is_exp(char* str)
{
	while(*str != 0)
		if (isop(*str) != true && isdigit(*str) != true && *str != ' ')
			return false;
		else
			str++;
	return true;
}

int shell_eval_math_exp(const char* expression)
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

char *shell_combine_strings(char **str_array, size_t size) {
  size_t total_length = 0;

  // Calculate the total length of the combined string
  for (size_t i = 0; i < size; i++) {
    total_length += strlen(str_array[i]) + 1;
  }
  total_length--;

  // Allocate a buffer for the combined string
  char *combined_str = malloc(total_length + 1);

  // Copy the strings into the combined string buffer
  size_t pos = 0;
  for (size_t i = 0; i < size; i++) {
    size_t str_len = strlen(str_array[i]);
    memcpy(combined_str + pos, str_array[i], str_len);
	combined_str[pos+str_len] = ' ';
    pos += str_len+1;
  }

  // Null-terminate the combined string
  combined_str[total_length] = '\0';

  return combined_str;
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
		printf("%s ", argv[i]);
	putchar('\n');
}

void set_variable(shell_list_t* node, const char* name, const char* data)
{
	node->name = malloc(strlen(name));
	strcpy(node->name, name);

	if (is_exp(data))
	{
		node->data = malloc(sizeof(int));
		*(int*)(node->data) = shell_eval_math_exp(data);
		node->type = SHELL_TYPE_INT;
	}
	else
	{
		node->data = data;
		node->type = SHELL_TYPE_STRING;
	}
	node->next = 0;
}

void set(char** argv, int argc)
{
	char* data = shell_combine_strings(argv+2, argc-2);

	// checks if variables with that name already exists
	// if it does, we only change is value
	shell_list_t* curr= shell_variables;
	while (curr)
	{
		if (strcmp(curr->name, argv[1]) == 0)
		{
			set_variable(curr, argv[1], data);
			return;
		}
		curr = curr->next;
	}

	// create new var and insert it into the beginning of the list
	shell_list_t* node = malloc(sizeof(shell_list_t));
	set_variable(node, argv[1], data);
	
    if(shell_variables == NULL) 
    	shell_variables = node;
	else 
	{
		node->next = shell_variables;
		shell_variables = node;
	}
}

void vars(char** argv, int argc)
{
	shell_list_t* curr = shell_variables;
  	while(curr != NULL)
	{
		printf("$%s = ", curr->name);
		
		switch (curr->type) 
		{
			case SHELL_TYPE_INT:
				printf("%d\n", *(int*)(curr->data));
				break;
			case SHELL_TYPE_STRING:
				printf("%s\n", curr->data);
				break;
		}
		curr = curr->next;
	}
}

void help(char** argv, int argc);

struct shell_command shell_callback[] = {
	// COMMAND	MIN ARGS	CALLBACK FUNC
	{"echo", 	2, 			echo},
	{"set", 	3, 			set},
	{"help", 	1, 			help},
	{"vars",	1,			vars}
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

		// TODO
		// replace all variables for their values
		// shell_insert_vars_values(char** argv, int argc)
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

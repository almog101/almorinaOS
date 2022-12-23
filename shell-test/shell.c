#include <stdio.h>
#include "string.h"
#include <stdint.h>

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
int shell_parse(const char* line, char*** argv, int argc)
{
	char** args = 0;
	uint8_t args_count = 0;

	char* start = line;
	char* end = line;

	do
	{
		end = strchr(start, ' ');
		if (end == 0)
		{
			strcpy(argv[args_count], start);
			break;
		}
		memcpy(argv[args_count], start, (int)(end-start));

		start = end+1;
	} while(args_count++ < argc);
}

void e() {}

void shell_execute()
{
	char data[128] = "print hello%cworld @";
	char args[5][20] = {0};
	
	//shell_parse(data, 128, args, 5);

	if (strcmp(args[0], "add") == 0)
		e();//add(atoi(args[1]), atoi(args[2]));
	else if (strcmp(args[0], "print") == 0)
		e();//print(args[1], args);
	else if (strcmp(args[0], "help") == 0)
		e();//help();
}

void main()
{

}

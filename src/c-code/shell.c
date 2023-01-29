#include "fs.h"
#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "stdbool.h"
#include "shell.h"
#include "heap.h"
#include <vga.h>

const char almorina_title_I[] = {0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x20, '\n', 0x20, 0x2F, 0x20, 0x5F, 0x20, 0x5C, 0x7C, 0x20, 0x7C, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x28, 0x5F, 0x29, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7C, 0x20, 0x20, 0x5F, 0x20, 0x20, 0x2F, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x7C, '\n', 0};
const char almorina_title_II[] = {0x2F, 0x20, 0x2F, 0x5F, 0x5C, 0x20, 0x5C, 0x20, 0x7C, 0x5F, 0x20, 0x5F, 0x5F, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x5F, 0x20, 0x5F, 0x5F, 0x20, 0x5F, 0x20, 0x5F, 0x20, 0x5F, 0x5F, 0x20, 0x20, 0x20, 0x5F, 0x5F, 0x20, 0x5F, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x5C, 0x20, 0x60, 0x2D, 0x2D, 0x2E, 0x20, '\n', 0x7C, 0x20, 0x20, 0x5F, 0x20, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x27, 0x5F, 0x20, 0x60, 0x20, 0x5F, 0x20, 0x5C, 0x20, 0x2F, 0x20, 0x5F, 0x20, 0x5C, 0x7C, 0x20, 0x27, 0x5F, 0x5F, 0x7C, 0x20, 0x7C, 0x20, 0x27, 0x5F, 0x20, 0x5C, 0x20, 0x2F, 0x20, 0x5F, 0x60, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x60, 0x2D, 0x2D, 0x2E, 0x20, 0x5C, '\n', 0};
const char almorina_title_III[] = {0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x28, 0x5F, 0x29, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x28, 0x5F, 0x7C, 0x20, 0x5C, 0x20, 0x5C, 0x5F, 0x2F, 0x20, 0x2F, 0x5C, 0x5F, 0x5F, 0x2F, 0x20, 0x2F, '\n', 0x5C, 0x5F, 0x7C, 0x20, 0x7C, 0x5F, 0x2F, 0x5F, 0x7C, 0x5F, 0x7C, 0x20, 0x7C, 0x5F, 0x7C, 0x20, 0x7C, 0x5F, 0x7C, 0x5C, 0x5F, 0x5F, 0x5F, 0x2F, 0x7C, 0x5F, 0x7C, 0x20, 0x20, 0x7C, 0x5F, 0x7C, 0x5F, 0x7C, 0x20, 0x7C, 0x5F, 0x7C, 0x5C, 0x5F, 0x5F, 0x2C, 0x5F, 0x7C, 0x5C, 0x5F, 0x5F, 0x5F, 0x2F, 0x5C, 0x5F, 0x5F, 0x5F, 0x5F, 0x2F, '\n', 0};
extern shell_list_t* shell_variables = 0;
extern reboot();

void print_greetings()
{
	printf("Welcome to\n");
	printf("%s", almorina_title_I);
	printf("%s", almorina_title_II);
	printf("%s\n", almorina_title_III);
}

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
		default:
			n = nums[index];
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

char *shell_combine_strings(char **str_array, uint64_t size) {
  uint64_t total_length = 0;

  // Calculate the total length of the combined string
  for (uint64_t i = 0; i < size; i++) {
    total_length += strlen(str_array[i]) + 1;
  }
  total_length--;

  // Allocate a buffer for the combined string
  char *combined_str = malloc(total_length + 1);

  // Copy the strings into the combined string buffer
  uint64_t pos = 0;
  for (uint64_t i = 0; i < size; i++) {
    uint64_t str_len = strlen(str_array[i]);
    strncpy(combined_str + pos, str_array[i], str_len);
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
	char* start = line;
	char* end = line;

    int arg_count = count(line, ' ') + 1;
    char** args = (char**)malloc(sizeof(char*) * (arg_count));

	for (int i = 0; i < arg_count; i++)
	{
		// find the end of the argumnet
		end = strchr(start, ' ');
		if (end == 0)
			end = line + size;

		// copy the argument into new string
		char* arg = malloc(end-start + 1);
		memcpy(arg, start, end-start);
		arg[end-start] = 0;
		
		args[i] = (char*)malloc(sizeof(char) * (end-start));
		args[i] = arg;

		start = end + 1;
	}

    *argv = args;
	return arg_count;
}

void echo(char** argv, int argc)
{
	for (int i = 1; i < argc; i++)
	{
		int len = strlen(argv[i]);
		for(int j = 0; j < len; j++)
		{
			if(argv[i][j] != '\'' && argv[i][j] != '"')
				putc(argv[i][j]);
		}
	}
	putc('\n');
}

void set_variable(shell_list_t* node, const char* name, const char* data)
{
	int name_len = strlen(name);
	node->name = malloc(name_len);
	strcpy(node->name, name);
	node->name[name_len] = 0;

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
	shell_list_t* curr = shell_variables;
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

	printf("[%s]\n", node[0].name);
	
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

void exit(char** argv, int argc)
{
	reboot();
}

void print_fs_tree(fs_inode_t* dir, int level)
{
	for (int i =0; i<NUM_OF_BLOCKS_IN_INODE; i++)
	{
		if (dir->blocks[i] == 0)
			continue;

		for (int j = 0; j<BLOCK_SIZE; j+=sizeof(fs_dir_entry))
		{
			fs_dir_entry* ent = dir->blocks[i] + j;
			if ( ent->is_taken == 1 )
			{
				for (int _ = 0; _<level; _++) puts("|---");
				printf("%s -> %d\n", ent->name,  ent->inode);
				if (ent->inode->mode == INODE_TYPE_DIR)
					print_fs_tree(ent->inode, level+1);
			}
		}
	}
}

void tree(char** argv, int argc)
{
	print_fs_tree(ramfs_root, 0);
}

void touch(char** argv, int argc)
{
	char* path = shell_combine_strings(argv+1, argc-1);
	fs_inode_t* dir = fs_get_entry_dir(ramfs_device, ramfs_root, path);
	if (dir == 0)
	{
		puts("path doesnt exist!\n");
		return;
	}

	int len;
	char filename[FS_MAX_FILENAME_SIZE + 1] = {0};
	strncpy(filename, fs_extract_filename_from_path(path, &len), len);
	filename[len] = 0;

	printf("new file [%s] in inode %d\n", filename, dir);
	fs_dir_add_entry(ramfs_device, dir, filename, INODE_TYPE_FILE);
}

void mkdir(char** argv, int argc)
{
	char* path = shell_combine_strings(argv+1, argc-1);
	fs_inode_t* dir = fs_get_entry_dir(ramfs_device, ramfs_root, path);
	if (dir == 0)
	{
		puts("path doesnt exist!\n");
		return;
	}

	int len;
	char filename[FS_MAX_FILENAME_SIZE + 1] = {0};
	strncpy(filename, fs_extract_filename_from_path(path, &len), len);
	filename[len] = 0;

	printf("new dir [%s] in inode %d\n", filename, dir);
	fs_dir_add_entry(ramfs_device, dir, filename, INODE_TYPE_DIR);
}

fs_dir_entry* file_exist(char** argv, int argc)
{
	char* filename = shell_combine_strings(argv+1, argc-1);
	fs_inode_t* dir = ramfs_root;
	fs_dir_entry* file = -1;

	for (int i =0; i<NUM_OF_BLOCKS_IN_INODE; i++)
	{
		if (dir->blocks[i] == 0)
			continue;

		for (int j= 0; j<BLOCK_SIZE; j+=sizeof(fs_dir_entry))
		{
			fs_dir_entry* ent = dir->blocks[i] + j;
			if (strcmp(ent->name, filename) == 0)
			{
				file = ent;
				goto check;
			}
		}
	}

	check:
	if (file == -1)
	{
		puts("file doesn't exist\n");
		free(filename);
	}

	return file;
}

void edit(char** argv, int argc)
{
	fs_dir_entry* file = file_exist(argv, argc);

	if (file == -1)
		return;

	puts("Enter new file contents: ");
	char data[100] = {0};
	fgets(data, sizeof(data));
	fs_inode_write_data(ramfs_device, file->inode, data);
}

void cat(char** argv, int argc) // change
{
	fs_dir_entry* file = file_exist(argv, argc);

	if (file == -1)
		return;

	char* data = fs_inode_get_data(ramfs_device, file->inode);
	puts(data);
	free(data);
}

void ls(char** argv, int argc)
{
	char* path = shell_combine_strings(argv+1, argc-1);
	fs_inode_t* dir = fs_get_entry_dir(ramfs_device, ramfs_root, path);
	if (dir == 0)
	{
		puts("path doesnt exist!\n");
		return;
	}

	for (int i =0; i<NUM_OF_BLOCKS_IN_INODE; i++)
	{
		if (dir->blocks[i] == 0)
			continue;

		for (int j = 0; j<BLOCK_SIZE; j+=sizeof(fs_dir_entry))
		{
			fs_dir_entry* ent = dir->blocks[i] + j;
			if(ent->name[0] == 0)
			{
				i = NUM_OF_BLOCKS_IN_INODE;
				break;
			}
			printf("%s\n", ent->name);
		}
	}
}

void help(char** argv, int argc);

struct shell_command shell_callback[] = {
	// COMMAND	MIN ARGS	CALLBACK FUNC
	{"echo", 	2, 			echo},
	{"set", 	3, 			set},
	{"help", 	1, 			help},
	{"vars",	1,			vars},
	{"exit",	1,			exit},
	{"touch",	2,			touch},
	{"mkdir",	2,			mkdir},
	{"tree",	1,			tree},
	{"edit",	2,			edit},
	{"cat",		2,			cat},
	{"ls",		1,			ls}
};

void help(char** argv, int argc)
{
	set_fg_color(DARKGREY);
	for (int i = 0; i < sizeof(shell_callback) / sizeof(struct shell_command); i++)
		printf("%s ", shell_callback[i].command);
	putc('\n');
}

void shell_execute(char** argv, int argc)
{
	if (argc < 1)
		return;

	for (int i = 0; i < sizeof(shell_callback) / sizeof(struct shell_command); i++)
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

void shell_main()
{
	char line[100] = {0};
	
	do 
	{
		set_fg_color(LIGHTGREY);
		printf("command?> ");
		set_fg_color(PINK);
		fgets(line, sizeof(line));
		set_fg_color(DARKGREY);

		for (char* i = line; *i != 0; i++)
			if (*i == '\n') 
				*i = 0;

		char** args;
		int argc = shell_parse(line, &args);

		shell_execute(args, argc);

		//clean-up

		//for (int i = 0; i < argc; i ++)
		//	free(args[i]);
		//free(args);

	} while(1);
}

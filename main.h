#define JSH_RL_BUFFSIZE 1024
#define JSH_TOKEN_BUFFSIZE 64
#define JSH_TOKEN_DELIM " \t\r\n\a"

int main (int argc, char** argv);
void jsh_loop(void);

char* jsh_read_line(void);
char* jsh_read_line_2(void);

char** jsh_split_line(char* line);
int jsh_launch(char** args);

int jsh_num_builtins();
int jsh_execute(char** args);
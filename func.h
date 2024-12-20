extern char* builtin_str[];
extern int (*builtin_func[]) (char**);
int jsh_cd(char** args);
int jsh_help(char** args);
int jsh_exit(char** args);
int jsh_wtgw(char** args);
void clr_reset();
void clr_yellow();
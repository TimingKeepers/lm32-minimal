#ifndef __SHELL_H
#define __SHELL_H

const char *fromhex(const char *hex, int *v);
const char *fromdec(const char *dec, int *v);

struct shell_cmd {
	char *name;
	int (*exec) (const char *args[]);
};
extern struct shell_cmd __cmd_begin[], __cmd_end[];

/* Put the structures in their own section */
#define DEFINE_COMMAND(_name) \
	static struct shell_cmd __cmd_ ## _name \
	__attribute__((section(".cmd"), __used__))

char *env_get(const char *var);
int env_set(const char *var, const char *value);
void env_init();

int shell_exec(const char *buf);
void shell_interactive();

#endif

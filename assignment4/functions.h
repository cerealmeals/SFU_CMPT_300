#include <sys/stat.h>

void ls();
void lsi();
void lsl();
void lsil();

void output_l(struct stat* info);
void if_else_permisions(mode_t mode, int mask, char c);
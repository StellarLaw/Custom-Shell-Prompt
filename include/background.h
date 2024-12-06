#ifndef BACKGROUND_H
#define BACKGROUND_H


#include <sys/types.h> 
extern int job_count;  


void add_job(pid_t pid, char *command_line);
void remove_job(pid_t pid);
void list_jobs();
void sigchld_handler(int sig);

#endif

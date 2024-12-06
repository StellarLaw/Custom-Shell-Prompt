#include "background.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_JOBS 10

typedef struct {
    int job_number;
    pid_t pid;
    char *command_line;
} job;

job jobs[MAX_JOBS];
int job_count = 0;
int next_job_number = 1;

void add_job(pid_t pid, char *command_line) {
    if (job_count >= MAX_JOBS) {
        fprintf(stderr, "Maximum background jobs reached.\n");
        return;
    }
    jobs[job_count].job_number = next_job_number++;
    jobs[job_count].pid = pid;
    jobs[job_count].command_line = strdup(command_line); 
    job_count++;
    printf("[%d] %d\n", jobs[job_count - 1].job_number, pid); 
}

void remove_job(pid_t pid) {
    for (int i = 0; i < job_count; i++) {
        if (jobs[i].pid == pid) {
            
            printf("[%d] + done %s\n", jobs[i].job_number, jobs[i].command_line);
            free(jobs[i].command_line); 
            
            for (int j = i; j < job_count - 1; j++) {
                jobs[j] = jobs[j + 1];
            }
            job_count--;
            break;
        }
    }
}

void list_jobs() {
    if (job_count == 0) {
        printf("No active background jobs.\n");
        return;
    }
    for (int i = 0; i < job_count; i++) {
        printf("[%d]+ %d %s\n", jobs[i].job_number, jobs[i].pid, jobs[i].command_line);
    }
}



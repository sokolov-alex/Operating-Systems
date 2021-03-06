#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <rdtsc.h>

int
main(int argc, char * argv[])
{
    if(argc < 2) {
        printf("usage : ./program 100");
        exit(0);
    }
    
    int loops = atoi(argv[1]);
    
    int i = 0;
    unsigned long long total = 0;
    
    for(;i < loops;i++) {
        
        int pipefd[2];
        pid_t cpid;
        char buf;
        
        pipe(pipefd); // create the pipe
    
        cpid = fork(); // duplicate the current process
        if (cpid == -1) {
            printf("fork failed!\n");
            exit(-1);
        }
        if (cpid == 0) { //Child
            unsigned long long end;
            end = rdtsc();
            write(pipefd[1], &end, sizeof(end));
            return 0;
        } else {
            unsigned long long start, end, diff;
            start = rdtsc();
            read(pipefd[0], &end, sizeof(end));
            diff = (end - start);
            total += diff;
            wait(NULL);
        }
        
    }

    printf("process context switch : %llu cycles; Total : %llu \n", total/loops, total);
    return 0;
}
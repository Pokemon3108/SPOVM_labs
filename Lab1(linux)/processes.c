#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>


int main(int argc, char*argv[]) {
    pid_t p=fork();
    time_t t;
    
    switch (p){
        case -1:
            printf("Error");
            break;
        case 0:
	    //execle("xterm", "new window","/home/pokemon/Рабочий стол/Linux_labs/Lab1/time", argv);
	    execlp("xterm", "new window", "/home/pokemon/Рабочий стол/Linux_labs/Lab1/time", NULL);
        default:
            t=time(NULL);
	    puts("Parent:");
            puts(ctime(&t));
	    wait(0);
	    
            break;

    }

}

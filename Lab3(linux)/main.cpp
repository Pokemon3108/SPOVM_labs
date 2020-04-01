#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <cstring>

#define MAX 100

int main() {
    using namespace std;

    sem_t *sem = sem_open("sem10", O_CREAT, S_IROTH | S_IWOTH, 1);
    if (sem == NULL) {
        cout << "null" << endl;
        return 0;
    }

    int fd[2];
    pipe(fd);

    pid_t p;
    p = fork();
    while (true) {
        switch (p) {
            case -1:
                cout << "Error" << endl;
                return 0;
            case 0: {

                sem_wait(sem);
                close(fd[1]);
                char *gotten_str = (char *) malloc(sizeof(char) * MAX);
                read(fd[0], gotten_str, MAX);

                if (!strcmp(gotten_str, "q")) {
                    break;
                }
                cout << "String in client process: " << gotten_str << endl;

                sem_post(sem);
                usleep(100000);
                continue;
            }

            default:

                sem_wait(sem);
                close(fd[0]); //закрыть чтение
                string str;
                cout << "Input string (server), 'q' for exit" << endl;
                getline(cin, str);
                write(fd[1], str.c_str(), str.length());
                if (str == "q") {
                    break;
                }
                sem_post(sem);
                usleep(100000);
                continue;

        }

        sem_unlink("sem10");
        return 0;
    }


}

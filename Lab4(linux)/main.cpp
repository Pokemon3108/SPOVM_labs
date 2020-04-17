#include <iostream>
#include <unistd.h>
#include <list>
#include <pthread.h>
#include <vector>
#include <string.h>

using namespace std;
void* printThreadInformation(void* arg);

int currentThread=0;
vector<int> threadNumbers;
pthread_mutex_t mutex;

int main() {

    list<pthread_t> threadList;
    int number;
    pthread_t thread;
    pthread_mutex_init(&mutex, NULL);

    while (true){
        char c;
        cin.get(c);
        switch(c){
            case '+':

                threadNumbers.push_back(number);
                if (pthread_create(&thread,NULL,printThreadInformation,&number)) {
                    cout << "Thread was not created";
                    break;
                }
                threadList.push_back(thread);
                sleep(1);
                break;

            case '-':
                pthread_cancel(threadList.back());
                threadList.pop_back();
                threadNumbers.pop_back();
                break;

            case 'q':
                while (!threadList.empty()){
                    pthread_cancel(threadList.back());
                    threadList.pop_back();
                }
                pthread_mutex_destroy(&mutex);
                return 0;

        }
        rewind(stdin);

    }

}


void* printThreadInformation(void* arg){
    int number=*(int*)arg;
    *(int*)arg+=1;

    while (true) {
        if (number!=threadNumbers[currentThread]) continue;
        pthread_mutex_lock(&mutex);
        string str="Thread: "+to_string(number);
        for (int i=0; i<str.length(); ++i){
            printf("%c", str[i]);
        }
        printf("\n");
        ++currentThread;
        if (currentThread==threadNumbers.size()) currentThread=0;
        sleep(1);
        pthread_mutex_unlock(&mutex);

    }
}
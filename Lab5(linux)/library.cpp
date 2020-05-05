#include <iostream>
#include <pthread.h>
#include <vector>
#include <aio.h>
#include <unistd.h>
#include <cstring>
#include <sys/stat.h>
#include <fcntl.h>


pthread_mutex_t mutex;
bool isEnd=false;
using namespace std;

extern "C" void* read_f(void* arg){
    cout<<"Start reading"<<endl;
    pthread_mutex_lock(&mutex);
    vector<string> files;
    files.push_back("file1.txt");
    files.push_back("file2.txt");
    files.push_back("file3.txt");


    vector<string>::iterator it;
    for (it=files.begin(); it!=files.end(); ++it){
        aiocb readFile;
        readFile.aio_buf=(char*)arg;
        readFile.aio_nbytes=100;
        readFile.aio_sigevent.sigev_notify=SIGEV_NONE;

        int file;
        struct stat stat;
        int size;

        file=open(it->c_str(),O_RDWR, S_IRUSR | S_IWUSR);
        fstat(file,&stat);
        size=stat.st_size;

        readFile.aio_nbytes=size;
        readFile.aio_fildes=file;
        readFile.aio_offset=0;

        aio_read(&readFile);
        while (aio_error(&readFile)==EINPROGRESS);

        //cout<<"File: "<<((char*)arg)[0]<<endl;

        pthread_mutex_unlock(&mutex);
        usleep(30000);
        pthread_mutex_lock(&mutex);

        close(file);
    }
    pthread_mutex_unlock(&mutex);
    isEnd=true;
}

extern "C" void* write_f(void* arg){
    cout<<"Start writing"<<endl;
    int file;
    file=open("result.txt",O_WRONLY | O_TRUNC | O_CREAT);

    cout<<"file opened"<<endl;

    usleep(30000);

    aiocb writeFile;
    writeFile.aio_offset=0;

    while(!isEnd){
        pthread_mutex_lock(&mutex);

        writeFile.aio_fildes=file;
        writeFile.aio_buf=(char*)arg;
        writeFile.aio_sigevent.sigev_notify=SIGEV_NONE;
        writeFile.aio_nbytes=strlen((char*)arg);

        aio_write(&writeFile);
        while (aio_error(&writeFile)==EINPROGRESS);
        //cout<<"File write: "<<((char*)arg)[0]<<endl;

        writeFile.aio_offset+=strlen((char*)arg);
        pthread_mutex_unlock(&mutex);

        usleep(30000);
    }
    close(file);
    pthread_mutex_destroy(&mutex);
}


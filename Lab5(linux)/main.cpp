#include <iostream>
#include <pthread.h>
#include <dlfcn.h>

using namespace std;

typedef void *(*foo)(void *arg);


int main() {

    char *buffer = (char *) calloc(100, sizeof(char));
    void *handle = dlopen("/home/pokemon/Рабочий стол/Linux_labs/Lab5/Client/library.so", RTLD_LAZY);

    if (!handle){
        cout<<"error"<<endl;
        return 0;
    }

    foo reader_f = (foo) dlsym(handle, "read_f");
    foo writer_f = (foo) dlsym(handle, "write_f");


    pthread_t writer, reader;
    pthread_create(&reader, NULL, reader_f, buffer);
    pthread_create(&writer, NULL, writer_f, buffer);


    pthread_join(reader, NULL);
    pthread_join(writer, NULL);

    dlclose(handle);
    delete[] buffer;
    return 0;

}

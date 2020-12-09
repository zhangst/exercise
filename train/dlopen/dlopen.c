#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

void main_print(void) {
    printf("main_print\n");
}

int main(int argc, char **argv) {

    void * handle;
    void (*p)(void);
    char * error;

    handle = dlopen("./libdl.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    dlerror();

    *(void **)(&p) = dlsym(handle, "so_print");
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
    }

    (*p)();
    dlclose(handle);
    exit(EXIT_SUCCESS);
    
    return 0;
}

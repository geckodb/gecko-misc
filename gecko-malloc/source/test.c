#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define NUM_THREADS 4
#define ALLOCS 2000
#define MAX_ALLOC 0x100000

void *allocator(void **foo)
{
	for (unsigned int i=0;i< 100000;++i) {
        int index = i++ % ALLOCS;
        int tmp = rand() % MAX_ALLOC;
        free(foo[index]);
        foo[index] = malloc(tmp);
        if (foo[index] == NULL)
            fprintf(stdin,"cannot allocate");
        else
        {
            memset(foo[index],1,tmp);
            char *cmp = foo[index];
            for (int j=0;j<tmp;++j)
            {
            	if (*cmp != 1)
            		printf("Memory conflict");
            	++cmp;
    		}
            memset(foo[index],0,tmp);
            cmp = foo[index];
            for (int j=0;j<tmp;++j)
            {
            	if (*cmp != 0)
            		printf("Memory conflict");
            	++cmp;
    		}
        }
    }
    return NULL;
}

int main(void)
{
    pthread_t threads[NUM_THREADS];
    int rc;

    long t;
    void *memory[NUM_THREADS * ALLOCS] = {0};
    for (t = 0; t < NUM_THREADS; t++)
    {
        //printf("In main: creating thread %ld\n", t);
        rc = pthread_create(threads + t, NULL, allocator, memory+ALLOCS*t);
        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    //getchar();
    pthread_exit(NULL);
}

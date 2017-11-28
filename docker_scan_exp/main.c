#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <assert.h>
#include <sys/time.h>
#include <string.h>

#define MAX_SAMPLES 50

typedef struct thread_data_t {
    size_t tid;
    const uint32_t *begin, *end;
    uint32_t *result_set;
    size_t result_set_size;
} thread_data_t;

#define STOP_TIMER()                                                            \
({                                                                              \
    long start;                                                                 \
    struct timeval timecheck;                                                   \
    gettimeofday(&timecheck, NULL);                                             \
    start = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec / 1000;     \
    start;                                                                      \
})


/* taken from: https://stackoverflow.com/questions/822323/how-to-generate-a-random-number-in-c */
int randint(int n) {
    if ((n - 1) == RAND_MAX) {
        return rand();
    } else {
        // Chop off all of the values that would cause skew...
        long end = RAND_MAX / n; // truncate skew
        assert (end > 0L);
        end *= n;

        // ... and ignore results from rand() that fall above that limit.
        // (Worst case the loop condition should succeed 50% of the time,
        // so we can expect to bail out of this loop pretty quickly.)
        int r;
        while ((r = rand()) >= end);

        return r % n;
    }
}

void *scan_function(void *args) {
    thread_data_t *local = (thread_data_t *) args;
    //fprintf(stderr, "thread#%zu is starting...\n", local->tid);
    local->result_set_size = 0;
    for (const uint32_t *lhs = local->begin; lhs < local->end; lhs++) {
        if (*lhs % 2 == 0) {
            local->result_set[local->result_set_size++] = (uint32_t) (lhs - local->begin);
        }

    }
  //  fprintf(stderr, "thread#%zu finished\n", local->tid);
    return NULL;
}

#define TARGET_GB 2
#define MAX_NUM_REQUESTS 2048

int main() {

    size_t num_fields = (TARGET_GB * 1024 * 1024) / sizeof(uint32_t);


    uint32_t *col = malloc (num_fields * sizeof(uint32_t));
    for (size_t i = 0; i < num_fields; i++) {
        col[i] = randint(10000);
    }

    fprintf(stdout, "num_requests;num_threads;job_process_wallclock_time_msec;job_merge_wallclock_time_msec;job_full_wallclock_time_msec;sample_num;batch_num\n");
    fflush(stdout);


    for (size_t NUM_REQUESTS = 2048; NUM_REQUESTS <= MAX_NUM_REQUESTS; NUM_REQUESTS *= 2) {
        for (size_t THREAD_POOL_SIZE = 1; THREAD_POOL_SIZE <= NUM_REQUESTS; THREAD_POOL_SIZE *= 2) {


            for (size_t sample = 0; sample < MAX_SAMPLES; sample++) {

                long total_time_process = 0;

                size_t max_spawned_threads = (NUM_REQUESTS < THREAD_POOL_SIZE) ? NUM_REQUESTS : THREAD_POOL_SIZE;
                int64_t remaining_requests = NUM_REQUESTS;

                pthread_t *thread_pool = malloc(max_spawned_threads * sizeof(pthread_t));
                thread_data_t *thread_data = malloc(max_spawned_threads * sizeof(thread_data_t));

             //   fprintf(stderr, "======================================================================\n");
                //   fprintf(stderr, "#requests: %zu\t#threads: %zu, sample:%zu\n", NUM_REQUESTS, max_spawned_threads, sample);
                //   fprintf(stderr, "======================================================================\n");
                //   fprintf(stderr, "\n\n");

                for (unsigned i = 0; i < max_spawned_threads; i++) {
                    thread_data[i].tid = i;
                    thread_data[i].begin = col;
                    thread_data[i].end = col + num_fields;
                    thread_data[i].result_set = malloc(num_fields * sizeof(uint32_t));
                }

                size_t num_batches = 0;

                do {
                    num_batches++;
                    long time_process_begin = STOP_TIMER();

                    for (unsigned i = 0; i < max_spawned_threads; i++) {
                        if ((pthread_create(thread_pool + i, NULL, scan_function, thread_data + i))) {
                            fprintf(stderr, "error: pthread_create failed\n");
                            return EXIT_FAILURE;
                        }
                    }

                    for (unsigned i = 0; i < max_spawned_threads; i++) {
                        pthread_join(thread_pool[i], NULL);
                    }

                    long time_process_end = STOP_TIMER();

                    long this_time_process = time_process_end - time_process_begin;

                    total_time_process += this_time_process;
                    remaining_requests -= max_spawned_threads;

                    //           fprintf(stderr, "\t + request batched processing time (this/total): %ld msec / %ld msec\n", this_time_process, total_time_process);

                } while (remaining_requests > 0);

                long time_merge_begin = STOP_TIMER();

                size_t final_result_set_size = 0;
                for (unsigned i = 0; i < max_spawned_threads; i++) {
                    final_result_set_size += thread_data[i].result_set_size;
                }

                assert(final_result_set_size > 0);

                void *result_position_list = malloc(final_result_set_size * sizeof(uint32_t));
                size_t result_list_offset = 0;
                for (unsigned i = 0; i < max_spawned_threads; i++) {
                    size_t local_result_set_bytes = thread_data[i].result_set_size * sizeof(uint32_t);
                    if(local_result_set_bytes > 0) {
                        memcpy(result_position_list + result_list_offset,
                               thread_data[i].result_set, local_result_set_bytes);
                        result_list_offset += local_result_set_bytes;
                    }

                }

                long time_merge_end = STOP_TIMER();

                long total_time_merge = time_merge_end - time_merge_begin;



                //        fprintf(stderr, "\t ----------------------------------------\n\t =         %ld\n", (total_time_process));
                //      fprintf(stderr, "\n\n\n\n");
                fprintf(stdout, "%zu;%zu;%ld;%ld;%ld;%ld;%zu\n", NUM_REQUESTS, THREAD_POOL_SIZE, total_time_process, total_time_merge, (total_time_process + total_time_merge), sample, num_batches);
                fflush(stdout);

                for (unsigned i = 0; i < max_spawned_threads; i++) {
                    free(thread_data[i].result_set);
                }

                free(thread_data);
                free(thread_pool);
                free (result_position_list);

            }


        }
    }

    free (col);

    return 0;
}

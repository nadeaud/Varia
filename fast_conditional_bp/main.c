
#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <getopt.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <time.h>

#define DEFAULT_INNER 1000
#define DEFAULT_OUTER 10
#define DEFAULT_THREAD 2

#define BILLION 1E9

double * time_per_iteration;
uint64_t * total_time;

struct args_t {
    int id;
    int help;
    int inner;
    int outer;
    int thread;
    int thread_min;
    int thread_max;
    pthread_barrier_t * barrier;
};

// Cpu hog to ensure that there will be enough cpu work
double do_ops(double x, int max) {
    volatile double y =0;
    int i;
    for(i=0; i<max; i++)
    {
        x += i;
        y += (x-1)/(x+1);
    }
    return y;
}

void inner_loop(int repeat, int calculations, int j) {
    volatile double x=j;
    volatile double y=0;
    int i;

    for(i=0; i<repeat; i++) {
        x = x+0.5;
        y = (x-1)/(x+1);
        y = do_ops(x, calculations);
    }
    (void) y;
}

void* pthread_task(void * arg){

    //pid_t pid = syscall(__NR_gettid);
//    printf("Thread %d starts.\n", pid);

    struct args_t * args = (struct args_t *) arg;

    struct timespec t1, t2;

    uint64_t interval=0;

    pthread_barrier_wait(args->barrier);

    clock_gettime(CLOCK_MONOTONIC, &t1);
    inner_loop(args->outer, args->inner, 1);
    clock_gettime(CLOCK_MONOTONIC, &t2);
    interval = BILLION*(t2.tv_sec - t1.tv_sec) + (t2.tv_nsec - t1.tv_nsec);

    total_time[args->id] = interval;
    interval = (double)interval /(double)args->outer;

//    printf("Mean time for the calculations : %f usec\n", mean);
//    printf("Number of inner iterations : %d\n", args->inner);
//    printf("Average time per iteration : %f\n", mean/args->inner);
    time_per_iteration[args->id] = interval;
    return NULL;
}

static void parse_opts(struct args_t * args, int argc, char ** argv) {

    // Default arguments

    args->help = 0;
    args->inner = DEFAULT_INNER;
    args->outer = DEFAULT_OUTER;
    args->thread = DEFAULT_THREAD;
    args->thread_min = 0;
    args->thread_max = 0;

    int opt;

    struct option options[] = {
        { "help",   0,0,'h'},
        { "inner",  1,0,'r'},
        { "outer",  1,0,'r'},
        { "thread", 1,0,'r'},
        { "start",  1,0,'r'},
        { "end",    1,0,'r'},
        { 0, 0, 0, 0}
    };

    int idx;

    while((opt = getopt_long(argc, argv, "hi:o:t:s:e:", options, &idx)) != -1) {
        switch(opt) {
            case 'h' :
                args->help = 1;
                break;
            case 'i' :
                args->inner = atol(optarg);
                break;
            case 'o' :
                args->outer = atol(optarg);
                break;
            case 't' :
                args->thread = atol(optarg);
                break;
            case 's' :
                args->thread_min = atol(optarg);
                break;
            case 'e' :
                args->thread_max = atol(optarg);
                break;
            default :
                break;
        }
    }
}

void write_results(struct args_t * arg) {

    char buffer[1024];
    snprintf(buffer,1023, "results_nsec.csv"); //%d_threads_%d_inner_%d_outer.csv", arg->thread, arg->inner, arg->outer);
    FILE * f = fopen(buffer, "a+");
    //fprintf(f, "thread;average timer per iteration (us)\n");
    //int i;
    //for(i=0; i<arg->thread; i++)
    //    fprintf(f,"%d;%f\n", i, time_per_iteration[i]);
    double average = 0;
    uint64_t total_average=0;
    double total_average_double;

    int i;
    for(i=0 ; i<arg->thread; i++) {
        average += time_per_iteration[i];
        total_average += total_time[i];
    }
    average = (double)average/(double)arg->thread;
    total_average_double = (double)total_average/(double)arg->thread;

    fprintf(f,"%d,%f,%f,\n", arg->thread, average, total_average_double);
    fclose(f);

    fprintf(stdout, "Average time of %f nsec for %d threads.\n", average, arg->thread);
}


void help() {
    printf("This program spawns thread(s) that run a simple calculation in a loop. It is intended to be used to measure the impact of a breakpoint on a program in an non-stop mode with GDB.\n");
    printf("Arguments available : \n");
    printf("--help (-h) : print the help\n");
    printf("--outer (-o) : the number of time a thread calls the function that runs calculations\n");
    printf("--inner (-i) : the number of time the number the calculation is done inside the program\n");
    printf("--thread (-t) : the number of thread to spawn\n");
    exit(0);
}

void run_experiment(struct args_t * args){

    struct args_t * arg;

    pthread_t *th = malloc(sizeof(pthread_t) * args->thread);
    pthread_barrier_t *barrier  = malloc(sizeof(pthread_barrier_t));

    pthread_barrier_init(barrier, NULL, args->thread + 1);

    arg = malloc(args->thread * sizeof(struct args_t));
    time_per_iteration = malloc(args->thread * sizeof(double));
    total_time = malloc(args->thread * sizeof(double));

    args->barrier = barrier;

    int i;
    for(i=0 ; i<args->thread ; i++) {
        arg[i] = *args;
        arg[i].id = i;
        pthread_create(&th[i], NULL, pthread_task, &arg[i]);
    }

    pthread_barrier_wait(barrier);

    for(i=0; i<args->thread ; i++) {
        pthread_join(th[i], NULL);
    }

    write_results(args);

    pthread_barrier_destroy(barrier);

    free(time_per_iteration);
    free(arg);
    free(barrier);
    free(th);


}

int main(int argc, char ** argv) {

    struct args_t args;
    //struct args_t * arg;

    parse_opts(&args, argc, argv);

    if(args.help)
        help();

    if(args.thread_min > 0 && args.thread_max > args.thread_min) {
        int i;
        for(i = args.thread_min; i<=args.thread_max; i++) {
            args.thread = i;
            run_experiment(&args);
        }
    } else {
        run_experiment(&args);
    }

    return 0;
}

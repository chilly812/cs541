/*
 * Yufan Xue
 * Mar. 09, 2014
 *
 * CS 441/541 project-02-part2
 *
 */
#include "pirates-ninjas.h"

/*
 *
 */
int ninja_count = 0;
int pirate_count = 0;

/*
 * Semaphore(s)
 */
semaphore_t mutex_register;
semaphore_t mutex_ninja_count;
semaphore_t mutex_pirate_count;
semaphore_t mutex_department;
semaphore_t mutex_team;

/*
 *
 */
int time_to_run;
double tstart;

/*
 *
 */
int* pirate_enter;
int* pirate_left;
int* ninja_enter;
int* ninja_left;

int main(int argc, char * argv[]) {
    int ret;
    /*
     * Initialize:
     * - 
     * - 
     * - 
     */
    srandom(time(NULL));

    /*  Number of Pirates */
    int pirates_number = 0;

    /*  Number of Ninjas */
    int ninjas_number = 0;
    
    /*  Time to run in seconds */
    time_to_run = 0;

    int valid = check_command_line(argc,argv,&pirates_number,&ninjas_number,&time_to_run);

    printf("Time to Live (seconds)  : %d \n",time_to_run);
    printf("Number of Pirates       : %d \n",pirates_number);
    printf("Number of Ninjas        : %d \n",ninjas_number);
    printf("-------------------------------\n");

    pthread_t pirates_threads[pirates_number];

    pthread_t ninjas_threads[ninjas_number];

    if(valid)
    {
        /*
         * Initialize:
         * - semaphore_t mutex_register = 1;
         * - semaphore_t mutex_ninja_count = 1;
         * - semaphore_t mutex_pirate_count = 1;
         * - semaphore_t mutex_department = 1;
         * - semaphore_t mutex_team = 2;
         */
        if( 0 != (ret = semaphore_create(&mutex_register, 1)) ) {
            fprintf(stderr, "Error: semaphore_create() failed with %d\n", ret);
            return -1;
        }
        if( 0 != (ret = semaphore_create(&mutex_ninja_count, 1)) ) {
            fprintf(stderr, "Error: semaphore_create() failed with %d\n", ret);
            return -1;
        }
        if( 0 != (ret = semaphore_create(&mutex_pirate_count, 1)) ) {
            fprintf(stderr, "Error: semaphore_create() failed with %d\n", ret);
            return -1;
        }
        if( 0 != (ret = semaphore_create(&mutex_department, 1)) ) {
            fprintf(stderr, "Error: semaphore_create() failed with %d\n", ret);
            return -1;
        }
        if( 0 != (ret = semaphore_create(&mutex_team, 2)) ) {
            fprintf(stderr, "Error: semaphore_create() failed with %d\n", ret);
            return -1;
        }
        /*
         * int* pirate_enter;
         * int* pirate_left;
         * int* ninja_enter;
         * int* ninja_left;
         */
        pirate_enter = (int *)malloc(sizeof(int) * pirates_number);
        pirate_left = (int *)malloc(sizeof(int) * pirates_number);
        ninja_enter = (int *)malloc(sizeof(int) * ninjas_number);
        ninja_left = (int *)malloc(sizeof(int) *ninjas_number);
        /*
         * Create threads
         */
        /* Create Pirates */
        for(int i = 0 ; i <pirates_number ; i++)
        {      
            // printf("In Main(): Creating thread %d\n", i);
            ret = pthread_create(&pirates_threads[i],
                             NULL,
                             pirate,
                             (void *)(intptr_t)i);
            if(0 != ret ) {
                fprintf(stderr, "Error: Cannot Create thread\n");
                exit(-1);
            }
        }

        /* Create Ninjas */
        for(int i = 0 ; i <ninjas_number ;i++)
        {
            // printf("In Main(): Creating thread %d\n", i);
            ret = pthread_create(&ninjas_threads[i],
                             NULL,
                             ninja,
                             (void *)(intptr_t)i);
            if(0 != ret ) {
                fprintf(stderr, "Error: Cannot Create thread\n");
                exit(-1);
            }
        }
        tstart = (double)clock()/CLOCKS_PER_SEC;

    }else{
        printf("Error: valid input command line\n");
        pthread_exit(NULL);
    }

    /*
     * Join Thread(s)
     */
     /* Join producer thread */
    for(int i = 0; i < pirates_number; ++i ) {
        ret = pthread_join(pirates_threads[i], NULL);
        if( 0 != ret ) {
            fprintf(stderr, "Error: Cannot Join Thread %d\n", i);
            exit(-1);
        }
    }
    /* Join consumer thread */
    for(int i = 0; i < ninjas_number; ++i ) {
        ret = pthread_join(ninjas_threads[i], NULL);
        if( 0 != ret ) {
            fprintf(stderr, "Error: Cannot Join Thread %d\n", i);
            exit(-1);
        }
    }
    /*
     * Output enter & left
     */
    for(int i = 0 ; i < pirates_number ; i++)
    {
        printf("Pirate %d : Entered     %d / Left   %d\n",i,pirate_enter[i],pirate_left[i]);
    }
    for(int i = 0 ; i < ninjas_number ; i++)
    {
        printf("Ninja  %d : Entered     %d / Left   %d\n",i,ninja_enter[i],ninja_left[i]);
    }

    /*
     * Cleanup
     */
    if( 0 != (ret = semaphore_destroy(&mutex_register)) ) {
        fprintf(stderr, "Error: semaphore_destroy() failed with %d\n", ret);
        return -1;
    }
    if( 0 != (ret = semaphore_destroy(&mutex_department)) ) {
        fprintf(stderr, "Error: semaphore_destroy() failed with %d\n", ret);
        return -1;
    }
    if( 0 != (ret = semaphore_destroy(&mutex_team)) ) {
        fprintf(stderr, "Error: semaphore_destroy() failed with %d\n", ret);
        return -1;
    }
    if( 0 != (ret = semaphore_destroy(&mutex_ninja_count)) ) {
        fprintf(stderr, "Error: semaphore_destroy() failed with %d\n", ret);
        return -1;
    }
    if( 0 != (ret = semaphore_destroy(&mutex_pirate_count)) ) {
        fprintf(stderr, "Error: semaphore_destroy() failed with %d\n", ret);
        return -1;
    }
    
    pthread_exit(NULL);

    return 0;
}

void *pirate(void *threadid)
{
    int ret;
    int tid = (intptr_t)threadid;
    double tstop;
    double ttime;
    while(1)
    {
        tstop = (double)clock()/CLOCKS_PER_SEC;
        ttime= tstop-tstart;
        ttime = ttime * 10;
        // printf("Time = %f",ttime);
        if(ttime > time_to_run)
        {
           pthread_exit(NULL);
        }
        printf("Pirate      %d | Waiting \n",tid);
        /*
         * Critical Section to update buffer
         */
         /*  Wait the semaphore mutex  */
        if( 0 != (ret = semaphore_wait(&mutex_register)) ) {
            fprintf(stderr, "Error: semaphore_wait() failed with %d in thread %d\n", ret, tid);
            pthread_exit(NULL);
        }
        if( 0 != (ret = semaphore_wait(&mutex_pirate_count)) ) {
            fprintf(stderr, "Error: semaphore_wait() failed with %d in thread %d\n", ret, tid);
            pthread_exit(NULL);
        }
        /* Add one pirate */
        pirate_count++;
        if(pirate_count ==1)
        {
            /* Wait department */
            if( 0 != (ret = semaphore_wait(&mutex_department)) ) {
                fprintf(stderr, "Error: semaphore_wait() failed with %d in thread %d\n", ret, tid);
                pthread_exit(NULL);
            }
        }

        if( 0 != (ret = semaphore_post(&mutex_pirate_count)) ) {
            fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, tid);
            pthread_exit(NULL);
        }

        if( 0 != (ret = semaphore_post(&mutex_register)) ) {
            fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, tid);
            pthread_exit(NULL);
        }

        /* Wait team */
        if( 0 != (ret = semaphore_wait(&mutex_team)) ) {
            fprintf(stderr, "Error: semaphore_wait() failed with %d in thread %d\n", ret, tid);
            pthread_exit(NULL);
        }
        pirate_enter[tid]++;
        printf("Pirate      %d | Costume preparation\n",tid);
        int random_num = random() % 5000;
        // printf("I sleep %d",random_num);
        usleep(random_num);

        if( 0 != (ret = semaphore_post(&mutex_team)) ) {
            fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, tid);
            pthread_exit(NULL);
        }
        if( 0 != (ret = semaphore_wait(&mutex_pirate_count)) ) {
            fprintf(stderr, "Error: semaphore_wait() failed with %d in thread %d\n", ret, tid);
            pthread_exit(NULL);
        }
        pirate_left[tid]++;
        pirate_count--;
        printf("Pirate      %d | Leaving \n",tid);
        if(pirate_count == 0)
        {
            if( 0 != (ret = semaphore_post(&mutex_department)) ) {
                fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, tid);
                pthread_exit(NULL);
            } 
        }
        if( 0 != (ret = semaphore_post(&mutex_pirate_count)) ) {
            fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, tid);
            pthread_exit(NULL);
        }
        random_num = random() % 1000;
        // printf("I sleep %d",random_num);
        usleep(random_num);
    }    
}

void *ninja(void *threadid)
{
    int ret;
    int tid = (intptr_t)threadid;
    double ttime;
    double tstop;
    while(1)
    {   
        tstop = (double)clock()/CLOCKS_PER_SEC;
        ttime= tstop-tstart;
        ttime = ttime * 10;
        // printf("Time = %f",ttime);
        if(ttime > time_to_run)
        {
           pthread_exit(NULL);
        }
        printf("Ninja       %d | Waiting \n",tid);
        /*
         * Critical Section to update buffer
         */
         /*  Wait the semaphore mutex  */
        if( 0 != (ret = semaphore_wait(&mutex_register)) ) {
            fprintf(stderr, "Error: semaphore_wait() failed with %d in thread %d\n", ret, tid);
            pthread_exit(NULL);
        }
        if( 0 != (ret = semaphore_wait(&mutex_ninja_count)) ) {
            fprintf(stderr, "Error: semaphore_wait() failed with %d in thread %d\n", ret, tid);
            pthread_exit(NULL);
        }
        /* Add one pirate */
        ninja_count++;
        if(ninja_count ==1)
        {
            /* Wait department */
            if( 0 != (ret = semaphore_wait(&mutex_department)) ) {
                fprintf(stderr, "Error: semaphore_wait() failed with %d in thread %d\n", ret, tid);
                pthread_exit(NULL);
            }
        }

        if( 0 != (ret = semaphore_post(&mutex_ninja_count)) ) {
            fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, tid);
            pthread_exit(NULL);
        }

        if( 0 != (ret = semaphore_post(&mutex_register)) ) {
            fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, tid);
            pthread_exit(NULL);
        }

        /* Wait team */
        if( 0 != (ret = semaphore_wait(&mutex_team)) ) {
            fprintf(stderr, "Error: semaphore_wait() failed with %d in thread %d\n", ret, tid);
            pthread_exit(NULL);
        }
        ninja_enter[tid]++;
        printf("Ninja       %d | Costume preparation\n",tid);
        int random_num = random() % 5000;
        usleep(random_num);

        if( 0 != (ret = semaphore_post(&mutex_team)) ) {
            fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, tid);
            pthread_exit(NULL);
        }
        if( 0 != (ret = semaphore_wait(&mutex_ninja_count)) ) {
            fprintf(stderr, "Error: semaphore_wait() failed with %d in thread %d\n", ret, tid);
            pthread_exit(NULL);
        }
        ninja_left[tid]++;
        ninja_count--;
        printf("Ninja       %d | Leaving \n",tid);
        if(ninja_count == 0)
        {
            if( 0 != (ret = semaphore_post(&mutex_department)) ) {
                fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, tid);
                pthread_exit(NULL);
            } 
        }
        if( 0 != (ret = semaphore_post(&mutex_ninja_count)) ) {
            fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, tid);
            pthread_exit(NULL);
        }
        random_num = random() % 1000;
        // printf("I sleep %d",random_num);
        usleep(random_num);
    }
}

int check_command_line(int argc, char * argv[], int *pirates_number, int* ninjas_number, int* time_to_run)
{

    switch(argc)
    {
        case 2:
        /* time to run */
        if( !is_valid_int(argv[1]))
            return 0;
        else
        {
            int convert_int = strtol(argv[1],NULL,10);
            if(convert_int < 0)
                return 0;
            else
            {
                *time_to_run = convert_int;
                *pirates_number = 5;
                *ninjas_number = 5;
                return 1;
            }
        }
        break;

        case 3:
        if( is_valid_int(argv[1]) && is_valid_int(argv[2]) )
        {
            int convert_int1 = strtol(argv[1],NULL,10);
            int convert_int2 = strtol(argv[2],NULL,10);
            if( convert_int1 > 0 && convert_int2 >0)
            {
                *time_to_run = convert_int1;
                *pirates_number = convert_int2;
                return 1;

            }else
                return 0;
        }else
            return 0;
        break;
        
        case 4:
        if( is_valid_int(argv[1]) && is_valid_int(argv[2]) && is_valid_int(argv[3]) )
        {
            int convert_int1 = strtol(argv[1],NULL,10);
            int convert_int2 = strtol(argv[2],NULL,10);
            int convert_int3 = strtol(argv[3],NULL,10);
            if( convert_int1 > 0 && convert_int2 >0 && convert_int3 > 0)
            {
                *time_to_run = convert_int1;
                *pirates_number = convert_int2;
                *ninjas_number = convert_int3;
                return 1;

            }else
                return 0;
        }else
            return 0;
        break;
        
        default:
        return 0;
        break;
    }
}


int is_valid_int(char* str){
    //TODO
    int start = 0;
        
    while( *str != '\0')
    {
        if(start)
        {
            if(!isdigit(*str)){
                return 0;
            }
        }else if(*str == '+' || *str== '-'|| isdigit(*str)){
            //printf("here");
            start =1;
        }else if(!isspace(*str))
        {
            return 0;
        }
        str++;          
    }
    return 1;
}

// void output_buffer()
// {
//     printf("[");
//     for(int i = 0 ; i <buffer_size ; i++)
//     {
//         printf("    %d",buffer[i]);
//         if(i == in)
//             printf("^");
//         if(i == out)
//             printf("v");
//     }
//     printf("    ]\n");
// }


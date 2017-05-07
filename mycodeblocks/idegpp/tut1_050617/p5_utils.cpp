#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "p5_utils.h"

clock_t start, end1;
double cpu_time_used;
int start_time;

int printTime(){
    time_t now;
    struct tm *local;

    time (&now);
    local =  localtime(&now);
    printf("Time: %s", asctime(local));

    return 0;
}

int printHour(){
    time_t now;
    struct tm *local;

    time (&now);
    local =  localtime(&now);
    int hr = (local->tm_hour-7 < 0) ? (24+local->tm_hour - 7) : (local->tm_hour -7);
    hr = ( hr > 11) ? (hr - 12) : hr;
    printf("Time %d:%d:%d\n", (hr),(local->tm_min),local->tm_sec);

    return 0;

}

int prt_cmd_list(int n, char *aptr[]){
    while(--n > 0)
    {
        printf("%s ", *++aptr);
    }
    NL;
    return 0;
}

int pre_run(int argc, char *argv[]){

     prt_cmd_list(argc, argv);
     printf(" **** Start of Run ***  at  ");
     printHour();
     start = clock();
     //sleep in secs.....sleep(2);
    return 0;

}

int post_run(){
    end1 = clock();
    cpu_time_used = ((double) (end1 - start)) / CLOCKS_PER_SEC;

    printf("\n **** END of Run ***  at  ");
    printHour();
    return 0;

}

int fn_print_gpp_version() {

  if( __cplusplus == 201103L ) std::cout << "Version is C++11\n" ;
  else if( __cplusplus == 199711L ) std::cout << "Version is C++98\n" ;
  else std::cout << "pre-standard C++\n" ;
  return 0;
}

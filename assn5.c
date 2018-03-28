#include <stdio.h>

#define MAX_PROCS (100)
#define TIME_Q (100)

void FCFS(int *arr, int *burst, int count) {
    int start[MAX_PROCS]; //start time array
    int end[MAX_PROCS];  //end time array
    int ready[MAX_PROCS];  //ready queue
    int clock = 0; //clock time counter
    /*turn around time, wait time, response time*/
    int ta = 0, wait = 0, resp = 0;
    /*indices for next job, current job and insert into RQ, count of jobs, numbers from file*/
    int next = 0, curr = 0, insert = 0;

    for (int i = 0; i < count; ++i) {
        //if no jobs in queue, need to put one in
        if (curr == insert) {
            ready[insert++] = next;  //put next job in ready queue
            clock = arr[next];  //if next job hasn't arrived yet, jump clock to arrival time
            next++;
        }

        //run the next job in the RQ
        start[curr] = clock;
        clock += burst[curr];
        end[curr] = clock;
        ta += end[curr] - arr[curr];
        wait += start[curr] - arr[curr];
        resp += start[curr] - arr[curr];

        //check to see if new jobs have arrived
        while (next < count && arr[next] < clock) {
            //if yes, put them on the RQ
            ready[insert++] = next++;
        }
        curr++;  //go to next job
    }

    printf("First Come, First Served\n");

    //calculate stats
    printf("Avg Resp: %.2f  Avg TA: %.2f  Avg Wait: %.2f\n\n", (float) resp / count, (float) ta / count,
           (float) wait / count);
}

void RoundRobin(int *arr, int *burst, int count) {

    int ready[MAX_PROCS], remain[MAX_PROCS], start[MAX_PROCS], end[MAX_PROCS];
    int clock = 0;
    int ta = 0, wait = 0, resp = 0;  //turnaround, wait and response time
    int next = 0, curr = 0, insert = 0;

    while (next < count || curr != insert) { //still within arrays & not full
        /*fill empty ready queue*/
        if (curr == insert) { //no jobs in ready queue, need to put one in
            ready[insert] = next;  //ready contains pointer to which job
            remain[next] = burst[next];
            clock = arr[next];
            start[next] = -1;
            next++;
            insert++;
        }

        //run the next job in the RQ
        if (start[ready[curr]] < 0) { start[ready[curr]] = clock; }

        /*if it will have time left*/
        if (remain[ready[curr]] > TIME_Q) {
            clock += TIME_Q;
            remain[ready[curr]] -= TIME_Q;
        } else { /*if it is done*/
            clock += remain[ready[curr]];
            remain[ready[curr]] -= remain[ready[curr]];
            end[ready[curr]] = clock;
            wait += end[ready[curr]] - burst[ready[curr]] - arr[ready[curr]];
            ta += end[ready[curr]] - arr[ready[curr]];
            resp += start[ready[curr]] - arr[ready[curr]];
        }

        /*move arrived bursts to ready queue*/
        while (next < count && arr[next] <= clock) {
            ready[insert] = next;
            remain[next] = burst[next];
            start[next] = -1;
            next++;
            insert++;
        }

        if (remain[ready[curr]] > 0) {
            /*burst still has time, add to bottom of queue*/
            ready[insert] = ready[curr];
            insert++;
        }

        /*increment to next job in ready queue*/
        curr++;
    }
    printf("Round Robin with Time Quantum of %d\n", TIME_Q);

    //calculate stats
    printf("Avg Resp: %.2f  Avg TA: %.2f  Avg Wait: %.2f\n\n", (float) resp / count, (float) ta / count,
           (float) wait / count);
}

int main(int argc, char *argv[]) {
    printf("\nAssignment 5 FCFS & Round Robin by Kris Olson\n");
    printf("Project Team: Kris Olson, Joseph Jacobson\n\n");

    FILE *fp;
    int arr[MAX_PROCS];  //arrival time array
    int burst[MAX_PROCS];  //burst time array
    int count = 0;  //count of jobs

    //read args
    if (argc != 2) {
        fp = stdin;
    } else {
        fp = fopen(argv[1], "r");
        if (NULL == fp) {
            printf("File not found.\n");
            return -1;
        }
    }

    /*read from file into arrays*/
    while (fscanf(fp, "%d %d", &arr[count], &burst[count]) == 2) {
        count++; //count of jobs
    }

    FCFS(arr, burst, count);
    RoundRobin(arr, burst, count);

    return 0;
}

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int rpid = 1;
int cpu[1000];
double turntime = 0;
double waittime = 0;


typedef struct {
    int pid;
    int arrtime;
    int burtime;
    int iburtime[5];
    int iortime[5];
    int ibur_rem[5];
    int prior;
    int extime;
    int io_index;
    int completed;
} Process;

typedef struct {
    Process* qlist[1000];
    int front, rear;
} queue;


void initqueue(queue* q) {
    q->front = q->rear = 0;
}

void enqueue(queue* q, Process* p) {
    q->qlist[q->rear++] = p;
}

Process* dequeue(queue* q) {
    if (q->front == q->rear) return -2;
    return q->qlist[q->front++];
}

int isEmpty(queue* q) {
    if (q->front == q->rear) return 1;
    else return 0;
}


void Makesort2(Process* arr[], int size, int mode, int mode2, int time) {
    if (arr == NULL) {
        printf("There is nothing to sort. error in Makesort\n");
        return -1;
    }
    if (mode == 0 && mode2 == 0) {
        for (int i = 0;i < size - 1;i++) {
            for (int j = i + 1;j < size;j++) {
                if ((time - arr[i]->arrtime + arr[i]->burtime) / arr[i]->burtime > (time - arr[j]->arrtime + arr[j]->burtime) / arr[j]->burtime) {
                    Process* temp = arr[j];
                    arr[j] = arr[i];
                    arr[i] = temp;
                }
            }
        }
    }
    else if (mode == 1 && mode2 == 0) {
        for (int i = 0;i < size - 1;i++) {
            for (int j = i + 1;j < size;j++) {
                if ((time - arr[i]->arrtime + arr[i]->burtime) / arr[i]->burtime < (time - arr[j]->arrtime + arr[j]->burtime) / arr[j]->burtime) {
                    Process* temp = arr[j];
                    arr[j] = arr[i];
                    arr[i] = temp;
                }
            }
        }
    }

}


void Makesort(Process* arr[], int size, int mode, int mode2) {
    if (arr == NULL) {
        printf("There is nothing to sort. error in Makesort\n");
        return -1;
    }
    if (mode == 0 && mode2 == 0) {
        for (int i = 0;i < size - 1;i++) {
            for (int j = i + 1;j < size;j++) {
                if (arr[i]->prior > arr[j]->prior) {
                    Process* temp = arr[j];
                    arr[j] = arr[i];
                    arr[i] = temp;
                }
            }
        }
    }
    else if (mode == 1 && mode2 == 0) {
        for (int i = 0;i < size - 1;i++) {
            for (int j = i + 1;j < size;j++) {
                if (arr[i]->prior < arr[j]->prior) {
                    Process* temp = arr[j];
                    arr[j] = arr[i];
                    arr[i] = temp;
                }
            }
        }
    }
    else if (mode == 1 && mode2 == 1) {
        for (int i = 0;i < size - 1;i++) {
            for (int j = i + 1;j < size;j++) {
                if (arr[i]->burtime - arr[i]->extime < arr[j]->burtime - arr[j]->extime) {
                    Process* temp = arr[j];
                    arr[j] = arr[i];
                    arr[i] = temp;
                }
            }
        }
    }
    else if (mode == 0 && mode2 == 1) {
        for (int i = 0;i < size - 1;i++) {
            for (int j = i + 1;j < size;j++) {
                if (arr[i]->burtime - arr[i]->extime > arr[j]->burtime - arr[j]->extime) {
                    Process* temp = arr[j];
                    arr[j] = arr[i];
                    arr[i] = temp;
                }
            }
        }
    }
    else if (mode == 1 && mode2 == 2) {
        for (int i = 0;i < size - 1;i++) {
            for (int j = i + 1;j < size;j++) {
                if (arr[i]->arrtime < arr[j]->arrtime) {
                    Process* temp = arr[j];
                    arr[j] = arr[i];
                    arr[i] = temp;
                }
            }
        }
    }
    else if (mode == 0 && mode2 == 2) {
        for (int i = 0;i < size - 1;i++) {
            for (int j = i + 1;j < size;j++) {
                if (arr[i]->arrtime > arr[j]->arrtime) {
                    Process* temp = arr[j];
                    arr[j] = arr[i];
                    arr[i] = temp;
                }
            }
        }
    }
    else if (mode == 1 && mode2 == 3) {
        for (int i = 0;i < size - 1;i++) {
            for (int j = i + 1;j < size;j++) {
                Process* one = arr[i];
                Process* two = arr[j];
                if (one->ibur_rem[one->io_index] < two->ibur_rem[two->io_index]) {
                    Process* temp = arr[j];
                    arr[j] = arr[i];
                    arr[i] = temp;
                }
            }
        }
    }
    else if (mode == 0 && mode2 == 3) {
        for (int i = 0;i < size - 1;i++) {
            for (int j = i + 1;j < size;j++) {
                Process* one = arr[i];
                Process* two = arr[j];
                if (one->ibur_rem[one->io_index] > two->ibur_rem[two->io_index]) {
                    Process* temp = arr[j];
                    arr[j] = arr[i];
                    arr[i] = temp;
                }
            }
        }
    }
    else if (mode == 1 && mode2 == 4) {
        for (int i = 0;i < size - 1;i++) {
            for (int j = i + 1;j < size;j++) {
                Process* one = arr[i];
                Process* two = arr[j];
                if (-one->arrtime + (one->burtime - one->extime) < -two->arrtime + (two->burtime - two->extime)) {
                    Process* temp = arr[j];
                    arr[j] = arr[i];
                    arr[i] = temp;
                }
            }
        }
    }
    else if (mode == 0 && mode2 == 4) {
        for (int i = 0;i < size - 1;i++) {
            for (int j = i + 1;j < size;j++) {
                Process* one = arr[i];
                Process* two = arr[j];
                if (-one->arrtime + (one->burtime - one->extime) > -two->arrtime + (two->burtime - two->extime)) {
                    Process* temp = arr[j];
                    arr[j] = arr[i];
                    arr[i] = temp;
                }
            }
        }
    }
    printf("if there is no message about error. sort is completed\n");
}
void Processcr(queue* list) {
    int temp;
    srand(time(NULL));
    do {
        printf("how many processes you want to make? (larger or same with 1)\n");
        scanf("%d", &temp);
    } while (temp < 1);
    for (int m = 0;m < temp;m++) {
        Process* new = (Process*)malloc(sizeof(Process));
        new->pid = rpid++;
        new->arrtime = rand() % 30;
        new->burtime = rand() % 30 + 6;
        int iotimes = rand() % 5 + 1;
        int max = 0;
        int last = 0;
        for (int j = 0;j < iotimes;j++) {
            do { max = rand() % ((int)(new->burtime / (iotimes)) + 1) + last; } while ((max >= new->burtime - iotimes + j + 1) || (last >= max));
            last = max;
            new->iortime[j] = max;
            new->iburtime[j] = rand() % 10 + 1;
            new->ibur_rem[j] = new->iburtime[j];
        }
        for (int z = iotimes;z < 5;z++) {
            new->iortime[z] = 0;
            new->iburtime[z] = 0;
            new->ibur_rem[z] = 0;

        }
        new->prior = rand() % 6;
        new->extime = 0;
        new->io_index = 0;
        new->completed = 0;
        enqueue(list, new);
    }
}

void Processmk(queue* list) {
    Process* new = (Process*)malloc(sizeof(Process));
    new->pid = rpid++;
    srand(time(NULL));
    int temp;
    do {
        printf("insert the arrival time: ");
        scanf("%d", &temp);
    } while (temp < 0);
    new->arrtime = temp;
    do {
        printf("insert the cpu burst time: (larger than 6) ");
        scanf("%d", &temp);
    } while (temp <= 6);
    new->burtime = temp;
    int iotimes = rand() % 5 + 1;
    do {
        printf("insert the io request time : (1~5) ");
        scanf("%d", &temp);
    } while (temp < 1 || temp >= 6);
    int max = 0;
    int last = 0;
    for (int j = 0;j < temp;j++) {
        do { max = rand() % ((int)(new->burtime / (iotimes)) + 1) + last; } while ((max >= new->burtime - iotimes + j + 1) || (last >= max));
        last = max;
        new->iortime[j] = max;
        new->iburtime[j] = rand() % 10 + 1;
        new->ibur_rem[j] = new->iburtime[j];
        printf("%d\n", new->iortime[j]);
    }

    for (int z = temp;z < 5;z++) {
        new->iortime[z] = 0;
        new->iburtime[z] = 0;
        new->ibur_rem[z] = 0;

    }
    new->prior = rand() % 6;
    new->extime = 0;
    new->io_index = 0;
    new->completed = 0;
    printf("you create process %d\n", new->pid);
    printf("arrival time : %d , burst time : %d\n", new->arrtime, new->burtime);
    for (int i = 0;i < temp;i++) {
        printf("iorequest time : %d , ioburst time : %d\n", new->iortime[i], new->iburtime[i]);
    }
    enqueue(list, new);
}

void pchart(int time, queue* list) {
    int i = 0;
    turntime = 0;
    waittime = 0;
    int last;
    int lasttime;
    int j;
    while (i <= time) {
        last = cpu[i];
        lasttime = i;
        for (j = i + 1;j <= time;j++) {
            if (cpu[j] != last) break;
        }
        if (last == -1) printf("idle %d ~ %d\n", i, j);
        else printf("process %d %d ~ %d\n", last, i, j);

        i = j;
    }
    for (int k = 0; k < list->rear;k++) {
        int check1 = list->qlist[k]->pid;
        int last1 = 0;
        int lastcheck = list->qlist[k]->arrtime;
        turntime += list->qlist[k]->burtime;
        int laststart = 0;
        for (int j = 0;j <= time;j++) {
            if (cpu[j] == check1 && !last1 && j != time) {
                waittime += j - lastcheck;
                last1 = 1;
                laststart = j;
            }
            if (cpu[j] != check1 && last1) {
                last1 = 0;
                lastcheck = j;
            }
        }
    }
    turntime += waittime;
    if (list->rear != list->front) {
        waittime = waittime / (double)(list->rear - list->front);
        turntime = turntime / (double)(list->rear - list->front);
    }
    else {
        waittime = 0;
        turntime = 0;
    }
    printf("process end\n");
}





void pprior(queue* list) {
    Makesort(list, rpid - 1, 0, 2);
    queue* wait = (queue*)malloc(sizeof(queue));
    queue* ready = (queue*)malloc(sizeof(queue));
    initqueue(wait);
    initqueue(ready);
    Process* burst = NULL;

    for (int f1 = 0;f1 < list->rear;f1++) {
        for (int j = 0;j < 5;j++) {
            list->qlist[f1]->ibur_rem[j] = list->qlist[f1]->iburtime[j];

        }
        list->qlist[f1]->extime = 0;
        list->qlist[f1]->completed = 0;
        list->qlist[f1]->io_index = 0;
    }
    printf("preemptive prior start \n");

    int lastindex = 0;
    int time = 0;
    while (1) {
        int check = 0;
        for (int i = 0;i < rpid - 1;i++) {
            Process* test = list->qlist[i];
            if (test->completed == 0) check = 1;
        }
        if (check == 0) {
            pchart(time - 1, list);
            break;
        }

        for (int i = lastindex;i < rpid - 1;i++) {
            if (list->qlist[i]->arrtime <= time) {
                enqueue(ready, list->qlist[i]);
                lastindex++;
            }
            else break;
        }

        if (!isEmpty(wait)) {
            Makesort(wait->qlist + wait->front, wait->rear - wait->front, 0, 3);

            for (int i = wait->front;i < wait->rear;i++) {
                wait->qlist[i]->ibur_rem[wait->qlist[i]->io_index]--;
                if (wait->qlist[i]->ibur_rem[wait->qlist[i]->io_index] == 0) {
                    printf("waiting queue completed, pid:%d\n", wait->qlist[i]->pid);
                    wait->qlist[i]->io_index++;
                    enqueue(ready, dequeue(wait));
                }

            }
        }
        else printf("no process in waiting queue\n");

        if (burst == NULL) {
            if (!isEmpty(ready)) {
                Makesort(ready->qlist + ready->front, ready->rear - ready->front, 0, 0);
                burst = dequeue(ready);
                cpu[time] = burst->pid;
                printf("debug44");

                burst->extime++;

                if (burst->extime == burst->burtime) {
                    burst->completed = 1;
                    burst = NULL;
                }


                else if (burst->extime == burst->iortime[burst->io_index]) {
                    enqueue(wait, burst);
                    printf("debug33");

                    burst = NULL;
                }
            }
            else {
                cpu[time] = -1;
            }
        }
        else {
            if (!isEmpty(ready)) {
                Makesort(ready->qlist + ready->front, ready->rear - ready->front, 0, 0);
                Process* temp2 = dequeue(ready);
                if (burst->prior > temp2->prior) {
                    enqueue(ready, burst);
                    printf("stolen");
                    burst = temp2;
                }
                else {
                    enqueue(ready, temp2);
                }
                cpu[time] = burst->pid;
                burst->extime++;
                if (burst->extime == burst->burtime) {
                    printf("debug11");
                    burst->completed = 1;
                    burst = NULL;
                }

                else if (burst->extime == burst->iortime[(burst->io_index)]) {
                    enqueue(wait, burst);
                    printf("debug22");
                    burst = NULL;
                }
            }
            else {
                cpu[time] = burst->pid;
                burst->extime++;
                if (burst->extime == burst->burtime) {
                    printf("debug11");
                    burst->completed = 1;
                    burst = NULL;
                }

                else if (burst->extime == burst->iortime[(burst->io_index)]) {
                    enqueue(wait, burst);
                    printf("debug22");
                    burst = NULL;
                }
            }
        }
        time++;

    }
    printf("preemptive sjf completed\n");
}



void psjf(queue* list) {
    Makesort(list, rpid - 1, 0, 2);
    queue* wait = (queue*)malloc(sizeof(queue));
    queue* ready = (queue*)malloc(sizeof(queue));
    initqueue(wait);
    initqueue(ready);
    Process* burst = NULL;

    for (int f1 = 0;f1 < list->rear;f1++) {
        for (int j = 0;j < 5;j++) {
            list->qlist[f1]->ibur_rem[j] = list->qlist[f1]->iburtime[j];

        }
        list->qlist[f1]->extime = 0;
        list->qlist[f1]->completed = 0;
        list->qlist[f1]->io_index = 0;
    }
    printf("preemptive sjf start \n");

    int lastindex = 0;
    int time = 0;
    while (1) {
        int check = 0;
        for (int i = 0;i < rpid - 1;i++) {
            Process* test = list->qlist[i];
            if (test->completed == 0) check = 1;
        }
        if (check == 0) {
            pchart(time - 1, list);
            break;
        }

        for (int i = lastindex;i < rpid - 1;i++) {
            if (list->qlist[i]->arrtime <= time) {
                enqueue(ready, list->qlist[i]);
                lastindex++;
            }
            else break;
        }

        if (!isEmpty(wait)) {
            Makesort(wait->qlist + wait->front, wait->rear - wait->front, 0, 3);

            for (int i = wait->front;i < wait->rear;i++) {
                wait->qlist[i]->ibur_rem[wait->qlist[i]->io_index]--;
                if (wait->qlist[i]->ibur_rem[wait->qlist[i]->io_index] == 0) {
                    printf("waiting queue completed, pid:%d\n", wait->qlist[i]->pid);
                    wait->qlist[i]->io_index++;
                    enqueue(ready, dequeue(wait));
                }

            }
        }
        else printf("no process in waiting queue\n");

        if (burst == NULL) {
            if (!isEmpty(ready)) {
                Makesort(ready->qlist + ready->front, ready->rear - ready->front, 0, 1);
                burst = dequeue(ready);
                cpu[time] = burst->pid;
                printf("debug44");

                burst->extime++;

                if (burst->extime == burst->burtime) {
                    burst->completed = 1;
                    burst = NULL;
                }


                else if (burst->extime == burst->iortime[burst->io_index]) {
                    enqueue(wait, burst);
                    printf("debug33");

                    burst = NULL;
                }
            }
            else {
                cpu[time] = -1;
            }
        }
        else {
            if (!isEmpty(ready)) {
                Makesort(ready->qlist + ready->front, ready->rear - ready->front, 0, 1);
                Process* temp2 = dequeue(ready);
                if (burst->burtime - burst->extime > temp2->burtime - temp2->extime) {
                    enqueue(ready, burst);
                    printf("stolen");
                    burst = temp2;
                }
                else {
                    enqueue(ready, temp2);
                }
                cpu[time] = burst->pid;
                burst->extime++;
                if (burst->extime == burst->burtime) {
                    printf("debug11");
                    burst->completed = 1;
                    burst = NULL;
                }

                else if (burst->extime == burst->iortime[(burst->io_index)]) {
                    enqueue(wait, burst);
                    printf("debug22");
                    burst = NULL;
                }
            }
            else {
                cpu[time] = burst->pid;
                burst->extime++;
                if (burst->extime == burst->burtime) {
                    printf("debug11");
                    burst->completed = 1;
                    burst = NULL;
                }

                else if (burst->extime == burst->iortime[(burst->io_index)]) {
                    enqueue(wait, burst);
                    printf("debug22");
                    burst = NULL;
                }
            }
        }
        time++;

    }
    printf("preemptive sjf completed\n");
}




void hrrn(queue* list) {
    Makesort(list, rpid - 1, 0, 2);
    queue* wait = (queue*)malloc(sizeof(queue));
    queue* ready = (queue*)malloc(sizeof(queue));
    initqueue(wait);
    initqueue(ready);
    Process* burst = NULL;

    for (int f1 = 0;f1 < list->rear;f1++) {
        for (int j = 0;j < 5;j++) {
            list->qlist[f1]->ibur_rem[j] = list->qlist[f1]->iburtime[j];

        }
        list->qlist[f1]->extime = 0;
        list->qlist[f1]->completed = 0;
        list->qlist[f1]->io_index = 0;
    }
    printf("hrrn start \n");

    int lastindex = 0;
    int time = 0;
    while (1) {
        int check = 0;
        for (int i = 0;i < rpid - 1;i++) {
            Process* test = list->qlist[i];
            if (test->completed == 0) check = 1;
        }
        if (check == 0) {
            pchart(time - 1, list);
            break;
        }

        for (int i = lastindex;i < rpid - 1;i++) {
            if (list->qlist[i]->arrtime <= time) {
                enqueue(ready, list->qlist[i]);
                lastindex++;
            }
            else break;
        }

        if (!isEmpty(wait)) {
            Makesort(wait->qlist + wait->front, wait->rear - wait->front, 0, 3);


            for (int i = wait->front;i < wait->rear;i++) {
                wait->qlist[i]->ibur_rem[wait->qlist[i]->io_index]--;
                printf("waiting queue uncompleted, pid:%d lefttime:%d\n", wait->qlist[i]->pid, wait->qlist[i]->ibur_rem[wait->qlist[i]->io_index]);

                if (wait->qlist[i]->ibur_rem[wait->qlist[i]->io_index] <= 0) {
                    printf("waiting queue completed, pid:%d\n", wait->qlist[i]->pid);
                    (wait->qlist[i]->io_index)++;
                    enqueue(ready, dequeue(wait));
                }

            }
        }
        else printf("no process in waiting queue\n");

        if (burst == NULL) {
            if (!isEmpty(ready)) {
                Makesort2(ready->qlist + ready->front, ready->rear - ready->front, 0, 0, time);
                burst = dequeue(ready);
                cpu[time] = burst->pid;
                printf("debug44");

                burst->extime++;

                if (burst->extime == burst->burtime) {
                    burst->completed = 1;
                    burst = NULL;
                }


                else if (burst->extime == burst->iortime[burst->io_index]) {
                    enqueue(wait, burst);
                    printf("debug33");

                    burst = NULL;
                }
            }
            else {
                cpu[time] = -1;
            }
        }
        else {
            cpu[time] = burst->pid;
            burst->extime++;
            if (burst->extime == burst->burtime) {
                printf("debug11");
                burst->completed = 1;
                burst = NULL;
            }

            else if (burst->extime == burst->iortime[(burst->io_index)]) {
                enqueue(wait, burst);
                printf("debug22");
                burst = NULL;
            }
        }
        time++;
    }
    printf("hrrn completed\n");
}


void prior(queue* list) {
    Makesort(list, rpid - 1, 0, 2);
    queue* wait = (queue*)malloc(sizeof(queue));
    queue* ready = (queue*)malloc(sizeof(queue));
    initqueue(wait);
    initqueue(ready);
    Process* burst = NULL;

    for (int f1 = 0;f1 < list->rear;f1++) {
        for (int j = 0;j < 5;j++) {
            list->qlist[f1]->ibur_rem[j] = list->qlist[f1]->iburtime[j];

        }
        list->qlist[f1]->extime = 0;
        list->qlist[f1]->completed = 0;
        list->qlist[f1]->io_index = 0;
    }
    printf("prior start \n");

    int lastindex = 0;
    int time = 0;
    while (1) {
        int check = 0;
        for (int i = 0;i < rpid - 1;i++) {
            Process* test = list->qlist[i];
            if (test->completed == 0) check = 1;
        }
        if (check == 0) {
            pchart(time - 1, list);
            break;
        }

        for (int i = lastindex;i < rpid - 1;i++) {
            if (list->qlist[i]->arrtime <= time) {
                enqueue(ready, list->qlist[i]);
                lastindex++;
            }
            else break;
        }

        if (!isEmpty(wait)) {
            Makesort(wait->qlist + wait->front, wait->rear - wait->front, 0, 3);


            for (int i = wait->front;i < wait->rear;i++) {
                wait->qlist[i]->ibur_rem[wait->qlist[i]->io_index]--;
                printf("waiting queue uncompleted, pid:%d lefttime:%d\n", wait->qlist[i]->pid, wait->qlist[i]->ibur_rem[wait->qlist[i]->io_index]);

                if (wait->qlist[i]->ibur_rem[wait->qlist[i]->io_index] <= 0) {
                    printf("waiting queue completed, pid:%d\n", wait->qlist[i]->pid);
                    (wait->qlist[i]->io_index)++;
                    enqueue(ready, dequeue(wait));
                }

            }
        }
        else printf("no process in waiting queue\n");

        if (burst == NULL) {
            if (!isEmpty(ready)) {
                Makesort(ready->qlist + ready->front, ready->rear - ready->front, 0, 0);
                burst = dequeue(ready);
                cpu[time] = burst->pid;
                printf("debug44");

                burst->extime++;

                if (burst->extime == burst->burtime) {
                    burst->completed = 1;
                    burst = NULL;
                }


                else if (burst->extime == burst->iortime[burst->io_index]) {
                    enqueue(wait, burst);
                    printf("debug33");

                    burst = NULL;
                }
            }
            else {
                cpu[time] = -1;
            }
        }
        else {
            cpu[time] = burst->pid;
            burst->extime++;
            if (burst->extime == burst->burtime) {
                printf("debug11");
                burst->completed = 1;
                burst = NULL;
            }

            else if (burst->extime == burst->iortime[(burst->io_index)]) {
                enqueue(wait, burst);
                printf("debug22");
                burst = NULL;
            }
        }
        time++;
    }
    printf("prior completed\n");
}


void sjf(queue* list) {
    Makesort(list, rpid - 1, 0, 2);
    queue* wait = (queue*)malloc(sizeof(queue));
    queue* ready = (queue*)malloc(sizeof(queue));
    initqueue(wait);
    initqueue(ready);
    Process* burst = NULL;

    for (int f1 = 0;f1 < list->rear;f1++) {
        for (int j = 0;j < 5;j++) {
            list->qlist[f1]->ibur_rem[j] = list->qlist[f1]->iburtime[j];

        }
        list->qlist[f1]->extime = 0;
        list->qlist[f1]->completed = 0;
        list->qlist[f1]->io_index = 0;
    }
    printf("sjf start \n");

    int lastindex = 0;
    int time = 0;
    while (1) {
        int check = 0;
        for (int i = 0;i < rpid - 1;i++) {
            Process* test = list->qlist[i];
            if (test->completed == 0) check = 1;
        }
        if (check == 0) {
            pchart(time - 1, list);
            break;
        }

        for (int i = lastindex;i < rpid - 1;i++) {
            if (list->qlist[i]->arrtime <= time) {
                enqueue(ready, list->qlist[i]);
                lastindex++;
            }
            else break;
        }

        if (!isEmpty(wait)) {
            Makesort(wait->qlist + wait->front, wait->rear - wait->front, 0, 3);

            for (int i = wait->front;i < wait->rear;i++) {
                wait->qlist[i]->ibur_rem[wait->qlist[i]->io_index]--;
                if (wait->qlist[i]->ibur_rem[wait->qlist[i]->io_index] == 0) {
                    printf("waiting queue completed, pid:%d\n", wait->qlist[i]->pid);
                    wait->qlist[i]->io_index++;
                    enqueue(ready, dequeue(wait));
                }

            }
        }
        else printf("no process in waiting queue\n");

        if (burst == NULL) {
            if (!isEmpty(ready)) {
                Makesort(ready->qlist + ready->front, ready->rear - ready->front, 0, 1);
                burst = dequeue(ready);
                cpu[time] = burst->pid;
                printf("debug44");

                burst->extime++;

                if (burst->extime == burst->burtime) {
                    burst->completed = 1;
                    burst = NULL;
                }


                else if (burst->extime == burst->iortime[burst->io_index]) {
                    enqueue(wait, burst);
                    printf("debug33");

                    burst = NULL;
                }
            }
            else {
                cpu[time] = -1;
            }
        }
        else {
            cpu[time] = burst->pid;
            burst->extime++;
            if (burst->extime == burst->burtime) {
                printf("debug11");
                burst->completed = 1;
                burst = NULL;
            }

            else if (burst->extime == burst->iortime[(burst->io_index)]) {
                enqueue(wait, burst);
                printf("debug22");
                burst = NULL;
            }
        }
        time++;


    }
    printf("sjf completed\n");
}



void roundrobin(queue* list) {
    Makesort(list, rpid - 1, 0, 2);
    queue* wait = (queue*)malloc(sizeof(queue));
    queue* ready = (queue*)malloc(sizeof(queue));
    initqueue(wait);
    initqueue(ready);
    Process* burst = NULL;

    int insert;
    int ctime = 0;

    do {
        printf("insert the time quantum of round robin");
        scanf("%d", &insert);
    } while (insert <= 0);

    for (int f1 = 0;f1 < list->rear;f1++) {
        for (int j = 0;j < 5;j++) {
            list->qlist[f1]->ibur_rem[j] = list->qlist[f1]->iburtime[j];

        }
        list->qlist[f1]->extime = 0;
        list->qlist[f1]->completed = 0;
        list->qlist[f1]->io_index = 0;
    }
    printf("round robin start \n");

    int lastindex = 0;
    int time = 0;
    while (1) {
        int check = 0;
        for (int i = 0;i < rpid - 1;i++) {
            Process* test = list->qlist[i];
            if (test->completed == 0) check = 1;
        }
        if (check == 0) {
            pchart(time - 1, list);
            break;
        }

        for (int i = lastindex;i < rpid - 1;i++) {
            if (list->qlist[i]->arrtime <= time) {
                enqueue(ready, list->qlist[i]);
                lastindex++;
            }
            else break;
        }

        if (!isEmpty(wait)) {
            Makesort(wait->qlist + wait->front, wait->rear - wait->front, 0, 3);

            for (int i = wait->front;i < wait->rear;i++) {
                wait->qlist[i]->ibur_rem[wait->qlist[i]->io_index]--;
                if (wait->qlist[i]->ibur_rem[wait->qlist[i]->io_index] == 0) {
                    printf("waiting queue completed, pid:%d\n", wait->qlist[i]->pid);
                    wait->qlist[i]->io_index++;
                    enqueue(ready, dequeue(wait));
                }

            }
        }
        else printf("no process in waiting queue\n");

        if (burst == NULL) {
            if (!isEmpty(ready)) {
                burst = dequeue(ready);
                cpu[time] = burst->pid;
                printf("debug44");

                burst->extime++;
                ctime++;
                if (burst->extime == burst->burtime) {
                    burst->completed = 1;
                    ctime = 0;
                    burst = NULL;
                }
                else if (ctime == insert) {
                    enqueue(ready, burst);
                    ctime = 0;
                    burst = NULL;
                }

                else if (burst->extime == burst->iortime[burst->io_index]) {
                    enqueue(wait, burst);
                    printf("debug33");
                    ctime = 0;
                    burst = NULL;
                }
            }
            else {
                cpu[time] = -1;
            }
        }
        else {
            cpu[time] = burst->pid;
            burst->extime++;
            ctime++;
            if (burst->extime == burst->burtime) {
                printf("debug11");
                burst->completed = 1;
                burst = NULL;
                ctime = 0;
            }
            else if (ctime == insert) {
                enqueue(ready, burst);
                burst = NULL;
                ctime = 0;
            }
            else if (burst->extime == burst->iortime[(burst->io_index)]) {
                enqueue(wait, burst);
                printf("debug22");
                burst = NULL;
                ctime = 0;
            }
        }
        time++;


    }
    printf("round robin completed\n");
}




void fcfs(queue* list) {
    Makesort(list, rpid - 1, 0, 2);
    queue* wait = (queue*)malloc(sizeof(queue));
    queue* ready = (queue*)malloc(sizeof(queue));
    initqueue(wait);
    initqueue(ready);
    Process* burst = NULL;
    for (int f1 = 0;f1 < list->rear;f1++) {
        for (int j = 0;j < 5;j++) {
            list->qlist[f1]->ibur_rem[j] = list->qlist[f1]->iburtime[j];

        }
        list->qlist[f1]->extime = 0;
        list->qlist[f1]->completed = 0;
        list->qlist[f1]->io_index = 0;
    }
    printf("fcfs start \n");

    int lastindex = 0;
    int time = 0;
    while (1) {
        int check = 0;
        for (int i = 0;i < rpid - 1;i++) {
            Process* test = list->qlist[i];
            if (test->completed == 0) check = 1;
        }
        if (check == 0) {
            pchart(time - 1, list);
            break;
        }

        for (int i = lastindex;i < rpid - 1;i++) {
            if (list->qlist[i]->arrtime <= time) {
                enqueue(ready, list->qlist[i]);
                lastindex++;
            }
            else break;
        }

        if (!isEmpty(wait)) {
            Makesort(wait->qlist + wait->front, wait->rear - wait->front, 0, 3);

            for (int i = wait->front;i < wait->rear;i++) {
                wait->qlist[i]->ibur_rem[wait->qlist[i]->io_index]--;
                if (wait->qlist[i]->ibur_rem[wait->qlist[i]->io_index] == 0) {
                    printf("waiting queue completed, pid:%d\n", wait->qlist[i]->pid);
                    wait->qlist[i]->io_index++;
                    enqueue(ready, dequeue(wait));
                }

            }
        }
        else printf("no process in waiting queue\n");

        if (burst == NULL) {
            if (!isEmpty(ready)) {
                burst = dequeue(ready);
                cpu[time] = burst->pid;
                printf("debug44");

                burst->extime++;
                if (burst->extime == burst->burtime) {
                    burst->completed = 1;
                    burst = NULL;
                }
                else if (burst->extime == burst->iortime[burst->io_index]) {
                    enqueue(wait, burst);
                    printf("debug33");

                    burst = NULL;
                }
            }
            else {
                cpu[time] = -1;
            }
        }
        else {
            cpu[time] = burst->pid;
            burst->extime++;
            if (burst->extime == burst->burtime) {
                printf("debug11");
                burst->completed = 1;
                burst = NULL;
            }
            else if (burst->extime == burst->iortime[(burst->io_index)]) {
                enqueue(wait, burst);
                printf("debug22");
                burst = NULL;
            }
        }
        time++;


    }
    printf("fcfs completed\n");
}


void multiprocess(queue* list) {
    if (list->rear - list->front <= 1) {
        printf("processes should be 2 or more than 2\n");
        return -1;
    }
    Process* cpus[10][10];
    int temp;
    do {
        printf("you can insert the number of processor (1~10)\n");
        scanf("%d", &temp);
    } while (temp < 1 || temp>10);
    int temp1;

    do {
        printf("you can insert the rate of sequence processes(1~10(int) 10 is total percentage)\n");
        printf("high value makes high percentage of sequence process\n");
        scanf("%d", &temp1);
    } while (temp1 < 1 || temp1>10);

    int size = list->rear - list->front;
    int seqs = size * temp1 / (10);
    int pars = size - seqs;

    for (int i = 0;i < 10;i++) {
        for (int j = 0;j < 10;j++) {
            cpus[i][j] = NULL;
        }
    }


    Makesort(list, rpid - 1, 0, 2);
    queue* wait = (queue*)malloc(sizeof(queue));
    queue* ready = (queue*)malloc(sizeof(queue));
    initqueue(wait);
    initqueue(ready);
    for (int f1 = 0;f1 < list->rear;f1++) {
        for (int j = 0;j < 5;j++) {
            list->qlist[f1]->ibur_rem[j] = list->qlist[f1]->iburtime[j];

        }
        list->qlist[f1]->extime = 0;
        list->qlist[f1]->completed = 0;
        list->qlist[f1]->io_index = 0;
    }
    printf("multiprocess start \n");

    int lastindex = seqs;
    int time = 0;
    int check1 = 0;
    while (1) {
        int check = 0;
        printf("isempty wait: %d isempty ready: %d\n", isEmpty(wait), isEmpty(ready));

        for (int i = 0;i < rpid - 1;i++) {
            Process* test = list->qlist[i];
            printf("remain time %d\n", test->burtime - test->extime);
            if (test->completed == 0) check = 1;
        }
        if (check == 0) {

            break;
        }
        printf("complete: %d", list->qlist[0]->completed);
        for (int i = 0;i < seqs;i++) {
            if (list->qlist[i]->arrtime <= time && i == 0 && check1 == 0) {
                enqueue(ready, list->qlist[i]);
                check1 = 1;
            }
            if (list->qlist[i]->arrtime <= time && i != 0 && list->qlist[i - 1]->completed && check1 == i) {
                enqueue(ready, list->qlist[i]);
                check1 = i + 1;
            }

        }

        for (int i = lastindex;i < rpid - 1;i++) {
            if (list->qlist[i]->arrtime <= time) {
                enqueue(ready, list->qlist[i]);
                lastindex++;
            }
            else break;
        }

        if (!isEmpty(wait)) {
            Makesort(wait->qlist + wait->front, wait->rear - wait->front, 0, 3);

            for (int i = wait->front;i < wait->rear;i++) {
                wait->qlist[i]->ibur_rem[wait->qlist[i]->io_index]--;
                if (wait->qlist[i]->ibur_rem[wait->qlist[i]->io_index] == 0) {
                    printf("waiting queue completed, pid:%d\n", wait->qlist[i]->pid);
                    wait->qlist[i]->io_index++;
                    enqueue(ready, dequeue(wait));
                }

            }
        }
        else printf("no process in waiting queue\n");

        for (int cpu1 = 0;cpu1 < 10;cpu1++) {
            if (cpus[cpu1][0] != NULL) {
                cpus[cpu1][0]->extime++;
                if (cpus[cpu1][0]->extime == cpus[cpu1][0]->iortime[cpus[cpu1][0]->io_index]) {
                    enqueue(wait, cpus[cpu1][0]);
                    for (int cpu3 = 1;cpu3 <= 9;cpu3++) {
                        cpus[cpu1][cpu3 - 1] = cpus[cpu1][cpu3];
                    }
                    cpus[cpu1][9] = NULL;
                }
                else if (cpus[cpu1][0]->extime >= cpus[cpu1][0]->burtime) {
                    cpus[cpu1][0]->completed = 1;
                    for (int cpu3 = 1;cpu3 <= 9;cpu3++) {
                        cpus[cpu1][cpu3 - 1] = cpus[cpu1][cpu3];
                    }
                    cpus[cpu1][9] = NULL;
                }
            }
            else {
                if (!isEmpty(ready)) {
                    if (cpus[cpu1][0] == NULL) {
                        cpus[cpu1][0] = dequeue(ready);
                        cpus[cpu1][0]->extime++;
                        if (cpus[cpu1][0]->extime == cpus[cpu1][0]->iortime[cpus[cpu1][0]->io_index]) {
                            enqueue(wait, cpus[cpu1][0]);
                            for (int cpu3 = 1;cpu3 <= 9;cpu3++) {
                                cpus[cpu1][cpu3 - 1] = cpus[cpu1][cpu3];
                            }
                            cpus[cpu1][9] = NULL;
                        }
                        else if (cpus[cpu1][0]->extime >= cpus[cpu1][0]->burtime) {
                            cpus[cpu1][0]->completed = 1;
                            for (int cpu3 = 1;cpu3 <= 9;cpu3++) {
                                cpus[cpu1][cpu3 - 1] = cpus[cpu1][cpu3];
                            }
                            cpus[cpu1][9] = NULL;
                        }
                    }
                }
            }
            if (cpu1 == 9 && !isEmpty(ready)) {
                while (!isEmpty(ready)) {
                    int max = 9;
                    int maxcpu = -1;
                    for (int t1 = 0;t1 < 10;t1++) {
                        for (int t2 = 0;t2 < 10;t2++) {
                            if (cpus[t1][t2] == NULL) {
                                if (t2 < max) {
                                    maxcpu = t1;
                                    max = t2;
                                    break;
                                }
                            }
                        }
                    }
                    if (maxcpu == -1) break;
                    else {
                        cpus[maxcpu][max] = dequeue(ready);
                    }
                }

            }
        }
        time++;

    }
    printf("time : %d", time - 1);
}

void agingsjf(queue* list) {
    Makesort(list, rpid - 1, 0, 2);
    queue* wait = (queue*)malloc(sizeof(queue));
    queue* ready = (queue*)malloc(sizeof(queue));
    initqueue(wait);
    initqueue(ready);
    Process* burst = NULL;

    for (int f1 = 0;f1 < list->rear;f1++) {
        for (int j = 0;j < 5;j++) {
            list->qlist[f1]->ibur_rem[j] = list->qlist[f1]->iburtime[j];

        }
        list->qlist[f1]->extime = 0;
        list->qlist[f1]->completed = 0;
        list->qlist[f1]->io_index = 0;
    }
    printf("aging preemptive sjf start \n");

    int lastindex = 0;
    int time = 0;
    while (1) {
        int check = 0;
        for (int i = 0;i < rpid - 1;i++) {
            Process* test = list->qlist[i];
            if (test->completed == 0) check = 1;
        }
        if (check == 0) {
            pchart(time - 1, list);
            break;
        }

        for (int i = lastindex;i < rpid - 1;i++) {
            if (list->qlist[i]->arrtime <= time) {
                enqueue(ready, list->qlist[i]);
                lastindex++;
            }
            else break;
        }

        if (!isEmpty(wait)) {
            Makesort(wait->qlist + wait->front, wait->rear - wait->front, 0, 3);

            for (int i = wait->front;i < wait->rear;i++) {
                wait->qlist[i]->ibur_rem[wait->qlist[i]->io_index]--;
                if (wait->qlist[i]->ibur_rem[wait->qlist[i]->io_index] == 0) {
                    printf("waiting queue completed, pid:%d\n", wait->qlist[i]->pid);
                    wait->qlist[i]->io_index++;
                    enqueue(ready, dequeue(wait));
                }

            }
        }
        else printf("no process in waiting queue\n");

        if (burst == NULL) {
            if (!isEmpty(ready)) {
                Makesort(ready->qlist + ready->front, ready->rear - ready->front, 0, 4);
                burst = dequeue(ready);
                cpu[time] = burst->pid;
                printf("debug44");

                burst->extime++;

                if (burst->extime == burst->burtime) {
                    burst->completed = 1;
                    burst = NULL;
                }


                else if (burst->extime == burst->iortime[burst->io_index]) {
                    enqueue(wait, burst);
                    printf("debug33");

                    burst = NULL;
                }
            }
            else {
                cpu[time] = -1;
            }
        }
        else {
            if (!isEmpty(ready)) {
                Makesort(ready->qlist + ready->front, ready->rear - ready->front, 0, 4);
                Process* temp2 = dequeue(ready);
                if (burst->burtime - burst->extime > temp2->burtime - temp2->extime) {
                    enqueue(ready, burst);
                    printf("stolen");
                    burst = temp2;
                }
                else {
                    enqueue(ready, temp2);
                }
                cpu[time] = burst->pid;
                burst->extime++;
                if (burst->extime == burst->burtime) {
                    printf("debug11");
                    burst->completed = 1;
                    burst = NULL;
                }

                else if (burst->extime == burst->iortime[(burst->io_index)]) {
                    enqueue(wait, burst);
                    printf("debug22");
                    burst = NULL;
                }
            }
            else {
                cpu[time] = burst->pid;
                burst->extime++;
                if (burst->extime == burst->burtime) {
                    printf("debug11");
                    burst->completed = 1;
                    burst = NULL;
                }

                else if (burst->extime == burst->iortime[(burst->io_index)]) {
                    enqueue(wait, burst);
                    printf("debug22");
                    burst = NULL;
                }
            }
        }
        time++;

    }
    printf("preemptive sjf completed\n");
}


void printqueue(queue* list) {
    for (int j = (list->front);j < (list->rear);j++) {
        Process* temp = list->qlist[j];
        printf("pid: %d arrival time : %d burst time: %d prior:%d\n", temp->pid, temp->arrtime, temp->burtime, temp->prior);
        for (int k = 0;k < 5;k++) {
            printf("iortime : %d\n", temp->iortime[k]);
        }
    }
}


void compareall(queue* list) {
    double a[6] = { 0 };
    double b[6] = { 0 };
    for (int i = 0;i <= 5;i++) {
        if (i == 0) {
            fcfs(list);
            a[i] = turntime;
            b[i] = waittime;
        }
        if (i == 1) {
            sjf(list);
            a[i] = turntime;
            b[i] = waittime;
        }
        if (i == 2) {
            psjf(list);
            a[i] = turntime;
            b[i] = waittime;
        }
        if (i == 3) {
            prior(list);
            a[i] = turntime;
            b[i] = waittime;
        }
        if (i == 4) {
            pprior(list);
            a[i] = turntime;
            b[i] = waittime;
        }
        if (i == 5) {
            roundrobin(list);
            a[i] = turntime;
            b[i] = waittime;
        }
    }
    for (int i = 0;i <= 5;i++) {
        if (i == 0) {
            printf("fcfs turnaround time:%.2lf wait time : %.2lf\n", a[i], b[i]);
        }
        if (i == 1) {
            printf("sjf turnaround time:%.2lf wait time : %.2lf\n", a[i], b[i]);

        }
        if (i == 2) {
            printf("preemptive sjf turnaround time:%.2lf wait time : %.2lf\n", a[i], b[i]);

        }
        if (i == 3) {
            printf("prior turnaround time:%.2lf wait time : %.2lf\n", a[i], b[i]);

        }
        if (i == 4) {
            printf("preemptive prior turnaround time:%.2lf wait time : %.2lf\n", a[i], b[i]);

        }
        if (i == 5) {
            printf("round robin turnaround time:%.2lf wait time : %.2lf\n", a[i], b[i]);
        }
    }
}

int main() {
    int temp = 0;
    queue* proclist = (queue*)(malloc(sizeof(queue)));
    initqueue(proclist);
    while (temp != -1) {
        printf("-1:exit\n");
        printf("0:create processes random\n");
        printf("1:create process insert\n");
        printf("2:process list\n");
        printf("3:FCFS\n");
        printf("4:Round Robin\n");
        printf("5.SJF(Non preemptive)\n");
        printf("6.SJF(Preemptive)\n");
        printf("7.prior\n");
        printf("8.prior preemptive\n");
        printf("9.Compare all(evaluation)\n");
        printf("10.Multi process testing\n");
        printf("11.preemptive sjf using aging\n");
        printf("12.hrrn scheduling\n");








        scanf("%d", &temp);
        if (temp == 0) {
            Processcr(proclist);
        }
        if (temp == 1) {
            Processmk(proclist);
        }
        if (temp == 2) {
            printqueue(proclist);
        }
        if (temp == 3) {
            fcfs(proclist);
        }
        if (temp == 4) {
            roundrobin(proclist);
        }
        if (temp == 5) {
            sjf(proclist);
        }
        if (temp == 6) {
            psjf(proclist);
        }
        if (temp == 7) {
            prior(proclist);
        }
        if (temp == 8) {
            pprior(proclist);
        }
        if (temp == 9) {
            compareall(proclist);
        }
        if (temp == 10) {
            multiprocess(proclist);
        }

        if (temp == 11) {
            agingsjf(proclist);
        }
        if (temp == 12) {
            hrrn(proclist);
        }
        if (temp == -1) {
            exit(0);
        }
    }
}

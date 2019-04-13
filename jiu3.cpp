
#include <stdio.h>
#include <time.h>
#include <memory.h>
#include <string.h>
#include <thread>
#include <algorithm>

#define MAXN 10
#define MAXNN 100

#ifdef WIN32
#include <conio.h>
#endif

typedef struct squarelist {
    int     i[MAXNN];
    int     time;                                           // ??????
    int     resultCount, partResultCount, totalCount;       // ?????????????????????
    struct squarelist  *next;
} SQUARENODE;

typedef struct square
{
    int working;                    // ??????????????????
    int scope_start, scope_end;     // ??????????????????????N*N??????????????
    int i[MAXNN];                   // 1~NN
    int remain[MAXNN];              // 1~N
    int totalCount, resultCount, partResultCount;
    int t_start, time;
    int stdsum;
    int rsum[MAXN], csum[MAXN];           // 0~N-1
    SQUARENODE  head, *tail;
    int threshold_max[MAXN], threshold_min[MAXN];
} SQUARE;

void    beginWorker(SQUARE *ps);
int exam(SQUARE *ps);
void msquare(SQUARE *ps, int no);

int     exam(SQUARE *ps);
void    msquare( SQUARE *ps, int no);
void    fileWrite( char *filename, SQUARE *ps, int count) ;
void    initSquare( SQUARE  *ps, int start, int end);

SQUARENODE *sl_append(SQUARE *ps); 
void    sl_free( SQUARENODE *head);
void    calcThreshold( SQUARE *ps );
#include <stdio.h>
#include <time.h>
#include <memory.h>
#include <string.h>
#include <thread>
#include <algorithm>

#define MAXN 10
#define MAXNN 100

#ifdef WIN32
#include <conio.h>
#endif
#include"stdlib.h"

typedef struct squarelist {
    int     i[MAXNN];
    int     time;                                           // ??????
    int     resultCount, partResultCount, totalCount;       // ?????????????????????
    struct squarelist  *next;
} SQUARENODE;

typedef struct square
{
    int working;                    // ??????????????????
    int scope_start, scope_end;     // ??????????????????????N*N??????????????
    int i[MAXNN];                   // 1~NN
    int remain[MAXNN];              // 1~N
    int totalCount, resultCount, partResultCount;
    int t_start, time;
    int stdsum;
    int rsum[MAXN], csum[MAXN];           // 0~N-1
    SQUARENODE  head, *tail;
    int threshold_max[MAXN], threshold_min[MAXN];
} SQUARE;

void    beginWorker(SQUARE *ps);
int exam(SQUARE *ps);
void msquare(SQUARE *ps, int no);

int     exam(SQUARE *ps);
void    msquare( SQUARE *ps, int no);
void    fileWrite( char *filename, SQUARE *ps, int count) ;
void    initSquare( SQUARE  *ps, int start, int end);

SQUARENODE *sl_append(SQUARE *ps); 
void    sl_free( SQUARENODE *head);
void    calcThreshold( SQUARE *ps );



int NN, N;

int main(int argc, char *argv[])
{
    int groupSize, totals, i;
    SQUARE s[MAXNN];
    std::thread t[MAXNN];
    int sumtotalCount, sumpartResultCount, sumresultCount;

    printf("input a number(3-5) and group size(1~16): ");
    scanf("%d,%d", &N, &groupSize);

    NN = N * N;
    if (N <= 0 || N > 9 || groupSize <= 0 || groupSize > NN)
    {
        printf("N: 3~9;  group size: 1~N*N");
        return -1;
    }

    totals = NN / groupSize;
    if (NN % groupSize)
        totals++;

    for (i = 0; i < totals; i++)
    {
        initSquare(&(s[i]), i * groupSize + 1, std::min(i * groupSize + groupSize, NN));
    }

    for (i = 0; i < totals; i++)
    {
        t[i] = std::thread(beginWorker, &(s[i]));
    }

    printf("???????         ??????????  ?????? ???\r\n");

    int finished = false;
    while (!finished)
    {
        sumresultCount = 0;
        sumpartResultCount = 0;
        sumtotalCount = 0;

        for (i = 0; i < totals; i++)
        {
            finished |= s[i].working;
            sumresultCount += s[i].resultCount;
            sumpartResultCount += s[i].partResultCount;
            sumtotalCount += s[i].totalCount;
        }
        finished = !finished;

        printf("%-16d %-12d  %-8d %.1f\r", sumtotalCount, sumpartResultCount, sumresultCount, (clock() - s[0].t_start) / 1000.0);

        _sleep(1000);

        
        {
            break;
        }
        
    }
    printf("\r\n");

    char filename[256];
    sprintf(filename, "%dx%d result.txt", N, N);
    fileWrite(filename, s, totals );

    for(i=0;i<totals;i++)
        sl_free(&(s[i].head));

    getchar();
}

void beginWorker(SQUARE *ps)
{
    ps->working = true;
    ps->t_start = clock();

    int i;

    for (i = ps->scope_start; i <= ps->scope_end; i++)
    {
        ps->i[0] = i;
        ps->remain[i] = 0;
        ps->rsum[0] = i;
        ps->csum[0] = i;
        msquare(ps, 1);
        ps->remain[i] = 1;
    }

    ps->working = false;
}

void msquare(SQUARE *ps, int no)
{
    int i;
    int r, c, rsum, csum, tempcsum, temprsum;

    r = no / N;
    c = no % N;

    rsum = ps->rsum[r];
    csum = ps->csum[c];

    //calcThreshold(ps);

    for (i = 1; i <= NN; i++)
    {
        if (ps->remain[i] == 0)
            continue;

        tempcsum = csum + i;
        temprsum = rsum + i;

        if (temprsum > ps->stdsum || tempcsum > ps->stdsum)
            break;

        if (c == N - 1 && temprsum != ps->stdsum)
            continue;
        if (r == N - 1 && tempcsum != ps->stdsum)
            continue;

        if (c > 1)
        {
            if ((temprsum + ps->threshold_max[c] < ps->stdsum) || temprsum + ps->threshold_min[c] > ps->stdsum)
                continue;
        }
        if (r > 1)
        {
            if ((tempcsum + ps->threshold_max[r] < ps->stdsum) || tempcsum + ps->threshold_min[r] > ps->stdsum)
                continue;
        }
        ps->totalCount++;

        ps->i[no] = i;
        ps->remain[i] = 0;
        ps->rsum[r] = temprsum;
        ps->csum[c] = tempcsum;

        if (no == NN - 1)
        {
            ps->partResultCount++;
            if (exam(ps))
            {
                ps->resultCount++;
                ps->time = clock() - ps->t_start;
                sl_append(ps);
                //print(ps->tail, false);
            }
        }
        else
        {
            msquare(ps, no + 1);
        }
        ps->remain[i] = 1;
        ps->i[no] = 0;
        //calcThreshold(ps);
    }
    ps->rsum[r] = rsum;
    ps->csum[c] = csum;
}
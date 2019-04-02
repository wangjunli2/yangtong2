#pragma warning(disable:4996)

#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <string.h>

#define     MAXNN   100
#define     MAXN    10
int  N;
int  NN;
int  interval;
typedef struct square
{
    int i[MAXNN];
    int remain[MAXNN];
    int totalCount, resultCount, partResultCount;
    int t_start, t_current;
    int stdsum;
    int rsum[MAXN],csum[MAXN];
    int working;
    int threshold_max[MAXN], threshold_min[MAXN];
} SQUARE;

typedef struct squarelist {
    int     i[MAXNN];
    struct squarelist  *next;
} SQUARENODE;

int     exam(SQUARE *ps);
void    print(SQUARE s, int direct);
void    msquare( SQUARE *ps, int no);
void    fileWrite( char *filename, SQUARENODE head) ;
void    initSquare( SQUARE  *ps);
void    beginWorker(SQUARE *ps);

SQUARENODE *sl_append(SQUARE *ps); 
void    sl_free( SQUARENODE head);


SQUARENODE  head, *tail;

int main(int argc, char *argv[])
{
    SQUARE s;

    printf("input a number(3-5)and interval:");
    scanf("%d,%d", &N, &interval);
    
    NN = N*N;

    head.next = NULL;
    tail = &head;

    initSquare(&s);

    //std::thread t1(beginWorker, &s);
    msquare(&s, 0);
    memcpy( s.i, tail->i, NN*sizeof(int));
    print( s, true);

    char    filename[256];
    sprintf(filename, "%dx%d result.txt", N,N);
    fileWrite( filename, head );
    sl_free(head);
    return 0;
}

void    beginWorker(SQUARE *ps) 
{
    ps->working = true;
    msquare( ps, 0);
}

void    initSquare( SQUARE  *ps) 
{

    ps->t_start = clock();
    ps->resultCount = ps->totalCount = ps->partResultCount = 0;

    int     i, temp;

    ps->working = false;
    ps->t_start = clock();
    temp = 0;    
    for(i=1;i<=NN;i++) {
        ps->i[i-1] = 0;
        ps->remain[i] = 1;
        temp += i;
    }
    ps->stdsum = temp / N;
    ps->t_current = 0;

    ps->threshold_max[N-1] = 0;
    temp = NN;
    for(i=1;i<N;i++) {
        ps->threshold_max[N-1-i] = temp+ps->threshold_max[N-1-i+1];
        temp--;
    }

    ps->threshold_min[N-1] = 0;
    temp = 1;
    for(i=1;i<N;i++) {
        ps->threshold_min[N-1-i] = temp+ps->threshold_min[N-1-i+1];
        temp++;
    }
        
}


SQUARENODE *sl_append(SQUARE *ps) 
{
    SQUARENODE  *newnode;

    newnode = new SQUARENODE;

    memcpy( newnode->i, ps->i, NN*sizeof(int) );
    newnode->next = tail->next;
    tail->next = newnode;
    tail = newnode;

    return newnode;
}

void    sl_free( SQUARENODE head)
{
    SQUARENODE  *node;

    while( head.next!=NULL) {
        node = head.next;
        head.next = node->next;
        delete node;
    }
}

void    calcThreshold( SQUARE *ps) 
{
    int     i,count = 0;

    ps->threshold_max[N-1] = 0;
    for(i=NN-1;i>=0;i--) {
        if (ps->remain[i]) {
            count ++;
            ps->threshold_max[N-1-count] = ps->threshold_max[N-1-count -1];
            if (count==N-1)
                break;
        }

    }

}

void    msquare( SQUARE *ps, int no)
{
    int     i;
    int     r, c, rsum, csum, temprsum, tempcsum;

    r = no / N;
    c = no % N;

    if (r==0) {
        ps->csum[c] = 0;
        if (c==0) {
            for(i=0;i<N;i++)
                ps->rsum[i]=0;
        }
    }
    rsum = ps->rsum[r];
    csum = ps->csum[c];

    

    for(i=1;i<=NN;i++) {
        
        tempcsum = csum + i;
        temprsum = rsum + i;
        if (temprsum>ps->stdsum || tempcsum>ps->stdsum)
            break;
		if ( ps->remain[i]==0 )
            continue;
		if ( c>1 ) {
			if ( (rsum+ ps->threshold_max[c-1]<ps->stdsum) || rsum+ps->threshold_min[c-1]>ps->stdsum)
				continue;
			
		}
		if ( r>1 ) {
			if ( (csum+ ps->threshold_max[r-1]<ps->stdsum) || csum+ps->threshold_min[r-1]>ps->stdsum)
				continue;
		}
		

		
	
        if ( c==N-1 && temprsum!=ps->stdsum)
            continue;
        if ( r==N-1 && tempcsum!=ps->stdsum)
            continue;
            
        ps->totalCount ++;

        ps->i[no] = i;
        ps->remain[i] = 0;
        ps->rsum[r] = temprsum;
        ps->csum[c] = tempcsum;
        if (no==NN-1) {
            ps->partResultCount ++;
            if (exam(ps)) {
                ps->resultCount ++;
                ps->t_current = clock()- ps->t_start;
                sl_append( ps );
                print(*ps, false);
            }
        } else
        {
            msquare( ps, no + 1);
        }
        ps->remain[i] = 1;
        ps->i[no] = 0;
    }
    ps->rsum[r] = rsum;
    ps->csum[c] = csum;

}

void fileWrite( char *filename, SQUARENODE head) 
{
    FILE    *fp;
    SQUARENODE *n;
    char    line[4096], temp[1024];
    int     i, serialnum = 1;

    fp=fopen(  filename, "w");
    n = head.next;

    while(n) {
        sprintf( line, "%6d [ ", serialnum++);
        for(i=0;i<NN;i++) {
            sprintf(temp, "%2d ", n->i[i]);
            strcat( line, temp);
        } 
        strcat( line,  "]\n");
        fwrite( line, strlen(line), 1, fp);

        n = n->next;
    }

    fclose(fp);
}

void print(SQUARE s, int direct)
{
    int j;

    if (s.resultCount%interval==0 || (direct) ) {
        printf("(%-6d)\n", s.resultCount);
        for (j = 0; j < NN; j++)
        {
			printf("%2d ", s.i[j]);
			if((j+1)%N==0)
				printf("\n");
		}
        printf("totalCount:(%d)\t\ttime:(%ld)ms\r\n\n\n", s.totalCount ,clock() - s.t_start);
    }
}

// 检查数组i各元素是否满足九宫格要求，如果满足返回非0。
int exam(SQUARE *ps)
{
    int j;
    int sum1, sum2;
    
    // 初始化各求和变量，准备求和检查
    sum1 = 0;
    sum2 = 0;

    // 按行/列、对角线求和
    for (j = 0; j < N; j++)
    {
        sum1 += ps->i[j * N + j];     //M[j][j];
        sum2 += ps->i[j * N + N - 1 - j]; //M[j][2 - j];
    }

    // 检查对角线是否相等
    if (sum1 != ps->stdsum || ps->stdsum != sum2)
        return 0;
    else
        return 1;
}
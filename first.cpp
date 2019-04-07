#include <stdio.h>
#include <conio.h>
#include <time.h>

#define NN      9
#define N       3
#define stdsum  15
typedef struct square {
    int     i[NN];
    int     totalCount, resultCount, partResultCount;
    int     t_start;
} SQUARE;


int     exam( SQUARE *ps );
void    print(SQUARE s );
int     Used( int i[], int n);

int main(int argc, char *argv[])
{
    SQUARE  s;

    s.t_start = clock();
    s.resultCount = s.totalCount = s.partResultCount = 0;

    int     *i;

    i = s.i;
    for (i[0] = 1; i[0] <= NN; i[0]++)
    {
        for (i[1] = 1; i[1] <= NN; i[1]++)
        {
            if ( Used(i, 1) )
                continue;
            for (i[2] = 1; i[2] <= NN; i[2]++)
            {
                if ( Used(i,2) )
                    continue;
                for (i[3] = 1; i[3] <= NN; i[3]++)
                {
                    if ( Used(i,3) )
                        continue;
                    for (i[4] = 1; i[4] <= NN; i[4]++)
                    {
                        if ( Used(i,4) )
                            continue;
                        for (i[5] = 1; i[5] <= NN; i[5]++)
                        {
                            if ( Used(i,5) )
                                continue;
                            for (i[6] = 1; i[6] <= NN; i[6]++)
                            {
                                if ( Used(i,6) )
                                    continue;
                                for (i[7] = 1; i[7] <= NN; i[7]++)
                                {
                                    if ( Used(i,7) )
                                        continue;
                                    for (i[8] = 1; i[8] <= NN; i[8]++)
                                    {
                                        if ( Used(i,8) )
                                            continue;
										/*for(i[9] = 1; i[9] <= NN; i[9]++)
										{
											if ( Used(i,9) )
												continue;
											for(i[10] = 1; i[10] <= NN; i[10]++)
											{
												if ( Used(i,10) )
													continue;
												for(i[11] = 1; i[11] <= NN; i[11]++)
												{
													if ( Used(i,11) )
														continue;
													for(i[12] = 1; i[12] <= NN; i[12]++)
													{
														if ( Used(i,12) )
															continue;
														for(i[13] = 1; i[13] <= NN; i[13]++)
														{
															if ( Used(i,13) )
																continue;
															for(i[14] = 1; i[14] <= NN; i[14]++)
															{
																if ( Used(i,14) )
																	continue;
																for(i[15] = 1; i[15] <= NN; i[15]++)
																{
																	if ( Used(i,15) )
																		continue;*/

																	s.totalCount++;

																	//如果满足则打印排列好的九宫格
																	if ( exam( &s ) )
																	{
																		s.resultCount++;
																		print( s );
																	}
																/*}
															}
														}
													}
												}
											}
										}*/
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

   return 0;
}

int     Used( int i[], int n)
{
    int     j, used = 0;

    for(j=0;j<n;j++) {
        if (i[j]==i[n]) {
            used = 1;
            break;
        }
    }
    return used;
}

void    print( SQUARE s )
{
    int     j;

    printf("(%d)\n", s.resultCount);
    for (j = 0; j < NN; j++)
	{
		printf("%d ", s.i[j]);
		if ( (j+1)%N==0)
			printf("\n");
	}

    printf("\ntotalCount:(%d)\tpartReasultCount:(%d)\ttime:(%.2f)s\r\n", s.totalCount, s.partResultCount, (float)(clock() - s.t_start)/1000);

}


// 检查数组i各元素是否满足九宫格要求，如果满足返回非0。
int     exam( SQUARE *ps )
{
    int     j, k, cflag, n;
    int     sum1, sum2, rsum[N], csum[N];


    // 初始化各求和变量，准备求和检查
    for (j = 0; j < N; j++)
    {
        csum[j] = 0;
        rsum[j] = 0;
    }
    sum1 = 0;
    sum2 = 0;

    // 按行/列、对角线求和
    for (j = 0; j < N; j++)
    {
        for (k = 0; k < N; k++)
        {
            csum[k] += ps->i[j*N+k];     
            rsum[j] += ps->i[j*N+k];     
        }
        sum1 += ps->i[j*N+j];           
        sum2 += ps->i[j*N+N-1-j];         
    }

    


    // 检查对角线是否相等
    if (sum1 != stdsum || stdsum != sum2)
        return 0;

    (ps->partResultCount)++;
    // 检查是否满足各行各列相等
    cflag = 1;
    for (j = 0; j < N; j++)
    {
        if (csum[j] != stdsum || rsum[j] != stdsum)
        {
            cflag = 0;
            break;
        }
    }    
    return cflag;

}
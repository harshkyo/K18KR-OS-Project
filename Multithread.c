#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
int T_Res, T_proces, Executed = 0;
int *res, *safeSeq;
int **aloc, **max_Req, **Req;
pthread_mutex_t ResLock;
pthread_cond_t cond;
bool getSafeSeq()
{
        int tempRes[T_Res];
        for(int i=0; i<T_Res; i++)
	{
		tempRes[i] = res[i];
        }
	bool completed[T_proces];
        for(int i=0; i<T_proces; i++)
	{
		completed[i] = false;
        }
	int ncompleted=0;
        while(ncompleted < T_proces)
	{
                bool safe = false;
                for(int i=0; i<T_proces; i++)
		{
                        if(!completed[i])
			{
                                bool is_safe = true;
                                for(int j=0; j<T_Res; j++)
                                {
				        if(Req[i][j] > tempRes[j])
					{
                                                is_safe = false;
                                                break;
                                        }
                                }
				if(is_safe)
				{
                                        for(int j=0; j<T_Res; j++)
                                        {
					        tempRes[j] += aloc[i][j];
                                        }
					safeSeq[ncompleted] = i;
                                        completed[i] = true;
                                        ++ncompleted;
                                        safe = true;
                                }
                        }
                }
                if(!safe)
		{
                        for(int k=0; k<T_proces; k++)
				safeSeq[k] = -1;
                        return false;
                }
        }
        return true;
}
void* pCode(void *pcode)
{
        int a = *((int *) pcode);
        pthread_mutex_lock(&ResLock);
        while(a != safeSeq[Executed])
        {
	        pthread_cond_wait(&cond, &ResLock);
	}
        printf("\nProcess %d", a+1);
        printf("\nResource Allocated : ");
        for(int i=0; i<T_Res; i++)
        {
	        printf("%3d", aloc[a][i]);
	}
        printf("\nResource Needed    : ");
        for(int i=0; i<T_Res; i++)
        {
	        printf("%3d", Req[a][i]);
	}
        printf("\nResource Available : ");
        for(int i=0; i<T_Res; i++)
        {
	        printf("%3d", res[i]);
	}
        printf("\n"); 
	sleep(1);
	for(int i=0; i<T_Res; i++)
	{
                res[i] += aloc[a][i];
	}
        printf("\nResource available after execution	: ");
        for(int i=0; i<T_Res; i++)
	{
                printf("%3d", res[i]);
        }
	printf("\n\n");
        sleep(1);
        Executed++;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&ResLock);
	pthread_exit(NULL);
}
int main() {
	srand(time(NULL));
        printf("\nTotal processes: ");
        scanf("%d", &T_proces);
        printf("\nTotal resources: ");
        scanf("%d", &T_Res);
        res = (int *)malloc(T_Res * sizeof(*res));
        printf("\nCurrently Available resources:\n\n");
        for(int i=0; i<T_Res; i++)
	{
		printf("R%d: ",i+1);
                scanf("%d", &res[i]);
	}
        aloc = (int **)malloc(T_proces * sizeof(*aloc));
        for(int i=0; i<T_proces; i++)
	{
                aloc[i] = (int *)malloc(T_Res * sizeof(**aloc));
        }
	max_Req = (int **)malloc(T_proces * sizeof(*max_Req));
        for(int i=0; i<T_proces; i++)
        {
	        max_Req[i] = (int *)malloc(T_Res * sizeof(**max_Req));
        }
	printf("\n");
        for(int i=0; i<T_proces; i++)
	{
                printf("\nResource allocated to process %d :\n", i+1);
                for(int j=0; j<T_Res; j++)
        	{
			printf("R%d: ",j+1);
			scanf("%d", &aloc[i][j]);
		}
        }
        printf("\n");
        for(int i=0; i<T_proces; i++)
	{
                printf("\nMaximum resource Required by process %d :\n", i+1);
                for(int j=0; j<T_Res; j++)
		{
			printf("R%d: ",j+1);
                        scanf("%d", &max_Req[i][j]);
		}
        }
        printf("\n");
        Req = (int **)malloc(T_proces * sizeof(*Req));
        for(int i=0; i<T_proces; i++)
        {
	        Req[i] = (int *)malloc(T_Res * sizeof(**Req));
	}
        for(int i=0; i<T_proces; i++)
        {
	        for(int j=0; j<T_Res; j++)
                {
		        Req[i][j] = max_Req[i][j] - aloc[i][j];
		}
	}
	safeSeq = (int *)malloc(T_proces * sizeof(*safeSeq));
        for(int i=0; i<T_proces; i++)
	{
		safeSeq[i] = -1;
	}
        if(!getSafeSeq())
	{
                printf("\nThe processes results in unsafe state\n\n");
                exit(-1);
        }
        printf("\n\nSafe Sequence : ");
        for(int i=0; i<T_proces; i++)
	{
                printf("%-3d", safeSeq[i]+1);
        }
        printf("\nExecuting Processes...\n\n");
        sleep(1);
	pthread_t processes[T_proces];
        pthread_attr_t attr;
        pthread_attr_init(&attr);
	int processNumber[T_proces];
	for(int i=0; i<T_proces; i++)
	{
		processNumber[i] = i;
	}
        for(int i=0; i<T_proces; i++)
        {
	        pthread_create(&processes[i], &attr, pCode, (void *)(&processNumber[i]));
        }
	for(int i=0; i<T_proces; i++)
        {
	        pthread_join(processes[i], NULL);
        }
	printf("\nAll Processes Finished\n");
        free(res);
        for(int i=0; i<T_proces; i++)
	{
                free(aloc[i]);
                free(max_Req[i]);
		free(Req[i]);
        }
        free(aloc);
        free(max_Req);
	free(Req);
        free(safeSeq);
}

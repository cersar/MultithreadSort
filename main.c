/*
 * main.c
 *
 *  Created on: Jun 9, 2018
 *      Author: yangsen
 */


#include <pthread.h>
#include <stdio.h>

#define MAX_LEN 100

typedef struct {
	int start;
	int end;
} Index;


void swap(int a[],int i,int j);
int partation_single(int a[],int begin,int end);
void quick_sort(int* array,int begin,int end);
void * sortRun(void *args);
void* Merge(void * args);
int initArray();

int array[MAX_LEN];
int result[MAX_LEN];
int n;




int main() {
	pthread_t tid[3]; /* the thread identifier */
	pthread_attr_t attr; /* set of thread attributes */
	Index index[2];

	n = initArray();

	index[0].start = 0;
	index[0].end = n/2;

	index[1].start = n/2;
	index[1].end = n;




	/* get the default attributes */
	pthread_attr_init(&attr);

	/* create the thread */
	for (int i = 0; i < 2; ++i) {
		pthread_create(&tid[i], &attr, sortRun, &index[i]);
	}
	/* wait for the thread to exit */
	for (int i = 0; i < 2; ++i) {
		pthread_join(tid[i], NULL);
	}

	pthread_create(&tid[2], &attr, Merge, &index[1].start);
	pthread_join(tid[2], NULL);

	for(int i =0;i<n;++i){
		printf("%d\t",result[i]);
	}
	printf("\n");

}

int initArray(){
	FILE *fp;
	fp = fopen("input", "r");
	int i=0;

	while(fscanf(fp, "%d", &array[i])!=EOF){
		printf("%d\t",array[i++]);
		if(i>MAX_LEN){
			printf("error:最多接受100个数字！\n");
			break;
		}
	}
	printf("\n");
	fclose(fp);
	return i;
}

void swap(int a[],int i,int j)
{
	int temp=a[i];
	a[i]=a[j];
	a[j]=temp;
}

int partation_single(int a[],int begin,int end)
{
	int key=a[end],cur=begin-1;
	for(int i=begin;i<=end;i++)
	{
		if(a[i]<key)
		{
			cur++;
			swap(a,i,cur);
		}
	}
	cur++;
	swap(a,end,cur);
	return cur;
}

void * sortRun(void * args){
	Index * index = (Index *)args;
	int begin = index->start;
	int end = index->end-1;
	quick_sort(array,begin,end);
	pthread_exit(0);
}

void quick_sort(int* array,int begin,int end)
{

	if(begin<end)
	{
		int cur=partation_single(array,begin,end);
		quick_sort(array,begin,cur-1);
		quick_sort(array,cur+1,end);
	}
}

void* Merge(void * args)
{
	int midIndex = *((int *)args);
    int i = 0, j=midIndex, k = 0;
    while(i!=midIndex && j!=n)
    {
        if(array[i] > array[j])
            result[k++] = array[j++];
        else
            result[k++] = array[i++];
    }
    while(i != midIndex)
        result[k++] = array[i++];
    while(j != n)
        result[k++] = array[j++];
    pthread_exit(0);
}


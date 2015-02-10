/*
 * bucketSort.c
 *
 *  Created on: Oct 20, 2014
 *      Author: dev
 */


#include <stdio.h>
#include <stdlib.h>

#include <ziggurat.h>
#include <rnd.h>
#include <appMacros.h>
#include <math.h>
#include <string.h>
#include <lib.h>
#include <sys/time.h>
#include <time.h>

#define DEBUG 0

#define DEF_P_SIZE 16
#define DEF_MAX_NUM 100
#define DEF_NUM_BUCKETS 5
#define DEF_BUCKET_SIZE

#define MAX 9999;

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		printUsage(argv[ARGS_APP_NAME]);
		return FAILURE;
	}

	struct timeval begin, end;
	clock_t Tstart, Tend;

	int print=FALSE;
	int problemSize=0;
	parseArgs(argv,&problemSize,&print,argc);
	int bucketSize=0;
	if(problemSize == 0)
	{
		printf("Please enter a proper number for problem size\n");
		printUsage(argv[ARGS_APP_NAME]);
		return FAILURE;
	}

	bucketSize=problemSize/DEF_NUM_BUCKETS;
	if(print==TRUE)
	{
		printf("size of each bucket %d\n",bucketSize);
	}
	float* input=(float*) calloc(problemSize,sizeof(float));

	/**
	 * 1. Get data from the distribution
	 * 2. Split the data into buckets
	 * 3. Sort the data in each bucket
	 * 4. Append all the post sorted data -- and we done
	 **/
	random_number_generator_normal(input,problemSize,DEF_MAX_NUM);

#if DEBUG
	printf("============== PRIOR TO SORTING ====================\n");
	for(int i=0;i<problemSize;i++)
	{
	  printf("RNG at index %d we have %f\n",i,input[i]);
	}
#endif

	float** bucket_ref = (float**) malloc(DEF_NUM_BUCKETS*sizeof(float*));

	float results[DEF_NUM_BUCKETS];

	for(int i=0;i<DEF_NUM_BUCKETS;i++)
	{
		int sm=(i+1)*bucketSize;
		float result=kthsmallest(input,problemSize,sm);
		results[i]=result;
#if DEBUG
		printf("%d smallest element is %f ",sm,result);
#endif
	}


	/**
	 * Partition data into buckets
	 */
	for(int i=0;i<DEF_NUM_BUCKETS;i++)
	{

		//printf("===========from %d to %d======\n",i*bucketSize,bucketSize+(i*bucketSize));
		bucket_ref[i] = malloc(sizeof(float)*bucketSize);
		float result=results[i];
		for(int j=i*bucketSize,k=0; k<bucketSize&&j<bucketSize+i*bucketSize;j++,k++)
		{
			if((input[j] <= result))
			{
				bucket_ref[i][k]=input[j];
				input[j]=MAX;
			}
		}
	}

	/**
	 * Print DATA in buckets
	 */
#if DEBUG
	for(int i=0;i<DEF_NUM_BUCKETS;i++)
	{
		printf("The bucket ====== %d\n",i);
		for(int j=0;j<bucketSize;j++)
		{
			printf("Bucket %d value %f\n",i,bucket_ref[i][j]);
		}
	}
#endif


	Tstart=clock();
	gettimeofday(&begin,NULL);
	for(int i=0;i<DEF_NUM_BUCKETS;i++)
	{
		quickSort(bucket_ref[i],0,bucketSize-1);
	}
	gettimeofday(&end,NULL);
	Tend = clock(); // timing ends
	double TimeElapsed =(end.tv_sec - begin.tv_sec) +((end.tv_usec - begin.tv_usec)/1000000.0);
	printf("CLOCK:Total time taken in secs to find sorting %d numbers is %f\n",problemSize,(double)(Tend - Tstart) / CLOCKS_PER_SEC);
	printf("GETTIMEOFDAY:Total time taken in secs to find sorting %d numbers is %f\n",problemSize,TimeElapsed);
	if(print==TRUE)
	{
		printf("============== POST SORTING ====================\n");
		for(int i=0;i<DEF_NUM_BUCKETS;i++)
		{
			for(int j=0;j<bucketSize;j++)
			{
				printf("Bucket %d value %f\n",i,bucket_ref[i][j]);
			}
		}
	}
	return SUCCESS;
}




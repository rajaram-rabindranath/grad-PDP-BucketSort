/*
 * test.c


 *
 *  Created on: Oct 24, 2014
 *      Author: dev
 */
#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>

#include <ziggurat.h>
#include <rnd.h>
#include <appMacros.h>
#include <math.h>
#include <string.h>
#include <lib.h>
#include <sys/time.h>
#include <time.h>

#define DEBUG 0
#define MAX 999

int main(int argc, char** argv)
{
	MPI_Init( &argc, &argv );
	int len;
	char procname[MPI_MAX_PROCESSOR_NAME];

	int size=0;
	int my_rank=0;
	MPI_Comm_size( MPI_COMM_WORLD, &size );
	MPI_Comm_rank( MPI_COMM_WORLD, &my_rank );
	MPI_Get_processor_name(procname,&len);

	struct timeval begin, end;
	clock_t Tstart, Tend;

	if(argc < 2)
	{
		if(my_rank==0) // master says
		{
			printUsage(argv[ARGS_APP_NAME]);
		}
		return FAILURE;
	}

	int print=FALSE;
	int problemSize=0;
	parseArgs(argv,&problemSize,&print,argc);
	if(problemSize == 0)
	{
		if(my_rank==0) // master says
		{
			printf("Please enter a proper number for problem size\n");
			printUsage(argv[ARGS_APP_NAME]);
		}
		return FAILURE;
	}

	int bucketSize=0;
	bucketSize=problemSize/size;
	if(problemSize%size !=0)
	{

		if(my_rank == 0) // master says
		{
			printf("===== This program  has some constraints =====\n");
			printf("You have mentioned the problem size to be %d\n",problemSize);
			printf("And have chosen to use %d processes\n",size);
			printf("The problem size %d is not clearly divisible by %d\n",problemSize,size);
			printf("Please give a problem size and processes combination that is clearly divisible\n");
			printf("Thanks\n");
		}
		return FAILURE;
	}

	float** bucket_ref=NULL;

	if(my_rank == 0)
	{

		if(print==TRUE)
		{
			printf("Num buckets==Num Processes which is %d\n",size);
			printf("size of each bucket %d\n",bucketSize);
		}

		float* input=(float*) calloc(problemSize,sizeof(float));

		// master generate random numbers
		random_number_generator_normal(input,problemSize,100);

		#if DEBUG
		printf("============== PRIOR TO SORTING ====================\n");
		for(int i=0;i<problemSize;i++)
		{
		  printf("RNG at index %d we have %f\n",i,input[i]);
		}
		#endif

		bucket_ref = (float**) malloc(size*sizeof(float*));
		float results[size];

		// MASTER FINDS THE PARTITION POINTS
		for(int i=0;i<size;i++)
		{
			int sm=(i+1)*bucketSize;
			float result=kthsmallest(input,problemSize,sm);
			results[i]=result;
			#if DEBUG
			printf("%d smallest element is %f\n ",sm,result);
			#endif
		}

		/**
		 * MASTER Partitions data into buckets
		 */
		for(int i=0;i<size;i++)
		{
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
		for(int i=0;i<size;i++)
		{
			printf("The bucket ====== %d\n",i);
			for(int j=0;j<bucketSize;j++)
			{
				printf("Bucket %d value %f\n",i,bucket_ref[i][j]);
			}
		}
		#endif

		// send data to workers and also work on my data
		Tstart=clock();
		gettimeofday(&begin,NULL);

		// give all workers something to do
		for(int i=1;i<size;i++)
		{
			MPI_Send((void*)bucket_ref[i],bucketSize,MPI_FLOAT,i,0,MPI_COMM_WORLD);
		}

		quickSort(bucket_ref[0],0,bucketSize-1);
	}
	else // workers
	{
		float* my_work = malloc(sizeof(float)*bucketSize);
		int master =0;
		MPI_Recv(my_work, bucketSize, MPI_FLOAT,master, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		quickSort(my_work,0,bucketSize-1);
		MPI_Send(my_work,bucketSize,MPI_FLOAT,master,0,MPI_COMM_WORLD);
	}

	if(my_rank == 0)
	{
		for(int i=1;i<size;i++)
		{
			MPI_Recv(bucket_ref[i],bucketSize,MPI_FLOAT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		}

		gettimeofday(&end,NULL);
		Tend = clock(); // timing ends

		if(print==TRUE)
		{
			int printSake=0;
			printf("============== POST SORTING ====================\n");
			for(int i=0;i<size;i++)
			{
				printSake=i*bucketSize;
				for(int j=0;j<bucketSize;j++)
				{
					printf("%d value %f\n",printSake+j,bucket_ref[i][j]);
				}
			}
			printf("==============================================\n");
		}

		double TimeElapsed =(end.tv_sec - begin.tv_sec) +((end.tv_usec - begin.tv_usec)/1000000.0);
		printf("============== TIME TAKEN ====================\n");
		printf("CLOCK_T:Total time taken in secs to find sort %d numbers is %f\n",problemSize,(double)(Tend - Tstart) / CLOCKS_PER_SEC);
		printf("GETTIMEOFDAY:Total time taken in secs to sort %d numbers is %f\n",problemSize,TimeElapsed);
	}
	MPI_Finalize();
	return SUCCESS;
}


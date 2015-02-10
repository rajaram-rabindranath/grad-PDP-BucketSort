/*
 * lib.c
 * SOME OF THE FUNCTIONS IN THIS FILE HAVE BEEN TAKEN FROM THE INTERNET
 *  Created on: Oct 24, 2014
 *      Author: dev, STACKOVERFLOW, IBM
 */

#include <stdio.h>
#include <stdlib.h>
#include <appMacros.h>
#include <string.h>
#include <lib.h>

void printBucket(float* bucket,int low, int high)
{
	for(int i=low;i<high;i++)
	{
		printf("the val %f \n",bucket[i]);
	}

}

void printUsage(char* appName)
{
	printf("---------------- Wrong number of arguments ---------------\n");
	printf("%s <PROBLEM_SIZE>\n",appName);
	printf("%s <-t PROBLEM_SIZE> \n",appName);
	printf("---------------- Wrong number of arguments ---------------\n");
}

int parseArgs(char** argv,int* problemSize,int* print,int arg_count)
{
	if(arg_count == 3)
	{
		if(!strcmp(argv[ARGS_P_SIZE_ID],ARGS_P_SIZE_ID_CODE))
		{
			*print=TRUE;
			*problemSize=atoi(argv[ARGS_P_SIZE]);
		}
	}
	else if(arg_count == 2)
	{
		*problemSize =atoi(argv[1]);
	}
	return SUCCESS;
}

/**
 * CODE REFERENCED FROM THE NET STACKOVERFLOW
 */
void quickSort(float a[], int l, int r)
{

   float j=0.0f;
   //printf("Low %d and high %d\n",l,r);
   if( l < r )
   {
       j = partition( a, l, r);
       quickSort( a, l, j-1);
       quickSort( a, j+1, r);
   }
}


/**
 * CODE REFERENCED FROM THE NET STACKOVERFLOW
 */
int partition(float a[], int l, int r)
{
   float pivot=0.0f,t=0.0f;
   int i, j;
   pivot = a[l];
   i = l; j = r+1;

   while(1)
   {
		do ++i; while( a[i] <= pivot && i <= r );
		do --j; while( a[j] > pivot );
		if( i >= j ) break;
		t = a[i];
		a[i] = a[j];
		a[j] = t;
   }

   t = a[l]; a[l] = a[j]; a[j] = t;
   return j;
}


/**
 * CODE REFERENCED FROM THE NET IBM
 */
void  swap(float *a,float *b)
{
	float t=*a;
	*a=*b;
	*b=t;
}


/**
 * CODE REFERENCED FROM THE NET IBM
 */
float kthsmallest(float  A[],int n, int k)
{
	int i=0,flag=0;
	int p,q;
	float r;
	p=0;
	r=n-1;
	k--;
	while(!flag)
	{
		q=partition_for_K(A,p,r);
		if(q==k)
		{
			flag=1;
		}
		else if(k<q)
		{
			r=q-1;
		}
		else
		{
			p=q+1;
		}
	}
	return A[k];
}

/**
 * CODE REFERENCED FROM THE NET IBM
 */
float partition_for_K(float A[],int p,int r)
{
	int i,j;
	float pivot;
	i=p-1;
	pivot=A[r];
	for(j=p;j<r;j++)
	{
	   if(A[j]<pivot)
	  {
		i++;
		swap(&A[j],&A[i]);
	   }
	}
	swap(&A[i+1],&A[r]);
	return i+1;
}

/*
 * lib.h
 *
 *  Created on: Oct 24, 2014
 *      Author: dev
 */

#ifndef LIB_H_
#define LIB_H_
void printBucket(float* bucket,int low, int high);
void printUsage(char* appName);
int parseArgs(char** argv,int* pSize,int *print,int argc);
int partition(float a[], int l, int r);
void quickSort(float a[], int l, int r);
float partition_for_K(float A[],int p,int r);
void  swap(float *a,float *b);
float kthsmallest(float  A[],int n, int k);


#endif /* LIB_H_ */

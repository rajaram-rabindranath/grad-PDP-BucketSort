FOLDER STRUCTURE:
MPI 	-- contains MPI code in "src" folder along with Makefile and a runner script.
OMP 	-- contains OpenMP code in "src" folder along with Makefile and a runner script.
SEQ 	-- contains Sequential code in "src" folder along with Makefile and a runner script.


HOW - TO:

VERY IMPOTANT NOTICE:

When running the program -- please make sure that 
	FOR Sequential code -- problem size has to be divisible by 5
	FOR OpenMP code	    -- problem size has to be divisible by NUM_PROCS spawned
	FOR MPI code	    -- problem size has to be divisible by Total num procs created

-- Please do load the following modules:

$ module load intel
$ module load intel-mpi

Inside each of the aforementioned folders is a runner script to submit the program 
in SBATCH and here are their names, what they do and how to run them.

------------------------------------------------ SEQ FOLDER ---------------------------------------------------
runner_SEQ.sh -- the runner script for Sequential code
e.g
	$ ./runner_SEQ.sh


The runner file has to be modified to change problem size as follows
(The things to be modified are right at the top of the file and have FIXME tags)

// to set the problem size fill the primeLimits array like so 
### FIXME --- Here is where one can set the problem size --- like so --- probSize(20 30 40) below is an example
probSize=(100000000);

--- to run in non batch and to execute on local machine do the following:

	./bucketSort <-t probSize>  --- to print output + time
	./bucketSort <probSize>  --- to print output + time

 $ make
 $ ./bucketSort -t 45


----------------------------------------------MPI------------------------------------------------------
runner_MPI.sh -- the runner script for MPI and requires an argument for Nodes
e.g

	$ ./runner_MPI.sh 4 // if no args are passes node count is defaulted to "2"

The runner file has to be modified to change problem size and the number of cores as follows
The things to be modified are right at the top of the file and have FIXME tags

// To set num cores fill the taskList array
## FIXME here is where one can set the number of processes /node like so --- taskList=(3 4 4)
taskList=(4)

// to set the problem size fill the primeLimits array like so 
## FIXME here is where one can set probSise--- probSize=(24)
probSize=(25)


--- to run in non batch and to execute local machine do the following:

	# to print output + time
	$ mpirun -n <node> ./bucketSort_MPI <-t probSize> 
	# only print time
	$ mpirun -n <node> ./bucketSort_MPI <probSize> 
 
 $ make
 $ mpirun -n 4 ./bucketSort_MPI 45


------------------------------------------------OMP---------------------------------------------------
runner_OMP.sh -- the runner script for OMP

	$ ./runner_OMP.sh

The runner file has to be modified to change problem size and the number of cores as follows
The things to be modified are right at the top of the file and have FIXME tags

--- EXAMPLE
// To set num cores fill the taskList array
## FIXME here is where one can set the number of processes /node like so --- taskList=(3 4 4)
taskList=(4)

// to set the problem size fill the primeLimits array like so 
## FIXME here is where one can set probSize like so --- probSize=(24)
probSize=(25)



--- to run in non batch and to execute local machine do the following:

	# to print output + time
	$ ./bucketSort_OMP <-t probSize> 

	# to print just time
	$ ./bucketSort_OMP <probSize> 


 $ make
 $ ./bucketSort_OMP -t 45


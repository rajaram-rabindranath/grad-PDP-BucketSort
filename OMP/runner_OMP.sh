#!/bin/bash

jobname="OMP_BSORT"

rm -rf ALL_SLURMS
mkdir ALL_SLURMS ## store all the SLURMs created to this location


## FIXME here is where one can set the number of processes/node like so --- taskList=(3 4 4)
taskList=(2 4 8)
## FIXME here is where one can set the problem size --- like so probSize=(24)
probSize=(1000 300)



func()
{
directive="#!/bin/bash\n\n#SBATCH --partition=$1\n#SBATCH --nodes=1\n#SBATCH --ntasks-per-node=$2\n#SBATCH --job-name=$jobname\n#SBATCH --time=00:20:00\n#SBATCH --mail-user=rajaramr@buffalo.edu\n#SBATCH --output=result/Result_OMP_BSORT_%j.out\n#SBATCH --error=result/Result_OMP_BSORT_error_%j.out\n
echo \"SLURM Enviroment Variables:\"\n
echo \"Job ID = \"\$SLURM_JOB_ID\n
echo \"Job Name = \"\$SLURM_JOB_NAME\n
echo \"Job Node List = \"\$SLURM_JOB_NODELIST\n
echo \"Number of Nodes = \"\$SLURM_NNODES\n
echo \"Tasks per Nodes = \"\$SLURM_NTASKS_PER_NODE\n
echo \"CPUs per task = \"\$SLURM_CPUS_PER_TASK\n
echo \"/scratch/jobid = \"\$SLURMTMPDIR\n
echo \"submit Host = \"\$SLURM_SUBMIT_HOST\n
echo \"Subimt Directory = \"\$SLURM_SUBMIT_DIR\n
echo \n"
echo -e $directive
}

sbatch_omp_xtra="ulimit -s unlimited\nmodule load intel\nmodule list\n\nNPROCS=\`srun --nodes=\${SLURM_NNODES} bash -c 'hostname' | wc -l\`\necho \"NPROCS=\"\$NPROCS\nexport OMP_NUM_THREADS=\$NPROCS\nexport | grep OMP\n
#\n\n./bucketSort_OMP \$1\n\n#\necho \"All Dones!\"\n"

for tasks in "${taskList[@]}"
do
   :

	func "general-compute" $tasks > SLURM_OMP_$tasks
	echo -e $sbatch_omp_xtra >> SLURM_OMP_$tasks

	for size in "${probSize[@]}"
	do
	   :

	sbatch SLURM_OMP_$tasks $size
	done	

	mv SLURM_OMP_$tasks ALL_SLURMS/
done

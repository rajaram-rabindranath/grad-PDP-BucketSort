#!/bin/bash

if [ $# -eq 0 ]
then
	nodes=2
else
	nodes=$1
fi

echo "Number of nodes are" $nodes


## FIXME here is where one can set the number of processes/node like so --- taskList=(3 4 4)
taskList=(8)
## FIXME here is where one can set the problem size --- like so probSize=(24)
probSize=(10000000)

job_name="MPI_bucket"


rm -rf ALL_SLURMS
mkdir ALL_SLURMS ## store all the SLURMs created to this location

func()
{
directive="#!/bin/bash\n\n#SBATCH --partition=$2\n#SBATCH --nodes=$3\n#SBATCH --ntasks-per-node=$1\n#SBATCH --job-name=$job_name\n#SBATCH --time=00:50:00\n#SBATCH --mail-user=rajaramr@buffalo.edu\n#SBATCH --output=result/Result_MPI_"$3"_"$1"_%j.out\n#SBATCH --error=result/Result_MPI_"$3"_"$1"_%j.out\n
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

sbatch_mpi="module load intel\nmodule load intel-mpi\nmodule list\nulimit -s unlimited\nexport I_MPI_PMI_LIBRARY=/usr/lib64/libpmi.so\nsrun ./bucketSort_MPI \$1\necho \"All Dones!\"\n"

### make a SPLURM file with config --- 8,12,16 processes and partition debug
for tasks in "${taskList[@]}"
do
   :

	func $tasks "general-compute" $nodes > SLURM_OMP_nodes"$nodes"_tasks"$tasks"
	echo -e $sbatch_mpi >> SLURM_OMP_nodes"$nodes"_tasks"$tasks"

	for size in "${probSize[@]}"
	do
	   :

	sbatch SLURM_OMP_nodes"$nodes"_tasks"$tasks" $size
	done	

	mv SLURM_OMP_nodes"$nodes"_tasks"$tasks" ALL_SLURMS/
done

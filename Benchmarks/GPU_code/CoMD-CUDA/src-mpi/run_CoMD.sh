../bin/CoMD-cuda 2>&1 | tee compile_log1
../bin/CoMD-cuda -e 2>&1 | tee compile_log2
../bin/CoMD-cuda -e -t setfl 2>&1 | tee compile_log3
../bin/CoMD-cuda -e -p Cu01.eam.alloy 2>&1 | tee compile_log4
../bin/CoMD-cuda -e -t setfl -p Cu01.eam.alloy 2>&1 | tee compile_log5
../bin/CoMD-cuda -l 3.5 2>&1 | tee compile_log6
../bin/CoMD-cuda --delta 0.1 -T 0 2>&1 | tee compile_log7
../bin/CoMD-cuda -x 40 -y 40 -z 40 2>&1 | tee compile_log8
../bin/CoMD-cuda -x 40 -y 40 -z 40 -N 10000 -n 100 2>&1 | tee compile_log9

# Performance
This file details the changes in perfomance of the application over the development phase with the following test conditions:

Test 1:
- 
M = 1 for all particles

random coordinates

N = 1000

2000 timesteps

dt = 0.0005

call with flags `-O2 -g` for compiler performance optimization and profiling

Test 2
-
M = 1 for all particles

random coordinates

N = 5000

2000 timesteps

dt = 0.0005

call with flags `-O2 -g` for compiler performance optimization and profiling

Test 3:
- 
M = 1 for all particles, random coordinates

N = 10000

2000 timesteps

dt = 0.0005

call with flags `-O2 -g` for compiler performance optimization and profiling

## Naive Implementation

brute force implementation, no optimizations (except for batching the file writes at the end to reduce system calls)

Test 1:
- CPU time: 7.109 s
- `Bodies::updateAcc`: 6.931 s, 97.5% of CPU time
- `NtWriteFile`: 0.082 s, 1.2% of CPU time

Test 2:
- CPU time: 190.169 s
- `Bodies::updateAcc`: 189.669 s, 99.7% of CPU time
- `NtWriteFile`: 0.201 s, 0.1% of CPU time

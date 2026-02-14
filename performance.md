# Performance
This file details the changes in perfomance of the application over the development phase with the following test conditions:

Test 1:
- 
M = 1 for all particles

random coordinates

N = 1000

2000 timesteps

dt = 0.0005

call with flags `-O3 -g` for compiler performance optimization and profiling

Test 2
-
M = 1 for all particles

random coordinates

N = 5000

2000 timesteps

dt = 0.0005

call with flags `-O3 -g` for compiler performance optimization and profiling

Test 3:
- 
M = 1 for all particles, random coordinates

N = 10000

2000 timesteps

dt = 0.0005

call with flags `-O3 -g` for compiler performance optimization and profiling

## Naive Implementation

brute force implementation, no optimizations (except for batching the file writes at the end to reduce system calls)

Test 1: `nbody -n 1000 -i 2000`
- CPU time: 7.352 s
- `Bodies::updateAcc`: 7.081 s, 98.7% of CPU time
- `NtWriteFile`: 0.093 s, 1.3% of CPU time

Test 2: `nbody -n 5000 -i 2000`
- CPU time: 180.463 s
- `Bodies::updateAcc`: 179.700 s, 99.6% of CPU time
- `NtWriteFile`: 0.094 s, 0.1% of CPU time

Test 3: `nbody -n 10000 -i 2000`
- CPU time: 723.588s
- `Bodies::updateAcc`: 721.224 s, 99.7% of CPU time
- `NtWriteFile`: 0.246 s, 0.0% of CPU time

<!-- perf stats on WSL
    Performance counter stats for './nbody -n 1000 -i 2000':

           7761.80 msec task-clock:u              #    0.965 CPUs utilized
                 0      context-switches:u        #    0.000 /sec
                 0      cpu-migrations:u          #    0.000 /sec
             40174      page-faults:u             #    5.176 K/sec
       32159365290      cycles:u                  #    4.143 GHz
          56355004      stalled-cycles-frontend:u #    0.18% frontend cycles idle
       84386112145      instructions:u            #    2.62  insn per cycle
                                                  #    0.00  stalled cycles per insn
        6027594364      branches:u                #  776.572 M/sec
           4419592      branch-misses:u           #    0.07% of all branches

       8.039829343 seconds time elapsed

       7.491532000 seconds user
       0.270313000 seconds sys

    Performance counter stats for './nbody -n 5000 -i 2000':

         189697.18 msec task-clock:u              #    0.981 CPUs utilized
                 0      context-switches:u        #    0.000 /sec
                 0      cpu-migrations:u          #    0.000 /sec
            261763      page-faults:u             #    1.380 K/sec
      798599108030      cycles:u                  #    4.210 GHz
         609139318      stalled-cycles-frontend:u #    0.08% frontend cycles idle
     2102824482679      instructions:u            #    2.63  insn per cycle
                                                  #    0.00  stalled cycles per insn
      150203061444      branches:u                #  791.804 M/sec
          22005966      branch-misses:u           #    0.01% of all branches

     193.319041050 seconds time elapsed

     187.970305000 seconds user
       1.719276000 seconds sys

             754735.80 msec task-clock:u              #    0.976 CPUs utilized
                 0      context-switches:u        #    0.000 /sec
                 0      cpu-migrations:u          #    0.000 /sec
            523388      page-faults:u             #  693.472 /sec
     3200592311008      cycles:u                  #    4.241 GHz
        2154581223      stalled-cycles-frontend:u #    0.07% frontend cycles idle
     8407747019013      instructions:u            #    2.63  insn per cycle
                                                  #    0.00  stalled cycles per insn
      600556126912      branches:u                #  795.717 M/sec
          45705242      branch-misses:u           #    0.01% of all branches

     772.907086763 seconds time elapsed

     750.797763000 seconds user
       3.929569000 seconds sys

-->

## Naive implementation w/ Optimized force calculation

Brute force implementation, exploiting the symmetry in the force calculation:

rather than calculating forces for 

```
    for i in n:
        for j in n:
```

we do:

```
    for i in n:
        for j > i in n:
```

Test 1: `nbody -n 1000 -i 2000`
- CPU time: 4.957 s
- `Bodies::updateAcc`: 3.457 s, 69.7% of CPU time
- `std::vector<double, std::allocator<double>>::operator[]`: 1.320, 26.6% of CPU time
- `Snapshot::Snapshot`: 0.095s, 1.9% of CPU time
- `NtWriteFile`: 0.085 s, 1.7% of CPU time

Test 2: `nbody -n 5000 -i 2000`
- CPU time: 180.463 s
- `Bodies::updateAcc`: 179.700 s, 99.6% of CPU time
- `NtWriteFile`: 0.094 s, 0.1% of CPU time

Test 3: `nbody -n 10000 -i 2000`
- CPU time: 723.588s
- `Bodies::updateAcc`: 721.224 s, 99.7% of CPU time
- `NtWriteFile`: 0.246 s, 0.0% of CPU time
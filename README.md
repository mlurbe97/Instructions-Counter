# Instructions counter

## Author

* Manel Lurbe Sempere (malursem@gap.upv.es)

## Compile and run

- Tested on IBM POWER 8 with Ubuntu 18.04.

- The code is ready to run SPEC CPU Benchmarks, install them and change the path to the benchmarks in the scheduler code before compile.

- Use [libperf](https://github.com/mlurbe97/Instructions-Counter/blob/master/doc/lib) library for C to compile the scheduler [Instructions_counter.c](https://github.com/mlurbe97/Instructions-Counter/blob/master/src/Instructions_counter.c).

- To run the scheduler just launch this script as super user -> [count_instructions](https://github.com/mlurbe97/Instructions-Counter/blob/master/doc/launch_scripts/count_instructions).

## Instructions to C array

The program in folder [doc/process_array](https://github.com/mlurbe97/Instructions-Counter/blob/master/doc/process_array) contains the program [instructions_to_C_array.py](https://github.com/mlurbe97/Instructions-Counter/blob/master/doc/process_array/instructions_to_C_array.py) that converts the output instructions from the main program to a C array, useful to use with my implementations of Linux Schedulers.
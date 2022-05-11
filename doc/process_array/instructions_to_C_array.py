#!/usr/bin/python3
# -*- coding: utf-8 -*-

## Instructions counter.
## Year: 2020
## Author: Manel Lurbe Sempere <malursem@gap.upv.es>

import sys

arguments =  sys.argv
numargs = len(sys.argv)

print("\nInstructions counter. Year: 2020 Author: Manel Lurbe Sempere <malursem@gap.upv.es>.\n")

if numargs != 3:
    print("Missing arguments")
    print("Usage: ./instructions_to_C_array.py Time workloadArray [a->2006 b->2017 c->2006&2017 d->mediabench e->Real Time]")
    sys.exit(1)

time = arguments[1]
workloadArray = arguments[2]
ini = 0
fin = 0
if workloadArray == 'a':
    print("Selected Benchmarks: SPEC CPU 2006\n")
    benchmarks = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29]
    ini = 0
    fin = 0
elif workloadArray == 'b':
    print("Selected Benchmarks: SPEC CPU 2017\n")
    benchmarks = [30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55]
    ini = 30
    fin = 30
elif workloadArray =='c':
    print("Selected Benchmarks: SPEC CPU 2006&2017\n")
    benchmarks = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55]
    ini = 0
    fin = 0
elif workloadArray =='d':
    print("Selected Benchmarks: mediabench\n")
    benchmarks = [56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 74, 75]
    ini = 56
    fin = 56
elif workloadArray =='e':
    print("Selected Benchmarks: Real Time\n")
    benchmarks = [88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100]
    ini = 88
    fin = 88
elif workloadArray =='z':
    print("Selected Benchmarks: All benchmarks: SPEC CPU 2006&2017 mediabench Real Time\n")
    benchmarks = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100]#101
    ini = 0
    fin = 0
else:
    print("Array value incorrect.")
    sys.exit(1)
listaInstructions=[]

for bench in benchmarks:
    existe = 0
    try:
        fitx = open("instructions["+time+"]/Instructions["+str(bench)+"].txt","r")
        for linea in fitx.read().split("\n"):
            if "PMU_COUNTS:" in linea:
                existe = 1
                instructions = linea.split("\t")[2]
                listaInstructions.append(instructions)
                #print("Fine with benchmark number: "+str(bench)+"\nDONE!\n")
                break
        if existe == 0:
            print("Error with benchmark number: "+str(bench)+"\nCan't find PMU_COUNTS\n")
            listaInstructions.append(str(0))
    except:
        print("Error with benchmark number: "+str(bench)+"\nCan't find the file\n")
        listaInstructions.append(str(0))

escribir = open("instructions["+time+"]/array.txt","w")
escribir.write("unsigned long int bench_Instructions [] = {\n\t")

not_working = open("instructions["+time+"]/not_working.txt","w")
not_working.write("Benchmarks not working\n")

working = open("instructions["+time+"]/working.txt","w")
working.write("Benchmarks working\n")

print(str(len(listaInstructions)))
cont = 0
conta = 0

for value in listaInstructions:
    if value == '0':
        not_working.write(str(cont)+" ")
    else:
        working.write(str(cont)+" ")
    if cont != len(listaInstructions)-1:
        escribir.write(value+",")
    else:
        escribir.write(value)
    if conta == 7:
        fin = ini+conta
        escribir.write("//"+str(ini)+"--"+str(fin)+"\n\t")
        ini = fin+1
        conta = 0
    else:
        conta = conta+1
    cont=cont+1
escribir.write("\n};")
escribir.close()
not_working.close()
working.close()
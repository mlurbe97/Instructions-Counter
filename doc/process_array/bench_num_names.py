#!/usr/bin/python3
# -*- coding: utf-8 -*-

## Instructions counter.
## Year: 2020
## Author: Manel Lurbe Sempere <malursem@gap.upv.es>

print("Instructions counter. Year: 2020 Author: Manel Lurbe Sempere <malursem@gap.upv.es>.")

benchmarks = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100]#101
benchmarksNames = ["perlbench","bzip2","gcc","mcf","gobmk","hmmer","sjeng","libquantum","h264ref","omnetpp","astar","xalancbmk","bwaves","gamess",
"milc","zeusmp","gromacs","cactusADM","leslie3d","namd","microbench","soplex","povray","GemsFDTD","lbm",
"tonto","calculix","null","null","null","perlbench_r checkspam","gcc_r","mcf_r","omnetpp_s","xalancbmk_s","x264_s","deepsjeng_r",
"leela_s","exchange2_s","xz_r 1","bwaves_r","cactuBSSN_r","lbm_r","wrf_s","pop2_s","imagick_r","nab_s","fotonik3d_r",
"roms_r","namd_r","parest_r","povray_r","xz_r 2","xz_r 3","exchange2_r","perlbench_r diffmail","jpeg djpeg","jpeg cjpeg","mpeg2 mpeg2encode",
"mpeg2 mpeg2decode","gsm toast","gsm untoast","adpcm rawcaudio","adpcm rawdaudio","g721 encode","g721 decode","pgp fes","pgp fdb","pegwit encrypt","pegwit decrypt",
"ghostscript","mesa","sphere","rasta","epic epic","epic unepic","jpeg cjpeg 2","jpeg djpeg 2","h263 enc","h263 dec","h264 enc","h264 dec",
"jpg2000 dec","jpg2000 enc","mpeg2 enc 2","mpeg2 dec 2","mpeg4 dec","mpeg4 enc","BinarySearchTree","BitwiseShift","Correlation","CorrelationFI","Factorial","Fibonacci",
"LinearSearch","LinkedList","MatrixMultiplication","PFactorial","SelectionSort","Softmax","EMSBench"]

file = open("benchmark_names_nums.txt","w")
#print(len(benchmarksNames))
#print(len(benchmarks))
for h in benchmarks:
    if h == 100:
        file.write(benchmarksNames[h]+" -> "+str(benchmarks[h]))
    else:
        file.write(benchmarksNames[h]+" -> "+str(benchmarks[h])+"\n")
file.close()

file = open("benchmark_names_array.txt","w")
file.write("char *bench_Names [] = {\n\t")
cont = 0
ini = 0
fin = 0
for h in benchmarks:
    if h == 100:
        file.write('"'+benchmarksNames[h]+'"')
    else:
        file.write('"'+benchmarksNames[h]+'",')
    if cont == 7:
        fin = ini+cont
        file.write("//"+str(ini)+"--"+str(fin)+"\n\t")
        ini = fin+1
        cont = 0
    else:
        cont = cont+1

if cont != 0:
    fin = fin+cont
    file.write("//"+str(ini)+"--"+str(fin)+"\n")
file.write("};")
file.close()
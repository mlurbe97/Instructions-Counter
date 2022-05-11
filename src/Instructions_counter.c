/*
  Instructions counter.
  Year: 2020
  Author: Manel Lurbe Sempere <malursem@gap.upv.es>
*/

/*************************************************************
 **                  Includes                               **
 *************************************************************/

#include <sys/types.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <err.h>
#include <sys/poll.h>
#include <sched.h>
#include "perf_util.h"

/*************************************************************
 **                   Structs                               **
 *************************************************************/
typedef struct {
  char *events;
  int delay;
  int pinned;
  int group;
  int verbose;
} options_t;

/*************************************************************
 **                   Variables Globales                    **
 *************************************************************/

static options_t options;
uint64_t pmu_counters[7];
char *events[7];

/*************************************************************
 **                   Benchmarks Spec2006-2017              **
 *************************************************************/

char *benchmarks[][200] = {
  //* SPEC CPU 2017 *//
  // 0 -> perlbench
  {NULL, NULL, NULL},
  // 1 -> bzip2
  {"/home/malursem/working_dir/spec_bin/bzip2.ppc64", "/home/malursem/working_dir/CPU2006/401.bzip2/data/all/input/input.combined", "200", NULL},
  // 2 -> gcc
  {"/home/malursem/working_dir/spec_bin/gcc.ppc64", "/home/malursem/working_dir/CPU2006/403.gcc/data/ref/input/scilab.i", "-o", "scilab.s", NULL},
  // 3 -> mcf
  {"/home/malursem/working_dir/spec_bin/mcf.ppc64", "/home/malursem/working_dir/CPU2006/429.mcf/data/ref/input/inp.in", NULL},
  // 4 -> gobmk [Doesn't work]
  {"/home/malursem/working_dir/spec_bin/gobmk.ppc64", "--quiet", "--mode", "gtp", NULL},
  // 5 -> hmmer
  {"/home/malursem/working_dir/spec_bin/hmmer.ppc64", "--fixed", "0", "--mean", "500", "--num", "500000", "--sd", "350", "--seed", "0", "/home/malursem/working_dir/CPU2006/456.hmmer/data/ref/input/retro.hmm", NULL},
  // 6 -> sjeng
  {"/home/malursem/working_dir/spec_bin/sjeng.ppc64", "/home/malursem/working_dir/CPU2006/458.sjeng/data/ref/input/ref.txt", NULL},
  // 7 -> libquantum
  {"/home/malursem/working_dir/spec_bin/libquantum.ppc64", "1397", "8", NULL},
  // 8 -> h264ref
  {"/home/malursem/working_dir/spec_bin/h264ref.ppc64", "-d", "/home/malursem/working_dir/CPU2006/464.h264ref/data/ref/input/foreman_ref_encoder_baseline.cfg", NULL},
  // 9 -> omnetpp
  {"/home/malursem/working_dir/spec_bin/omnetpp.ppc64", "/home/malursem/working_dir/CPU2006/471.omnetpp/data/ref/input/omnetpp.ini", NULL},
  // 10 -> astar
  {"/home/malursem/working_dir/spec_bin/astar.ppc64", "/home/malursem/working_dir/CPU2006/473.astar/data/ref/input/BigLakes2048.cfg", NULL},
  // 11 -> xalancbmk
  {"/home/malursem/working_dir/spec_bin/Xalan.ppc64", "-v", "/home/malursem/working_dir/CPU2006/483.xalancbmk/data/ref/input/t5.xml", "/home/malursem/working_dir/CPU2006/483.xalancbmk/data/ref/input/xalanc.xsl", NULL},
  // 12 -> bwaves
  {"/home/malursem/working_dir/spec_bin/bwaves.ppc64", NULL},
  // 13 -> gamess
  {"/home/malursem/working_dir/spec_bin/gamess.ppc64", NULL},
  // 14 -> milc
  {"/home/malursem/working_dir/spec_bin/milc.ppc64", NULL},
  // 15 -> zeusmp
  {"/home/malursem/working_dir/spec_bin/zeusmp.ppc64", NULL},
  // 16 -> gromacs
  {"/home/malursem/working_dir/spec_bin/gromacs.ppc64", "-silent", "-deffnm", "/home/malursem/working_dir/CPU2006/435.gromacs/data/ref/input/gromacs", "-nice", "0", NULL},
  // 17 -> cactusADM
  {"/home/malursem/working_dir/spec_bin/cactusADM.ppc64", "/home/malursem/working_dir/CPU2006/436.cactusADM/data/ref/input/benchADM.par", NULL},
  // 18 -> leslie3d
  {"/home/malursem/working_dir/spec_bin/leslie3d.ppc64", NULL},
  // 19 -> namd
  {"/home/malursem/working_dir/spec_bin/namd.ppc64", "--input", "/home/malursem/working_dir/CPU2006/444.namd/data/all/input/namd.input", "--iterations", "38", "--output", "namd.out", NULL},
  // 20 -> microbench
  {"/home/malursem/working_dir/microbenchArray160MBinf", "100", "0", "1024", "0"},
  // 21 -> soplex
  {"/home/malursem/working_dir/spec_bin/soplex.ppc64", "-s1", "-e","-m45000", "/home/malursem/working_dir/CPU2006/450.soplex/data/ref/input/pds-50.mps", NULL},
  //22 -> povray
  {"/home/malursem/working_dir/spec_bin/povray.ppc64", "/home/malursem/working_dir/CPU2006/453.povray/data/ref/input/SPEC-benchmark-ref.ini", NULL},
  // 23 -> GemsFDTD
  {"/home/malursem/working_dir/spec_bin/GemsFDTD.ppc64", NULL},
  // 24 -> lbm
  {"/home/malursem/working_dir/spec_bin/lbm.ppc64", "300", "reference.dat", "0", "1", "/home/malursem/working_dir/CPU2006/470.lbm/data/ref/input/100_100_130_ldc.of", NULL},
  // 25 -> tonto
  {"/home/malursem/working_dir/spec_bin/tonto.ppc64", NULL},
  // 26 -> calculix
  {"/home/malursem/working_dir/spec_bin/calculix.ppc64", "-i", "/home/malursem/working_dir/CPU2006/454.calculix/data/ref/input/hyperviscoplastic", NULL},
  // 27
  {NULL, NULL, NULL},
  // 28
  {NULL, NULL, NULL},
  // 29
  {NULL, NULL, NULL},
  //* SPEC CPU 2017 *//
  // 30 -> perlbench_r checkspam
  {"/home/malursem/working_dir_test/spec_bin2017/perlbench_r.ppc64", "-I/home/malursem/working_dir_test/CPU2017/500.perlbench_r/lib", "/home/malursem/working_dir_test/CPU2017/500.perlbench_r/checkspam.pl", "2500", "5", "25", "11", "150", "1", "1", "1", "1", NULL},
  // 31 -> gcc_r
  {"/home/malursem/working_dir/CPU2017_bin/cpugcc_r.ppc64", "/home/malursem/working_dir/CPU2017/502.gcc_r/gcc-smaller.c", "-O3", "-fipa-pta", "-o", "gcc-smaller.opts-O3_-fipa-pta.s", NULL},
  // 32 -> mcf_s
  {"/home/malursem/working_dir/CPU2017_bin/mcf_s.ppc64", "/home/malursem/working_dir/CPU2017/605.mcf_s/inp.in", NULL},
  // 33 -> omnetpp_s
  {"/home/malursem/working_dir/CPU2017_bin/omnetpp_s.ppc64", "-c", "General", "-r", "0", NULL},
  // 34 -> xalancbmk_s
  {"/home/malursem/working_dir/CPU2017_bin/xalancbmk_s.ppc64", "-v", "/home/malursem/working_dir/CPU2017/623.xalancbmk_s/t5.xml", "/home/malursem/working_dir/CPU2017/623.xalancbmk_s/xalanc.xsl", NULL},
  // 35 -> x264_s
  {"/home/malursem/working_dir/CPU2017_bin/x264_s.ppc64", "--pass", "1", "--stats", "x264_stats.log", "--bitrate", "1000", "--frames", "1000", "-o", "/home/malursem/working_dir/CPU2017/625.x264_s/BuckBunny_New.264", "/home/malursem/working_dir/CPU2017/625.x264_s/BuckBunny.yuv", "1280x720", NULL},
  // 36 -> deepsjeng_r
  {"/home/malursem/working_dir/CPU2017_bin/deepsjeng_r.ppc64", "/home/malursem/working_dir/CPU2017/531.deepsjeng_r/ref.txt", NULL},
  // 37 -> leela_s
  {"/home/malursem/working_dir/CPU2017_bin/leela_s.ppc64", "/home/malursem/working_dir/CPU2017/641.leela_s/ref.sgf", NULL},
  // 38 -> exchange2_s
  {"/home/malursem/working_dir/CPU2017_bin/exchange2_s.ppc64", "6", NULL},
  // 39 -> xz_r 1
  {"/home/malursem/working_dir/CPU2017_bin/xz_r.ppc64", "/home/malursem/working_dir/CPU2017/557.xz_r/cld.tar.xz", "160", "19cf30ae51eddcbefda78dd06014b4b96281456e078ca7c13e1c0c9e6aaea8dff3efb4ad6b0456697718cede6bd5454852652806a657bb56e07d61128434b474", "59796407", "61004416", "6", NULL},
  // 40 -> bwaves_r
  {"/home/malursem/working_dir/CPU2017_bin/bwaves_r.ppc64", NULL},
  // 41 -> cactuBSSN_r
  {"/home/malursem/working_dir/CPU2017_bin/cactuBSSN_r.ppc64", "/home/malursem/working_dir/CPU2017/507.cactuBSSN_r/spec_ref.par", NULL},
  // 42 -> lbm_r
  {"/home/malursem/working_dir/CPU2017_bin/lbm_r.ppc64", "3000", "reference.dat", "0", "0", "/home/malursem/working_dir/CPU2017/519.lbm_r/100_100_130_ldc.of", NULL},
  // 43 -> wrf_s
  {"/home/malursem/working_dir/CPU2017_bin/wrf_s.ppc64", NULL},
  // 44 -> pop2_s
  {"/home/malursem/working_dir/CPU2017_bin/speed_pop2.ppc64", NULL},
  // 45 -> imagick_r
  {"/home/malursem/working_dir/CPU2017_bin/imagick_r.ppc64", "-limit", "disk", "0", "/home/malursem/working_dir/CPU2017/538.imagick_r/refrate_input.tga", "-edge", "41", "-resample", "181%", "-emboss", "31", "-colorspace", "YUV", "-mean-shift", "19x19+15%", "-resize", "30%", "refrate_output.tga", NULL},
  // 46 -> nab_s
  {"/home/malursem/working_dir/CPU2017_bin/nab_s.ppc64", "3j1n", "20140317", "220", NULL},
  // 47 -> fotonik3d_r
  {"/home/malursem/working_dir/CPU2017_bin/fotonik3d_r.ppc64", NULL},
  // 48 -> roms_r
  {"/home/malursem/working_dir/CPU2017_bin/roms_r.ppc64", NULL},
  // 49 -> namd_r
  {"/home/malursem/working_dir/CPU2017_bin/namd_r.ppc64", "--input", "/home/malursem/working_dir/CPU2017/508.namd_r/apoa1.input", "--output", "apoa1.ref.output", "--iterations", "65", NULL},
  // 50 -> parest_r
  {"/home/malursem/working_dir/CPU2017_bin/parest_r.ppc64", "/home/malursem/working_dir/CPU2017/510.parest_r/ref.prm", NULL},
  // 51 -> povray_r
  {"/home/malursem/working_dir/CPU2017_bin/povray_r.ppc64", "/home/malursem/working_dir/CPU2017/511.povray_r/SPEC-benchmark-ref.ini", NULL},
  // 52 -> xz_r 2
  {"/home/malursem/working_dir_test/spec_bin2017/xz_r.ppc64", "/home/malursem/working_dir_test/CPU2017/557.xz_r/cpu2006docs.tar.xz", "250", "055ce243071129412e9dd0b3b69a21654033a9b723d874b2015c774fac1553d9713be561ca86f74e4f16f22e664fc17a79f30caa5ad2c04fbc447549c2810fae", "23047774", "23513385", "6e", NULL },
  // 53 -> xz_r 3
  {"/home/malursem/working_dir_test/spec_bin2017/xz_r.ppc64", "/home/malursem/working_dir_test/CPU2017/557.xz_r/input.combined.xz", "250", "a841f68f38572a49d86226b7ff5baeb31bd19dc637a922a972b2e6d1257a890f6a544ecab967c313e370478c74f760eb229d4eef8a8d2836d233d3e9dd1430bf", "40401484", "41217675", "7", NULL },
  // 54 -> exchange2_r
  {"/home/malursem/working_dir_test/spec_bin2017/exchange2_r.ppc64","6",NULL},
  // 55 -> perlbench_r diffmail
  {"/home/malursem/working_dir_test/spec_bin2017/perlbench_r.ppc64", "-I/home/malursem/working_dir_test/CPU2017/500.perlbench_r/lib", "/home/malursem/working_dir_test/CPU2017/500.perlbench_r/diffmail.pl", "4", "800", "10", "17", "19", "300", NULL},
  //* MEDIABENCH I *//
  //56 -> jpeg djpeg
  {"/home/malursem/benchmarks/Mediabench-for-IBM-Power/mediabench/jpeg/bin/djpeg", "-dct", "int", "-ppm", "-outfile", "/home/malursem/benchmarks/Mediabench-for-IBM-Power/mediabench/jpeg/data/testout.ppm", "/home/malursem/benchmarks/Mediabench-for-IBM-Power/mediabench/jpeg/data/imagen16k.jpg", NULL},
  //57 -> jpeg cjpeg
  {"/home/malursem/benchmarks/Mediabench-for-IBM-Power/mediabench/jpeg/bin/cjpeg", "-dct", "int", "-progressive", "-opt", "-outfile", "/home/malursem/benchmarks/Mediabench-for-IBM-Power/mediabench/jpeg/data/testout.jpg", "/home/malursem/benchmarks/Mediabench-for-IBM-Power/mediabench/jpeg/data/imagen16k.ppm", NULL}, 
  //58 -> mpeg2 mpeg2encode
  {"/home/malursem/benchmarks/Mediabench-for-IBM-Power/mediabench/mpeg2/bin/mpeg2encode", "/home/malursem/benchmarks/Mediabench-for-IBM-Power/mediabench/mpeg2/data/options.par", "/home/malursem/benchmarks/Mediabench-for-IBM-Power/mediabench/mpeg2/data/out.m2v", NULL},
  //59 -> mpeg2 mpeg2decode
  {"/home/malursem/benchmarks/Mediabench-for-IBM-Power/mediabench/mpeg2/bin/mpeg2decode", "-b", "/home/malursem/benchmarks/Mediabench-for-IBM-Power/mediabench/mpeg2/data/mei16v2.m2v", "-r", "-f", "-o0", "/home/malursem/benchmarks/Mediabench-for-IBM-Power/mediabench/mpeg2/data/rec%d", NULL},
  //60 -> gsm toast
  {"/home/malursem/benchmarks/Mediabench-for-IBM-Power/mediabench/gsm/bin/toast", "-fpl", "/home/malursem/benchmarks/Mediabench-for-IBM-Power/mediabench/gsm/data/clinton.pcm", NULL},
  //61 -> gsm untoast
  {"/home/malursem/benchmarks/Mediabench-for-IBM-Power/mediabench/gsm/bin/untoast", "-fpl", "/home/malursem/benchmarks/Mediabench-for-IBM-Power/mediabench/gsm/data/clinton.pcm.gsm", NULL},
  //62 -> adpcm rawcaudio
  {"/home/malursem/benchmarks/Mediabench-for-IBM-Power/mediabench/adpcm/bin/rawcaudio", "<", "/home/malursem/benchmarks/Mediabench-for-IBM-Power/mediabench/adpcm/data/clinton.pcm", ">", "/home/malursem/benchmarks/Mediabench-for-IBM-Power/mediabench/adpcm/results/out.adpcm", NULL},
  //63 -> adpcm rawdaudio
  {"/home/malursem/benchmarks/Mediabench-for-IBM-Power/mediabench/adpcm/bin/rawdaudio", "<", "/home/malursem/benchmarks/Mediabench-for-IBM-Power/mediabench/adpcm/data/clinton.adpcm", ">", "/home/malursem/benchmarks/Mediabench-for-IBM-Power/mediabench/adpcm/results/out.pcm", NULL},
  //64 -> g721 encode
  {"/home/malursem/benchmarks/Mediabench-for-IBM-Power/mediabench/g721/bin/encode", "-4", "-l", "<", "/home/malursem/benchmarks/Mediabench-for-IBM-Power/mediabench/g721/data/clinton.pcm", ">", "/home/malursem/benchmarks/Mediabench-for-IBM-Power/mediabench/g721/data/clinton.pcm.g721", NULL},
  //65 -> g721 decode
  {"/home/malursem/benchmarks/Mediabench-for-IBM-Power/mediabench/g721/bin/decode", "-4", "-l", "<", "/home/malursem/benchmarks/Mediabench-for-IBM-Power/mediabench/g721/data/clinton.g721", ">", "/home/malursem/benchmarks/Mediabench-for-IBM-Power/mediabench/g721/data/clinton.g721.pcm", NULL},
  //66 -> pgp fes
  {NULL, NULL, NULL},
  //67 -> pgp fdb
  {NULL, NULL, NULL},
  //68 -> pegwit encrypt
  {NULL, NULL, NULL},
  //69 -> pegwit decrypt
  {NULL, NULL, NULL},
  //70 -> ghostscript
  {NULL, NULL, NULL},
  //71 -> mesa
  {NULL, NULL, NULL},
  //72 -> sphere
  {NULL, NULL, NULL},
  //73 -> rasta
  {NULL, NULL, NULL},
  //74 -> epic epic
  {"/home/malursem/benchmarks/Mediabench-for-IBM-Power/mediabench/epic/bin/epic", "/home/malursem/benchmarks/Mediabench-for-IBM-Power/mediabench/epic/data/test_image.pgm", "-b", "25", NULL},
  //75 -> epic unepic
  {"/home/malursem/benchmarks/Mediabench-for-IBM-Power/mediabench/epic/bin/unepic", "/home/malursem/benchmarks/Mediabench-for-IBM-Power/mediabench/epic/data/test_image.pgm.E", NULL},
  //* MEDIABENCH II *//
  //76 -> jpeg cjpeg
  {NULL, NULL, NULL},
  //77 -> jpeg djpeg
  {NULL, NULL, NULL},
  //78 -> h263 enc
  {NULL, NULL, NULL},
  //79 -> h263 dec
  {NULL, NULL, NULL},
  //80 -> h264 enc
  {NULL, NULL, NULL},
  //81 -> h264 dec
  {NULL, NULL, NULL},
  //82 -> jpg2000 dec
  {NULL, NULL, NULL},
  //83 -> jpg2000 enc
  {NULL, NULL, NULL},
  //84 -> mpeg2 dec
  {NULL,NULL, NULL},
  //85 -> mpeg2 enc
  {NULL, NULL, NULL},
  //86 -> mpeg4 dec
  {NULL, NULL, NULL},
  //87 -> mpeg4 enc
  {NULL, NULL, NULL},
  //* ABench2020 *//
  //88 BinarySearchTree
  {"/home/malursem/benchmarks/ABench2020/BinarySearchTree/binary_search_tree", NULL, NULL},
  //89 BitwiseShift
  {"/home/malursem/benchmarks/ABench2020/BitwiseShift/bitwise_shift", NULL, NULL},
  //90 Correlation
  {"/home/malursem/benchmarks/ABench2020/Correlation/correlation", NULL, NULL},
  //91 CorrelationFI
  {"/home/malursem/benchmarks/ABench2020/CorrelationFI/correlation_fi", NULL, NULL},
  //92 Factorial
  {"/home/malursem/benchmarks/ABench2020/Factorial/factorial", NULL, NULL},
  //93 Fibonacci
  {"/home/malursem/benchmarks/ABench2020/Fibonacci/fibonacci", NULL, NULL},
  //94 LinearSearch
  {"/home/malursem/benchmarks/ABench2020/LinearSearch/linear_search", NULL, NULL},
  //95 LinkedList
  {"/home/malursem/benchmarks/ABench2020/LinkedList/linked_list", NULL, NULL},
  //96 MatrixMultiplication
  {"/home/malursem/benchmarks/ABench2020/MatrixMultiplication/matrix_multiplication", NULL, NULL}, 
  //97 PFactorial
  {"/home/malursem/benchmarks/ABench2020/PFactorial/pfactorial", NULL, NULL}, 
  //98 SelectionSort
  {"/home/malursem/benchmarks/ABench2020/SelectionSort/selection_sort", NULL, NULL},
  //99 Softmax
  {"/home/malursem/benchmarks/ABench2020/Softmax/softmax", NULL, NULL}, 
  //* EMSBench *//
  //100 emsbench
  {"/home/malursem/benchmarks/emsbench/emsbench-master/embedded/build/default-ubench/ubench-default.elf ", NULL, NULL}//,
  //* SPARK *//
  //101 spark-bench
  //{"/home/malursem/benchmarks/spark-bench/bin/spark-bench.sh", "/home/malursem/benchmarks/spark-bench/examples/minimal-example.conf", NULL}

};

/*************************************************************
 **                  get_counts                             **
 *************************************************************/

static void get_counts(perf_event_desc_t *fds, int num) {
  ssize_t ret;
  int i;

  for(i=0; i < num; i++) {
    uint64_t val;
    ret = read(fds[i].fd, fds[i].values, sizeof(fds[i].values));
    if (ret < (ssize_t)sizeof(fds[i].values)) {
      if (ret == -1){
        fprintf(stderr, "ERROR: cannot read values event %s\n", fds[i].name);
      } else{
        fprintf(stderr,"ERROR: could not read event %d\n", i);
      }
    }
    val = fds[i].values[0];
    pmu_counters[i] += val;
  }
}

/*************************************************************
 **                 measure                                 **
 *************************************************************/

int measure(pid_t *pid) {
  perf_event_desc_t *fds = NULL;
  int i, j, ret, num_fds = 0;
  int status;

  ret = perf_setup_list_events(options.events, &fds, &num_fds);
  if (ret || (num_fds == 0)) {
    exit(1);
  }
  fds[0].fd = -1;
  for(j=0; j < num_fds; j++) {
    fds[j].hw.disabled = 0; /* start immediately */
    /* request timing information necessary for scaling counts */
    fds[j].hw.read_format = PERF_FORMAT_SCALE;
    fds[j].hw.pinned = !j && options.pinned;
    fds[j].fd = perf_event_open(&fds[j].hw, *pid, -1, (options.group? fds[j].fd : -1), 0);
    if (fds[j].fd == -1) {
      fprintf(stderr, "ERROR: cannot attach event %s\n", fds[j].name);
    }
  }
  // Llibera els procesos
  kill(*pid, 18);
  waitpid(*pid, &status, WCONTINUED);
  if (WIFEXITED(status)) {
     fprintf(stderr,"ERROR: command process %d exited too early with status %d\n", *pid, WEXITSTATUS(status));
  }
  usleep(options.delay*1000);
  // Bloqueja els procesos
  ret = 0;
  kill(*pid, 19);
  waitpid(*pid, &status, WUNTRACED);
  if (WIFEXITED(status)) {
    fprintf(stderr,"INFO: Process %d finished with status %d\n", *pid, WEXITSTATUS(status));
    ret = 1;
  }
  get_counts(fds, num_fds);
  for(i=0; i < num_fds; i++) {
    close(fds[i].fd);
  }
  perf_free_fds(fds, num_fds);
  return ret;
}

/*************************************************************
 **                      initialize_events                       **
 *************************************************************/

int initialize_events() {
  perf_event_desc_t *fds = NULL;
  int i, ret, num_fds = 0;

  ret = perf_setup_list_events(options.events, &fds, &num_fds);
  if (ret || (num_fds == 0)) {
    exit(1);
  }
  for (i=0; i < num_fds; i++) {
    events[i] = strdup(fds[i].name);
    pmu_counters[i] = 0;
  }
  for(i=0; i < num_fds; i++) {
    close(fds[i].fd);
  }
  perf_free_fds(fds, num_fds);
  return num_fds;
}

/*************************************************************
 **                          usage                          **
 *************************************************************/

static void usage(void) {
  fprintf(stdout,"usage: [PROGRAM] [-h] [-P] [-g] [-t (temps secs)] [-d delay (msecs)] [-e cycles,event2,...] A prgA\n");
}

/*************************************************************
 **                     MAIN PROGRAM                        **
 *************************************************************/

int main(int argc, char **argv) {

  fprintf(stdout,"\nInstructions counter. Year: 2020 Author: Manel Lurbe Sempere <malursem@gap.upv.es>.\n");

  int c, ret, temps=-1, q=0, i;
  pid_t pid;
  int prg;
  cpu_set_t mask;
  int status, num_fds;
  FILE *fitxer;
  int max_quantums = -1;
  int nucli = -1;

  prg = -1;
  options.verbose = 0;
  options.delay = 0;

  while ((c=getopt(argc, argv,"he:d:t:pvgPA:Q:C:")) != -1) {
    switch(c) {
    case 'e':
      options.events = optarg;
      break;
    case 'P':
      options.pinned = 1;
      break;
    case 'g':
      options.group = 1;
      break;
    case 'd':
      options.delay = atoi(optarg);
      break;
    case 'h':
      usage();
      exit(0);
    case 'A':
      prg = atoi(optarg);
      break;
    case 't':
      temps = atoi(optarg);
      break;
    case 'v':
      options.verbose = 1;
      break;
    case 'Q':
      max_quantums = atoi(optarg);
      break;
    case 'C':
        nucli = atoi(optarg);
        break;
    default:
      fprintf(stderr, "Unknown error\n");
    }
  }
  setenv("OMP_NUM_THREADS", "1", 1);

  if (!options.events) {
    options.events = strdup("cycles,instructions");
  }

  if (options.delay < 1) {
    options.delay = 100;
  }

  if (prg < 0) {
    fprintf(stderr, "ERROR: The process needs to be specified.\n");
    return -1;
  }

  if (max_quantums < 0) {
    if (temps < 0) {
      fprintf(stderr, "ERROR. Max time or quanta should be especified.\n");
      exit (-1);
    }
    fprintf(stderr, "INFO: Time: %d, Delay: %d\n", temps, options.delay);
    max_quantums = temps * 1000 / options.delay;
    fprintf(stderr, "INFO: Number of quanta set to %d.\n", max_quantums);
  } else {
    fprintf(stderr, "INFO: Number of quanta defined to %d.\n", max_quantums);
  }

  if (nucli == -1) {
    CPU_ZERO(&mask);
    CPU_SET(48, &mask);
    CPU_SET(56, &mask);
    CPU_SET(64, &mask);
    CPU_SET(0, &mask);
  } else {
    CPU_ZERO(&mask);
    CPU_SET(nucli, &mask);
  }

  if (pfm_initialize() != PFM_SUCCESS) {
    fprintf(stderr,"ERROR: libpfm initialization failed\n");
  }
  // Preparation
  num_fds = initialize_events();
  do {
    pid = fork();
    switch (pid) {
      case -1: // ERROR
        fprintf(stderr,"ERROR: Couldn't create the child.\n");
        return -1;

      case 0: // Child
        // Descriptors for those who have input for standard input
        switch(prg) {
          case 4: // [Doesn't work]
            close(0);
            fitxer = fopen("/home/malursem/working_dir_test/CPU2006/445.gobmk/data/ref/input/13x13.tst", "r");
            if (fitxer == NULL) {
              fprintf(stderr,"ERROR. The file could not be opened: working_dir/CPU2006/445.gobmk/data/ref/input/13x13.tst.\n");
              return -1;
            }
            break;

          case 9:
            system("cp /home/malursem/working_dir_test/omnetpp_2006.ini /home/malursem/working_dir_test/omnetpp.ini  >/dev/null 2>&1");
            break;

          case 13:
            close(0);
            fitxer = fopen("/home/malursem/working_dir/CPU2006/416.gamess/data/ref/input/h2ocu2+.gradient.config", "r");
            if (fitxer == NULL) {
              fprintf(stderr,"ERROR. The file could not be opened: working_dir/CPU2006/416.gamess/data/ref/input/h2ocu2+.gradient.config.\n");
              return -1;
            }
            break;

          case 14:
            close(0);
            fitxer = fopen("/home/malursem/working_dir/CPU2006/433.milc/data/ref/input/su3imp.in", "r");
            if (fitxer == NULL) {
              fprintf(stderr,"ERROR. The file could not be opened: working_dir/CPU2006/433.milc/data/ref/input/su3imp.in.\n");
              return -1;
            }
            break;

          case 18:
            close(0);
            fitxer = fopen("/home/malursem/working_dir/CPU2006/437.leslie3d/data/ref/input/leslie3d.in", "r");
            if (fitxer == NULL) {
              fprintf(stderr,"ERROR. The file could not be opened: working_dir/CPU2006/437.leslie3d/data/ref/input/leslie3d.in.\n");
              return -1;
            }
            break;

          case 22:
            close(2);
            fitxer = fopen("/home/malursem/working_dir/povray.sal", "w");
            if (fitxer == NULL) {
              fprintf(stderr,"ERROR. The file could not be opened: working_dir/povray.sal\n");
              return -1;
            }
            break;

          case 48:
            close(0);
            fitxer = fopen("/home/malursem/working_dir_test/CPU2017/554.roms_r/ocean_benchmark2.in.x", "r");
            if (fitxer == NULL) {
              fprintf(stderr,"ERROR. The file could not be opened: working_dir_test/CPU2017/554.roms_r/ocean_benchmark2.in.x.\n");
              return -1;
            }
            break;
        }
        close(1);
        fitxer = fopen("/home/malursem/working_dir_test/rundir/sal.log", "w");
        close(2);
        fitxer = fopen("/home/malursem/working_dir_test/rundir/err.log", "w");
        execv(benchmarks[prg][0], benchmarks[prg]);
        fprintf(stderr,"ERROR: Couldn't launch the program %d.\n", prg);
        return -1;

      default:  // Parent                                                                               
        usleep (200000);  // Wait 200 ms
        // Assign the process to a core
        if (sched_setaffinity(pid, sizeof(mask), &mask) != 0) { // Instead of sizeof putting 1 was fine
          fprintf(stderr,"ERROR: Sched_setaffinity %d.\n", errno);
          exit(1);
        }
        // We stop the process
        kill (pid, 19);  
        waitpid(pid, &status, WUNTRACED);
        if (WIFEXITED(status)) {
          fprintf(stderr,"ERROR: command process %d exited too early with status %d\n", pid, WEXITSTATUS(status));
        }
    }
    for (; q<max_quantums; q++) {
      ret = measure(&pid);
      if (ret) {
        fprintf(stderr, "INFO: The process is complete before completing all quantums.\n");
        break;
      }
    }
  } while (q < max_quantums-1);
  kill(pid, 9);
  // Print values for cycles and instructions executed before exiting the scheduler.
  fprintf(stderr, "PMU_COUNTS:");
  for (i=0; i<num_fds; i++) {
    fprintf(stderr, "\t%"PRIu64"", pmu_counters[i]);
  }
  fprintf(stderr, "\n");  
  // Free libpfm resources cleanly
  pfm_terminate();
  return 0;
}

#!/bin/sh
# analysis.sh
# ASA 2018
# Gabriel Figueira, Rafael Andrade
# P1 (Sr. Joao Caracol)

set -e

ORIG_DIR=$(pwd)
cd $(dirname "$0")

EXECUTABLE="../asa.out"
TIME='command time -f "%U+%S"'
N_RUNS=1
N_EXCLUDE=0
TMP_FILE=$(mktemp)
DEFAULT_CSV="../pratical_analysis.csv"
TESTS_DIR="./students-tests"
DEFAULT_GEN="./gera"
N_INTERVALS=15
N_RUNS_PER_INTERVAL=2

memory(){
  if [ "$1" ]  && [ -f "$1" ] ; then
    printf "  - Heap allocation test: " >&2
    valgrind --tool=massif --massif-out-file="$TMP_FILE" ../asa.out <"$1"  >/dev/null 2>&1
    grep mem_heap_B "$TMP_FILE" | sed -e 's/mem_heap_B=\(.*\)/\1/' | sort -g | tail -n 1
    printf "done.\n" >&2
  fi
}

run_time(){
  #Using bash more precise timing
  if [ "$1" ]  && [ -f "$1" ] ; then
    printf "  - Time tests (doing: %d, excluding: %dx2): " "$N_RUNS" "$N_EXCLUDE" >&2
    TIME_SUM=$(for i in $(seq $N_RUNS); do $TIME $EXECUTABLE <"$1" 2>&1 1>/dev/null | bc; done \
                    | sort | head -n $(($N_RUNS - $N_EXCLUDE)) | tail -n +$(($N_EXCLUDE+1)) | tr '\n' + )
    printf "scale=4; (%s0)/%d\n"  "$TIME_SUM" $(($N_RUNS - 2*$N_EXCLUDE)) | bc
    printf "done.\n" >&2
 fi
}

csv_line(){

  if [ "$1" ] && [ -f "$1" ] ; then
    M=`head -n1 $1| cut -d ' ' -f 1`
    N=`head -n1 $1| cut -d ' ' -f 2`
    printf "%s,%s,%s,%.3f\n" "$M" "$N" `memory "$1"` `run_time "$1"`
  fi

}


example_tests(){
  for f in "$TESTS_DIR"/*.in ; do
    csv_line "$f"
  done >"$DEFAULT_CSV"
}


random_tests() {

  local F=`mktemp`
  local N_V
  echo "Starting big random tests" >&2
  for i in $(seq $N_INTERVALS); do
    local BASE=$(($i *50 + 100))
    local MAX=$(($BASE + 50))
    printf "Interval number %d; range: %d-%d\n" "$i" "$BASE" "$MAX" >&2
    for j in $(seq $N_RUNS_PER_INTERVAL); do
      N_V=`shuf -i $BASE-$MAX -n1`
      printf "Test %d/%d; size: %d\n" "$j" "$N_RUNS_PER_INTERVAL" "$N_V" >&2
      $DEFAULT_GEN $N_V 0 >$F
      csv_line "$F"
   done
  done >>"$DEFAULT_CSV"
  rm "$F"

}

>"$DEFAULT_CSV"
random_tests
rm "$TMP_FILE"
cd "$ORIG_DIR"

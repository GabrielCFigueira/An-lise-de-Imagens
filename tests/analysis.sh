#!/bin/sh
# analysis.sh
# ASA 2018
# Gabriel Figueira, Rafael Andrade
# P1 (Sr. Joao Caracol)

set -e

ORIG_DIR=$(pwd)
cd $(dirname "$0")

EXECUTABLE="../asa.out"
N_RUNS=5
N_EXCLUDE=1
TMP_FILE=$(mktemp)
DEFAULT_CSV="../pratical_analysis.csv"
DEFAULT_DIR="./students-tests"
DEFAULT_GEN="./gerador"
N_INTERVALS=9
N_RUNS_PER_INTERVAL=5

memory(){
  if [ "$1" ]  && [ -f "$1" ] ; then
    valgrind --tool=massif --massif-out-file="$TMP_FILE" ../asa.out <"$1"  >/dev/null 2>&1
    grep mem_heap_B "$TMP_FILE" | sed -e 's/mem_heap_B=\(.*\)/\1/' | sort -g | tail -n 1
  fi
}

run_time(){
  #Using bash more precise timing
  if [ "$1" ]  && [ -f "$1" ] ; then
    TIMEFORMAT="%U+%S"
    local TIME_SUM=$(for i in $(seq $N_RUNS); do $(which time) -f "$TIMEFORMAT" "$EXECUTABLE" <"$1" 2>&1 1>/dev/null | bc; done \
                      | sort | head -n $(($N_RUNS - $N_EXCLUDE)) | tail -n +$(($N_EXCLUDE+1)) | tr '\n' + )
    printf "scale=4; (%s0)/%d\n"  "$TIME_SUM" $(($N_RUNS - 2*$N_EXCLUDE)) | bc
  fi
}

csv_line(){

  if [ "$1" ] && [ -f "$1" ] ; then
    V=`head -n1 $1`
    E=`head -n2 $1 | tail -n +2`
    printf "%s,%s,%s,%.3f\n" "$V" "$E" `memory "$1"` `run_time "$1"`
  fi

}


csv_file(){
  for f in "$DEFAULT_DIR"/*.in ; do
    csv_line "$f"
  done >"$DEFAULT_CSV"
}


random_tests() {

  local F=`mktemp`
  echo "Starting big random tests" >&2
  for i in $(seq $N_INTERVALS); do
    printf "Interval number %d\n" "$i" >&2
    local BASE=$(($i * 100000))
    local MAX=$(($BASE + 100000))
    for j in $(seq $N_RUNS_PER_INTERVAL); do
      printf "Test %d/5\n" "$j" >&2
      local N_V=`shuf -i $BASE-$MAX -n1`
      local N_E=$((`shuf -i 2-6 -n1` * $N_V))
      local N_SCC=$(($N_V / `shuf -i 2-100 -n1`))
      local SEED=`shuf -i 1-4096 -n1`
      $DEFAULT_GEN $N_V $N_E $N_SCC 1 1000 $SEED >$F
      csv_line "$F"
    done
  done >>"$DEFAULT_CSV"

}

>"$DEFAULT_CSV"
random_tests
cd "$ORIG_DIR"

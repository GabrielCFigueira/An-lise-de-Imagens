#!/bin/sh

TESTS_DIR="./students-tests"
INPUTS=".in"
OUTPUTS_EXPECTED=".out"
OUTPUTS_GENERATED=".gen"
DEFAULT_EXE="../asa.out"
TEMP_FILE=$(mktemp)
TIME="command time -f %U"

run_single_test(){
	if [ -r "$1""$INPUTS" ]; then
		echo -n "Test ${1}\t"
		$TIME $EXEC < "$1""$INPUTS" >"$1""$OUTPUTS_GENERATED" 2>"$TEMP_FILE"

	fi
}

verify_single_test(){
	local time_used;
	if [ -r "$1""$OUTPUTS_GENERATED" ]; then

		time_used=`head "$TEMP_FILE"`

		cmp -s "$1""$OUTPUTS_GENERATED" "$1""$OUTPUTS_EXPECTED"

		if [ "$?" -eq 0 ]; then
			echo  "\b\t\tTest passed\t\t$time_used"
			rm "$1""$OUTPUTS_GENERATED"
			rm "$TEMP_FILE"
		else
			echo  "\t\tTest failed /!\\"
		fi
	fi
}



run_and_verify_all() {

	local base;
	for t in "$TESTS_DIR"/t[0-4][0-9]*"$INPUTS"; do
		base="${t%$INPUTS}"
		run_single_test "$base"
		verify_single_test "$base"
	done
}


if [ -n "$1" ]; then
	EXEC="$1"
else
	EXEC="$DEFAULT_EXE"
fi


run_and_verify_all;

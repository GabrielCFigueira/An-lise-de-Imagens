#!/bin/sh

TESTS_DIR="./students-tests"
INPUTS=".in"
OUTPUTS_EXPECTED=".out"
OUTPUTS_GENERATED=".gen"
DEFAULT_EXE="../asa.out"

run() {

	local base;
	for t in "$TESTS_DIR"/*"$INPUTS"; do
		echo $t
		base="${t%$INPUTS}"
		$EXEC < "$t" >"$base""$OUTPUTS_GENERATED"
	done
}

compare() {
	for attempt in "$TESTS_DIR"/*"$OUTPUTS_GENERATED"; do
		echo -n "Test of input ${attempt##*/}: "
		base="${attempt%$OUTPUTS_GENERATED}"
		cmp -s "$attempt" "$base""$OUTPUTS_EXPECTED"

		if [ "$?" -eq 0 ]; then
			echo  "\t\tTest passed"
			rm "$attempt"
		else
			echo  "\t\tTest failed /!\\"
		fi
	done
}


if [ -n "$1" ]; then
	EXEC="$1"
else
	EXEC="$DEFAULT_EXE"
fi


run;
compare;

#!/bin/bash

DEFAULT_RUN_COUNT=10

HOST=$(hostname)
if [ "$HOST" == "u2" ]
then
	SLEEP_PERIOD=1
else
	SLEEP_PERIOD=0.25
fi

NO_FILTER_PARAM="1"
FILTER_PARAM="2"

NO_FILTER_RESULTS=0
FILTER_RESULTS=0

TEMP_FILE=".compare_tmp"

print_ret() {
	if [ $1 -eq 0 ]
	then
		echo "✅ Passed"
	else
		echo "❌ Failed"
	fi
}

run_program() {
	echo "Running with parameters: $1 $2"
	timeout $SLEEP_PERIOD stdbuf -oL ./showBMP "$1" $2 > $TEMP_FILE
	time_took=`cat $TEMP_FILE | grep "Total runtime:" | cut -d " " -f3`
	echo "Command took $time_took ms"
	if [ $2 == $NO_FILTER_PARAM ]
	then
		NO_FILTER_RESULTS=$(echo "$NO_FILTER_RESULTS + $time_took" | bc)
	elif [ $2 == $FILTER_PARAM ]
	then
		FILTER_RESULTS=$(echo "$FILTER_RESULTS + $time_took" | bc)
	fi
}

RUN_COUNT=$1

if [ "$RUN_COUNT" == "" ]
then
	RUN_COUNT=$DEFAULT_RUN_COUNT
fi

echo "Running test $RUN_COUNT time(s)..."

for i in `seq 1 $RUN_COUNT`
do
	run_program gibson_500.bmp $NO_FILTER_PARAM

	echo "==== Checking Blur ===="
	cmp Blur.bmp Blur_correct.bmp
	print_ret $?
	printf "\n"

	echo "==== Checking Sharpen ===="
	cmp Sharpen.bmp Sharpen_correct.bmp
	print_ret $?
	printf "\n"

	run_program gibson_500.bmp $FILTER_PARAM

	echo "==== Checking Filtered + Blur ===="
	cmp Filtered_Blur.bmp Filtered_Blur_correct.bmp
	print_ret $?
	printf "\n"

	echo "==== Checking Filtered + Sharpen ===="
	cmp Filtered_Sharpen.bmp Filtered_Sharpen_correct.bmp
	print_ret $?
	printf "\n"

done

echo "======== RUNTIMES ======="
printf "With filter average:\t$(echo "scale=6; $FILTER_RESULTS / $RUN_COUNT" | bc) ms\n"
printf "No filter average:\t$(echo "scale=6; $NO_FILTER_RESULTS / $RUN_COUNT" | bc) ms\n"

echo "Test script was written by Ron Even"
echo "Thank you for using it!"
rm -f $TEMP_FILE

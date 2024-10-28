#!/bin/bash

# clear the folders:

	# remove result.txt
	if [ -f result.txt ]; then
		rm -f result.txt	
	fi

	# remove previous outputs
	if [ -e temp ]; then
		rm -f temp/test*/*
	fi

	# remove submission files
	rm -f submission/*

	# remove grades.txt
	if [ -f grades.txt ]; then
		rm -f grades.txt	
	fi

##############################################
############## START EVALUATION ##############
##############################################

for i in $(ls all_submits); do

	cp all_submits/"$i"/* submission/
	./run_tests $i

	# remove result.txt
	if [ -f result.txt ]; then
		rm -f result.txt	
	fi

	# remove previous outputs
	if [ -e temp ]; then
		rm -f temp/test*/*
	fi

	# remove submission files
	rm -f submission/*

done

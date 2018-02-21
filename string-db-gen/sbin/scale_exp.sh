#!/bin/bash

rm output/*

BACKUP_PATH=~/vol1/pinnecke/string-db-gen/datasets

SCENARIOS='synth social base instant'

for SCENARIO_NAME in $SCENARIOS
do
	for ((TARGET_DATASIZE_BYTE=1048576; TARGET_DATASIZE_BYTE <= 5242880 ; TARGET_DATASIZE_BYTE += 1048576))
	do  	
		PROFILE_NAME="$SCENARIO_NAME-size-${TARGET_DATASIZE_BYTE}B"
		DATASET_NAME=$PROFILE_NAME

		java -jar build/string-db-gen.jar -s ${SCENARIO_NAME} -t ${TARGET_DATASIZE_BYTE} -o output/dataset-temp.csv -i output/statistics-sizes.csv -u $PROFILE_NAME && cat output/statistics-sizes.csv
		mv output/dataset-temp.csv ${BACKUP_PATH}/${DATASET_NAME}.csv

	done
done



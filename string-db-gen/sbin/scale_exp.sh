#!/bin/bash

rm output/*

BACKUP_PATH=~/vol1/pinnecke/string-db-gen/datasets

SCENARIOS='base social synth instant'

for SCENARIO_NAME in $SCENARIOS
do
	for ((TARGET_DATASIZE_BYTE=5368709120; TARGET_DATASIZE_BYTE >= 1073741824 ; TARGET_DATASIZE_BYTE -= 1073741824))
	do  	
		PROFILE_NAME="$SCENARIO_NAME-size-${TARGET_DATASIZE_BYTE}B"
		DATASET_NAME=$PROFILE_NAME

		java -jar build/string-db-gen.jar -s ${SCENARIO_NAME} -t ${TARGET_DATASIZE_BYTE} -o output/dataset-temp.csv -i output/statistics-sizes.csv -u $PROFILE_NAME && cat output/statistics-sizes.csv
		mv output/dataset-temp.csv ${BACKUP_PATH}/${DATASET_NAME}.csv

	done
done



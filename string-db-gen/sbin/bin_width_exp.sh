
#!/bin/bash

TARGET_DATASIZE_BYTE=52428800

rm output/*

SCENARIOS='base social synth'

for ((sample=0; sample < 25 ; sample++))
do  
	for ((i=1; i <= 10000 ; i *= 10))
	do  
		for SCENARIO in $SCENARIOS
    	do 
    		BIN_WIDTH=$i
    		SCENARIO_NAME=$SCENARIO
    		PROFILE_NAME="$SCENARIO_NAME-profile-w-$BIN_WIDTH"
			java -jar build/string-db-gen.jar --clean-cache -w $BIN_WIDTH -a dewiki-dataset/files/dewiki-articles-word-freq.csv -b dewiki-dataset/files/dewiki-articles-next-words.txt -c dewiki-dataset/files/dewiki-articles-starter-words.csv -d dewiki-dataset/files/dewiki-articles-lengths.csv -e dewiki-dataset/files/twitter-msg-lengths.csv -s ${SCENARIO_NAME} -t ${TARGET_DATASIZE_BYTE} -o output/${SCENARIO_NAME}.csv -i "output/statistics.csv" -u $PROFILE_NAME && cat output/statistics.csv
		done			
	done
done




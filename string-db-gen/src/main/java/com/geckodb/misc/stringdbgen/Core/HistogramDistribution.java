package com.geckodb.misc.stringdbgen.Core;

import com.geckodb.misc.utils.FileUtils;

import java.io.*;
import java.util.*;

/**
 * Created by marcus on 13.02.18.
 */
public class HistogramDistribution {

    class Entry implements Comparable<Entry> {
        int length;
        int threshold;

        Entry(int length, int threshold) {
            this.length = length;
            this.threshold = threshold;
        }

        @Override public int compareTo(Entry other) {
            return Integer.compare(this.threshold, other.threshold);
        }
    }

    List<Entry> entries = new ArrayList<>();
    int thresholdMax = 0;
    Random random = new Random();

    public HistogramDistribution(String histogramFile) throws IOException {

        String line;
        int last = Integer.MAX_VALUE;
        BufferedReader reader = FileUtils.openRead(histogramFile);
        reader.readLine(); // Skip header

        // Get Bin size: a bit crappy but works for now
        while ((line = reader.readLine()) != null) {
            String[] fields = line.split(";");
            int length = Integer.valueOf(fields[0]);
            int threshold = Integer.valueOf(fields[2]);
            entries.add(new Entry(length, threshold));
            this.thresholdMax = Math.max(this.thresholdMax, threshold);
        }

        reader.close();

        Collections.sort(entries);
    }

    public int next() {
        int choose = Math.max(1, this.random.nextInt(this.thresholdMax));
        Entry needle = new Entry(0, choose);
        int index = Collections.binarySearch(this.entries, needle);

        int lower, upper;

        if (index < 0) {
            index = -index;
            lower = (index - 2 >= 0) ? this.entries.get(index - 2).length : this.entries.get(0).length; // in this case, the found entry is the lower bound
            upper = this.entries.get(index - 1).length; // in this case, the found entry is the upper bound
        } else {
            // exact match on boundary
            lower = (index == 0) ? 0 : this.entries.get(index - 1).length;
            upper = this.entries.get(index).length;
        }

        int span = Math.abs(upper - lower);
        int rand = this.random.nextInt(span);
        return lower + span/2;//+ rand;
    }

    public static void main(String... args) throws IOException {

        HistogramDistribution numberGenerator = new HistogramDistribution("/Users/marcus/git/gecko/gecko-misc/string-db-gen/cache/sentence-lengths.cache");
        System.setOut(new PrintStream(new FileOutputStream("/Users/marcus/temp/numbers15")));
        System.out.println("number");
        while(true) {
            System.out.println(numberGenerator.next());
           // System.err.println(numberGenerator.next());
        }


    }

}

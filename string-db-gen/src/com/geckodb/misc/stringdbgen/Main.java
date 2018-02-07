package com.geckodb.misc.stringdbgen;

import org.apache.commons.cli.*;

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.text.DecimalFormat;
import java.util.concurrent.atomic.AtomicReferenceArray;

public class Main {

    public static final String SCENARIO_NAME_SOCIAL = "social";
    public static final String SCENARIO_NAME_INSTNAT = "instant";
    public static final String SCENARIO_NAME_SYNTH = "synth";
    public static final String SCENARIO_NAME_SBASE = "base";

    public static String formatTimeSpan(long seconds) {
        long s = seconds % 60;
        long m = (seconds / 60) % 60;
        long h = (seconds / (60 * 60)) % 24;
        return String.format("%dh %02dmin %02dsec", h,m,s);
    }

    public static String formatByte(long bytes) {
        if (bytes < 1024) return bytes + " B";
        int exp = (int) (Math.log(bytes) / Math.log(1024));
        String pre = ("KMGTPE").charAt(exp-1) + ("i");
        return String.format("%.1f %sB", bytes / Math.pow(1024, exp), pre);
    }

    public static void main(String[] args) throws InterruptedException {

        Options options = new Options();

        Option scenario = new Option("s", "scenario", true, "scenario name ('"+SCENARIO_NAME_SOCIAL+"', '"+SCENARIO_NAME_INSTNAT+"', '"+SCENARIO_NAME_SYNTH+"', '"+SCENARIO_NAME_SBASE+"')");
        scenario.setRequired(true);
        options.addOption(scenario);

        Option output = new Option("o", "output", true, "output file name");
        output.setRequired(true);
        options.addOption(output);

        Option size = new Option("t", "target", true, "target size in byte");
        size.setRequired(true);
        options.addOption(size);

        Option wordfreq = new Option("a", "word-freq-file", true, "path to word frequency file");
        wordfreq.setRequired(false);
        options.addOption(wordfreq);

        Option nextwords = new Option("b", "next-words-file", true, "path to next words file");
        nextwords.setRequired(false);
        options.addOption(nextwords);

        Option starterwords = new Option("c", "starter-words-file", true, "path to starter words file");
        starterwords.setRequired(false);
        options.addOption(starterwords);


        CommandLineParser parser = new DefaultParser();
        HelpFormatter formatter = new HelpFormatter();
        CommandLine cmd;

        try {
            cmd = parser.parse(options, args);
            String scenarioName = cmd.getOptionValue('s');
            String outputFileName = cmd.getOptionValue('o');
            String targetSize = cmd.getOptionValue('t');


            String wordFrequencyFile = cmd.getOptionValue('a');
            String subsequentWordsFile = cmd.getOptionValue('b');
            String starterWordsFile = cmd.getOptionValue('c');

            wordFrequencyFile = (wordFrequencyFile == null) ? "dwiki-dataset/files/dewiki-articles-word-freq.csv" : wordFrequencyFile;
            subsequentWordsFile = (subsequentWordsFile == null) ? "dwiki-dataset/files/dewiki-articles-next-words.txt" : subsequentWordsFile;
            starterWordsFile = (starterWordsFile == null) ? "dwiki-dataset/files/dewiki-articles-starter-words.csv" : starterWordsFile;

            if (!Files.exists(Paths.get(wordFrequencyFile)) || !Files.exists(Paths.get(subsequentWordsFile)) || !Files.exists(Paths.get(starterWordsFile))) {
                System.out.println("Unable open analysis files (word frequency, next words, and/or starter words). " +
                        "Did you forgot to download these files ('https://www.dropbox.com/s/vhbq52vrti8isxx/dewiki-dataset.tar.gz?dl=1') or to put them into 'dwiki-dataset/files/' directory?");
                formatter.printHelp("stringdbgen", options);
                System.exit(1);
            }

            try {
                long maxSize[] = {Long.valueOf(targetSize)};

                if (!scenarioName.equalsIgnoreCase(SCENARIO_NAME_SOCIAL) &&
                        !scenarioName.equalsIgnoreCase(SCENARIO_NAME_INSTNAT) &&
                        !scenarioName.equalsIgnoreCase(SCENARIO_NAME_SYNTH) &&
                        !scenarioName.equalsIgnoreCase(SCENARIO_NAME_SBASE)) {
                    System.out.println("Unknown scenario name: '" + scenarioName + "'");
                    formatter.printHelp("stringdbgen", options);
                    System.exit(1);
                }

                try(FileOutputStream fileStream = new FileOutputStream(outputFileName, true))
                {
                    System.out.println("This is String-DB-Gen. Copyright (c) Marcus Pinnecke 2018\nhttps://github.com/geckodb/gecko-misc/string-db-gen");
                    System.setOut(new PrintStream(new BufferedOutputStream(fileStream)));


                    BenchmarkGenerator benchmarkGenerator = new BenchmarkGenerator(BenchmarkGenerator.Scenarios.INSTANT_MESSAGING_SERVICE,
                            wordFrequencyFile, subsequentWordsFile, starterWordsFile);

                    final int[] total = {0};
                    final long[] totalSize = {0};
                    long start = System.currentTimeMillis();

                    System.out.println("id;total_size_byte;length;string");

                    benchmarkGenerator.generateString(new BenchmarkGenerator.Callback() {

                        DecimalFormat formatter = new DecimalFormat("#.#######");

                        @Override
                        public BenchmarkGenerator.ContinueState consume(AtomicReferenceArray<String> strings) {
                            for (int i = 0; i < strings.length(); i++) {
                                String s = strings.get(i);
                                if (s != null) {
                                    totalSize[0] += s.length() + 1;
                                    System.out.println(total[0]++ + ";" + totalSize[0] + ";" + s.length() + ";" + strings.get(i));
                                }
                            }
                            System.out.flush();

                            long diff = System.currentTimeMillis() - start;
                            float percent = 100 * totalSize[0]/(float)(maxSize[0]);
                            long exp = (long) (diff * 100 / percent);
                            String eta = formatTimeSpan((long)(exp / 1000));
                            String elpased = formatTimeSpan((long)(diff / 1000));
                            System.err.println("Elapsed: " + elpased + "\t\tETA: " + eta + "\t\t" + formatByte(totalSize[0]) + " of " + formatByte(maxSize[0]) + "\t\t" + formatter.format(percent) + "%");

                            if (totalSize[0] >= maxSize[0]) {
                                System.exit(0);
                            }

                            return BenchmarkGenerator.ContinueState.CONTINUE;
                        }
                    });


            } catch (NumberFormatException e) {
                System.out.println("Unable to parse target size: '" + targetSize + "'");
                formatter.printHelp("stringdbgen", options);
                System.exit(1);
            }



            } catch (IOException e) {
                System.out.println("Output file cannot be opened:: '" + outputFileName + "'");
                formatter.printHelp("stringdbgen", options);
                System.exit(1);
            }

        } catch (ParseException e) {
            System.out.println(e.getMessage());
            formatter.printHelp("stringdbgen", options);
            System.exit(1);
        }

    }
}

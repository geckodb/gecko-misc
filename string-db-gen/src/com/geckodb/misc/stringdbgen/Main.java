package com.geckodb.misc.stringdbgen;

import com.geckodb.misc.stringdbgen.Core.BenchmarkGenerator;
import com.geckodb.misc.stringdbgen.Core.TextPreProcessor;
import com.geckodb.misc.utils.StringUtils;
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
    public static final String SCENARIO_NAME_BASE = "base";
    public static final String SCENARIO_NAME_BASE_ZIPF = "base-zipf";

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

        Option scenario = new Option("s", "scenario", true, "scenario name ('"+SCENARIO_NAME_SOCIAL+"', '"+SCENARIO_NAME_INSTNAT+"', '"+SCENARIO_NAME_SYNTH+"', '"+ SCENARIO_NAME_BASE +"', '" + SCENARIO_NAME_BASE_ZIPF + "')");
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

        Option lengthhist = new Option("d", "length-histogram-file", true, "path to sentence length histogram file");
        lengthhist.setRequired(false);
        options.addOption(lengthhist);

        Option cleanCache = new Option("x", "clean-cache", false, "cleans the cache and considers -a, -b, -c, -d, -w again");
        cleanCache.setRequired(false);
        options.addOption(cleanCache);

        Option cachePathOption = new Option("p", "cache-path", false, "sets directory in which the cache is stored (default is working directory)");
        cachePathOption.setRequired(false);
        options.addOption(cachePathOption);

        Option binwidth = new Option("w", "hist-bin-width", true, "bin width for histograms on sentence lengths (default is 1)");
        binwidth.setRequired(false);
        options.addOption(binwidth);



        CommandLineParser parser = new DefaultParser();
        HelpFormatter formatter = new HelpFormatter();
        CommandLine cmd;

        try {
            cmd = parser.parse(options, args);



            String wordFrequencyFile = cmd.getOptionValue('a');
            String subsequentWordsFile = cmd.getOptionValue('b');
            String starterWordsFile = cmd.getOptionValue('c');
            String lengthHistogramFile = cmd.getOptionValue('d');
            String cachePath = cmd.getOptionValue('p');
            boolean cleanCacheFlag = cmd.hasOption('x');
            int binSize = Integer.valueOf(cmd.getOptionValue('w') != null ? cmd.getOptionValue('w') : "1");

            wordFrequencyFile = (wordFrequencyFile == null) ? "dewiki-dataset/files/dewiki-articles-word-freq.csv" : wordFrequencyFile;
            subsequentWordsFile = (subsequentWordsFile == null) ? "dewiki-dataset/files/dewiki-articles-next-words.txt" : subsequentWordsFile;
            starterWordsFile = (starterWordsFile == null) ? "dewiki-dataset/files/dewiki-articles-starter-words.csv" : starterWordsFile;
            lengthHistogramFile = (lengthHistogramFile == null) ? "dewiki-dataset/files/dewiki-articles-lengths.txt" : lengthHistogramFile;

            cachePath = StringUtils.ensurePath((cachePath == null) ? System.getProperty("user.dir") + "/cache" : cachePath);

            TextPreProcessor preProcessor = new TextPreProcessor(wordFrequencyFile, subsequentWordsFile, starterWordsFile, lengthHistogramFile, binSize, cachePath);

            if (cleanCacheFlag || !preProcessor.cacheExists()) {
                if (!Files.exists(Paths.get(wordFrequencyFile)) || !Files.exists(Paths.get(subsequentWordsFile)) || !Files.exists(Paths.get(starterWordsFile))) {
                    System.out.println("Unable open analysis files (word frequency, next words, and/or starter words), and no cache file exists. " +
                            "Did you forgot to download these files ('https://www.dropbox.com/s/vhbq52vrti8isxx/dewiki-dataset.tar.gz?dl=1') or to put them into 'dewiki-dataset/files/' directory?");
                    formatter.printHelp("stringdbgen", options);
                    System.exit(1);
                }

                if (preProcessor.cacheExists()) {
                    preProcessor.cleanCache();
                }
                preProcessor.start();
            }

            if (preProcessor.cacheExists()) {
                if (cmd.getOptionValue('a') != null || cmd.getOptionValue('b') != null  ||
                        cmd.getOptionValue('c') != null ) {
                    System.err.println("NOTE: the cache is used for processing (-a, -b, -c, -d, -w options are ignored). Use --cache-clean to build a cache on new files.");
                }

                writeOutputFile(cmd, options, formatter, preProcessor.getCacheFileWordFrequency(), preProcessor.getCacheFileNextWords(), preProcessor.getCacheFileStarterWords());

            } else {
                System.err.println("Internal error: cache file cannot be opened.");
                System.exit(1);
            }




        } catch (ParseException e) {
            System.out.println(e.getMessage());
            formatter.printHelp("stringdbgen", options);
            System.exit(1);
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    private static void writeOutputFile(CommandLine cmd, Options options, HelpFormatter formatter,  String wordFrequencyFile,
        String subsequentWordsFile, String starterWordsFile)
    {
        String scenarioName = cmd.getOptionValue('s');
        String outputFileName = cmd.getOptionValue('o');
        String targetSize = cmd.getOptionValue('t');

        try {
            long maxSize[] = {Long.valueOf(targetSize)};

            if (!scenarioName.equalsIgnoreCase(SCENARIO_NAME_SOCIAL) &&
                    !scenarioName.equalsIgnoreCase(SCENARIO_NAME_INSTNAT) &&
                    !scenarioName.equalsIgnoreCase(SCENARIO_NAME_SYNTH) &&
                    !scenarioName.equalsIgnoreCase(SCENARIO_NAME_BASE) &&
                    !scenarioName.equalsIgnoreCase(SCENARIO_NAME_BASE_ZIPF)) {
                System.out.println("Unknown scenario name: '" + scenarioName + "'");
                formatter.printHelp("stringdbgen", options);
                System.exit(1);
            }

            try(FileOutputStream fileStream = new FileOutputStream(outputFileName, true))
            {
                System.out.println("This is String-DB-Gen. Copyright (c) Marcus Pinnecke 2018\nhttps://github.com/geckodb/gecko-misc/string-db-gen");
                System.setOut(new PrintStream(new BufferedOutputStream(fileStream)));

                BenchmarkGenerator.Scenario scenarios;
                if (scenarioName.equalsIgnoreCase(SCENARIO_NAME_SOCIAL)) {
                    scenarios = BenchmarkGenerator.Scenarios.SOCIAL_NETWORKING_SERVICE;
                } else if (scenarioName.equalsIgnoreCase(SCENARIO_NAME_INSTNAT)) {
                    scenarios = BenchmarkGenerator.Scenarios.INSTANT_MESSAGING_SERVICE;
                } else if (scenarioName.equalsIgnoreCase(SCENARIO_NAME_SYNTH)) {
                    scenarios = BenchmarkGenerator.Scenarios.SYNTHETIC_BENCHMARK;
                } else if (scenarioName.equalsIgnoreCase(SCENARIO_NAME_BASE)) {
                    scenarios = BenchmarkGenerator.Scenarios.PUBLIC_KNOWLEDGE_BASE;
                } else {
                    scenarios = BenchmarkGenerator.Scenarios.PUBLIC_KNOWLEDGE_BASE_ZIPF;
                }

                BenchmarkGenerator benchmarkGenerator = new BenchmarkGenerator(scenarios,
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
                        String eta = formatTimeSpan((long)(Math.max(0,(exp-diff)) / 1000));
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
    }
}

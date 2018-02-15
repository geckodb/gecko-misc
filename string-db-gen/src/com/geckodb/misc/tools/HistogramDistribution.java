package com.geckodb.misc.tools;

import org.apache.commons.cli.*;

import java.io.*;
import java.util.HashMap;
import java.util.Map;

/**
 * Created by marcus on 13.02.18.
 */
public class HistogramDistribution {

    String inputFileName;
    int binSize;

    public HistogramDistribution(String inputFileName, int binSize) {
        this.inputFileName = inputFileName;
        this.binSize = binSize;
    }

    public void createHistogram(PrintStream out) throws IOException {

        String line;
        BufferedReader reader = new BufferedReader(new FileReader(inputFileName));

        Map<Integer, Long> histogram = new HashMap<>();

        reader.readLine(); // Skip header

        while ((line = reader.readLine()) != null) {
            String[] fields = line.split(";");
            Integer length = Integer.valueOf(fields[0]);
            Integer binIndex = length / binSize;
            long count = histogram.getOrDefault(binIndex, 0L);
            histogram.put(binIndex, ++count);
        }

        long sum = 0;
        out.println("MaxLength;Count;Threshold");
        for (Map.Entry<Integer, Long> entry : histogram.entrySet()) {
            long maxLength = (1 + entry.getKey()) * binSize;
            long count = entry.getValue();
            out.println(maxLength + ";" + entry.getValue() + ";" + sum);
            sum += count;
        }
    }

    public static void main(String... args) {

        String inputFileName, line;
        Integer binSize;
        BufferedReader reader;

        Options options = new Options();
        options.addOption(Option.builder("f").longOpt("file")
                .desc("The file to be processed")
                .hasArg()
                .argName("FILE")
                .required().build());
        options.addOption(Option.builder("w").longOpt("bin-width")
                .desc("Tag attached for this processing")
                .hasArg()
                .argName("SIZE")
                .required(true).build());
        options.addOption(Option.builder("h").longOpt("help")
                .desc("Prints usage information")
                .hasArg(false)
                .required(false).build());

        String header = "Reads a CSV file <FILE> having the first column a <StringLength> integer field and creates a histogram distribution which is printed to standard output\n\n";
        String footer = "\nPlease report issues at https://github.com/geckodb/gecko-misc/issues";

        try {
            CommandLineParser clp = new DefaultParser();
            CommandLine cmd = clp.parse(options, args);

            if (cmd.hasOption("help")) {
                HelpFormatter formatter = new HelpFormatter();
                formatter.printHelp("java -jar hd.jar", header, options, footer, true);
                System.exit(1);
            }

            inputFileName = cmd.getOptionValue("file");
            binSize = Integer.valueOf(cmd.getOptionValue("bin-width"));

            HistogramDistribution histogramDistribution = new HistogramDistribution(inputFileName, binSize);
            histogramDistribution.createHistogram(System.out);

        } catch (Exception e) {
            e.printStackTrace();
            System.err.println("\n\n");

            HelpFormatter formatter = new HelpFormatter();
            formatter.printHelp("java -jar hd.jar", header, options, footer, true);
            System.exit(1);
        }

    }

}

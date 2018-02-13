package com.geckodb.misc.tools;

import com.geckodb.misc.utils.FileUtils;
import com.geckodb.misc.utils.StringUtils;
import com.geckodb.misc.utils.TextProcessing;
import org.apache.commons.cli.*;

import java.io.*;
import java.util.HashMap;
import java.util.Map;


public class StringLengths {

    public static void main(String... args)  {

        String inputFileName, line, tag;
        BufferedReader reader;

        Options options = new Options();
        options.addOption(Option.builder("f").longOpt("file")
                .desc("The file to be processed")
                .hasArg()
                .argName("FILE")
                .required().build());
        options.addOption(Option.builder("t").longOpt("tag")
                .desc("Tag attached for this processing")
                .hasArg()
                .argName("NAME")
                .required(false).build());
        options.addOption(Option.builder("h").longOpt("help")
                .desc("Prints usage information")
                .hasArg(false)
                .required(false).build());

        String header = "Counts the lengths in characters for each line in <FILE> and prints the result formatted as CSV to the standard output\n\n";
        String footer = "\nPlease report issues at https://github.com/geckodb/gecko-misc/issues";



        try {
            CommandLineParser clp = new DefaultParser();
            CommandLine cmd = clp.parse(options, args);

            if (cmd.hasOption("help")) {
                HelpFormatter formatter = new HelpFormatter();
                formatter.printHelp("java -jar sl.jar", header, options, footer, true);
                System.exit(1);
            }

            inputFileName = cmd.getOptionValue("file");
            tag = cmd.getOptionValue("tag");
            tag = tag != null ? tag : "Default";


            reader = new BufferedReader(new FileReader(inputFileName));

            System.out.println("StringLength;Tag");
            while ((line = reader.readLine()) != null) {
                System.out.println(line.length() + ";" + tag);
            }

        } catch (Exception e) {
            e.printStackTrace();
            System.err.println("\n\n");

            HelpFormatter formatter = new HelpFormatter();
            formatter.printHelp("java -jar sl.jar", header, options, footer, true);
            System.exit(1);
        }
    }

}

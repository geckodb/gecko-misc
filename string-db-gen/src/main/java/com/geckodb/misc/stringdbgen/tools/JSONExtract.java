package com.geckodb.misc.stringdbgen.tools;

import com.geckodb.misc.stringdbgen.utils.FileUtils;
import org.apache.commons.cli.*;
import org.json.JSONObject;

import java.io.BufferedReader;

public class JSONExtract {

    public static void main(String... args) {

        Options options = new Options();
        options.addOption(Option.builder("f").longOpt("file")
                .desc("The file to be processed")
                .hasArg(true)
                .argName("FILE")
                .required().build());
        options.addOption(Option.builder("k").longOpt("key")
                .desc("key that contains the text to be extracted")
                .hasArg(true)
                .argName("STRING")
                .required().build());
        options.addOption(Option.builder("h").longOpt("help")
                .desc("Prints usage information")
                .hasArg(false)
                .required(false).build());

        String header = "Reads file <FILE> of JSON documents per line, and extracts the value for the key <STRING>. The extracted value is printed to standard output.\n\n";
        String footer = "\nPlease report issues at https://github.com/geckodb/gecko-misc/issues";

        try {
            CommandLineParser clp = new DefaultParser();
            CommandLine cmd = clp.parse(options, args);

            if (cmd.hasOption("help")) {
                showUsage(options, header, footer);
            }

            String inputFileName = cmd.getOptionValue("file");
            String keyName = cmd.getOptionValue("k").toLowerCase();

            processJson(inputFileName, keyName);
        } catch (Exception e) {
            if (e.getMessage().compareTo("null") == 0) {
                e.printStackTrace();
            } else {
                System.out.println(e.getMessage() + "\n");
            }
            showUsage(options, header, footer);
        }
    }

    private static void showUsage(Options options, String header, String footer) {
        HelpFormatter formatter = new HelpFormatter();
        formatter.printHelp("java -jar je.jar", header, options, footer, true);
        System.exit(1);
    }

    private static void processJson(String inputFile, String keyName) {
        String line;

        try {
            BufferedReader reader = FileUtils.openRead(inputFile);
            while ((line = reader.readLine()) != null) {
                JSONObject document = new JSONObject(line);
                if(document.has(keyName)) {
                    String text = document.getString(keyName).trim();
                    if (!text.isEmpty()) {
                        text = text.replace('\n', ' ').replace('\r', ' ');
                        System.out.println(text);
                    }
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}

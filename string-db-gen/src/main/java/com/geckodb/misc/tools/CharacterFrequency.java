package com.geckodb.misc.tools;

import com.geckodb.misc.utils.TextProcessing;
import org.apache.commons.cli.*;

import java.io.BufferedReader;
import java.io.FileReader;

/**
 * Created by marcus on 08.02.18.
 */
public class CharacterFrequency {

    public static void main(String... args)
    {
        Options options;
        CommandLineParser parser;
        HelpFormatter formatter;
        CommandLine cmd;
        Option input;
        String inputFileName;
        BufferedReader reader;
        TextProcessing.LetterFrequencyAnalyzer analyzer;
        TextProcessing.LetterFrequencyAnalyzer.Dictionary dictionary = null;
        String line;

        options = new Options();
        input = new Option("f", "file", true, "File name to check");
        input.setRequired(true);
        options.addOption(input);
        parser = new DefaultParser();
        formatter = new HelpFormatter();

        System.out.println("Code;Character;Count");
        try {
            cmd = parser.parse(options, args);
            inputFileName = cmd.getOptionValue('f');
            reader = new BufferedReader(new FileReader(inputFileName));
            analyzer = new TextProcessing.LetterFrequencyAnalyzer();
            while ((line = reader.readLine()) != null) {
                dictionary = analyzer.put(line);
            }
            for (Character character : dictionary.keySet()) {
                System.out.println((int) character.charValue() + ";" + character + ";" + dictionary.get(character));
            }
        } catch (Exception e) {
            System.out.println(e.getMessage());
            formatter.printHelp("cf", options);
            System.exit(1);
        }
    }

}

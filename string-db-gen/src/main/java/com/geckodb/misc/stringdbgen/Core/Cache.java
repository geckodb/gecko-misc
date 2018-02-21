package com.geckodb.misc.stringdbgen.core;

import com.geckodb.misc.stringdbgen.tools.HistogramDistribution;
import com.geckodb.misc.stringdbgen.utils.FileUtils;
import com.geckodb.misc.stringdbgen.utils.StringUtils;

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.*;

import static com.geckodb.misc.stringdbgen.utils.FileUtils.openRead;
import static com.geckodb.misc.stringdbgen.utils.StringUtils.isWordString;

/**
 * Created by marcus on 09.02.18.
 */
public class Cache {

    HashMap<String, Integer> wordFrequencies = new HashMap<>();
    HashMap<String, ArrayList<SentenceGenerator.SubWord>> nextWords = new HashMap<>();
    ArrayList<String> starterWords = new ArrayList<>();
    static String path;

    public Path getCachePath() {
        return Paths.get(path);
    }

    public boolean cacheExists() {
        return (Files.exists(Paths.get(getCacheFileWordFrequency())) &&
                Files.exists(Paths.get(getCacheFileNextWords())) &&
                Files.exists(Paths.get(getCacheFileStarterWords())) &&
                Files.exists(Paths.get(getCacheFileBaseLengths()))  &&
                Files.exists(Paths.get(getCacheFileSocialLengths()))
        );
    }

    private void deleteFile(String file) throws IOException {
        System.err.print("  - " + file);
        Path path = Paths.get(file);
        if (Files.exists(path)) {
            Files.delete(path);
            System.err.println(" [OK]");
        } else System.err.println(" [NOT FOUND]");
    }

    public void cleanCache() throws IOException {
        System.err.println("Cleaning cache");
        deleteFile(getCacheFileWordFrequency());
        deleteFile(getCacheFileNextWords());
        deleteFile(getCacheFileStarterWords());
        deleteFile(getCacheFileBaseLengths());
        deleteFile(getCacheFileSocialLengths());
    }

    public static String getCacheFileWordFrequency() {
        return path + "/word-frequency.cache";
    }

    public static String getCacheFileNextWords() {
        return path + "/next-words.cache";
    }

    public static String getCacheFileStarterWords() {
        return path + "/starter-words.cache";
    }

    public static String getCacheFileBaseLengths() {
        return path + "/base-articles-lengths.cache";
    }

    public static String getCacheFileSocialLengths() {
        return path + "/social-msg--lengths.cache";
    }

    String wordFrequencyFile;
    String subsequentWordsFile;
    String starterWordsFile;
    String sentenceLengthHistogram;
    String twitterLengthHistogramFile;
    int histBinSize;

    public Cache(String wordFrequencyFile, String subsequentWordsFile, String starterWordsFile,
                 String sentenceLengthHistogram, String twitterLengthHistogramFile, int histBinSize, String path) {
        this.path = StringUtils.ensurePath(path);
        this.wordFrequencyFile = wordFrequencyFile;
        this.subsequentWordsFile = subsequentWordsFile;
        this.starterWordsFile = starterWordsFile;
        this.sentenceLengthHistogram = sentenceLengthHistogram;
        this.twitterLengthHistogramFile = twitterLengthHistogramFile;
        this.histBinSize = histBinSize;
    }

    public void start() throws IOException {
        System.err.print("Pre-processing is running. This might take some minutes.\n");
        buildCache(wordFrequencyFile, subsequentWordsFile, starterWordsFile);
        System.err.println("Writing cache");
        storeCache(sentenceLengthHistogram, twitterLengthHistogramFile, histBinSize);
    }

    private void buildCache(String wordFrequencyFile, String subsequentWordsFile, String starterWordsFile) {
        System.err.println("  - processing word frequencies ('" +  wordFrequencyFile + "')");
        parseWordFrequency(wordFrequencyFile);
        System.err.println("  - processing next words ('" +  subsequentWordsFile + "')");
        parseNextWordsFile(subsequentWordsFile);
        System.err.println("  - processing starter words ('" +  starterWordsFile + "')");
        parseStarterWordsFile(starterWordsFile);
    }

    public void storeCache(String sentenceLengthHistogramFile, String twitterLengthHistogramFile, int binSize) throws IOException {
        if (!Files.exists(getCachePath())) {
            Files.createDirectory(getCachePath());
        }

        System.err.println("  - writing word frequencies cache to '"+getCacheFileWordFrequency()+"'");
        dumpWordFrequency(getCacheFileWordFrequency());
        System.err.println("  - writing next words cache to '"+getCacheFileNextWords()+"'");
        dumpNextWords(getCacheFileNextWords());
        System.err.println("  - writing starter words cache to '"+getCacheFileStarterWords()+"'");
        dumpStarterWords(getCacheFileStarterWords());
        System.err.println("  - writing histogram for sentence lengths to '"+getCacheFileBaseLengths()+"' (from '" +  sentenceLengthHistogramFile + "')");
        createHistogramForSentenceLengthFile(getCacheFileBaseLengths(), sentenceLengthHistogramFile, binSize);
        System.err.println("  - writing histogram for sentence lengths to '"+getCacheFileSocialLengths()+"' (from '" +  twitterLengthHistogramFile + "')");
        createHistogramForSentenceLengthFile(getCacheFileSocialLengths(), twitterLengthHistogramFile, binSize);
    }

    private void createHistogramForSentenceLengthFile(String dstFile, String srcFile, int binSize) {
        try {
            HistogramDistribution hist = new HistogramDistribution(srcFile, binSize);
            BufferedOutputStream buf = new BufferedOutputStream(new FileOutputStream(dstFile));
            hist.createHistogram(new PrintStream(buf), false);
            buf.flush();
            buf.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void dumpStarterWords(String file) {
        try {
            System.err.print("  - " + file);
            BufferedWriter writer = FileUtils.openWrite(file);
            for (String word : starterWords) {
                writer.write(word + "\n");
            }
            writer.flush();
            writer.close();
            System.err.println("\t[DONE]");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void dumpNextWords(String file) {
        try {
            System.err.print("  - " + file);
            BufferedWriter writer = FileUtils.openWrite(file);
            for (Map.Entry<String, ArrayList<SentenceGenerator.SubWord>> entries : nextWords.entrySet()) {
                writer.write(entries.getKey());
                for (SentenceGenerator.SubWord subWord : entries.getValue()) {
                    writer.write(" " + subWord.word + "@" + subWord.freq);
                }
                writer.write("\n");
            }
            writer.flush();
            writer.close();
            System.err.println("\t[DONE]");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void dumpWordFrequency(String file) {
        try {
            BufferedWriter writer = FileUtils.openWrite(file);
            writer.write("Frequency;Length;Word\n");
            for (Map.Entry<String, Integer> entries : wordFrequencies.entrySet()) {
                writer.write(entries.getValue() + ";" + entries.getKey().length() + ";" + entries.getKey());
                writer.write("\n");
            }
            writer.flush();
            writer.close();
            System.err.println("\t[DONE]");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void parseStarterWordsFile(String file) {
        try {
            BufferedReader reader = openRead(file);
            String line = reader.readLine(); // skip header
            while ((line = reader.readLine()) != null) {
                String word = StringUtils.cleanup(line);
                if (word != null && word.length() > 0 && wordFrequencies.containsKey(word)) {
                    starterWords.add(word);
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void parseNextWordsFile(String file) {
        try {
            BufferedReader reader = openRead(file);
            String line, first, sub;
            String words[];
            int freq;
            ArrayList<SentenceGenerator.SubWord> subWords = new ArrayList<>();
            while ((line = reader.readLine()) != null) {
                words = line.split(" ");
                first = StringUtils.cleanup(words[0]);
                if (first.length() > 0 && wordFrequencies.containsKey(first)) {
                    subWords.clear();
                    for (int i = 1; i < words.length; i++) {
                        sub = StringUtils.cleanup(words[i]);
                        if (sub.length() > 0) {
                            freq = wordFrequencies.getOrDefault(sub, 0);
                            if (freq > 0) {
                                subWords.add(new SentenceGenerator.SubWord(sub, freq));
                            }
                        }
                    }
                    if (!subWords.isEmpty()) {
                        nextWords.put(first, subWords);
                    }
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void parseWordFrequency(String file) {
        try {
            BufferedReader reader = openRead(file);
            String line, freqString, word;
	    int freq;
            while ((line = reader.readLine()) != null) {
                freqString = line.substring(0, line.indexOf(";"));
                line = line.substring(line.indexOf(";") + 1);
                word = StringUtils.cleanup(line.substring(line.indexOf(";") + 1));
                if (word.length() > 0 && isWordString(word)) {
                    try {
                        freq = Integer.valueOf(freqString);
                        wordFrequencies.put(word, freq);
                    } catch (NumberFormatException e)
                    { }
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}

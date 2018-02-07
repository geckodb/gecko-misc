package com.geckodb.misc.stringdbgen;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.InputStreamReader;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Random;

/**
 * Created by marcus on 07.02.18.
 */
public class TextGenerator {

    class SubWord {
        String word;
        int freq;

        public SubWord(String word, int freq) {
            this.word = word;
            this.freq = freq;
        }
    }

    HashMap<String, Integer> wordFrequencies = new HashMap<>();
    HashMap<String, ArrayList<SubWord>> subsequentWords = new HashMap<>();
    ArrayList<String> starterWords = new ArrayList<>();
    Random random = new Random();

    public TextGenerator(String wordFrequencyFile, String subsequentWordsFile, String starterWordsFile) {
        System.err.println("Loading word frequencies...");
        parseWordFrequency(wordFrequencyFile);
        System.err.println("Loading sequent words...");
        parseSubsequentWordsFile(subsequentWordsFile);
        System.err.println("Loading starter words...");
        parseStarterWordsFile(starterWordsFile);
        System.err.println("Start data generator...");
    }

    public String generate(int stringLength) {
        StringBuilder sentence = new StringBuilder();
        String last = starterWords.get(random.nextInt(starterWords.size()));
        sentence.append(last).append(" ");
        while (sentence.length() < stringLength) {
            ArrayList<SubWord> subWords = subsequentWords.get(last);
            if (subWords != null) {
                int max = 0, agg = 0;
                String word = null;
                for (SubWord w : subWords) {
                    max += w.freq;
                }
                int choose = random.nextInt(max);
                for (SubWord w : subWords) {
                    if (choose >= agg && choose < agg + w.freq) {
                        word = w.word;
                        break;
                    } else agg += w.freq;
                }
                assert (word != null);
                SubWord sw = subWords.get(random.nextInt(subWords.size()));
                last = sw.word;
            } else {
                last = starterWords.get(random.nextInt(starterWords.size()));
            };
            sentence.append(last).append(" ");
        }
        return sentence.toString().substring(0, stringLength);
    }

    private void parseStarterWordsFile(String file) {
        try (FileInputStream is = new FileInputStream(file)) {

            InputStreamReader isr = new InputStreamReader(is, Charset.defaultCharset());
            BufferedReader br = new BufferedReader(isr);

            String line = br.readLine(); // skip header

            long numLines = 0;
            do {

                line = br.readLine();


                String word = StringUtils.cleanup(line);

               // System.out.println(word);

                if (word != null && word.length() > 0) {

                    if (wordFrequencies.containsKey(word)) {
                        starterWords.add(word);
                     //   System.out.println("add ... " + word);
                    }
                }

            } while (line != null);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void parseSubsequentWordsFile(String file) {
        try (FileReader fr = new FileReader(file)) {

            BufferedReader br = new BufferedReader(fr);
            String line = null;
            long numLines = 0;
            do {
                line = br.readLine();
                if (line != null) {
                    String words[] = line.split(" ");
                    String first = words[0];
                    if (wordFrequencies.containsKey(first)) {
                        ArrayList<SubWord> subWords = new ArrayList<>();
                        for (int i = 1; i < words.length; i++) {
                            String sub = words[i];

                            int freq = wordFrequencies.getOrDefault(sub, 0);
                            if (freq > 0) {
                                subWords.add(new SubWord(sub, freq));
                            }
                        }
                        if (!subWords.isEmpty()) {
                            subsequentWords.put(first, subWords);
                        }
                    }

                    //System.out.println(line);

                }
            }
            while (line != null);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void parseWordFrequency(String file) {
        try (FileReader fr = new FileReader(file)) {

            BufferedReader br = new BufferedReader(fr);
            String line = null;
            long numLines = 0;
            do {
                line = br.readLine();
                if (line != null) {
                    String freqString = line.substring(0, line.indexOf(";"));

                    line = line.substring(line.indexOf(";") + 1);
                    String word = line.substring(line.indexOf(";") + 1);
                    //  System.out.println(freqString + "    " + word);

                    try {
                        int freq = Integer.valueOf(freqString);
                        wordFrequencies.put(word, freq);
                    } catch (NumberFormatException e) {

                    }
                }
            } while (line != null);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

}

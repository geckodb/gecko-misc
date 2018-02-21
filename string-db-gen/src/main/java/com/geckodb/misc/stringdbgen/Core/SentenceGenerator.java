package com.geckodb.misc.stringdbgen.Core;

import java.io.BufferedReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Random;

import static com.geckodb.misc.utils.FileUtils.openRead;

/**
 * Created by marcus on 07.02.18.
 */
public class SentenceGenerator {

    public static class SubWord {
        String word;
        int freq;

        public SubWord(String word, int freq) {
            this.word = word;
            this.freq = freq;
        }
    }

    HashMap<String, Integer> wordFrequencies = new HashMap<>();
    HashMap<String, ArrayList<SubWord>> nextWords = new HashMap<>();
    ArrayList<String> starterWords = new ArrayList<>();
    Random random = new Random();

    public SentenceGenerator(String wordFrequencyFile, String subsequentWordsFile, String starterWordsFile) {
        System.err.println("Loading word frequencies: " + wordFrequencyFile);
        parseWordFrequency(wordFrequencyFile);
        System.err.println("Loading next words: " + subsequentWordsFile);
        parseNextWordsFile(subsequentWordsFile);
        System.err.println("Loading starter words: " + starterWordsFile);
        parseStarterWordsFile(starterWordsFile);
        System.err.println("Start data generator...");
    }

    //StringBuilder sentence;

    public String generate(int stringLength) {
        String last = starterWords.get(random.nextInt(starterWords.size()));
        String sentence = last + " ";
            while (sentence.length() < stringLength) {
                ArrayList<SubWord> subWords = nextWords.get(last);
                if (subWords != null) {
                    int max = 0, agg = 0;
                    String word = null;
                    for (SubWord w : subWords) {
                        max += w.freq;
                    }
                    int choose = random.nextInt(max);
                    for (int i = 0; i < subWords.size(); i++) {
                        SubWord w = subWords.get(i);
                        if (choose >= agg && choose < agg + w.freq) {
                            word = w.word;
                            break;
                        } else agg += w.freq;
                    }
                    assert (word != null);

                    if (word != null && last != null) {
                        last = ((word.compareTo(last) != 0)) ? word : null;
                    } else {
                        last = null;
                    }

                }

                if (last == null || last.length() == 0) {
                    last = starterWords.get(random.nextInt(starterWords.size()));
                } else {
                    if (!sentence.endsWith(last + " ")) {
                        sentence += last + " ";
                    } else {
                        last = starterWords.get(random.nextInt(starterWords.size()));
                    }

                }
            }
        return String.valueOf(sentence.subSequence(0, stringLength));
    }

    private void parseStarterWordsFile(String file) {
        try {
            BufferedReader reader = openRead(file);
            String word = reader.readLine(); // skip header
            while ((word = reader.readLine()) != null) {
                    starterWords.add(word);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void parseNextWordsFile(String file) {
        try {
            BufferedReader reader = openRead(file);
            String line;
            while ((line = reader.readLine()) != null) {
                String words[] = line.split(" ");
                String first = words[0];
                ArrayList<SentenceGenerator.SubWord> subWords = new ArrayList<>();
                for (int i = 1; i < words.length; i++) {
                    String sub = words[i];
                    String[] parts = sub.split("@");
                    subWords.add(new SentenceGenerator.SubWord(parts[0], Integer.valueOf(parts[1])));
                }
                nextWords.put(first, subWords);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void parseWordFrequency(String file) {
        try {
            BufferedReader reader = openRead(file);
            String line;
            while ((line = reader.readLine()) != null) {
                String freqString = line.substring(0, line.indexOf(";"));
                line = line.substring(line.indexOf(";") + 1);
                String word = line.substring(line.indexOf(";") + 1);
                if (word.length() > 0) {
                    try {
                        int freq = Integer.valueOf(freqString);
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

package com.geckodb.misc.utils;


import java.util.HashMap;

public class TextProcessing {

    public static class LetterFrequencyAnalyzer {

        public static class Dictionary extends HashMap<Character, Long> {};

        Dictionary dictionary = new Dictionary();

        public Dictionary put(String s) {
            s = s.toLowerCase();
            for (int i = 0; i < s.length(); i++) {
                char c = s.charAt(i);
                if (StringUtils.isSimpleChar(c)) {
                    dictionary.put(c, dictionary.getOrDefault(c, 0L) + 1);
                }
            }
            return dictionary;
        }

    }





}

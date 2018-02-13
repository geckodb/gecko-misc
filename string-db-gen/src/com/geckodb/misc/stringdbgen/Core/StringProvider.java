package com.geckodb.misc.stringdbgen.Core;

import org.apache.commons.math3.distribution.NormalDistribution;
import org.apache.commons.math3.distribution.ZipfDistribution;

import java.util.Random;

/**
 * Created by marcus on 06.02.18.
 */
public class StringProvider {

    public static abstract class NumberGenerator
    {
        public int minLen;
        public int maxLen;
        public int span;

        public NumberGenerator() {
        }
        public abstract int next();
    }

    public static class UniformNumberGenerator extends NumberGenerator {

        Random uniformRandom = new Random();

        @Override
        public int next() {
            return minLen + (span > 0 ? this.uniformRandom.nextInt(span) : 0);
        }
    }

    public static class ZipfNumberGenerator extends NumberGenerator {

        ZipfDistribution zipfRandom = null;
        int i = 0;

        @Override
        public int next() {
            if (zipfRandom == null) {
                zipfRandom = new ZipfDistribution(span, 1.1);
            }
            return (int) (minLen + span * zipfRandom.probability(i ++ % span));
        }
    }

    public static class ZipfNumberGenerator2 extends NumberGenerator {

        ZipfDistribution zipfRandom = null;
        int i = 0;

        @Override
        public int next() {
            if (zipfRandom == null) {
                zipfRandom = new ZipfDistribution(span, 1);
            }
            return (int) Math.max(1, minLen + span * zipfRandom.probability(i ++ % span));
        }
    }

    public static class NormalNumberGenerator extends NumberGenerator {

        NormalDistribution normalfRandom;
        int i = 0;

        @Override
        public int next() {
            if (normalfRandom == null) {
                normalfRandom = new NormalDistribution(span/2, span/8);
            }
            double x;
            do {
                x = normalfRandom.sample();
            } while (x < 0 || x > 280);
            if (x < 0 || x > 280) {
                System.exit(0);
            }
            return (int) (minLen + x);
        }
    }

    NumberGenerator numberGenerator;
    Random uniformRandom;
    TextGenerator textGenerator;

    public StringProvider(NumberGenerator generator, String wordFrequencyFile, String subsequentWordsFile, String starterWordsFile) {
        this.numberGenerator = generator;
        this.uniformRandom = new Random();
        textGenerator = new TextGenerator(wordFrequencyFile, subsequentWordsFile, starterWordsFile);
    }

    public String next() {
        int stringLength = numberGenerator.next();
        return textGenerator.generate(stringLength);
    }
}

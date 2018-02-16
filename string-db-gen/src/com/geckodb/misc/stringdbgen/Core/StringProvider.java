package com.geckodb.misc.stringdbgen.Core;

import com.geckodb.misc.tools.HistogramDistribution;
import org.apache.commons.math3.distribution.NormalDistribution;
import org.apache.commons.math3.distribution.ZipfDistribution;

import java.io.IOException;
import java.util.Random;

/**
 * Created by marcus on 06.02.18.
 */
public class StringProvider {

    private static StringProvider instance;

    private StringProvider(NumberGenerator generator, String wordFrequencyFile, String subsequentWordsFile, String starterWordsFile) {
        this.numberGenerator = generator;
        this.uniformRandom = new Random();
        textGenerator = new TextGenerator(wordFrequencyFile, subsequentWordsFile, starterWordsFile);
    }

    public static synchronized StringProvider getInstance (NumberGenerator generator, String wordFrequencyFile, String subsequentWordsFile, String starterWordsFile) {
        if (StringProvider.instance == null) {
            StringProvider.instance = new StringProvider (generator, wordFrequencyFile, subsequentWordsFile, starterWordsFile);
        }
        return StringProvider.instance;
    }

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
                zipfRandom = new ZipfDistribution(span, 1);
            }
            return (int) (minLen + span * zipfRandom.probability(i ++ % span));
        }
    }

    public static class ZipfNumberGeneratorE1 extends NumberGenerator {

        ZipfDistribution zipfRandom = null;
        int i = 0;

        @Override
        public int next() {
            if (zipfRandom == null) {
                zipfRandom = new ZipfDistribution(span, 1);
            }
            return (int) (minLen + span * zipfRandom.probability(i ++ % span)) + 2500;
        }
    }

    public static class ZipfNumberGeneratorE2 extends NumberGenerator {

        ZipfDistribution zipfRandom = null;
        int i = 0;

        @Override
        public int next() {
            if (zipfRandom == null) {
                zipfRandom = new ZipfDistribution(span, 2);
            }
            return (int) (minLen + span * zipfRandom.probability(i ++ % span)) + 2500;
        }
    }

    public static class ZipfNumberGeneratorE3 extends NumberGenerator {

        ZipfDistribution zipfRandom = null;
        int i = 0;

        @Override
        public int next() {
            if (zipfRandom == null) {
                zipfRandom = new ZipfDistribution(span, 3);
            }
            return (int) (minLen + span * zipfRandom.probability(i ++ % span)) + 2500;
        }
    }

    public static class HistogramNumberGenerator extends NumberGenerator {

        NonUniformNumberGenerator gen = null;

        @Override
        public int next() {
            if (gen == null) {
                try {
                    gen = new NonUniformNumberGenerator(TextPreProcessor.getCacheFileArticleLengths());
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            return gen.next();
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



    public String next() {
        int stringLength = numberGenerator.next();
        return textGenerator.generate(stringLength);
    }
}

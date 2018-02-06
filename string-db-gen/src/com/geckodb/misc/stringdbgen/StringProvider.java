package com.geckodb.misc.stringdbgen;

import org.apache.commons.math3.distribution.NormalDistribution;
import org.apache.commons.math3.distribution.ZipfDistribution;

import java.util.Iterator;
import java.util.Random;

/**
 * Created by marcus on 06.02.18.
 */
public class StringProvider implements Iterable<String> {

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
    StringBuilder builder;

    public StringProvider(NumberGenerator generator) {
        this.numberGenerator = generator;
        this.builder = new StringBuilder();
        this.uniformRandom = new Random();
    }


    @Override
    public Iterator<String> iterator() {
        return new Iterator<String>() {
            @Override
            public boolean hasNext() {
                return true;
            }

            @Override
            public String next() {

                builder.delete(0, builder.length());
                int stringLength = numberGenerator.next();
                while (stringLength-- > 0) {
                    builder.append((char) uniformRandom.nextInt(255));
                }

                return builder.toString();
            }
        };
    }
}

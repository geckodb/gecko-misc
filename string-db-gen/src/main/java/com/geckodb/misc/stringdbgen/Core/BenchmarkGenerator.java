package com.geckodb.misc.stringdbgen.Core;

import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReferenceArray;

/**
 * Created by marcus on 06.02.18.
 */
public final class BenchmarkGenerator {

    public static class Scenario {
        public int minLength, maxLength;
        public StringProvider.NumberGenerator numberGenerator;

        public Scenario(int minLength, int maxLength, Class<? extends StringProvider.NumberGenerator> numberGenerator) {
            this.minLength = minLength;
            this.maxLength = maxLength;

            try {
                this.numberGenerator = numberGenerator.newInstance();
                this.numberGenerator.minLen = minLength;
                this.numberGenerator.maxLen = maxLength;
                this.numberGenerator.span = (maxLength - minLength);
            } catch (InstantiationException e) {
                e.printStackTrace();
            } catch (IllegalAccessException e) {
                e.printStackTrace();
            }

        }
    }

    public BenchmarkGenerator(Scenario scenario, String wordFrequencyFile, String subsequentWordsFile, String starterWordsFile) {
        this.scenario = scenario;
        this.wordFrequencyFile = wordFrequencyFile;
        this.subsequentWordsFile = subsequentWordsFile;
        this.starterWordsFile = starterWordsFile;
        this.provider = StringProvider.getInstance(scenario.numberGenerator, wordFrequencyFile, subsequentWordsFile, starterWordsFile);
    }

    public static abstract class Scenarios {
        public static final Scenario SOCIAL_NETWORKING_SERVICE = new Scenario(1, 280, StringProvider.HistogramNumberGeneratorSocial.class);
        public static final Scenario INSTANT_MESSAGING_SERVICE = new Scenario(1, 65536, StringProvider.ZipfNumberGenerator.class);
        public static final Scenario SYNTHETIC_BENCHMARK = new Scenario(10, 50, StringProvider.UniformNumberGenerator.class);
        public static final Scenario PUBLIC_KNOWLEDGE_BASE = new Scenario(0, 2776418, StringProvider.HistogramNumberGeneratorBase.class);
        public static final Scenario PUBLIC_KNOWLEDGE_BASE_ZIPF1 = new Scenario(0, 2776418, StringProvider.ZipfNumberGeneratorE1.class);
        public static final Scenario PUBLIC_KNOWLEDGE_BASE_ZIPF2 = new Scenario(0, 2776418, StringProvider.ZipfNumberGeneratorE2.class);
        public static final Scenario PUBLIC_KNOWLEDGE_BASE_ZIPF3 = new Scenario(0, 2776418, StringProvider.ZipfNumberGeneratorE3.class);
    }

    Scenario scenario;
    AtomicInteger total = new AtomicInteger(0);
    AtomicInteger numCreated = new AtomicInteger(0);
    int numThreads = 16;
    int numElementsToGenerate = 1024;
    int upperBoundByThread = numElementsToGenerate / numThreads;
    AtomicReferenceArray<String> atomicReferenceArray = new AtomicReferenceArray<String>(numElementsToGenerate);

    public enum ContinueState { CONTINUE, STOP }

    public interface Callback<T> {
        ContinueState consume(T args);
    }

    String wordFrequencyFile;
    String subsequentWordsFile;
    String starterWordsFile;

    Thread[] threads = new Thread[numThreads];
    StringProvider provider;

    public void generateString(Callback<AtomicReferenceArray<String>> callback, Callback<Void> onFinish) {




        do {
            numCreated.set(0);
            for (int idx = 0; idx < numThreads; idx++) {
                threads[idx] = new Thread(new Runnable() {


                    @Override
                    public void run() {
                        for (int i = 0; i < upperBoundByThread; i++) {
                            String text = provider.next();
                            atomicReferenceArray.set(numCreated.get(), text);
                            numCreated.getAndAdd(1);
                        }
                    }
                });
                threads[idx].start();
            }

            for (int idx = 0; idx < numThreads; idx++) {
                try {
                    threads[idx].join();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }

        } while (callback.consume(atomicReferenceArray) != ContinueState.STOP);

        onFinish.consume(null);

    }
}

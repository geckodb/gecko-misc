package com.geckodb.misc.stringdbgen;

import java.util.Iterator;
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
    }

    public static final class Scenarios {
        public static final Scenario SOCIAL_NETWORKING_SERVICE = new Scenario(1, 280, StringProvider.NormalNumberGenerator.class);
        public static final Scenario INSTANT_MESSAGING_SERVICE = new Scenario(1, 65536, StringProvider.ZipfNumberGenerator.class);
        public static final Scenario SYNTHETIC_BENCHMARK = new Scenario(10, 50, StringProvider.UniformNumberGenerator.class);
        public static final Scenario PUBLIC_KNOWLEDGE_BASE = new Scenario(160, 2776418, StringProvider.ZipfNumberGenerator.class);
    }

    Scenario scenario;
    AtomicInteger total = new AtomicInteger(0);
    AtomicInteger numCreated = new AtomicInteger(0);
    int numThreads = 1024;
    int numElementsToGenerate = 1024;
    int upperBoundByThread = numElementsToGenerate / numThreads;
    AtomicReferenceArray<String> atomicReferenceArray = new AtomicReferenceArray<String>(numElementsToGenerate);

    enum ContinueState { CONTINUE, STOP }

    interface Callback {
        ContinueState consume(AtomicReferenceArray<String> strings);
    }

    String wordFrequencyFile;
    String subsequentWordsFile;
    String starterWordsFile;

    void generateString(Callback callback) {

        Thread[] threads = new Thread[numThreads];

        do {
            numCreated.set(0);
            for (int idx = 0; idx < numThreads; idx++) {
                threads[idx] = new Thread(new Runnable() {

                    StringProvider provider = new StringProvider(scenario.numberGenerator, wordFrequencyFile, subsequentWordsFile, starterWordsFile);
                    Iterator<String> it = provider.iterator();


                    @Override
                    public void run() {
                        for (int i = 0; i < upperBoundByThread; i++) {
                            String string = it.next();
                            atomicReferenceArray.set(numCreated.get(), string);
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


    }
}

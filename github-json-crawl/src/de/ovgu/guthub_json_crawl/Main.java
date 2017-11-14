package de.ovgu.guthub_json_crawl;

import static de.ovgu.guthub_json_crawl.DateUtils.formatDuration;

public class Main {

    public static void main(String[] args) {

        RepositoryCrawler crawler = new RepositoryCrawler();

        int since = 0;
        while (true) {
            try {
                int status = crawler.crawl(since);
                if (status != -1) {
                    since = status;
                } else {
                    long suspentTimeMs = RepositoryCrawler.CONTINUE - System.currentTimeMillis();
                    System.err.println("Suspend thread for " + formatDuration(suspentTimeMs));
                    while (System.currentTimeMillis() < RepositoryCrawler.CONTINUE) {
                        try {
                            Thread.sleep(suspentTimeMs + 5000);
                        } catch (Exception e2) {
                            e2.printStackTrace();
                        }
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
            }

        }



	// write your code here
    }
}

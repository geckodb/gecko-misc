package com.geckodb.misc.tools;

import com.geckodb.misc.utils.FileUtils;

import java.io.*;

/**
 * Created by marcus on 09.02.18.
 */
public class MaxLength {

    public static void main(String... args) throws FileNotFoundException {

        String file = "/Users/marcus/Downloads/dewiki-latest-pages-articles-clean.txt\"";
          int linnum = 0;
        int max = 0;

        try {
            BufferedReader reader = FileUtils.openRead(file);
            String line;

            while ((line = reader.readLine()) != null) {
                if ((linnum++ % 1000) == 0) {
                    System.err.println(100* linnum++ / 69089992.0 + "%");
                }

                max = Math.max(linnum, line.length());
            }

            System.out.println(">> " + max);

        } catch (Exception e) {
            e.printStackTrace();
        }

    }

}

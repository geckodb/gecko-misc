package com.geckodb.misc.stringdbgen.tools;

import com.geckodb.misc.stringdbgen.utils.FileUtils;

import java.io.*;

/**
 * Created by marcus on 12.02.18.
 */
public class ExtractText {

    public static void main(String... args) throws IOException {

        BufferedReader reader = FileUtils.openRead("/Users/marcus/Downloads/dewiki-latest-pages-articles.xml");
        System.setOut(new PrintStream(new BufferedOutputStream(new FileOutputStream("/Users/marcus/Downloads/dewiki-latest-pages-articles-clean.txt"))));
        String line;

        boolean inArticle = false;
        String article = "";

        while ((line = reader.readLine()) != null) {

            if (line.contains("<text xml:space=\"preserve\">") && !inArticle) {
                inArticle = true;
                int endPos = (line.contains("</text")) ? line.indexOf("</text") : line.length();
                String sub = article = line.substring(line.indexOf("<text xml:space=\"preserve\">") + "<text xml:space=\"preserve\">".length(), endPos);
                if (sub.startsWith("#REDIRECT") || sub.startsWith("__NOTOC__") || sub.startsWith("#redirect") || sub.startsWith("#WEITERLEITUNG")) {
                    inArticle = false;
                    continue;
                }
                article = sub;

            } else if (inArticle && !line.contains("</text")) {
                article += line;
            }
            if (inArticle && line.contains("</text")) {
                inArticle = false;
                article += line.substring(0, line.indexOf("</text"));

                System.out.println(article);

            }


        }

    }

}

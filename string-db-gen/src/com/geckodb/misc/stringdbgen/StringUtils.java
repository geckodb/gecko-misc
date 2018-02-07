package com.geckodb.misc.stringdbgen;

/**
 * Created by marcus on 07.02.18.
 */
public class StringUtils {

    public static String cleanup(String s) {
        if (s != null && !s.contains("¬")
                && !s.contains("…")
                && !s.contains("'']")
                && !s.contains("((")
                && !s.contains("))")
                && !s.contains(",")
                && !s.contains("|")
                && !s.contains("*")
                && !s.contains("=")
                && !s.contains("\\")
                && !s.contains("&")
                && !s.contains("!")
                && !s.contains("„")
                && !s.contains(";")
                && !s.contains("“")
                && !s.contains("_")
                && !s.contains("!")
                && !s.contains(" ")
                && !s.contains("/")



                ) {
            return s.replace("\r", " ").replace("\n", " ").replace("\t", " ").replace("[", " ").replace("]", " ").replace("(", " ").replace(")", " ").
                    replace("'", " ").replace("{", " ").replace("}", " ").replace("|", " ").replace("nbsp;", " ").replace("#", " ").replace(":", " ").replace("$", " ")
                    .replace("=", " ")
                    .replace(",", " ")
                    .replace(".", " ")
                    .replace("lt;/sup", " ")
                    .replace("lt;", " ")
                    .replace("br;", " ")
                    .replace("gt;", " ")
                    .replace("lt;", " ")

                    .replace("sup;", " ")




                    .replace("&quot;", " ").replace("&amp;", " ").replace("-", " ").toLowerCase().trim();
        } else return "";
    }

}

package com.geckodb.misc.stringdbgen.utils;

/**
 * Created by marcus on 07.02.18.
 */
public class StringUtils {

    public static boolean isSimpleChar(char c) {
        return (c == '\n' || c == '\r' || c == 32 || (c >= 48 && c < 58) || (c >= 65 && c < 91) || (c >= 97 && c < 123));
    }

    public static boolean isWordString(String s) {
        for (int i = 0; i < s.length(); i++) {
            char c = s.charAt(i);
            if (!(isSimpleChar(c)) || c == 32) {
                return false;
            }
        }
        return true;
    }

    public static boolean isSimpleString(String s) {
        for (int i = 0; i < s.length(); i++) {
            char c = s.charAt(i);
            if (!(isSimpleChar(c))) {
                return false;
            }
        }
        return true;
    }

    public static String cleanup(String s) {
        if (s != null && isSimpleString(s)) {
            return s.replace("\r", " ")
                    .replace("\n", " ")
                    .replace("\t", " ")
                    .replace("[", " ")
                    .replace("]", " ")
                    .replace("(", " ")
                    .replace(")", " ")
                    .replace("'", " ")
                    .replace("{", " ")
                    .replace("}", " ")
                    .replace("|", " ")
                    .replace("nbsp;", " ")
                    .replace("#", " ")
                    .replace(":", " ")
                    .replace("$", " ")
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

    public static String ensurePath(String path) {
        if (path.endsWith("/") && path.length() > 2)
            return path.substring(0, path.indexOf("/") - 1);
        else return path;
    }
}

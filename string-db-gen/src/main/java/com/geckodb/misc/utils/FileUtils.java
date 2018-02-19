package com.geckodb.misc.utils;

import java.io.*;
import java.nio.charset.Charset;

/**
 * Created by marcus on 09.02.18.
 */
public class FileUtils {

    public static final BufferedReader openRead(String file) throws FileNotFoundException {
        FileInputStream is = new FileInputStream(file);
        InputStreamReader isr = new InputStreamReader(is, Charset.defaultCharset());
        return new BufferedReader(isr);
    }

    public static final BufferedWriter openWrite(String file) throws FileNotFoundException {
        FileOutputStream os = new FileOutputStream(file);
        OutputStreamWriter osw = new OutputStreamWriter(os, Charset.defaultCharset());
        return new BufferedWriter(osw);
    }

}

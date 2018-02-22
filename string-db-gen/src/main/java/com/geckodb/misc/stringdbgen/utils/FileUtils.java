package com.geckodb.misc.stringdbgen.utils;

import www.kosoft.util.ReverseLineInputStream;

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

    public static final BufferedReader openReadReverse(String file) throws FileNotFoundException {
        return new BufferedReader (new InputStreamReader (new ReverseLineInputStream(new File(file))));
    }


    public static final BufferedWriter openWrite(String file) throws IOException {
        return openWriteEx(file, false);
    }

    public static final BufferedWriter openWriteEx(String file, boolean append) throws IOException {
        FileWriter writer = new FileWriter(file, append);
        return new BufferedWriter(writer);
    }

}

package de.ovgu.guthub_json_crawl;

import org.json.JSONArray;
import org.json.JSONObject;

import java.io.*;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.Date;

import static java.awt.SystemColor.text;

/**
 * Created by marcus on 14.11.17.
 */
public class ResourceGet {

    public String get(String url, String type) throws IOException {

        URL u;
        InputStream is = null;
        DataInputStream dis;
        String s;

        StringBuffer contents = new StringBuffer();

        u = new URL(url);
        is = u.openStream();
        dis = new DataInputStream(new BufferedInputStream(is));
        while ((s = dis.readLine()) != null) {
            contents.append(s);
        }

        String dir = "raw/" + type;
        new File(dir).mkdirs();

        try(  PrintWriter out = new PrintWriter( dir + "/" + url.substring(url.lastIndexOf("/")) + "-" + System.currentTimeMillis() + ".txt" )  ){
            out.println( contents );
        }

        return contents.toString();
    }
}

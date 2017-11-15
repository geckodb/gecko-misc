package de.ovgu.guthub_json_crawl;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.util.EntityUtils;
import org.json.JSONArray;
import org.json.JSONObject;

import java.io.*;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
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

        HttpClient client = new DefaultHttpClient();
        HttpGet get = new HttpGet(url);
        get.setHeader("Authorization", "token " <YOUR GITHUB TOKEN HERE>);
        HttpResponse responseGet = client.execute(get);
        HttpEntity resEntityGet = responseGet.getEntity();
        if (resEntityGet != null) {
            is = resEntityGet.getContent();

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
        } else {
            System.err.println("No response or NULL");
            return "";
        }



    }
}

package de.ovgu.guthub_json_crawl;

import org.json.JSONArray;
import org.json.JSONObject;

import java.io.BufferedInputStream;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.MalformedURLException;
import java.net.URL;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

import static de.ovgu.guthub_json_crawl.DateUtils.formatDuration;

/**
 * Created by marcus on 14.11.17.
 */
public class RepositoryCrawler {

    public static final long TIME_START = System.currentTimeMillis();
    public static long CONTINUE = 0;



    public int crawl(int since) throws IOException {

        String contents;
        SimpleDateFormat dateFormatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS");


        try {
            ResourceGet getter = new ResourceGet();
            contents = getter.get("https://api.github.com/repositories?since=" + since, "github/repositories");

            // Convert JSON string to JSONObject
            JSONArray repositoryEntries = new JSONArray(contents.toString());

            for (int i = 0; i < repositoryEntries.length(); i++) {
                JSONObject owner = repositoryEntries.getJSONObject(i).getJSONObject("owner");
                System.out.println(owner.getString("repos_url"));
            }

            int nextId = repositoryEntries.getJSONObject(repositoryEntries.length() - 1).getInt("id");
            float percent = (float) (nextId / 110000000.0);
            long duration = System.currentTimeMillis() - TIME_START;
            long eta = (long) (duration / percent);
            System.err.println("duration/ETA/time/next/progress: " +
                    formatDuration(duration) + "/" +
                    formatDuration(eta) + "/" +
                    dateFormatter.format(new Date()) + "/" +
                    nextId + "/" +
                    (percent * 100) + "%" );
            return nextId;


        } catch (IOException ioe) {

            ResourceGet getter = new ResourceGet();
            contents = getter.get("https://api.github.com/rate_limit", "github/rate-limit");
            JSONObject object = new JSONObject(contents);
            long reset = object.getJSONObject("resources").getJSONObject("core").getLong("reset") * 1000;
            System.err.println("API limit reached: continue at " + reset + " (" + dateFormatter.format(reset) +")");
            CONTINUE = reset;
            return -1;

        }
    }

}

package de.ovgu.guthub_json_crawl;

/**
 * Created by marcus on 14.11.17.
 */
public class DateUtils {

    public static final String formatDuration(long duration) {
        long seconds = duration / 1000;
        long minutes = seconds / 60;
        long hours = minutes / 60;
        long days = hours / 24;
        long weeks = days / 7;
        long months = weeks / 4;
        long years = months / 12;
        return years + "y " + months % 12 + "m " + weeks % 4 + "w " + days + "d " + hours % 24 + "h " + minutes % 60 + "min " + seconds % 60 + "sec";
    }

}

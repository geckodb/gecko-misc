package com.geckodb.misc.stringdbgen;

import com.geckodb.misc.stringdbgen.utils.FileUtils;

import java.io.*;
import java.sql.*;
import java.util.ArrayList;

/**
 * Created by marcus on 21.02.18.
 */
public class Benchmark {

    interface InsertCommandFormatter {
        long insertTuple(ArrayList<Integer> ids, ArrayList<String> xs, ArrayList<String> ys);
        void dropTable();
        void createTable();
        long getDatabaseSize();
    }

    static class SQLInsertCommandFormatter implements InsertCommandFormatter {

        private final String url = "jdbc:postgresql://localhost/BenchTest";
        private final String user = "postgres";
        private final String password;
        Connection conn = null;

        public SQLInsertCommandFormatter() {
            password = System.getenv().get("POSTGRES_PWD");
            connect();
        }

        public Connection connect() {
            try {
                conn = DriverManager.getConnection(url, user, password);
                System.out.println("Connected to the PostgreSQL server successfully.");
            } catch (SQLException e) {
                System.out.println(e.getMessage());
            }

            return conn;
        }

        @Override
        public long insertTuple(ArrayList<Integer> ids, ArrayList<String> xs, ArrayList<String> ys) {
            String sql = "";
            int num = ids.size();
            System.err.print("Insert " + num + " tuples...");
            for (int i = 0; i < num; i++) {
                sql += "INSERT INTO \"BenchTable\".\"TNB_TABLE\" (id, x, y) VALUES ('"+ids.get(i)+"', '"+xs.get(i)+"', '"+ys.get(i)+"');";
            }
            try {

                Statement stmt = conn.createStatement();
                long begin = System.currentTimeMillis();
                stmt.execute(sql);
                long end = System.currentTimeMillis();
                System.err.println(" [DONE]");
                return (end - begin);
            } catch (SQLException ex) {
                System.out.println(ex.getMessage());
            }
            return Long.MAX_VALUE;
        }

        @Override
        public void dropTable() {
            String statement = "DROP TABLE IF EXISTS \"BenchTable\".\"TNB_TABLE\";";

            try (PreparedStatement pstmt = conn.prepareStatement(statement, Statement.NO_GENERATED_KEYS)) {
                pstmt.executeUpdate();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        @Override
        public void createTable() {
            String statement = "CREATE TABLE \"BenchTable\".\"TNB_TABLE\"\n" +
                                "(\n" +
                                "    id integer NOT NULL,\n" +
                                "    x text COLLATE pg_catalog.\"default\" NOT NULL,\n" +
                                "    y text COLLATE pg_catalog.\"default\" NOT NULL,\n" +
                                "    CONSTRAINT \"TNB_TABLE_pkey\" PRIMARY KEY (id)\n" +
                                ")\n" +
                                "WITH (\n" +
                                "    OIDS = FALSE\n" +
                                ")\n" +
                                "TABLESPACE pg_default;\n" +
                                "\n" +
                                "ALTER TABLE \"BenchTable\".\"TNB_TABLE\"\n" +
                                "    OWNER to postgres;";

            try (PreparedStatement pstmt = conn.prepareStatement(statement, Statement.NO_GENERATED_KEYS)) {
                pstmt.executeUpdate();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        @Override
        public long getDatabaseSize() {

            String sql = "SELECT total\n" +
                    "FROM (\n" +
                    "SELECT *, pg_size_pretty(total_bytes) AS total\n" +
                    "    , pg_size_pretty(index_bytes) AS INDEX\n" +
                    "    , pg_size_pretty(toast_bytes) AS toast\n" +
                    "    , pg_size_pretty(table_bytes) AS TABLE\n" +
                    "  FROM (\n" +
                    "  SELECT *, total_bytes-index_bytes-COALESCE(toast_bytes,0) AS table_bytes FROM (\n" +
                    "      SELECT c.oid,nspname AS table_schema, relname AS TABLE_NAME\n" +
                    "              , c.reltuples AS row_estimate\n" +
                    "              , pg_total_relation_size(c.oid) AS total_bytes\n" +
                    "              , pg_indexes_size(c.oid) AS index_bytes\n" +
                    "              , pg_total_relation_size(reltoastrelid) AS toast_bytes\n" +
                    "          FROM pg_class c\n" +
                    "          LEFT JOIN pg_namespace n ON n.oid = c.relnamespace\n" +
                    "          WHERE relkind = 'r' \n" +
                    "  ) a \n" +
                    ") a ) b WHERE table_name = 'TNB_TABLE';";

                    try {
                        Statement st = conn.createStatement();
                        ResultSet rs = st.executeQuery(sql);
                        while (rs.next()) {
                            String sizeStr = rs.getString(1);
                            return SizeStrToByte(sizeStr);
                        }
                        rs.close();
                        st.close();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }

            return 0;
        }

        private long SizeStrToByte(String sizeStr) {
            sizeStr = sizeStr.toLowerCase();
            String value = sizeStr.substring(0, sizeStr.indexOf(" "));
            if (sizeStr.endsWith("kb")) {
                return Long.valueOf(value) * 1024;
            } else if (sizeStr.endsWith("mb")) {
                return Long.valueOf(value) * 1024 * 1024;
            } else {
                return Long.valueOf(value) * 1024 * 1024 * 1024;
            }
        }
    }

    public static void main(String... args) throws FileNotFoundException {

        int numBulkInserts = 100;
        int batchNum = 0;
        int bulkInsertTargetSize = 10000;
        int bytesTotal = 0;
        int tuplesTotal = 0;
        InsertCommandFormatter formatter = new SQLInsertCommandFormatter();

        formatter.dropTable();
        formatter.createTable();

        System.setOut(new PrintStream(new BufferedOutputStream(new FileOutputStream("/Users/marcus/temp/results-insert.csv"))));
        System.out.println("TimeMs;TotalSize;NumTuples;DbSize");
        try {
            String datasetPath = "/Users/marcus/git/gecko/gecko-misc/string-db-gen/output/synth.csv";
            ArrayList<Integer> insertTuplesId = new ArrayList<>();
            ArrayList<String> insertTuplesX = new ArrayList<>();
            ArrayList<String> insertTuplesY = new ArrayList<>();
            int uniqueId = 0;
            BufferedReader forwardReader = FileUtils.openRead(datasetPath);
            BufferedReader backwardReader = FileUtils.openReadReverse(datasetPath);
            String string1, string2;
            forwardReader.readLine(); // Skip header

            while (numBulkInserts-- > 0) {
                int bulkInsertSize = 0;

                while (bulkInsertSize < bulkInsertTargetSize) {
                    if (((string1 = forwardReader.readLine()) == null) || ((string2 = backwardReader.readLine()) == null)) {
                        forwardReader.close();
                        backwardReader.close();
                        forwardReader = FileUtils.openRead(datasetPath);
                        forwardReader.readLine();
                        backwardReader = FileUtils.openReadReverse(datasetPath);
                        string1 = forwardReader.readLine();
                        string2 = backwardReader.readLine();
                    }

                    String[] components1 = string1.split(";");
                    String[] components2 = string2.split(";");

                    insertTuplesId.add(uniqueId++);
                    insertTuplesX.add(components1[3]);
                    insertTuplesY.add(components2[3]);
                    bytesTotal += 8 + components1[3].length() + components2[3].length();
                    bulkInsertSize++;
                    tuplesTotal++;
                }

                System.err.println(batchNum++ + " DB total size (inserted): " + (bytesTotal / 1024 / 1024.0) + "MiB, DB total size (actual):" + formatter.getDatabaseSize());

                long duration = formatter.insertTuple(insertTuplesId, insertTuplesX, insertTuplesY);
                System.out.println(duration + ";" + bytesTotal + ";" +  tuplesTotal + ";" + formatter.getDatabaseSize()) ;
                System.out.flush();
                insertTuplesId.clear();
                insertTuplesX.clear();
                insertTuplesY.clear();
            }






        } catch (Exception e) {
            e.printStackTrace();
        }

    }

}

package com.yahoo;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;


public class Yahoo_Converter {
	
	public static void main(String[] args) {
		List<Yahoo_Model> yahooModalDataList = populateDataIntoModel("C:\\Users\\gopimsi\\eclipse-workspace\\yahoo\\src\\main\\java\\com\\yahoo\\Yahoo_Input_Data.txt"); 
		
		List<String> neoStmtList = appendNeo4JStatements(yahooModalDataList);
		String outpuFilePath = "C:\\Users\\gopimsi\\eclipse-workspace\\yahoo\\src\\main\\java\\com\\yahoo\\Yahoo_Output_Data.txt";
		writeToOutputFile(outpuFilePath, neoStmtList);
  }
	
	
	static void writeToOutputFile(String OutpuFilePath, List<String> neoStmtList) {
		BufferedWriter bw = null;
		FileWriter fw = null;
		try {
			fw = new FileWriter(OutpuFilePath);
			bw = new BufferedWriter(fw);
			
			
			for(String neoStmt : neoStmtList) {
				bw.write(neoStmt);
				bw.newLine();
			}
			
			} catch (Exception e) {
    	e.printStackTrace();
		}
		finally {
			try {
				if (bw != null)
					bw.close();
				if (fw != null)
					fw.close();
			} catch (IOException ex) {
				ex.printStackTrace();
			}
		}
		
	}
	
	/**
	 * This method takes the file path of input file
	 * and generates an array of Neo4J Statement
	 * @param yahooInputDataFilePath
	 * @return
	 */
  static List<Yahoo_Model> populateDataIntoModel(String yahooInputDataFilePath) {
	  List<Yahoo_Model> yahooModalDataList = new ArrayList<Yahoo_Model>();
	  BufferedReader reader;
		try {
			reader = new BufferedReader(new FileReader(
					yahooInputDataFilePath));
			String line = reader.readLine();
			while (line != null) {
				if(!line.startsWith("Date")) {
					String splittedLine [] = line.split(",");
					Yahoo_Model yahooModel = new Yahoo_Model();
					yahooModel.setDate(splittedLine[0]);
					yahooModel.setOpen(Float.valueOf(splittedLine[1]));
					yahooModel.setHigh(Float.valueOf(splittedLine[2]));
					yahooModel.setLow(Float.valueOf(splittedLine[3]));
					yahooModel.setClose(Float.valueOf(splittedLine[4]));
					yahooModel.setVolume(Integer.parseInt(splittedLine[5]));
					yahooModel.setOpenInt(Integer.parseInt(splittedLine[6]));
					yahooModalDataList.add(yahooModel);
				}
				line = reader.readLine();
			}
			reader.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		 return yahooModalDataList;
			}
	  
  static List<String> appendNeo4JStatements(List<Yahoo_Model> yahooDataModelList) {
	  List<String> neo4JStmts = new LinkedList<String>();
	  for (Yahoo_Model yadaDataObj : yahooDataModelList) {
//	  String appdendNeoStmt = 
//	  "CREATE (a:Node {date: " + yadaDataObj.getDate()  +"})\n"+
//	  "CREATE (b:Node {open: " + yadaDataObj.getOpen()  +"})\n"+
//	  "CREATE (c:Node {high: " + yadaDataObj.getHigh()  +"})\n"+
//	  "CREATE (d:Node {low: " + yadaDataObj.getLow()  +"})\n"+
//	  "CREATE (e:Node {close: " + yadaDataObj.getClose()  +"})\n"+
//	  "CREATE (f:Node {volume: " + yadaDataObj.getVolume()  +"})\n"+
//	  "CREATE (g:Node {openInt: " + yadaDataObj.getOpenInt()  +"})\n"+
//	  "CREATE (a)-[:LINK]->(b)";
	  
	  String createStmts = 
	  "CREATE (neo4j:yahoo {Date: \""+yadaDataObj.getDate()+"\"})\n"+
	  "CREATE (Open:openstock {value: \""+yadaDataObj.getOpen()+"\"})\n"+
	  "CREATE (High:highstock {value: \""+yadaDataObj.getHigh()+"\"})\n"+
	  "CREATE (Low:lowstock {value: \""+yadaDataObj.getLow()+"\"})\n"+
	  "CREATE (Close:closestock {value: \""+yadaDataObj.getClose()+"\"})\n"+
	  "CREATE (Volume:volumestock {value: \""+yadaDataObj.getVolume()+"\"})\n"+
	  "CREATE (Openint:openintstock {value: \""+yadaDataObj.getOpenInt()+"\"})\n"+
	  "\n";

	  String matchStmts = 
			  "MATCH (a:yahoo), (b:openstock) WHERE a.Date="+yadaDataObj.getDate()+" AND b.value="+yadaDataObj.getOpen()+" CREATE (a)- [r:open_stock]->(b) RETURN a,b\n"+
			  "MATCH (a:yahoo), (c:highstock) WHERE a.Date="+yadaDataObj.getDate()+" AND c.value="+yadaDataObj.getHigh()+" CREATE (a)- [r:high_stock]->(c) RETURN a,c\n"+
			  "MATCH (a:yahoo), (d:lowstock) WHERE a.Date="+yadaDataObj.getDate()+" AND d.value="+yadaDataObj.getLow()+" CREATE (a)- [r:low_stock]->(d) RETURN a,d\n"+
			  "MATCH (a:yahoo), (e:closestock) WHERE a.Date="+yadaDataObj.getDate()+" AND e.value="+yadaDataObj.getClose()+" CREATE (a)- [r:close_stock]->(e) RETURN a,e\n"+
			  "MATCH (a:yahoo), (f:volumestock) WHERE a.Date="+yadaDataObj.getDate()+" AND f.value="+yadaDataObj.getVolume()+" CREATE (a)- [r:volume_stock]->(f) RETURN a,f\n"+
			  "MATCH (a:yahoo), (g:openintstock) WHERE a.Date="+yadaDataObj.getDate()+" AND g.value="+yadaDataObj.getOpenInt()+" CREATE (a)- [r:openint_stock]->(g) RETURN a,g\n"+
			  "MATCH (n) return n\n";
	  neo4JStmts.add(createStmts+matchStmts);
	  }
	  return neo4JStmts;
  }
  
}
	
  



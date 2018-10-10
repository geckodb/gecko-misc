package com.google_webgraph;

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


public class Google_Converter {
	
	public static void main(String[] args) {
		List<Google_Model> yahooModalDataList = populateDataIntoModel("C:\\Users\\gopimsi\\eclipse-workspace\\google-webgraph\\src\\main\\java\\com\\google_webgraph\\Google_Input.txt"); 
		List<String> neoStmtList = appendNeo4JStatements(yahooModalDataList);
		String outpuFilePath = "C:\\Users\\gopimsi\\eclipse-workspace\\google-webgraph\\src\\main\\java\\com\\google_webgraph\\Google_Output_Data.txt";
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
  static List<Google_Model> populateDataIntoModel(String yahooInputDataFilePath) {
	  List<Google_Model> googleDataModelList = new ArrayList<Google_Model>();
	  BufferedReader reader;
		try {
			reader = new BufferedReader(new FileReader(
					yahooInputDataFilePath));
			String line = "";
			
//			while (line != null && line.trim().length() != 0) {
			while((line = reader.readLine()) != null && line.trim().length()!=0) {
				int counter=0;
				Google_Model googleModel = new Google_Model();
//				while((line = reader.readLine()) != null && line.trim().length()!=0) {
					while (line != null && line.trim().length() != 0) {
				if(!line.startsWith("From")) {
					counter++;
					String splittedLine [] = line.split("\\s+");
					
					if(counter == 1) {
					googleModel.setA(Integer.parseInt(splittedLine[0]));
					googleModel.setB(Integer.parseInt(splittedLine[1]));
					}
					
					if(counter == 2) {
						googleModel.setC(Integer.parseInt(splittedLine[1]));
					}
					if(counter == 3) {
						googleModel.setD(Integer.parseInt(splittedLine[1]));
					}
					if(counter == 4) {
						googleModel.setE(Integer.parseInt(splittedLine[1]));
						googleDataModelList.add(googleModel);
					}
//					line = reader.readLine();
				}
				line = reader.readLine();
			}
				
//				line = reader.readLine();
			}
			reader.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		 return googleDataModelList;
			}
  
  
  	
	  
  static List<String> appendNeo4JStatements(List<Google_Model> yahooDataModelList) {
	  List<String> neo4JStmts = new LinkedList<String>();
	  for (Google_Model googleDataObj : yahooDataModelList) {
	  
	  String createStmts = 
			  		  "create (a:node_from {f_id: "+googleDataObj.getA()+"})\n"+
					  "create (b:node_to {to_id: "+googleDataObj.getB()+"})\n"+
					  "create (c:node_to {to_id: "+googleDataObj.getC()+"})\n"+
					  "create (d:node_to {to_id: "+googleDataObj.getD()+"})\n"+
					  "create (e:node_to {to_id: "+googleDataObj.getE()+"})\n\n";
	  String matchStmts = 
			  		  "MERGE (a:node_from {f_id: "+googleDataObj.getA()+"})\n"+
					  "MERGE (b:node_to {to_id: "+googleDataObj.getB()+"})\n"+
					  "MERGE (c:node_to {to_id: "+googleDataObj.getC()+"})\n"+
					  "MERGE (d:node_to {to_id: "+googleDataObj.getD()+"})\n"+
					  "MERGE (e:node_to {to_id: "+googleDataObj.getE()+"})\n\n";
	  String relationStmnts = 
					  "create (a)-[:directional_edge]->(b)\n"+
					  "create (a)-[:directional_edge]->(c)\n"+
					  "create (a)-[:directional_edge]->(d)\n"+
					  "create (a)-[:directional_edge]->(e)\n"+
					  "MATCH (n) return n\n\n";
	  
	  neo4JStmts.add(createStmts+matchStmts+relationStmnts);
	  }
	  return neo4JStmts;
  }
  
}
	
  



package Mag.MagImplementation;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.Set;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;


public class MAG_Converter {
	
	public static void main(String[] args) {
		List<String> outputData = convertJsonToNeo4j("C:\\Users\\gopimsi\\eclipse-workspace\\MagImplementation"
		    + "\\src\\main\\java\\Mag\\MagImplementation\\MAG_Input.txt");
		
				
		String outpuFilePath = "C:\\Users\\gopimsi\\eclipse-workspace\\MagImplementation"
					    + "\\src\\main\\java\\Mag\\MagImplementation\\MAG_Output.txt";
		writeToOutputFile( outpuFilePath, outputData);
		
		System.out.println("Output has been generated to below file :\n" + outpuFilePath ); 
  }
	
	
	/**
	 * This method takes the file path of input file
	 * and generates an array of Neo4J Statement
	 * @param magFileLocationPath
	 * @return
	 */
  static List<String> convertJsonToNeo4j(String magFileLocationPath) {
	 JSONParser parser = new JSONParser();
	 
	 List<MagDataModel> magDataModelList = new LinkedList<MagDataModel>();	
	 
	 try {
	        Object obj = parser.parse(new FileReader(magFileLocationPath));
	        JSONArray jsonArray = (JSONArray) obj;
	        int length = jsonArray.size();
	        for (int i =0; i< length; i++) {
	            JSONObject jsonObject = (JSONObject) jsonArray.get(i);
	            Set s = jsonObject.entrySet();
	            Iterator iter = s.iterator();
	            LinkedList ll = new LinkedList();
	            LinkedList lm = new LinkedList();
	            while(iter.hasNext()){
	                Map.Entry me = (Map.Entry) iter.next();
	                ll.add(me.getValue());
	                lm.add(me.getKey());
	            }
	            String refStr = ll.get(0).toString();
	            
	            List<String> refArr =  convertStringListToOptional(jsonObject, "references");
	            List<String> fosArr =  convertStringListToOptional(jsonObject, "fos");
	            List<String> refUrl = convertStringListToOptional(jsonObject, "url");
	            String title =  convertStringToOptional(jsonObject,"title");
	            String lang =  convertStringToOptional(jsonObject,"lang");
	            String id = convertStringToOptional(jsonObject,"id");
	            String abstr = convertStringToOptional(jsonObject,"abstract");
	            int year = convertIntToOptional(jsonObject, "year");
	            
	            MagDataModel magDataModelObj = new MagDataModel();
	            magDataModelObj.setReferences(refArr);
	            magDataModelObj.setYear(year);
	            magDataModelObj.setAbstract(abstr);
	            magDataModelObj.setId(id);
	            magDataModelObj.setFos(fosArr);
	            magDataModelObj.setTitle(title);
	            magDataModelObj.setLang(id);
	            magDataModelObj.setUrl(refUrl);
	            magDataModelList.add(magDataModelObj);
	            }            
	    } catch (Exception e) {
	        e.printStackTrace();
	    }
	 return appendNeo4JStatements(magDataModelList);
	}
  
  
  static List<String> appendNeo4JStatements(List<MagDataModel> magDataModelList) {
	  List<String> neo4JStmts = new LinkedList<String>();
	  for (MagDataModel magDataObj : magDataModelList) {
	  
	  String createStmnts = "";
	  String mergeStmnts = "";
	  
	  if( magDataObj.getTitle()!=null && magDataObj.getTitle().length() >1 ) {
		  createStmnts = createStmnts + "create(neo4j:paper {title: "+magDataObj.getTitle()+"})\n";
		  
	  }
	  if( magDataObj.getLang()!=null && magDataObj.getLang().length() >1 ) {
		  createStmnts = createStmnts + "create(language:text {language: "+magDataObj.getLang()+"})\n";
		  mergeStmnts = mergeStmnts + "MATCH (a:paper), (b:text) WHERE a.title="+magDataObj.getTitle()+
				  " AND b.language="+magDataObj.getLang()+" CREATE (a)-[:language]->(b)\n";
	  }
	  if( magDataObj.getYear()!=null) {
		  createStmnts = createStmnts + "create(year:number {year: "+magDataObj.getYear()+"})\n";
		  mergeStmnts = mergeStmnts + "MATCH (a:paper), (b:year) WHERE a.title="+magDataObj.getTitle()+
				  " AND b.year="+magDataObj.getYear()+" CREATE (a)-[:year]->(b)\n";
	  }
	  if( magDataObj.getReferences()!=null &&  magDataObj.getReferences().size() > 1) {
		  createStmnts = createStmnts + "create(references:references {references: "+magDataObj.getReferences()+"})\n";
		  mergeStmnts = mergeStmnts + "MATCH (a:paper), (b:references) WHERE a.title="+magDataObj.getTitle()+
				  " AND b:references="+magDataObj.getReferences()+
				  " CREATE (a)-[:references]->(b)\n";
		  
	  }
	  if( magDataObj.getAbstract()!=null && magDataObj.getAbstract().length() >1 ) {
		  createStmnts = createStmnts + "create(abstract: abstract {abstract: "+magDataObj.getAbstract()+"})\n";
		  mergeStmnts = mergeStmnts + "MATCH (a:paper), (b:abstract) WHERE a.title="+magDataObj.getTitle()+
				  " AND b:abstract="+magDataObj.getAbstract()+
				  " CREATE (a)-[:abstract]->(b)\n";		  
	  }
	  if( magDataObj.getUrl()!=null &&  magDataObj.getUrl().size() > 1) {
		  createStmnts = createStmnts + "create(url:url {url: "+magDataObj.getUrl()+"})\n";
		  mergeStmnts = mergeStmnts + "MATCH (a:paper), (b:url) WHERE a.title="+magDataObj.getTitle()+
				  " AND b:url="+magDataObj.getUrl()+
				  " CREATE (a)-[:url]->(b)\n";	

	  }
	  if( magDataObj.getId()!=null && magDataObj.getId().length() >1 ) {
		  createStmnts = createStmnts + "create(id:id {id: "+magDataObj.getId()+"})\n";
		  mergeStmnts = mergeStmnts + "MATCH (a:paper), (b:id) WHERE a.title="+magDataObj.getTitle()+
				  " AND b:id="+magDataObj.getId()+
				  " CREATE (a)-[:id]->(b)\n";			  
	  }
	  if( magDataObj.getFos()!=null &&  magDataObj.getFos().size() > 1) {
		  createStmnts = createStmnts + "create(fos:fos {fos: "+magDataObj.getFos()+"})\n";
		  mergeStmnts = mergeStmnts + "MATCH (a:paper), (b:fos) WHERE a.title="+magDataObj.getTitle()+
				  " AND b:fos="+magDataObj.getFos()+
				  " CREATE (a)-[:fos]->(b)\n";	
	  }
	  
	  createStmnts = createStmnts + "\n";
	  mergeStmnts = mergeStmnts + "MATCH (n) RETURN n\n\n";
	  
	  
	  

//	  MATCH (a:paper), (b:text) WHERE a.title="System and Method for Maskless Direct Write Lithography" AND b.language="English" CREATE (a)-[:language]->(b)
//	  MATCH (a:paper), (b:number) WHERE a.title="System and Method for Maskless Direct Write Lithography" AND b.year="2015" CREATE (a)-[:published]->(b)
//	  MATCH (a:paper), (b:references) WHERE a.title="System and Method for Maskless Direct Write Lithography" AND b.references="354c172f-d877-4e60-a7eb-c1b1cf03ce4d" CREATE (a)-[:reference]->(b)
//	  MATCH (a:paper), (b:abstract) WHERE a.title="System and Method for Maskless Direct Write Lithography" AND b.abstract="A system and method for maskless direct write lithography are disclosed" CREATE (a)-[:abstract]->(b)
//	  MATCH (a:paper), (b:url) WHERE a.title="System and Method for Maskless Direct Write Lithography" AND b.url="http://www.freepatentsonline.com/y2016/0211117.html" CREATE (a)-[:url]->(b)
//	  MATCH (a:paper), (b:id) WHERE a.title="System and Method for Maskless Direct Write Lithography" AND b.id="0000002e-c2f2-4e25-9341-60d39130ac7a" CREATE (a)-[:id]->(b)
//	  MATCH (a:paper), (b:fos) WHERE a.title="System and Method for Maskless Direct Write Lithography" AND b.fos="Electronic engineering" CREATE (a)-[:fos]->(b)
//
//	  MATCH (n) RETURN n
	  
	  
	  
	  
	  
	  
	  
	  neo4JStmts.add(createStmnts+mergeStmnts);
	  }
	  return neo4JStmts;
  }
  
  static String convertStringToOptional(JSONObject jsonObject, String nodeName) {
	  String returnStr="";
      Object returnStrNull = jsonObject.get(nodeName);
      Optional<Object> abstrOptional = Optional.ofNullable(returnStrNull);
      if(abstrOptional.isPresent()) {
    	  returnStr = jsonObject.get(nodeName).toString();
    	  if(returnStr.contains("\\")){
    		  returnStr.replace("\\", "\\\\");
    	       }
      }
      
      if(null!= jsonObject.get(nodeName) && nodeName.equals("title")) {
    	  if(returnStr.contains("\\")){
    		  returnStr.replace("\\", "\\\\");
    	       }
      }
      
  return returnStr;
  }
  
  static List<String> convertStringListToOptional(JSONObject jsonObject, String nodeName) {
	  List<String> strList = new ArrayList<String>();
      Object returnStrNull = jsonObject.get(nodeName);
      Optional<Object> abstrOptional = Optional.ofNullable(returnStrNull);
      if(abstrOptional.isPresent()) {
    	  strList.add(jsonObject.get(nodeName).toString());
      }
  return strList;
  }
  
  static Integer convertIntToOptional(JSONObject jsonObject, String nodeName) {
	  Integer intVal = null;
	  Object returnStrNull = jsonObject.get(nodeName);
	  if(returnStrNull != null) {
		  intVal = Integer.parseInt(returnStrNull.toString());
	  }
  return intVal;
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
}

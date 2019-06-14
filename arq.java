import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class arq {
	
	String fullPath;
	String name;
	
	
	public arq(String fullPath,String name) {
		this.fullPath = fullPath;
		this.name = name;
	}


	public String getFullPath() {
		return fullPath;
	}


	public void setFullPath(String fullPath) {
		this.fullPath = fullPath;
	}


	public String getName() {
		return name;
	}


	public void setName(String name) {
		this.name = name;
	}
	
	public void readFile(File myObj) {
		try {
			Scanner myReader = new Scanner(myObj); 
		      while (myReader.hasNextLine()) {
		        String data = myReader.nextLine();
		        System.out.println(data);
		      }
		      myReader.close();
		    } catch (FileNotFoundException e) {
		      System.out.println("An error occurred.");
		      e.printStackTrace();
		 } 
	}
}

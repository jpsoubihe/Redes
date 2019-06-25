package RMI_Server;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.io.Serializable;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.util.ArrayList;
import java.util.Scanner;

public class Server implements Compute, Serializable {
	String[] arquivos = {"julia@gmail.com","ana@gmail.com","jpsoubihe@gmail.com","ze@gmail.com","maria_silva@gmail.com"}; //we can turn this into an arraylist?
	long t1;
	long t2;
	long time = 0;	
    	
    public Server() {
    }

    public String sayHello() {
        return "Hello, world!";
    }
    
     public boolean addXP(String xp,String email) throws IOException{
    	 t1 = System.currentTimeMillis();
    	 File a = new File(email);
    	 int count = 0;
    	 Scanner leitor = new Scanner(a);
    	 while(leitor.hasNextLine()) {
    		 leitor.nextLine();
    		 count++;
    	 }
    	 count++;
    	 leitor.close();
    	 String experience = "(" + (count - 5) + ") "+ xp + "\n";
    	 FileWriter writer = new FileWriter(email,true);
    	 writer.write(experience);
    	 writer.close();
    	 t2 = System.currentTimeMillis();
    	 time = t2 - t1;
    	 System.out.println("Current Time in milliseconds = " + time);
    	 return true;

     }
    
    public ArrayList<Profile> buildProf() throws FileNotFoundException{
    	t1 = System.currentTimeMillis();
    	ArrayList<Profile> profs = new ArrayList<Profile>();
    	for(int i = 0;i < arquivos.length;i++)
    		profs.add(new Profile(new File(arquivos[i])));
    	t2 = System.currentTimeMillis();
    	time = t2 - t1;
    	System.out.println("Current Time in milliseconds = " + time);
    	return profs;
    }
    
    public ArrayList<String> getHabilities(String residence) throws FileNotFoundException{
    	t1 = System.currentTimeMillis();
    	ArrayList<Profile> profiles = this.buildProf();
    	ArrayList<String> habilidades = new ArrayList<String>();
    	for(int i = 0;i < profiles.size();i++) {
    		if(profiles.get(i).getRes().equals(residence)) {
    			habilidades.add(profiles.get(i).getName() + " " + profiles.get(i).getSurname());
    			habilidades.add(profiles.get(i).getHab());
    		}
    	}
    	t2 = System.currentTimeMillis();
    	time = t2 - t1;
    	System.out.println("Current Time in milliseconds = " + time);
    	return habilidades;
    }
    
    public ArrayList<String> getGraduation(String grad) throws FileNotFoundException { 
    	t1 = System.currentTimeMillis();
    	ArrayList<Profile> profiles = this.buildProf();
    	ArrayList<String> nomes = new ArrayList<String>();
    	for(int i = 0;i < profiles.size();i++) {	
    		if(profiles.get(i).getGrad().equals(grad)) {
    			//System.out.println("Entrou " + i + " count = " + count);
    			nomes.add(profiles.get(i).getName() + " " + profiles.get(i).getSurname());
    		}	
    	}
    	t2 = System.currentTimeMillis();
    	time = t2 - t1;
    	System.out.println("Current Time in milliseconds = " + time);
    	return nomes;
    }
    
     public String returnProfile(String email) throws FileNotFoundException {
    	 t1 = System.currentTimeMillis();
    	 File prof_file = new File(email);
    	 Scanner leitor = new Scanner(prof_file);
    	 String perfil;
    	 perfil = leitor.nextLine();
    	 while(leitor.hasNextLine()) {
    		 perfil += '\n' + leitor.nextLine();
    	 }
    	 leitor.close();
    	 t2 = System.currentTimeMillis();
    	 time = t2 - t1;
    	 System.out.println("Current Time in milliseconds = " + time);
    	 return perfil;
     }
    
    public boolean checkLogin(String login) throws RemoteException{
    	t1 = System.currentTimeMillis();
    	for(int i = 0;i < arquivos.length;i++) {
    		if(login.equals(arquivos[i])){
    			t2 = System.currentTimeMillis();
    	    	time = t2 - t1;
    	    	System.out.println("Current Time in milliseconds = " + time);
    	    	return true;
    		}
    			
    	}
    	t2 = System.currentTimeMillis();
    	time = t2 - t1;
    	System.out.println("Current Time in milliseconds = " + time);
    	return false;
    }
    
    public String profileGrad(String formation) throws FileNotFoundException {
    	//String[] arquivos = {"julia@gmail.com","ana@gmail.com","jpsoubihe@gmail.com"};
    	String res;
    	Profile p;
    	Scanner sc = null;
    	for(String i: arquivos) {
    		 sc = new Scanner(new File(i));
    		
    		
    	}
    	sc.close();
    	return null;
    }
    
    public ArrayList<String> experiences(String email) throws FileNotFoundException{
    	t1 = System.currentTimeMillis();
    	int i;
    	Scanner leitor = new Scanner(new File(email));
    	for(i = 0;i < 5;i++)
    		leitor.nextLine();
    	leitor.skip("Experiência: ");
    	ArrayList<String> experience = new ArrayList<String>();
		while(leitor.hasNextLine()) {
			leitor.next();
			leitor.skip(" ");
			experience.add(leitor.nextLine());
		}	
		leitor.close();
		t2 = System.currentTimeMillis();
		time = t2 - t1;
		System.out.println("Current Time in milliseconds = " + time);
    	return experience;
    }
    public static void main(String args[]) {
        
        try {
            Server obj = new Server();
            Compute stub = (Compute) UnicastRemoteObject.exportObject(obj, 0);

            // Bind the remote object's stub in the registry
            Registry registry = LocateRegistry.getRegistry();
            registry.bind("Compute", stub);

            System.err.println("Server ready");
        } catch (Exception e) {
            System.err.println("Server exception: " + e.toString());
            e.printStackTrace();
        }
    }
}

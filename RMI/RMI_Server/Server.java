package RMI_Server;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
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
    	
    	
    public Server() {
    }

    public String sayHello() {
        return "Hello, world!";
    }
    
    boolean addXP(String xp,String email) throws FileNotFoundException{
    	File a = new File(email);
    	int count = 0;
    	Scanner leitor = new Scanner(prof_file);
    	while(sc.hasNextLine()) {
    		sc.nextLine();
    		count++;
    	}
    	sc.nextLine();
    	count++;
    	//write in file a exp count - 5 + 1
    }
    
    public ArrayList<Profile> buildProf() throws FileNotFoundException{
    	ArrayList<Profile> profs = new ArrayList<Profile>();
    	for(int i = 0;i < arquivos.length;i++)
    		profs.add(new Profile(new File(arquivos[i])));
//    	p = new Profile(a);
//    	System.out.println(p.toString());
//		return p;
		return profs;
    }
    
    public ArrayList<String> getHabilities(String residence) throws FileNotFoundException{
    	ArrayList<Profile> profiles = this.buildProf();
    	ArrayList<String> habilidades = new ArrayList<String>();
    	for(int i = 0;i < profiles.size();i++) {
    		if(profiles.get(i).getRes().equals(residence)) {
    			habilidades.add(profiles.get(i).getName() + " " + profiles.get(i).getSurname());
    			habilidades.add(profiles.get(i).getHab());
    		}
    	}
    	return habilidades;
    }
    
    public ArrayList<String> getGraduation(String grad) throws FileNotFoundException { 
    	ArrayList<Profile> profiles = this.buildProf();
    	ArrayList<String> nomes = new ArrayList<String>();
    	for(int i = 0;i < profiles.size();i++) {	
    		if(profiles.get(i).getGrad().equals(grad)) {
    			//System.out.println("Entrou " + i + " count = " + count);
    			nomes.add(profiles.get(i).getName() + " " + profiles.get(i).getSurname());
    		}	
    	}
    	return nomes;
    }
    
     public String returnProfile(String email) throws FileNotFoundException {
    	File prof_file = new File(email);
    	Scanner leitor = new Scanner(prof_file);
    	String perfil;
    	perfil = leitor.nextLine();
    	while(leitor.hasNextLine()) {
    		perfil += '\n' + leitor.nextLine();
    	}
    	leitor.close();
    	return perfil;
    }
    
    public boolean checkLogin(String login) throws RemoteException{
    	for(int i = 0;i < arquivos.length;i++) {
    		if(login.equals(arquivos[i]))
    			return true;
    	}
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
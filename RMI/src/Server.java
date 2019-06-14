import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.io.Serializable;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.util.Scanner;

public class Server implements Compute, Serializable {
        
    public Server() {}

    public String sayHello() {
        return "Hello, world!";
    }
    public void buildProf(File a) throws IOException {
    	String st;
    	Profile p;
    	try (ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream("Profile.ser"))){
   		 	
    		p = new Profile(a);
    		
   		 	//System.out.println(p.toString());
   		 	//return p;
			//return p;
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	//return null;
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
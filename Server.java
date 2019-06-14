import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;

//import java.io.BufferedReader;
//import java.io.File;
//import java.io.FileNotFoundException;
//import java.io.FileReader;
//import java.io.IOException;
//import java.io.RandomAccessFile;
//import java.util.ArrayList;
//import java.util.Scanner;
//
//public class server {
//
//	public static void main(String[] args) throws IOException {
//		Scanner sc = new Scanner(System.in);
//		BufferedReader br;
//		String email;
//		String name;
//		String surname;
//		String res;
//		String grad;
//		String hab;
//		String auxiliar;
//		ArrayList<String> experience;
//		String[] arquivos = {"julia@gmail.com","ana@gmail.com","jpsoubihe@gmail.com"};
//		ArrayList<Profile> pVector = new ArrayList<Profile>();
//		Profile aux;
//				
//		for(int j = 0;j < arquivos.length;j++) {
//			File a = new File(arquivos[j] + ".txt");
//			pVector.add(new Profile(a));
//		}
//		
//		System.out.println(pVector.toString());
//		
//		
//		
//		
//	}
//}
public class Server implements Hello {
        
    public Server() {}

    public String sayHello() {
        return "Hello, world!";
    }
        
    public static void main(String args[]) {
        
        try {
            Server obj = new Server();
            Hello stub = (Hello) UnicastRemoteObject.exportObject(obj, 0);

            // Bind the remote object's stub in the registry
            Registry registry = LocateRegistry.getRegistry();
            registry.bind("Hello", stub);

            System.err.println("Server ready");
        } catch (Exception e) {
            System.err.println("Server exception: " + e.toString());
            e.printStackTrace();
        }
    }
}

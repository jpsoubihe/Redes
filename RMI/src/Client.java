import java.io.File;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Scanner;

public class Client {

    private Client() {}

    public static void main(String[] args) {

        String host = (args.length < 1) ? null : args[0];
    
        File a = new File("ana@gmail.com");
        try {
            Scanner sc = new Scanner(System.in);
//            System.out.println(sc.nextLine());
//            System.out.println(sc.nextLine());

            Registry registry = LocateRegistry.getRegistry(host);
            Compute stub = (Compute) registry.lookup("Compute");
            System.out.println("Bem vindo! O que gostaria?");
            System.out.println("(0) Sair;");
            System.out.println("(1) listar todas as pessoas formadas em um determinado curso;");
            System.out.println("(2) listar as habilidades dos perfis que moram em uma determinada cidade;");
            System.out.println("(3) acrescentar uma nova experiência em um perfil;");
            System.out.println("(4) dado o email do perfil, retornar sua experiência;");
            System.out.println("(5) listar todas as informações de todos os perfis;");
            System.out.println("(6) dado o email de um perfil, retornar suas informações.");
            int func = sc.nextInt();
            String response = stub.sayHello();
            while(func != 0) {
            	if(func == 1) {
                	System.out.println(response + "Tarefa 1");
                	stub.buildProf(a);
            	}
            	else if(func == 2) {
            		System.out.println(response + "Tarefa 2"); 		
				            	}
            	else if(func == 3) {
            		System.out.println(response + "Tarefa 3");
				}
            	else if(func == 4) {
            		System.out.println(response + "Tarefa 4");
				}	
            	else if(func == 5) {
            		System.out.println(response + "Tarefa 5");
				}	
            	else if(func == 6) {
            		System.out.println(response + "Tarefa 6");
				}
            	else {
            		System.out.println(response + "Não há essa funcionalidade, tente outra vez!");
            	}
            	func = sc.nextInt();
            }

            
            
            //Profile prof = stub.buildProf(a);
            //System.out.println(prof.toString());
           // sc.close();
        } catch (Exception e) {
            System.err.println("Client exception: " + e.toString());
            e.printStackTrace();
        }
    }
}
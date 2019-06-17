package RMI_Client;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.Serializable;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.ArrayList;
import java.util.Scanner;

import RMI_Server.Compute;
import RMI_Server.Profile;

public class Client implements Serializable{

    private Client() {}

    public static void main(String[] args) {

        String host = (args.length < 1) ? null : args[0];
        //File a = new File("ana@gmail.com");
        try {
        	System.out.println("Olá! Digite seu email: ");
            Scanner sc = new Scanner(System.in);
            String login = sc.next();

            Registry registry = LocateRegistry.getRegistry(host);
            Compute stub = (Compute) registry.lookup("Compute");
            while(stub.checkLogin(login) == false) {
            	System.out.println("Login incorreto! Tente novamente: ");
            	login = sc.next();
            }
           
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
            	if(func == 1) { // printing name + surname of those who has the formation searched on the client Terminal
            		System.out.println("Digite a formação acadêmica a ser pesquisada:");  
            		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
                	String input = reader.readLine();
            		ArrayList<String> nomes = stub.getGraduation(input);
                	System.out.println("Perfis que possuem a formação desejada:");
            		for(int i = 0;i < nomes.size();i++)
            			System.out.println(nomes.get(i));
            	}
                else if(func == 2) {// printing name + surname + habilities of those who lives in the city searched
                	System.out.println("Digite a cidade a ser pesquisada:"); 
                	BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
                	String input = reader.readLine();
            		ArrayList<String> habilities = stub.getHabilities(input);
                	for(int j = 0;j < habilities.size(); j++) {
                		System.out.println("Nome: " + habilities.get(j++));
                		System.out.println("Habilidades: " + habilities.get(j));	
                	}
    				            	}
                else if(func == 3) {
                	System.out.println(response + "Tarefa 3");
    			}
                else if(func == 4) { //printing the experiences of the current profile on the client Terminal
                	System.out.println("Digite o email do perfil buscado:");
                	ArrayList<String> xp;
                	xp = stub.experiences(sc.next());
                	System.out.println("Experiências:");
                	for(int j = 1;j <= xp.size();j++) {
                		System.out.println("("+ j +") " + xp.get(j - 1));
                	}
                	
    			}	
                else if(func == 5) { //printing the profiles on the client Terminal
                	ArrayList<Profile> p = stub.buildProf();
                    for(int i = 0;i < p.size();i++) {
                    	System.out.println("Email: " + p.get(i).getEmail());
                    	System.out.println("Nome: " + p.get(i).getName() + " Sobrenome: " + p.get(i).getSurname());
                    	System.out.println("Residência: " + p.get(i).getRes());
                    	System.out.println("Formação Acadêmica: " + p.get(i).getGrad());
                    	System.out.println("Habilidades: " + p.get(i).getHab());
                    	System.out.print("Experiências: ");
                    	for(int j = 1;j <= p.get(i).getExperience().size();j++) {
                    		System.out.println("("+ j +") " + p.get(i).getExperience().get(j-1));
                    	}
                    	System.out.println();
                    }
    			}	
                else if(func == 6) { //printing the respective profile on the client Terminal
                	System.out.println("Digite o email do perfil buscado:");  	
                	System.out.println(stub.returnProfile(sc.next()));
    			}
                else {
                	System.out.println(response + "Não há essa funcionalidade, tente outra vez!");
                }
            	
                System.out.println();
                System.out.println("(0) Sair;");
                System.out.println("(1) listar todas as pessoas formadas em um determinado curso;");
                System.out.println("(2) listar as habilidades dos perfis que moram em uma determinada cidade;");
                System.out.println("(3) acrescentar uma nova experiência em um perfil;");
                System.out.println("(4) dado o email do perfil, retornar sua experiência;");
                System.out.println("(5) listar todas as informações de todos os perfis;");
                System.out.println("(6) dado o email de um perfil, retornar suas informações.");
                func = sc.nextInt();
           }
            sc.close();
        } catch (Exception e) {
            System.err.println("Client exception: " + e.toString());
            e.printStackTrace();
        }
    }
}
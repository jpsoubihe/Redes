package RMI_Client;


import java.io.FileNotFoundException;
import java.io.IOException;
import java.rmi.Remote;
import java.rmi.RemoteException;
import java.util.ArrayList;


public interface Compute extends Remote {
    String sayHello() throws RemoteException;
    ArrayList<Profile> buildProf() throws RemoteException, FileNotFoundException;
    boolean checkLogin(String login) throws RemoteException;
    ArrayList<String> getGraduation(String grad) throws RemoteException,FileNotFoundException;
    ArrayList<String> getHabilities(String residence) throws RemoteException,FileNotFoundException;
    String returnProfile(String email)  throws RemoteException, FileNotFoundException;
    ArrayList<String> experiences(String email) throws RemoteException, FileNotFoundException;
    boolean addXP(String xp,String email) throws RemoteException, FileNotFoundException, IOException;
}

import java.io.File;
import java.io.IOException;
import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Compute extends Remote {
    String sayHello() throws RemoteException;
    void buildProf(File a) throws RemoteException, IOException;
}
import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

public class Profile {
	String email;
	String name;
	String surname;
	String res;
	String grad;
	String hab;
	ArrayList<String> experience;
	
	public Profile() {
		
	}
	
	public Profile(File a) throws FileNotFoundException {
		Scanner s = new Scanner(a);
		s.next();
		this.email = s.next();
		s.next();
		this.name = s.next();
		s.skip(" Sobrenome: ");
		this.surname = s.nextLine();
		s.skip("Residencia: ");
		this.res = s.nextLine();
		s.skip("Formacao Academica: ");
		this.grad = s.nextLine();
		s.skip("Habilidades: ");
		this.hab = s.nextLine();
		s.next();
		this.experience = new ArrayList<String>();
		while(s.hasNextLine()) {
			s.next();
			s.skip(" ");
			this.experience.add(s.nextLine());
		}
		s.close();
	}

	public String getEmail() {
		return email;
	}

	public void setEmail(String email) {
		this.email = email;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getSurname() {
		return surname;
	}

	public void setSurname(String surname) {
		this.surname = surname;
	}

	public String getGrad() {
		return grad;
	}

	public void setGrad(String grad) {
		this.grad = grad;
	}

	public String getHab() {
		return hab;
	}

	public void setHab(String hab) {
		this.hab = hab;
	}

	public ArrayList<String> getExperience() {
		return experience;
	}

	public void setExperience(ArrayList<String> experience) {
		this.experience = experience;
	}

	@Override
	public String toString() {
		return "Profile [email=" + email + ", name=" + name + ", surname=" + surname + ", res=" + res + ", grad=" + grad
				+ ", hab=" + hab + ", experience=" + experience + "]";
	}	
}
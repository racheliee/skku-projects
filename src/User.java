
public class User {
	String username;
	String password;
	
	public User() {
		username = "";
		password = "";
	}
	
	public User(String username, String password) {
		this.username = username;
		this.password = password;
	}
	
	public String getUserName() {
		return username;
	}
	
	public void setUserName(String newUserName) {
		username = newUserName;
	}
	
	public String getPassword() {
		return password;
	}

	public void setPassword(String newPassword) {
		password = newPassword;
	}
}

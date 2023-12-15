/**
 * User class that stores the username and password of a user.
 * It is a superclass of RegularUser and AdminUser.
 */
public class User {
	String username; // username of the user
	String password; // password of the user

	// constructor
	public User() {
		username = "";
		password = "";
	}

	// constructor
	public User(String username, String password) {
		this.username = username;
		this.password = password;
	}

	// getters and setters
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

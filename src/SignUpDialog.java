
import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintWriter;
import java.util.List;
import java.util.regex.Pattern;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.SwingConstants;
import javax.swing.border.EmptyBorder;
import javax.swing.JPasswordField;

public class SignUpDialog extends JDialog {

	private static final long serialVersionUID = 1L;
	private final JPanel contentPanel = new JPanel();
	private JTextField userNameTextField;
	private JPasswordField passwordField;
	private JLabel passwordLabel;
	private JLabel userNameLabel;
	private JLabel welcomeLabel;
	private JLabel infoLabel;
	private JLabel confirmPasswordLabel;
	private JPasswordField confirmPasswordField;

	public String username = "";
	public String password = "";
	private String confirmPassword = "";

	public boolean signUpSuccess = false;

	/**
	 * Create the dialog.
	 */
	public SignUpDialog(JFrame parentFrame, List<User> userList) {
		super(parentFrame, true);

		setBounds(100, 100, 381, 219);
		getContentPane().setLayout(new BorderLayout());
		contentPanel.setBorder(new EmptyBorder(5, 5, 5, 5));
		getContentPane().add(contentPanel, BorderLayout.NORTH);
		GridBagLayout gbl_contentPanel = new GridBagLayout();
		gbl_contentPanel.columnWidths = new int[] { 120, 246, 0 };
		gbl_contentPanel.rowHeights = new int[] { 0, 26, 26, 26, 26 };
		gbl_contentPanel.columnWeights = new double[] { 0.0, 1.0, Double.MIN_VALUE };
		gbl_contentPanel.rowWeights = new double[] { 0.0, 0.0, 0.0, 0.0, 0.0 };
		contentPanel.setLayout(gbl_contentPanel);
		{
			welcomeLabel = new JLabel("Welcom to SKKU Library!");
			GridBagConstraints gbc_welcomeLabel = new GridBagConstraints();
			gbc_welcomeLabel.gridwidth = 2;
			gbc_welcomeLabel.insets = new Insets(0, 0, 5, 0);
			gbc_welcomeLabel.gridx = 0;
			gbc_welcomeLabel.gridy = 0;
			contentPanel.add(welcomeLabel, gbc_welcomeLabel);
		}
		{
			infoLabel = new JLabel("Enter your new username and password :)");
			GridBagConstraints gbc_infoLabel = new GridBagConstraints();
			gbc_infoLabel.gridwidth = 2;
			gbc_infoLabel.insets = new Insets(0, 0, 5, 0);
			gbc_infoLabel.gridx = 0;
			gbc_infoLabel.gridy = 1;
			contentPanel.add(infoLabel, gbc_infoLabel);
		}
		{
			userNameLabel = new JLabel(" User Name: ");
			userNameLabel.setHorizontalAlignment(SwingConstants.TRAILING);
			GridBagConstraints gbc_userNameLabel = new GridBagConstraints();
			gbc_userNameLabel.fill = GridBagConstraints.BOTH;
			gbc_userNameLabel.insets = new Insets(0, 0, 5, 5);
			gbc_userNameLabel.gridx = 0;
			gbc_userNameLabel.gridy = 2;
			contentPanel.add(userNameLabel, gbc_userNameLabel);
		}
		userNameLabel.setLabelFor(userNameTextField);
		{
			userNameTextField = new JTextField();
			GridBagConstraints gbc_userNameTextField = new GridBagConstraints();
			gbc_userNameTextField.fill = GridBagConstraints.BOTH;
			gbc_userNameTextField.insets = new Insets(0, 0, 5, 0);
			gbc_userNameTextField.gridx = 1;
			gbc_userNameTextField.gridy = 2;
			contentPanel.add(userNameTextField, gbc_userNameTextField);
			userNameTextField.setColumns(10);
		}
		{
			passwordLabel = new JLabel(" Password: ");
			passwordLabel.setHorizontalAlignment(SwingConstants.TRAILING);
			GridBagConstraints gbc_passwordLabel = new GridBagConstraints();
			gbc_passwordLabel.anchor = GridBagConstraints.EAST;
			gbc_passwordLabel.fill = GridBagConstraints.VERTICAL;
			gbc_passwordLabel.insets = new Insets(0, 0, 5, 5);
			gbc_passwordLabel.gridx = 0;
			gbc_passwordLabel.gridy = 3;
			contentPanel.add(passwordLabel, gbc_passwordLabel);
		}
		{
			passwordField = new JPasswordField();
			GridBagConstraints gbc_passwordField = new GridBagConstraints();
			gbc_passwordField.insets = new Insets(0, 0, 5, 0);
			gbc_passwordField.fill = GridBagConstraints.HORIZONTAL;
			gbc_passwordField.gridx = 1;
			gbc_passwordField.gridy = 3;
			contentPanel.add(passwordField, gbc_passwordField);
		}
		{
			confirmPasswordLabel = new JLabel("Confirm Password:");
			GridBagConstraints gbc_confirmPasswordLabel = new GridBagConstraints();
			gbc_confirmPasswordLabel.anchor = GridBagConstraints.EAST;
			gbc_confirmPasswordLabel.insets = new Insets(0, 0, 0, 5);
			gbc_confirmPasswordLabel.gridx = 0;
			gbc_confirmPasswordLabel.gridy = 4;
			contentPanel.add(confirmPasswordLabel, gbc_confirmPasswordLabel);
		}
		{
			confirmPasswordField = new JPasswordField();
			GridBagConstraints gbc_confirmPasswordField = new GridBagConstraints();
			gbc_confirmPasswordField.fill = GridBagConstraints.HORIZONTAL;
			gbc_confirmPasswordField.gridx = 1;
			gbc_confirmPasswordField.gridy = 4;
			contentPanel.add(confirmPasswordField, gbc_confirmPasswordField);
		}

		{
			JPanel buttonPane = new JPanel();
			buttonPane.setLayout(new FlowLayout(FlowLayout.RIGHT));
			getContentPane().add(buttonPane, BorderLayout.SOUTH);
			{
				JButton okButton = new JButton("OK");
				okButton.addActionListener(new ActionListener() {
					public void actionPerformed(ActionEvent arg0) {
						username = userNameTextField.getText();
						password = String.valueOf(passwordField.getPassword());
						confirmPassword = String.valueOf(confirmPasswordField.getPassword());

						try {
							// get the user name
							if (isValidUserName(username, userList) && isValidPassword(password, confirmPassword)) {
								// add new user to userList
								userList.add(new User(username, password));

								// write the new user to the file
								writeUserFile(new User(username, password), userList);

								// show message that sign up was successful
								JOptionPane.showMessageDialog(null, "Sign up successful!\nPlease log in to proceed.",
										"Welcome to SKKU Library", JOptionPane.INFORMATION_MESSAGE, null);

								// dispose dialog if it was successful
								dispose();
							} else {
								throw new Exception();
							}
						} catch (Exception e) {
							String errorMsg = "";

							if (!isValidUserName(username, userList)) {
								if (username.equals(""))
									errorMsg += "Enter a username.\n";
								else
									errorMsg += "Username already exists. Please choose a different one.\n";
							}

							if (!isValidPassword(password, confirmPassword)) {
								errorMsg += "Enter a valid password.\n (A valid password is at least 8 characters long including digits, \nupper and lower case letters, and special characters)";
							}

							JOptionPane.showMessageDialog(null, errorMsg, "Sign Up Error", JOptionPane.ERROR_MESSAGE);
						}
					}
				});

				okButton.setActionCommand("OK");
				buttonPane.add(okButton);
				getRootPane().setDefaultButton(okButton);
			}
			{
				JButton cancelButton = new JButton("Cancel");
				cancelButton.addActionListener(new ActionListener() {
					public void actionPerformed(ActionEvent e) {
						dispose();
					}
				});
				cancelButton.setActionCommand("Cancel");
				buttonPane.add(cancelButton);
			}
		}

	}

	// checks is the username is valid
	private boolean isValidUserName(String username, List<User> userList) {

		for (User user : userList) {
			if (user.getUserName().equals(username) && user.getPassword().equals(password)) {
				// if the username already exists
				return true;
			}
		}

		// if username is empty, return false
		if (username.equals(""))
			return false;
		else
			return true;
	}

	// checks if the password is valid
	private boolean isValidPassword(String password, String cPassword) {
		// password must contain a digit, lowercase and uppercase letters, a special
		// character,
		// no whitespace, and be at least 8 characters long
		if (Pattern.matches("^(?=.*[0-9])(?=.*[a-z])(?=.*[A-Z])(?=.*[!@#&()–[{}]:;',?/*~$^+=<>]).{8,20}$", password)
				&& password.equals(cPassword)) {
			return true;
		} else {
			return false;
		}
	}

	public String getUserName() {
		return username;
	}

	public String getPassword() {
		return password;
	}

	public void writeUserFile(User newUser, List<User> userList) {
		FileOutputStream userFile = null;
		try {
			userFile = new FileOutputStream("users.txt");
		} catch (FileNotFoundException e) {
			JOptionPane.showMessageDialog(null, "User.txt not found", "Error", JOptionPane.ERROR_MESSAGE);
		}

		try (PrintWriter writer = new PrintWriter(userFile)) {
			for (int i = 0; i < userList.size(); i++) {
				writer.println(userList.get(i).getUserName());
				writer.println(userList.get(i).getPassword());
			}
		}

	}

}

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.border.EmptyBorder;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JTextField;
import javax.swing.SwingConstants;
import java.awt.GridBagLayout;
import java.awt.GridBagConstraints;
import java.awt.Insets;
import java.awt.event.ActionListener;
import java.util.List;
import java.awt.event.ActionEvent;
import javax.swing.JPasswordField;

/**
 * This class represents the dialog for logging in.
 * It extends the JDialog class and provides functionality for logging in.
 */
public class LogInDialog extends JDialog {

	private static final long serialVersionUID = 1L;
	private final JPanel contentPanel = new JPanel();
	private JTextField userNameTextField;

	private JLabel passwordLabel;
	private JLabel userNameLabel;
	private JButton signUpButton;
	private JPasswordField passwordField;
	private JLabel instructionLabel;

	public String username = ""; // username of the user
	public String password = ""; // password of the user

	// true if the log in was successful
	boolean isLogInSuccessful = false;

	// true if the user decided to sign up
	boolean willSignUp = false;

	// it is a current user that is logging in
	public User currentUser = null;

	/**
	 * Create the dialog.
	 */
	public LogInDialog(JFrame parentFrame, List<User> userList) {
		// main frame not accessible while dialog is not available
		super(parentFrame, true);

		// create dialog
		setBounds(100, 100, 377, 162);
		getContentPane().setLayout(new BorderLayout());
		contentPanel.setBorder(new EmptyBorder(5, 5, 5, 5));
		getContentPane().add(contentPanel, BorderLayout.NORTH);
		GridBagLayout gbl_contentPanel = new GridBagLayout();
		gbl_contentPanel.columnWidths = new int[] { 120, 246, 0 };
		gbl_contentPanel.rowHeights = new int[] { 0, 26, 26, 0 };
		gbl_contentPanel.columnWeights = new double[] { 0.0, 1.0, Double.MIN_VALUE };
		gbl_contentPanel.rowWeights = new double[] { 0.0, 0.0, 0.0, Double.MIN_VALUE };
		contentPanel.setLayout(gbl_contentPanel);
		{
			// instruction label
			instructionLabel = new JLabel("Enter your username and password:");
			GridBagConstraints gbc_instructionLabel = new GridBagConstraints();
			gbc_instructionLabel.gridwidth = 2;
			gbc_instructionLabel.insets = new Insets(0, 0, 5, 5);
			gbc_instructionLabel.gridx = 0;
			gbc_instructionLabel.gridy = 0;
			contentPanel.add(instructionLabel, gbc_instructionLabel);
		}
		{
			// user name label
			userNameLabel = new JLabel(" User Name: ");
			userNameLabel.setHorizontalAlignment(SwingConstants.TRAILING);
			GridBagConstraints gbc_userNameLabel = new GridBagConstraints();
			gbc_userNameLabel.fill = GridBagConstraints.BOTH;
			gbc_userNameLabel.insets = new Insets(0, 0, 5, 5);
			gbc_userNameLabel.gridx = 0;
			gbc_userNameLabel.gridy = 1;
			contentPanel.add(userNameLabel, gbc_userNameLabel);
		}
		userNameLabel.setLabelFor(userNameTextField);
		{
			// text field in which the user can write the user name
			userNameTextField = new JTextField();
			GridBagConstraints gbc_userNameTextField = new GridBagConstraints();
			gbc_userNameTextField.fill = GridBagConstraints.BOTH;
			gbc_userNameTextField.insets = new Insets(0, 0, 5, 0);
			gbc_userNameTextField.gridx = 1;
			gbc_userNameTextField.gridy = 1;
			contentPanel.add(userNameTextField, gbc_userNameTextField);
			userNameTextField.setColumns(10);
		}
		{
			// password label
			passwordLabel = new JLabel(" Password: ");
			passwordLabel.setHorizontalAlignment(SwingConstants.TRAILING);
			GridBagConstraints gbc_passwordLabel = new GridBagConstraints();
			gbc_passwordLabel.anchor = GridBagConstraints.EAST;
			gbc_passwordLabel.fill = GridBagConstraints.VERTICAL;
			gbc_passwordLabel.insets = new Insets(0, 0, 0, 5);
			gbc_passwordLabel.gridx = 0;
			gbc_passwordLabel.gridy = 2;
			contentPanel.add(passwordLabel, gbc_passwordLabel);
		}
		{
			// password field in which the user can write the password
			passwordField = new JPasswordField();
			GridBagConstraints gbc_passwordField = new GridBagConstraints();
			gbc_passwordField.fill = GridBagConstraints.HORIZONTAL;
			gbc_passwordField.gridx = 1;
			gbc_passwordField.gridy = 2;
			contentPanel.add(passwordField, gbc_passwordField);
		}

		{
			// button pane contains ok, cancel, and sign up buttons
			JPanel buttonPane = new JPanel();
			buttonPane.setLayout(new FlowLayout(FlowLayout.RIGHT));
			getContentPane().add(buttonPane, BorderLayout.SOUTH);
			{
				JButton okButton = new JButton("OK");
				okButton.addActionListener(new ActionListener() {
					public void actionPerformed(ActionEvent arg0) {
						username = userNameTextField.getText();
						password = String.valueOf(passwordField.getPassword());

						try {
							// get the user name
							if (isValidUser(username, password, userList)) {
								// if log in was successful, set visible false
								isLogInSuccessful = true;
								JOptionPane.showMessageDialog(null, "Log in successful!", "Welcome to SKKU Library",
										JOptionPane.INFORMATION_MESSAGE, null);
								setVisible(false);
							} else {
								throw new Exception();
							}
						} catch (Exception e) {
							// if the user does not exist, then show error message
							JOptionPane.showMessageDialog(null, "Wrong username or password. Check Again",
									"Log In Error", JOptionPane.ERROR_MESSAGE);
						}
					}
				});
				{
					// sign up button
					signUpButton = new JButton("Sign Up");
					signUpButton.addActionListener(new ActionListener() {
						public void actionPerformed(ActionEvent e) {
							willSignUp = true;
							setVisible(false);
						}
					});
					buttonPane.add(signUpButton);
				}
				okButton.setActionCommand("OK");
				buttonPane.add(okButton);
				getRootPane().setDefaultButton(okButton);
			}
			{
				// cancel button
				JButton cancelButton = new JButton("Cancel");
				cancelButton.addActionListener(new ActionListener() {
					public void actionPerformed(ActionEvent e) {
						// close dialog window
						dispose();
					}
				});
				cancelButton.setActionCommand("Cancel");
				buttonPane.add(cancelButton);
			}
		}

	}

	// checks if the username and password entered are valid
	private boolean isValidUser(String username, String password, List<User> userList) {
		boolean userMatch = false;

		for (User user : userList) {
			if (user.getUserName().equals(username) && user.getPassword().equals(password)) {
				currentUser = user;
				userMatch = true;
				break;
			}
		}

		return userMatch;
	}

	// reuturns true if the log in was successful
	public boolean isLogInSuccessful() {
		return isLogInSuccessful;
	}

	// returns true if the user will sign up instead of log in
	public boolean willSignUp() {
		return willSignUp;
	}

	// return the User that logged in
	public User getUser() {
		return currentUser;
	}

}

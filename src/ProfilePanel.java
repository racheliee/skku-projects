
import javax.swing.JPanel;
import java.awt.GridBagLayout;
import javax.swing.JButton;
import java.awt.GridBagConstraints;
import javax.swing.JLabel;
import java.awt.Insets;
import javax.swing.JTable;
import javax.swing.SwingConstants;
import javax.swing.table.DefaultTableModel;

import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import javax.swing.JScrollPane;

public class ProfilePanel extends JPanel {

	private static final long serialVersionUID = 1L;
	private JLabel usernameLabel;
	private JLabel usernameField;
	private JLabel passwordLabel;
	private JLabel lblNewLabel_2;
	private JLabel lblNewLabel_3;
	private JButton logOutButton;
	private JScrollPane bookScrollPane;
	private JTable borrowedBookTable;
	
	private Object data[][];
	/**
	 * Create the panel.
	 */
	public ProfilePanel(RegularUser user, LibraryMainPageGUI mainGUI) {
		GridBagLayout gridBagLayout = new GridBagLayout();
		gridBagLayout.columnWidths = new int[] { 47, 47, 47, 47, 47, 30, 47, 47, 47, 47 };
		gridBagLayout.rowHeights = new int[] { 39, 39, 39, 39, 39, 39, 39, 39, 39, 39 };
		gridBagLayout.columnWeights = new double[] { 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0 };
		gridBagLayout.rowWeights = new double[] { 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0 };
		setLayout(gridBagLayout);

		usernameLabel = new JLabel("Username: ");
		usernameLabel.setHorizontalAlignment(SwingConstants.LEFT);
		GridBagConstraints gbc_usernameLabel = new GridBagConstraints();
		gbc_usernameLabel.fill = GridBagConstraints.BOTH;
		gbc_usernameLabel.insets = new Insets(0, 0, 5, 5);
		gbc_usernameLabel.gridx = 2;
		gbc_usernameLabel.gridy = 2;
		add(usernameLabel, gbc_usernameLabel);

		usernameField = new JLabel(user.getUserName());
		GridBagConstraints gbc_usernameField = new GridBagConstraints();
		gbc_usernameField.fill = GridBagConstraints.BOTH;
		gbc_usernameField.gridwidth = 4;
		gbc_usernameField.insets = new Insets(0, 0, 5, 5);
		gbc_usernameField.gridx = 3;
		gbc_usernameField.gridy = 2;
		add(usernameField, gbc_usernameField);

		passwordLabel = new JLabel("Password: ");
		GridBagConstraints gbc_passwordLabel = new GridBagConstraints();
		gbc_passwordLabel.fill = GridBagConstraints.BOTH;
		gbc_passwordLabel.insets = new Insets(0, 0, 5, 5);
		gbc_passwordLabel.gridx = 2;
		gbc_passwordLabel.gridy = 3;
		add(passwordLabel, gbc_passwordLabel);

		lblNewLabel_2 = new JLabel(user.getPassword());
		GridBagConstraints gbc_lblNewLabel_2 = new GridBagConstraints();
		gbc_lblNewLabel_2.fill = GridBagConstraints.BOTH;
		gbc_lblNewLabel_2.gridwidth = 4;
		gbc_lblNewLabel_2.insets = new Insets(0, 0, 5, 5);
		gbc_lblNewLabel_2.gridx = 3;
		gbc_lblNewLabel_2.gridy = 3;
		add(lblNewLabel_2, gbc_lblNewLabel_2);

		lblNewLabel_3 = new JLabel("Borrowed Books: ");
		GridBagConstraints gbc_lblNewLabel_3 = new GridBagConstraints();
		gbc_lblNewLabel_3.insets = new Insets(0, 0, 5, 5);
		gbc_lblNewLabel_3.gridx = 2;
		gbc_lblNewLabel_3.gridy = 4;
		add(lblNewLabel_3, gbc_lblNewLabel_3);



		logOutButton = new JButton("Log Out");
		logOutButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				// let the mainGUI know that the log in button was pressed
				mainGUI.logOutPressed();
			}
		});
		
		bookScrollPane = new JScrollPane();
		GridBagConstraints gbc_bookScrollPane = new GridBagConstraints();
		gbc_bookScrollPane.gridheight = 3;
		gbc_bookScrollPane.gridwidth = 6;
		gbc_bookScrollPane.insets = new Insets(0, 0, 5, 5);
		gbc_bookScrollPane.fill = GridBagConstraints.BOTH;
		gbc_bookScrollPane.gridx = 3;
		gbc_bookScrollPane.gridy = 5;
		add(bookScrollPane, gbc_bookScrollPane);
		
		DefaultTableModel borrowedBookTableModel = new DefaultTableModel(data, new Object[] {"Book Title", "Author", "Borrowed Date", "Due Date"});
		borrowedBookTable = new JTable(borrowedBookTableModel);
		for(HardCopy copy: user.borrowedBooks) {
			borrowedBookTableModel.addRow(new Object[] {copy.getBook().getTitle(), copy.getBook().getAuthor(), copy.getBorrowDate(), copy.getDueDate()});
		}
		borrowedBookTableModel.fireTableDataChanged();
		
		bookScrollPane.setViewportView(borrowedBookTable);
		GridBagConstraints gbc_logOutButton = new GridBagConstraints();
		gbc_logOutButton.insets = new Insets(0, 0, 5, 5);
		gbc_logOutButton.gridx = 3;
		gbc_logOutButton.gridy = 8;
		add(logOutButton, gbc_logOutButton);

	}

}

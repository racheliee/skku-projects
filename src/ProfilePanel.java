
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
import java.awt.event.MouseAdapter;
import java.awt.event.ActionEvent;
import javax.swing.JScrollPane;
import java.awt.event.MouseEvent;

/**
 * This class represents the profile panel of the library management system. It
 * extends the JPanel class and provides functionality for displaying the
 * profile of the user.
 */
public class ProfilePanel extends JPanel {

	private static final long serialVersionUID = 1L;
	private JLabel usernameLabel;
	private JLabel usernameField;
	private JLabel passwordLabel;
	private JLabel passwordField;
	private JLabel borrowedBooksLabel;
	private JButton logOutButton;
	private JScrollPane bookScrollPane;
	private JTable borrowedBookTable;

	private Object data[][]; // Data for book information table
	BookRentDialog bookRentDialog; // Dialog for renting books

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

		// username label
		usernameLabel = new JLabel("Username: ");
		usernameLabel.setHorizontalAlignment(SwingConstants.LEFT);
		GridBagConstraints gbc_usernameLabel = new GridBagConstraints();
		gbc_usernameLabel.fill = GridBagConstraints.BOTH;
		gbc_usernameLabel.insets = new Insets(0, 0, 5, 5);
		gbc_usernameLabel.gridx = 2;
		gbc_usernameLabel.gridy = 2;
		add(usernameLabel, gbc_usernameLabel);

		// username field based on the user's username
		usernameField = new JLabel(user.getUserName());
		GridBagConstraints gbc_usernameField = new GridBagConstraints();
		gbc_usernameField.fill = GridBagConstraints.BOTH;
		gbc_usernameField.gridwidth = 4;
		gbc_usernameField.insets = new Insets(0, 0, 5, 5);
		gbc_usernameField.gridx = 3;
		gbc_usernameField.gridy = 2;
		add(usernameField, gbc_usernameField);

		// password label
		passwordLabel = new JLabel("Password: ");
		GridBagConstraints gbc_passwordLabel = new GridBagConstraints();
		gbc_passwordLabel.fill = GridBagConstraints.BOTH;
		gbc_passwordLabel.insets = new Insets(0, 0, 5, 5);
		gbc_passwordLabel.gridx = 2;
		gbc_passwordLabel.gridy = 3;
		add(passwordLabel, gbc_passwordLabel);

		// password field based on the user's password
		passwordField = new JLabel(user.getPassword());
		GridBagConstraints gbc_passwordField = new GridBagConstraints();
		gbc_passwordField.fill = GridBagConstraints.BOTH;
		gbc_passwordField.gridwidth = 4;
		gbc_passwordField.insets = new Insets(0, 0, 5, 5);
		gbc_passwordField.gridx = 3;
		gbc_passwordField.gridy = 3;
		add(passwordField, gbc_passwordField);

		// borrowed books label
		borrowedBooksLabel = new JLabel("Borrowed Books: ");
		GridBagConstraints gbc_borrowedBooksLabel = new GridBagConstraints();
		gbc_borrowedBooksLabel.insets = new Insets(0, 0, 5, 5);
		gbc_borrowedBooksLabel.gridx = 2;
		gbc_borrowedBooksLabel.gridy = 4;
		add(borrowedBooksLabel, gbc_borrowedBooksLabel);

		// log out button
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

		// populate borrowed books table
		DefaultTableModel borrowedBookTableModel = new DefaultTableModel(data,
				new Object[] { "Book Title", "Author", "Borrowed Date", "Due Date" });
		borrowedBookTable = new JTable(borrowedBookTableModel);
		borrowedBookTable.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseClicked(MouseEvent e) {
				// get the selected row
				int selectedRow = borrowedBookTable.getSelectedRow();
				// get the book title
				String title = borrowedBookTable.getValueAt(selectedRow, 0).toString();
				// open the book rent dialog
				bookRentDialog = new BookRentDialog(mainGUI.bookList, title, mainGUI.userList, mainGUI.getUserIndex(),
						mainGUI.frame);
				bookRentDialog.setVisible(true);

				// update the number of copies shown on the table
				if (bookRentDialog.isBookNumChanged()) {
					DefaultTableModel model = (DefaultTableModel) borrowedBookTable.getModel();
					model.removeRow(selectedRow);
					model.fireTableDataChanged();
				}
				bookRentDialog.dispose();

			}
		});
		borrowedBookTable.setDefaultEditor(Object.class, null);

		// add borrowed books to the table initially
		for (HardCopy copy : user.borrowedBooks) {
			borrowedBookTableModel.addRow(new Object[] { copy.getBook().getTitle(), copy.getBook().getAuthor(),
					copy.getBorrowDate(), copy.getDueDate() });
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

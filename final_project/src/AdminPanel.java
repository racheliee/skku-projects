import javax.swing.JPanel;
import javax.swing.JLabel;
import javax.swing.JButton;
import javax.swing.JTable;
import java.awt.GridBagLayout;
import java.awt.GridBagConstraints;
import java.awt.Insets;
import java.awt.event.ActionListener;
import java.util.List;
import java.awt.event.ActionEvent;
import javax.swing.JScrollPane;
import javax.swing.SwingConstants;
import javax.swing.table.DefaultTableModel;

/**
 * This class represents the admin panel of the library management system.
 * It extends the JPanel class and provides functionality for adding
 * announcements,
 * adding new books, displaying book information, and logging out.
 */
public class AdminPanel extends JPanel {

	private static final long serialVersionUID = 1L;

	private JButton addAnnouncementButton; // Button for adding announcements
	private JButton newArrivalButton; // Button for adding new books
	private JLabel bookInfoLabel; // Label for displaying book information
	private JScrollPane infoScrollPane; // Scroll pane for displaying book information table
	private JTable bookInfotable; // Table for displaying book information

	AnnouncementDialog announcementDialog; // Dialog for adding/editing announcements

	Object data[][]; // Data for book information table
	private JButton logOutButton; // Button for logging out

	/**
	 * Creates a new AdminPanel instance.
	 * 
	 * @param mainGUI  The main GUI instance
	 * @param bookList The list of books
	 */
	public AdminPanel(LibraryMainPageGUI mainGUI, List<Book> bookList) {
		GridBagLayout gridBagLayout_1 = new GridBagLayout();
		gridBagLayout_1.columnWidths = new int[] { 30, 0, 30 };
		gridBagLayout_1.rowHeights = new int[] { 30, 30, 30, 14, 30, 155, 30 };
		gridBagLayout_1.columnWeights = new double[] { 0.0, 1.0, 0.0 };
		gridBagLayout_1.rowWeights = new double[] { 0.0, 0.1, 0.1, 0.0, 0.1, 1.0 };
		setLayout(gridBagLayout_1);

		// add announcement button to add new announcements to the announcement panel
		addAnnouncementButton = new JButton("Add Announcement");
		addAnnouncementButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				announcementDialog = new AnnouncementDialog(mainGUI.announcementList, mainGUI.announcementList.size(),
						mainGUI.frame, true, true);
				announcementDialog.setVisible(true);

				// only add new announcement table row if applyEditButton is pressed (if just
				// dispose then no need to add new row)
				if (announcementDialog.isApplyEditButtonPressed()) {
					addAnnouncement(mainGUI,
							mainGUI.announcementList.get(mainGUI.announcementList.size() - 1).getTitle(),
							mainGUI.announcementList.get(mainGUI.announcementList.size() - 1).getContents(),
							mainGUI.announcementList.size() - 1);
				}

				announcementDialog.dispose();

			}
		});
		GridBagConstraints gbc_addAnnouncementButton = new GridBagConstraints();
		gbc_addAnnouncementButton.insets = new Insets(0, 0, 5, 5);
		gbc_addAnnouncementButton.gridx = 1;
		gbc_addAnnouncementButton.gridy = 1;
		add(addAnnouncementButton, gbc_addAnnouncementButton);

		// add new arrival button to add new books to the book list
		newArrivalButton = new JButton("Add New Book");
		newArrivalButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				AddNewBookDialog newBookDialog = new AddNewBookDialog(mainGUI.frame, bookList);
				newBookDialog.setVisible(true);

				// only add new arrival table row if addNewBookButton is pressed (if just
				// dispose
				// then no need to add new row)
				if (newBookDialog.isAddNewBookButtonPressed()) {
					addNewArrivalTableEntry(
							mainGUI.bookList.get(mainGUI.bookList.size() - 1).getTitle(),
							mainGUI.bookList.get(mainGUI.bookList.size() - 1).getAuthor(),
							mainGUI.bookList.get(mainGUI.bookList.size() - 1).getGenre(),
							mainGUI);
				}
			}
		});
		GridBagConstraints gbc_newArrivalButton = new GridBagConstraints();
		gbc_newArrivalButton.insets = new Insets(0, 0, 5, 5);
		gbc_newArrivalButton.gridx = 1;
		gbc_newArrivalButton.gridy = 2;
		add(newArrivalButton, gbc_newArrivalButton);

		// book information label
		bookInfoLabel = new JLabel("Book Information:");
		bookInfoLabel.setHorizontalAlignment(SwingConstants.LEFT);
		GridBagConstraints gbc_bookInfoLabel = new GridBagConstraints();
		gbc_bookInfoLabel.fill = GridBagConstraints.BOTH;
		gbc_bookInfoLabel.insets = new Insets(0, 0, 5, 5);
		gbc_bookInfoLabel.gridx = 1;
		gbc_bookInfoLabel.gridy = 4;
		add(bookInfoLabel, gbc_bookInfoLabel);

		// book information table
		infoScrollPane = new JScrollPane();
		GridBagConstraints gbc_infoScrollPane = new GridBagConstraints();
		gbc_infoScrollPane.insets = new Insets(0, 0, 5, 5);
		gbc_infoScrollPane.fill = GridBagConstraints.BOTH;
		gbc_infoScrollPane.gridx = 1;
		gbc_infoScrollPane.gridy = 5;
		add(infoScrollPane, gbc_infoScrollPane);

		// populate book information table
		DefaultTableModel bookInfoModel = new DefaultTableModel(data,
				new Object[] { "Book Title", "Author", "Genre", "Total Copies", "Num Available" });
		bookInfotable = new JTable(bookInfoModel);
		bookInfotable.setDefaultEditor(Object.class, null);

		// add book information to table
		for (Book book : bookList) {
			bookInfoModel.addRow(new Object[] { book.getTitle(), book.getAuthor(), book.getGenre(),
					String.valueOf(book.getCopies().size()), String.valueOf(book.getAvailableCopies().size()) });
		}
		bookInfoModel.fireTableDataChanged(); // update table when data is changed

		infoScrollPane.setViewportView(bookInfotable);

		// log out button to log out of the admin account
		logOutButton = new JButton("Log Out");
		logOutButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				mainGUI.logOutPressed();
			}
		});
		GridBagConstraints gbc_logOutButton = new GridBagConstraints();
		gbc_logOutButton.insets = new Insets(0, 0, 0, 5);
		gbc_logOutButton.gridx = 1;
		gbc_logOutButton.gridy = 6;
		add(logOutButton, gbc_logOutButton);

	}

	// add new arrival table entry to the new arrival table
	public void addNewArrivalTableEntry(String title, String author, String genre, LibraryMainPageGUI MainGUI) {
		MainGUI.newArrivalTableModel.addRow(new Object[] { title, author, genre });
		MainGUI.newArrivalTableModel.fireTableDataChanged();
	}

	// if the admin pressed announcements, they can change the announcements
	public void addAnnouncement(LibraryMainPageGUI mainGUI, String title, String contents, int announcementIndex) {

		mainGUI.announcementTableModel.addRow(new Object[] { announcementIndex, title });
		mainGUI.announcementTableModel.fireTableDataChanged();
	}
}

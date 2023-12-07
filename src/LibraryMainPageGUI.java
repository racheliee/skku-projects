
import java.awt.EventQueue;
import javax.swing.JFrame;
import java.awt.BorderLayout;
import javax.swing.JPanel;
import java.awt.FlowLayout;
import javax.swing.JLabel;
import java.awt.Font;
import javax.swing.ImageIcon;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import javax.swing.JList;
import javax.swing.JOptionPane;
import java.awt.GridBagLayout;
import java.awt.GridBagConstraints;
import java.awt.Insets;
import javax.swing.JButton;
import javax.swing.JTextField;
import javax.swing.JComboBox;
import javax.swing.SwingConstants;
import javax.swing.table.DefaultTableModel;

import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.awt.CardLayout;
import javax.swing.DefaultComboBoxModel;
import javax.swing.JTable;
import javax.swing.JScrollPane;

public class LibraryMainPageGUI extends JFrame {

	private static final long serialVersionUID = 1L;
	JFrame frame;
	private JPanel topPanel;
	private final JLabel appTitle = new JLabel("SKKU Library");
	private JLabel appBookIcon;
	private JPanel midPanel;

	private JButton searchButton;
	private JTextField searchTextField;
	private JComboBox<String> searchByGenreComboBox;
	private JLabel searchByGenreTitle;
	private JPanel rightPanel;
	private JLabel announceTitle;
	private JTable announcementTable;
	private DefaultTableModel announcementTableModel;
	public DefaultTableModel newArrivalTableModel;
	private JLabel newBooksLabel;
	public JButton logInButton;
	public JPanel changingPanel;
	public CardLayout cardLayout;

	// the panels of the library
	MainPagePanel mainPanel;
	ProfilePanel profilePanel;
	BookListPanel bookListPanel;
	AdminPanel adminPanel;

	// indicates this current class
	LibraryMainPageGUI mainPage = this;

	// keeps track of the users of the library. admin is always fixed as the first
	// user
	public List<User> userList;

	// keeps track of the books of the library
	public List<Book> bookList;

	// keeps track of the announcments of the library
	public List<Announcement> announcementList;

	// this double array is needed for the Jtable for the announcements
	public Object announcementData[][];

	// this double array is needed for the Jtable for the new arrivals
	public Object newArrivalData[][];

	// keeps track if a user has logged in or not
	boolean loggedIn = false;

	// keeps track of the current user
	public User currentUser = null;

	// keeps track of the current user's index in the userList
	public int userIndex = -1;
	private JTable newArrivalTable;

	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					LibraryMainPageGUI window = new LibraryMainPageGUI();
					window.frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * Create the application.
	 */
	public LibraryMainPageGUI() {
		initialize();
	}

	// test line

	/**
	 * Initialize the contents of the frame.
	 */
	private void initialize() {
		// scan the users and books from file and save each to its respective list
		readUserFile();
		readBookFile();
		// adds the default announcements of the library
		addDefaultAnnouncements();

		// create GUI
		frame = new JFrame();
		frame.setBounds(100, 100, 850, 560);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.getContentPane().setLayout(new BorderLayout(0, 0));

		topPanel = new JPanel();
		frame.getContentPane().add(topPanel, BorderLayout.NORTH);
		topPanel.setLayout(new FlowLayout(FlowLayout.CENTER, 5, 5));
		appTitle.addMouseListener(new MouseAdapter() {
			@Override
			public void mousePressed(MouseEvent arg0) {
				cardLayout.show(changingPanel, "MainPagePanel");
			}
		});
		appTitle.setIcon(new ImageIcon(LibraryMainPageGUI.class.getResource("/images/book2_r.png")));
		appTitle.setFont(new Font("Monaco", Font.PLAIN, 18));
		topPanel.add(appTitle);

		appBookIcon = new JLabel("");
		appBookIcon.setIcon(new ImageIcon(LibraryMainPageGUI.class.getResource("/images/book2_r.png")));
		topPanel.add(appBookIcon);

		rightPanel = new JPanel();
		frame.getContentPane().add(rightPanel, BorderLayout.EAST);
		GridBagLayout gbl_rightPanel = new GridBagLayout();
		gbl_rightPanel.columnWidths = new int[] { 180, 0 };
		gbl_rightPanel.rowHeights = new int[] { 0, 16, 0, 0, 0, 0 };
		gbl_rightPanel.columnWeights = new double[] { 1.0, Double.MIN_VALUE };
		gbl_rightPanel.rowWeights = new double[] { 0.0, 1.0, 6.0, 1.0, 6.0, Double.MIN_VALUE };
		rightPanel.setLayout(gbl_rightPanel);

		logInButton = new JButton("     Log In     ");
		logInButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				if (loggedIn == false) {
					LogInDialog logInDialog = new LogInDialog(LibraryMainPageGUI.this, userList);
					logInDialog.setVisible(true);

					// user decides to sign up
					if (logInDialog.willSignUp()) {
						SignUpDialog signUpDialog = new SignUpDialog(LibraryMainPageGUI.this, userList);
						signUpDialog.setVisible(true);
					}

					// if the log in was successful
					if (logInDialog.isLogInSuccessful()) {
						// change the log in button to username
						logInButton.setText(logInDialog.getUser().getUserName());

						// get current user
						currentUser = logInDialog.getUser();

						// set logged in as true
						loggedIn = true;
						userIndex = findUserIndex(currentUser.getUserName());
					}
					logInDialog.dispose();
				} else {
					// change panel to the profile panel if the user is already logged in
					// if the current user is a regular user, show the profile panel
					if (currentUser instanceof RegularUser) {
						// create panel
						profilePanel = new ProfilePanel((RegularUser) currentUser, mainPage);
						changingPanel.add(profilePanel, "ProfilePanel");

						cardLayout.show(changingPanel, "ProfilePanel");
					}
					// if current user is the admin, show the admin panel
					else if (currentUser instanceof AdminUser) {
						// create panel
						adminPanel = new AdminPanel(mainPage, bookList);
						changingPanel.add(adminPanel, "AdminPanel");

						cardLayout.show(changingPanel, "AdminPanel");
					}

				}
			}
		});
		GridBagConstraints gbc_logInButton = new GridBagConstraints();
		gbc_logInButton.insets = new Insets(0, 0, 5, 0);
		gbc_logInButton.gridx = 0;
		gbc_logInButton.gridy = 0;
		rightPanel.add(logInButton, gbc_logInButton);

		announceTitle = new JLabel("Announcement");
		announceTitle.setHorizontalAlignment(SwingConstants.CENTER);
		GridBagConstraints gbc_announceTitle = new GridBagConstraints();
		gbc_announceTitle.insets = new Insets(0, 0, 5, 0);
		gbc_announceTitle.gridx = 0;
		gbc_announceTitle.gridy = 1;
		rightPanel.add(announceTitle, gbc_announceTitle);

		announcementTableModel = new DefaultTableModel(announcementData, new Object[] { "Number", "Announcement" });
		announcementTable = new JTable(announcementTableModel);
		for (Announcement announcement : announcementList) {
			announcementTableModel
					.addRow(new Object[] { announcementList.indexOf(announcement), announcement.getTitle() });
		}
		announcementTableModel.fireTableDataChanged();

		announcementTable.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseClicked(MouseEvent e) {
				try {
					int selectedRow = announcementTable.getSelectedRow();
					if (selectedRow == -1) {
						throw new Exception();
					}

					// get the class name of current user using instanceof
					boolean isAdmin = currentUser instanceof AdminUser;
					AnnouncementDialog announcementDialog = new AnnouncementDialog(announcementList, selectedRow,
							LibraryMainPageGUI.this, isAdmin, false);
					announcementDialog.setVisible(true);
					announcementTableModel.setValueAt(announcementList.get(selectedRow).getTitle(), selectedRow, 1);
					announcementTableModel.fireTableDataChanged();
					announcementDialog.dispose();
				} catch (Exception e1) {
					System.out.println("click blank space");
					e1.printStackTrace();
				}

			}
		});

		GridBagConstraints gbc_announcementTable = new GridBagConstraints();
		gbc_announcementTable.insets = new Insets(0, 0, 5, 0);
		gbc_announcementTable.fill = GridBagConstraints.BOTH;
		gbc_announcementTable.gridx = 0;
		gbc_announcementTable.gridy = 2;
		announcementTable.setDefaultEditor(Object.class, null);
		rightPanel.add(announcementTable, gbc_announcementTable);

		newBooksLabel = new JLabel("New Arrivals");
		GridBagConstraints gbc_newBooksLabel = new GridBagConstraints();
		gbc_newBooksLabel.insets = new Insets(0, 0, 5, 0);
		gbc_newBooksLabel.gridx = 0;
		gbc_newBooksLabel.gridy = 3;
		rightPanel.add(newBooksLabel, gbc_newBooksLabel);

		newArrivalTableModel = new DefaultTableModel(newArrivalData,
				new Object[] { "Title", "Author", "Genre" });

		newArrivalTable = new JTable(newArrivalTableModel);
		newArrivalTable.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseClicked(MouseEvent e) {
				try {
					int selectedRow = newArrivalTable.getSelectedRow();
					if (selectedRow == -1) {
						throw new Exception();
					}

					// get the class name of current user using instanceof
					BookRentDialog bookDialog = new BookRentDialog(bookList,
							newArrivalTableModel.getValueAt(selectedRow, 0).toString(),
							userList, userIndex, LibraryMainPageGUI.this);
					bookDialog.setVisible(true);
					bookDialog.dispose();
				} catch (Exception e1) {
					System.out.println("click blank space");
					e1.printStackTrace();
				}
			}
		});
		GridBagConstraints gbc_newArrivalTable = new GridBagConstraints();
		gbc_newArrivalTable.fill = GridBagConstraints.BOTH;
		gbc_newArrivalTable.gridx = 0;
		gbc_newArrivalTable.gridy = 4;
		newArrivalTable.setDefaultEditor(Object.class, null);
		rightPanel.add(newArrivalTable, gbc_newArrivalTable);

		midPanel = new JPanel();
		frame.getContentPane().add(midPanel, BorderLayout.CENTER);
		GridBagLayout gbl_midPanel = new GridBagLayout();
		gbl_midPanel.columnWidths = new int[] { 0, 30, 0, 0, 30, 30, 0, 30, 30 };
		gbl_midPanel.rowHeights = new int[] { 0, 46, 0, 0, 0 };
		gbl_midPanel.columnWeights = new double[] { 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0 };
		gbl_midPanel.rowWeights = new double[] { 0.0, 0.0, 0.0, 1.0, Double.MIN_VALUE };
		midPanel.setLayout(gbl_midPanel);

		searchTextField = new JTextField();
		GridBagConstraints gbc_searchTextField = new GridBagConstraints();
		gbc_searchTextField.insets = new Insets(0, 0, 5, 5);
		gbc_searchTextField.gridwidth = 8;
		gbc_searchTextField.fill = GridBagConstraints.HORIZONTAL;
		gbc_searchTextField.gridx = 1;
		gbc_searchTextField.gridy = 1;
		midPanel.add(searchTextField, gbc_searchTextField);
		searchTextField.setColumns(10);

		searchButton = new JButton("Search");
		searchButton.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseClicked(MouseEvent e) {
				// search by title keyword
				String searchedBook = searchTextField.getText();

				// create the booklist panel and show the panel
				bookListPanel = new BookListPanel(searchedBook, (String) searchByGenreComboBox.getSelectedItem(),
						bookList, LibraryMainPageGUI.this, userList, mainPage);

				changingPanel.add(bookListPanel, "BookListPanel");
				cardLayout.show(changingPanel, "BookListPanel");

			}
		});
		GridBagConstraints gbc_searchButton = new GridBagConstraints();
		gbc_searchButton.insets = new Insets(0, 0, 5, 0);
		gbc_searchButton.gridx = 9;
		gbc_searchButton.gridy = 1;
		midPanel.add(searchButton, gbc_searchButton);

		searchByGenreTitle = new JLabel("Search by Genre:");
		GridBagConstraints gbc_searchByGenreTitle = new GridBagConstraints();
		gbc_searchByGenreTitle.gridwidth = 2;
		gbc_searchByGenreTitle.insets = new Insets(0, 0, 5, 5);
		gbc_searchByGenreTitle.gridx = 1;
		gbc_searchByGenreTitle.gridy = 2;
		midPanel.add(searchByGenreTitle, gbc_searchByGenreTitle);

		searchByGenreComboBox = new JComboBox<String>();
		searchByGenreComboBox.setModel(new DefaultComboBoxModel<String>(
				new String[] { "All", "Fiction", "Non-Fiction", "Mystery", "Romance" }));
		GridBagConstraints gbc_searchByGenreComboBox = new GridBagConstraints();
		gbc_searchByGenreComboBox.insets = new Insets(0, 0, 5, 5);
		gbc_searchByGenreComboBox.gridwidth = 6;
		gbc_searchByGenreComboBox.fill = GridBagConstraints.HORIZONTAL;
		gbc_searchByGenreComboBox.gridx = 3;
		gbc_searchByGenreComboBox.gridy = 2;
		midPanel.add(searchByGenreComboBox, gbc_searchByGenreComboBox);

		changingPanel = new JPanel();
		GridBagConstraints gbc_changingPanel = new GridBagConstraints();
		gbc_changingPanel.gridwidth = 10;
		gbc_changingPanel.insets = new Insets(0, 0, 0, 5);
		gbc_changingPanel.fill = GridBagConstraints.BOTH;
		gbc_changingPanel.gridx = 0;
		gbc_changingPanel.gridy = 3;
		midPanel.add(changingPanel, gbc_changingPanel);

		cardLayout = new CardLayout(0, 0);
		changingPanel.setLayout(cardLayout);

		// add panels to card layout
		mainPanel = new MainPagePanel();
		changingPanel.add(mainPanel, "MainPagePanel");

	}

	// reads the file to get users of the library
	public void readUserFile() {
		userList = new ArrayList<User>();

		userList.add(new AdminUser("admin", "admin"));

		FileInputStream userFile = null;
		try {
			userFile = new FileInputStream("users.txt");
		} catch (FileNotFoundException e) {
			JOptionPane.showMessageDialog(null, "users.txt not found", "Error", JOptionPane.ERROR_MESSAGE);
		}

		try (Scanner scanner = new Scanner(userFile)) {
			int isUsername = 0;

			while (scanner.hasNext()) {
				// current line is username
				if (isUsername % 2 == 0) {
					RegularUser newUser = new RegularUser();
					newUser.setUserName(scanner.nextLine());
					userList.add(newUser);
					isUsername++;
				}
				// current line is password
				else {
					userList.get(userList.size() - 1).setPassword(scanner.nextLine());
					isUsername--;
				}
			}
		}
	}

	// reads the file to get book information of the library
	public void readBookFile() {
		bookList = new ArrayList<Book>();
		FileInputStream bookFile = null;
		try {
			bookFile = new FileInputStream("books.txt");
		} catch (FileNotFoundException e) {
			JOptionPane.showMessageDialog(null, "books.txt not found", "Error", JOptionPane.ERROR_MESSAGE);
		}

		// read the file
		try (Scanner scanner = new Scanner(bookFile)) {
			while (scanner.hasNext()) {
				bookList.add(new Book(scanner.nextLine(), scanner.nextLine(), Integer.parseInt(scanner.nextLine()),
						scanner.nextLine(), scanner.nextLine()));

			}
		}
	}

	// adds default announcements of the library to the announcement list
	public void addDefaultAnnouncements() {
		announcementList = new ArrayList<Announcement>();

		announcementList.add(new Announcement("Tech Talk Workshop (Wednesday, 3PM - 5PM)",
				"Join us every Wednesday from 3 PM to 5 PM for our Tech Talk Workshop! Whether you're a tech enthusiast or a beginner, this interactive session is designed to explore the latest in technology trends, gadgets, and software. Dive into engaging discussions, share your insights, and stay updated on the ever-evolving world of technology. Don't miss out on this opportunity to expand your tech knowledge and connect with fellow enthusiasts. See you there!\n"));
		announcementList.add(new Announcement("Book Club Discussion (Thursday, 6:30PM - 8PM)",
				"Calling all book lovers! Join our Book Club Discussion every Thursday from 6:30 PM to 8 PM. Immerse yourself in lively conversations about the latest literary gems, timeless classics, and everything in between. Whether you're a fiction fanatic or a non-fiction connoisseur, this is the perfect space to share your thoughts, discover new perspectives, and connect with fellow bookworms. Grab your current read and join us for an evening of literary delight. Your next favorite book might be just a discussion away!\n"));
		announcementList.add(new Announcement("Children's Storytime (Saturday, 10:30AM-11:30AM)",
				"Make Saturday mornings magical with our Children's Storytime! From 10:30 AM to 11:30 AM, bring your little ones to the library for a delightful journey into the world of stories. Our engaging storytellers will captivate young imaginations with enchanting tales and interactive adventures. Designed for children of all ages, this is a wonderful opportunity to foster a love for reading and learning in a fun, supportive environment. Join us and watch as stories come to life every Saturday morning!"));
	}

	// returns the index of the user in the userlist
	public int findUserIndex(String username) {
		int index = 0;
		for (User user : userList) {
			if (user.getUserName().equals(username)) {
				return index;
			}
			index++;
		}
		return index;
	}

	// returns the index of the book in the booklist
	public int findBookIndex(List<Book> bookList, String title) {
		int index = 0;
		for (Book book : bookList) {
			if (book.getTitle().equals(title)) {
				return index;
			}
			index++;
		}
		return index;
	}

	// if the log out button was pressed in the profile panel, change the user to
	// null
	public void logOutPressed() {
		currentUser = null;
		loggedIn = false;
		logInButton.setText("     Log In     ");
		cardLayout.show(changingPanel, "MainPagePanel");
		userIndex = -1;
	}

	// // if the admin presses new arrivals, they can change the contents
	// public void addNewArrival() {
	// AddNewBookDialog newBookDialog = new
	// AddNewBookDialog(LibraryMainPageGUI.this, bookList);
	// newBookDialog.setVisible(true);
	// }

	// if the admin pressed announcements, they can change the announcements
	public void addAnnouncement(String title, String contents, int announcementIndex) {
		announcementTableModel.addRow(new Object[] { announcementIndex, title });
		announcementTableModel.fireTableDataChanged();
	}

	// returns the user index
	public int getUserIndex() {
		return userIndex;
	}

}

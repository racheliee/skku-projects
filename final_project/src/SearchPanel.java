import java.awt.GridBagConstraints;
import java.awt.Insets;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.List;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.ListSelectionModel;
import javax.swing.SwingWorker;
import javax.swing.table.DefaultTableModel;
import java.awt.BorderLayout;
import java.awt.FlowLayout;

/**
 * This class represents the search panel of the library management system.
 * It extends the JPanel class and provides functionality for searching books.
 */
public class SearchPanel extends JPanel {

	private static final long serialVersionUID = 1L;
	private JScrollPane resultScrollPane;
	private JPanel leftMarginPanel;
	private JPanel bottomMarginPanel;
	private JPanel rightMarginPanel;
	private JTable resultBooktable;

	String currentUsername; // username of the user

	// thread worker to update the panel list
	public SwingWorker<Book, Book> worker;
	// object data is used to fill in the table
	Object data[][];
	// dialog to rent books if user clicks on a book
	BookRentDialog bookRentDialog;
	// checks if there was a search result
	boolean searchExists = false;
	// true is the user is logged in

	/**
	 * Create the panel.
	 */
	public SearchPanel(String searchedBook, String genre, List<Book> bookList, JFrame parentFrame,
			List<User> userList, LibraryMainPageGUI mainGUI) {
		setLayout(new BorderLayout(0, 0));

		// create the margins to the left of the search table
		leftMarginPanel = new JPanel();
		FlowLayout fl_leftMarginPanel = (FlowLayout) leftMarginPanel.getLayout();
		fl_leftMarginPanel.setHgap(40);
		add(leftMarginPanel, BorderLayout.WEST);

		// create the margins to the bottom of the search table
		bottomMarginPanel = new JPanel();
		FlowLayout fl_bottomMarginPanel = (FlowLayout) bottomMarginPanel.getLayout();
		fl_bottomMarginPanel.setHgap(20);
		add(bottomMarginPanel, BorderLayout.EAST);

		// create the margins to the right of the search table
		rightMarginPanel = new JPanel();
		FlowLayout fl_rightMarginPanel = (FlowLayout) rightMarginPanel.getLayout();
		fl_rightMarginPanel.setVgap(10);
		add(rightMarginPanel, BorderLayout.SOUTH);

		resultScrollPane = new JScrollPane();
		add(resultScrollPane);
		GridBagConstraints gbc_resultScrollPane = new GridBagConstraints();
		gbc_resultScrollPane.gridwidth = 10;
		gbc_resultScrollPane.insets = new Insets(0, 0, 0, 5);
		gbc_resultScrollPane.fill = GridBagConstraints.BOTH;
		gbc_resultScrollPane.gridx = 0;
		gbc_resultScrollPane.gridy = 3;

		// create table to contain the book list
		resultBooktable = new JTable(
				new DefaultTableModel(data, new Object[] { "Book Title", "Author", "Genre", "Rentable Copies" }));

		// create thread worker for searching books
		worker = new Worker(bookList, searchedBook, genre);
		worker.execute();

		resultBooktable.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseClicked(MouseEvent e) {
				// get the selected row index
				int selectedRow = resultBooktable.getSelectedRow();
				// get the title of the clicked book from the table
				String title = resultBooktable.getValueAt(selectedRow, 0).toString();
				// open the book rent dialog
				bookRentDialog = new BookRentDialog(bookList, title, userList, mainGUI.getUserIndex(),
						parentFrame);
				bookRentDialog.setVisible(true);

				// update the number of copies shown on the table
				if (bookRentDialog.isBookNumChanged()) {
					DefaultTableModel model = (DefaultTableModel) resultBooktable.getModel();
					model.setValueAt(bookRentDialog.getNumberofRemainingCopies(), selectedRow, 3);
					model.fireTableDataChanged();
				}

				bookRentDialog.dispose();
			}
		});
		resultBooktable.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		// dont allow user to edit table but can select rows
		resultBooktable.setDefaultEditor(Object.class, null);
		resultScrollPane.setViewportView(resultBooktable);
	}

	// thread worker to update the panel list
	class Worker extends SwingWorker<Book, Book> {
		List<Book> bookList;
		String searchedBook;
		String genre;
		String[][] bookData;
		int index = 0;

		// constructor for the worker
		public Worker(List<Book> bookList, String searchedBook, String genre) {
			this.bookList = bookList;
			this.searchedBook = searchedBook;
			this.genre = genre;
		}

		@Override
		// look for matching books
		protected Book doInBackground() throws Exception {
			for (Book book : bookList) {
				if (isMatchingGenre(book) && isMatchingKeyword(book)) {
					searchExists = true;
					Thread.sleep(100);
					publish(book);
				}
			}

			// if no results were found, show users that none were found.
			if (searchExists == false) {
				JOptionPane.showMessageDialog(null, "Sorry, No books were found. :(");
			}

			return null;
		}

		// update the booklist GUI
		protected void process(List<Book> published) {
			Book book = published.get(0);
			DefaultTableModel model = (DefaultTableModel) resultBooktable.getModel();
			model.addRow(new Object[] { book.getTitle(), book.getAuthor(), book.getGenre(),
					String.valueOf(book.getAvailableCopies().size()) });
		}

		// checks if the book's genre matches the genre the user searched
		protected boolean isMatchingGenre(Book book) {
			if (genre.equals("All")) {
				return true;
			}

			if (genre.equals(book.getGenre())) {
				return true;
			} else {
				return false;
			}
		}

		// checks if the keyword searched matches the book
		protected boolean isMatchingKeyword(Book book) {
			if (searchedBook.equals("")) {
				return true;
			}
			// convert the title, author, and keyword to lowercase to make the search case
			// insensitive
			if (book.getTitle().toLowerCase().contains(searchedBook.toLowerCase())
					|| book.getAuthor().toLowerCase().contains(searchedBook.toLowerCase())) {
				return true;
			} else {
				return false;
			}

		}

	}

}

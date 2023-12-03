

import java.awt.GridBagConstraints;
import java.awt.Insets;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.List;

import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.ListSelectionModel;
import javax.swing.SwingWorker;
import javax.swing.table.DefaultTableModel;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.FlowLayout;

public class BookListPanel extends JPanel {

	private static final long serialVersionUID = 1L;
	private JScrollPane resultScrollPane;
	private JPanel panel;
	private JPanel panel_1;
	private JPanel panel_2;
	private JTable resultBooktable;
	
	public SwingWorker<Book, Book> worker;
	Object data[][];
	
	/**
	 * Create the panel.
	 */
	public BookListPanel(String searchedBook, String genre, List<Book> bookList) {
		
		setLayout(new BorderLayout(0, 0));
		
		panel = new JPanel();
		FlowLayout flowLayout = (FlowLayout) panel.getLayout();
		flowLayout.setHgap(40);
		add(panel, BorderLayout.WEST);
		
		panel_1 = new JPanel();
		FlowLayout flowLayout_1 = (FlowLayout) panel_1.getLayout();
		flowLayout_1.setHgap(20);
		add(panel_1, BorderLayout.EAST);
		
		panel_2 = new JPanel();
		FlowLayout flowLayout_2 = (FlowLayout) panel_2.getLayout();
		flowLayout_2.setVgap(10);
		add(panel_2, BorderLayout.SOUTH);
		
		resultScrollPane = new JScrollPane();
		add(resultScrollPane);
		GridBagConstraints gbc_resultScrollPane = new GridBagConstraints();
		gbc_resultScrollPane.gridwidth = 10;
		gbc_resultScrollPane.insets = new Insets(0, 0, 0, 5);
		gbc_resultScrollPane.fill = GridBagConstraints.BOTH;
		gbc_resultScrollPane.gridx = 0;
		gbc_resultScrollPane.gridy = 3;
		
		
//		worker = new Worker(bookList, searchedBoosk, genre);
//		worker.execute();
		
//		String[] columnNames = { "Book Title", "Author", "Genre", "RentableCopies" };
	


		// 100 rows of example data

		resultBooktable = new JTable(new DefaultTableModel(data, new Object[]{"Book Title", "Author", "Genre", "Rentable Copies"}));
		
		worker = new Worker(bookList, searchedBook, genre);
		worker.execute();
		
		resultBooktable.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseClicked(MouseEvent e) {
				// get the selected row index
				int selectedRow = resultBooktable.getSelectedRow();
				// get the selected row values
				String title = resultBooktable.getValueAt(selectedRow, 0).toString();
				String author = resultBooktable.getValueAt(selectedRow, 1).toString();
				String year = resultBooktable.getValueAt(selectedRow, 2).toString();
				String avail = resultBooktable.getValueAt(selectedRow, 3).toString();

				// print the selected row values to the console
				System.out.println("Selected Row: " + selectedRow);
				System.out.println("Selected Row Values: " + title + ", " + author + ", " + year + ", " + avail);

			}
		});
		resultBooktable.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		// dont allow user to edit table but can select rows
		resultBooktable.setDefaultEditor(Object.class, null);

		resultScrollPane.setViewportView(resultBooktable);
	}
	
	
	class Worker extends SwingWorker <Book, Book>{
		List<Book> bookList;
		String searchedBook;
		String genre;
		String[][] bookData;
		int index = 0;
		
		public Worker(List<Book> bookList, String searchedBook, String genre) {
			this.bookList = bookList;
			this.searchedBook = searchedBook;
			this.genre = genre;
		}

		@Override
		protected Book doInBackground() throws Exception {
			// TODO Auto-generated method stub
			for(Book book: bookList) {
				if(isMatchingGenre(book) && isMatchingKeyword(book)){
					Thread.sleep(100);
					publish(book);
				}
			}
			
			return null;
		}
		
		protected void process(List<Book> published) {
			Book book = published.get(0);
			System.out.println(book.getTitle());
			DefaultTableModel model = (DefaultTableModel) resultBooktable.getModel();			
			model.addRow(new Object[]{book.getTitle(), book.getAuthor(), book.getGenre(), String.valueOf(book.getAvailableCopies().size())});
		}
		
		// checks if the book's genre matches the genre the user searched
		protected boolean isMatchingGenre(Book book) {
			if(genre.equals("---")) {
				return true;
			}
			
			if(genre.equals(book.getGenre())) {
				return true;
			}else {
				return false;
			}
		}
		
		//checks if the keyword searched matches the book
		protected boolean isMatchingKeyword(Book book) {
			if(searchedBook.equals("")) {
				return true;
			}

			if(book.getTitle().contains(searchedBook) || book.getAuthor().contains(searchedBook)) {
				return true;
			}else {
				return false;
			}
			
		}
		
	}
	
}

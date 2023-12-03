
import java.awt.GridBagConstraints;
import java.awt.Insets;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.ListSelectionModel;
import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.util.List;

public class BookListPanel extends JPanel {

	private static final long serialVersionUID = 1L;
	private JScrollPane resultScrollPane;
	private JPanel panel;
	private JPanel panel_1;
	private JPanel panel_2;
	BookRentDialog bookRentDialog;

	List<Book> bookList;

	/**
	 * Create the panel.
	 */
	public BookListPanel(List<Book> bookList) {
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

		String[] columnNames = { "Book Title", "Author", "Genre", "rentableCopies" };

		// 100 rows of example data
		Object[][] data = { { "book1", "Author1", "year1", "avail1" }, { "book1", "Author1", "year1", "avail1" },
				{ "The Great Gatsby", "F. Scott Fitzgerald", "Fiction", "2" },
				{ "book1", "Author1", "year1", "avail1" },
				{ "book1", "Author1", "year1", "avail1" }, { "book1", "Author1", "year1", "avail1" },
				{ "book1", "Author1", "year1", "avail1" }, { "book1", "Author1", "year1", "avail1" },
				{ "book1", "Author1", "year1", "avail1" }, { "book1", "Author1", "year1", "avail1" },
				{ "book1", "Author1", "year1", "avail1" }, { "book1", "Author1", "year1", "avail1" },
				{ "book1", "Author1", "year1", "avail1" }, { "book1", "Author1", "year1", "avail1" },
				{ "book1", "Author1", "year1", "avail1" }, { "book1", "Author1", "year1", "avail1" },
				{ "book1", "Author1", "year1", "avail1" }, { "book1", "Author1", "year1", "avail1" },
				{ "book1", "Author1", "year1", "avail1" }, { "book1", "Author1", "year1", "avail1" },
				{ "book1", "Author1", "year1", "avail1" }, { "book1", "Author1", "year1", "avail1" },
				{ "book1", "Author1", "year1", "avail1" }, { "book1", "Author1", "year1", "avail1" },
				{ "book1", "Author1", "year1", "avail1" }, { "book1", "Author1", "year1", "avail1" },
				{ "book1", "Author1", "year1", "avail2" } };

		JTable resultBooktable = new JTable(data, columnNames);
		resultBooktable.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseClicked(MouseEvent e) {
				// get the selected row index
				int selectedRow = resultBooktable.getSelectedRow();
				// get the selected row values
				String title = resultBooktable.getValueAt(selectedRow, 0).toString();
				String currentUsername = "test";
				bookRentDialog = new BookRentDialog(bookList, title, currentUsername);
				bookRentDialog.setVisible(true);
			}
		});
		resultBooktable.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		// dont allow user to edit table but can select rows
		resultBooktable.setDefaultEditor(Object.class, null);

		resultScrollPane.setViewportView(resultBooktable);

	}

}

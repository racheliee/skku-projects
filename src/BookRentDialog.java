
import java.awt.BorderLayout;
import java.awt.FlowLayout;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.border.EmptyBorder;
import java.awt.GridBagLayout;
import java.awt.GridBagConstraints;
import javax.swing.JLabel;
import java.awt.Insets;
import javax.swing.ImageIcon;
import javax.swing.SwingConstants;
import java.awt.event.ActionListener;
import java.util.List;
import java.awt.event.ActionEvent;

public class BookRentDialog extends JDialog {

	private static final long serialVersionUID = 1L;
	private final JPanel bookInfoPanel = new JPanel();

	public boolean isBorrowSuccessful = false;
	public int numCopiesRemaining = 0;

	/**
	 * Launch the application.
	 */
	// public static void main(String[] args) {
	// try {
	// BookRentDialog dialog = new BookRentDialog();
	// dialog.setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
	// dialog.setVisible(true);
	// } catch (Exception e) {
	// e.printStackTrace();
	// }
	// }

	/**
	 * Create the dialog.
	 */
	public BookRentDialog(List<Book> bookList, String title, String currentUsername, JFrame parentFrame) {
		super(parentFrame, true);

		int bookIndex = findBookIndex(bookList, title);

		setBounds(100, 100, 498, 300);
		getContentPane().setLayout(new BorderLayout());
		bookInfoPanel.setBorder(new EmptyBorder(5, 5, 5, 5));
		getContentPane().add(bookInfoPanel, BorderLayout.CENTER);
		GridBagLayout gbl_bookInfoPanel = new GridBagLayout();
		gbl_bookInfoPanel.columnWidths = new int[] { 20, 20, 20, 20, 20, 20 };
		gbl_bookInfoPanel.rowHeights = new int[] { 20, 20, 0, 20, 20, 0, 20, 20, 20 };
		gbl_bookInfoPanel.columnWeights = new double[] { 0.3, 0.0, 0.0, 0.0 };
		gbl_bookInfoPanel.rowWeights = new double[] { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
		bookInfoPanel.setLayout(gbl_bookInfoPanel);
		{
			JPanel panel = new JPanel();
			GridBagConstraints gbc_panel = new GridBagConstraints();
			gbc_panel.gridheight = 8;
			gbc_panel.insets = new Insets(0, 0, 0, 5);
			gbc_panel.fill = GridBagConstraints.BOTH;
			gbc_panel.gridx = 0;
			gbc_panel.gridy = 0;
			bookInfoPanel.add(panel, gbc_panel);
			{
				JLabel bookCover = new JLabel("");
				bookCover.setIcon(
						new ImageIcon(BookRentDialog.class.getResource(bookList.get(bookIndex).getImagePath())));
				panel.add(bookCover);
			}
		}
		{
			JLabel bookTitleLabel = new JLabel("Title: ");
			bookTitleLabel.setHorizontalAlignment(SwingConstants.LEFT);
			GridBagConstraints gbc_bookTitleLabel = new GridBagConstraints();
			gbc_bookTitleLabel.fill = GridBagConstraints.BOTH;
			gbc_bookTitleLabel.insets = new Insets(0, 0, 5, 5);
			gbc_bookTitleLabel.gridx = 2;
			gbc_bookTitleLabel.gridy = 2;
			bookInfoPanel.add(bookTitleLabel, gbc_bookTitleLabel);
		}
		{
			JLabel bookTitle = new JLabel(bookList.get(bookIndex).getTitle());
			GridBagConstraints gbc_bookTitle = new GridBagConstraints();
			gbc_bookTitle.insets = new Insets(0, 0, 5, 5);
			gbc_bookTitle.gridx = 3;
			gbc_bookTitle.gridy = 2;
			bookInfoPanel.add(bookTitle, gbc_bookTitle);
		}
		{
			JLabel authorLabel = new JLabel("Author: ");
			GridBagConstraints gbc_authorLabel = new GridBagConstraints();
			gbc_authorLabel.fill = GridBagConstraints.BOTH;
			gbc_authorLabel.insets = new Insets(0, 0, 5, 5);
			gbc_authorLabel.gridx = 2;
			gbc_authorLabel.gridy = 3;
			bookInfoPanel.add(authorLabel, gbc_authorLabel);
		}
		{
			JLabel author = new JLabel(bookList.get(bookIndex).getAuthor());
			GridBagConstraints gbc_author = new GridBagConstraints();
			gbc_author.insets = new Insets(0, 0, 5, 5);
			gbc_author.gridx = 3;
			gbc_author.gridy = 3;
			bookInfoPanel.add(author, gbc_author);
		}
		{
			JLabel genreLabel = new JLabel("Genre: ");
			GridBagConstraints gbc_genreLabel = new GridBagConstraints();
			gbc_genreLabel.fill = GridBagConstraints.BOTH;
			gbc_genreLabel.insets = new Insets(0, 0, 5, 5);
			gbc_genreLabel.gridx = 2;
			gbc_genreLabel.gridy = 4;
			bookInfoPanel.add(genreLabel, gbc_genreLabel);
		}
		{
			JLabel genre = new JLabel(bookList.get(bookIndex).getGenre());
			genre.setHorizontalAlignment(SwingConstants.LEFT);
			GridBagConstraints gbc_genre = new GridBagConstraints();
			gbc_genre.insets = new Insets(0, 0, 5, 5);
			gbc_genre.gridx = 3;
			gbc_genre.gridy = 4;
			bookInfoPanel.add(genre, gbc_genre);
		}
		{
			JLabel availabilityLabel = new JLabel("Availability: ");
			availabilityLabel.setHorizontalAlignment(SwingConstants.LEFT);
			GridBagConstraints gbc_availabilityLabel = new GridBagConstraints();
			gbc_availabilityLabel.fill = GridBagConstraints.BOTH;
			gbc_availabilityLabel.insets = new Insets(0, 0, 0, 5);
			gbc_availabilityLabel.gridx = 2;
			gbc_availabilityLabel.gridy = 5;
			bookInfoPanel.add(availabilityLabel, gbc_availabilityLabel);
		}
		{
			JLabel avaliability = new JLabel(bookList.get(bookIndex).getAvailableCopies().size() + " copies");
			GridBagConstraints gbc_avaliability = new GridBagConstraints();
			gbc_avaliability.insets = new Insets(0, 0, 0, 5);
			gbc_avaliability.gridx = 3;
			gbc_avaliability.gridy = 5;
			bookInfoPanel.add(avaliability, gbc_avaliability);
		}
		{
			JPanel buttonPane = new JPanel();
			buttonPane.setLayout(new FlowLayout(FlowLayout.RIGHT));
			getContentPane().add(buttonPane, BorderLayout.SOUTH);
			{
				JButton borrowButton = new JButton("Borrow");
				if (bookList.get(bookIndex).getAvailableCopies().size() == 0) {
					borrowButton.setEnabled(false);
					borrowButton.setText("Not Available");
				}
				borrowButton.addActionListener(new ActionListener() {
					public void actionPerformed(ActionEvent e) {

						if (bookList.get(bookIndex).getAvailableCopies().size() != 0) {
							// set borrowing to success
							isBorrowSuccessful = true;

							// borrowing book
							bookList.get(bookIndex).borrowBook(currentUsername);
						}

						// set number of copies remaining
						numCopiesRemaining = bookList.get(bookIndex).getAvailableCopies().size();

						setVisible(false);

					}
				});
				borrowButton.setActionCommand("Borrow");
				buttonPane.add(borrowButton);
				getRootPane().setDefaultButton(borrowButton);
			}
			{
				JButton cancelButton = new JButton("Cancel");
				cancelButton.addActionListener(new ActionListener() {
					public void actionPerformed(ActionEvent arg0) {
						// close dialog if cancel was pressed
						dispose();
					}
				});
				cancelButton.setActionCommand("Cancel");
				buttonPane.add(cancelButton);
			}
		}
	}

	// returns true is the book was borrowed
	public boolean isBookBorrowed() {
		return isBorrowSuccessful;
	}

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

	public int getNumberofRemainingCopies() {
		return numCopiesRemaining;
	}

}

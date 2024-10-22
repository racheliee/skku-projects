
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
import javax.swing.JOptionPane;

import java.awt.Insets;
import javax.swing.ImageIcon;
import javax.swing.SwingConstants;
import java.awt.event.ActionListener;
import java.util.List;
import java.awt.event.ActionEvent;

/*
 * This class represents the dialog for borrowing/returning books.
 * It extends the JDialog class and provides functionality for borrowing/returning books.
 */
public class BookRentDialog extends JDialog {

	private static final long serialVersionUID = 1L;
	private final JPanel bookInfoPanel = new JPanel();

	public boolean isBorrowSuccessful = false; // checks if the borrow was successful
	public boolean isReturnSuccessful = false; // checks if the return was successful
	public int numCopiesRemaining = 0; // number of copies remaining

	/**
	 * Create the dialog.
	 */
	public BookRentDialog(List<Book> bookList, String title, List<User> userList, int userIndex, JFrame parentFrame) {
		super(parentFrame, true);

		int bookIndex = findBookIndex(bookList, title); // index of the book in the bookList

		setBounds(100, 100, 546, 300);
		getContentPane().setLayout(new BorderLayout());
		bookInfoPanel.setBorder(new EmptyBorder(5, 5, 5, 5));
		getContentPane().add(bookInfoPanel, BorderLayout.CENTER);
		GridBagLayout gbl_bookInfoPanel = new GridBagLayout();
		gbl_bookInfoPanel.columnWidths = new int[] { 20, 30, 20, 20, 20, 20, 20, 20 };
		gbl_bookInfoPanel.rowHeights = new int[] { 20, 20, 0, 20, 20, 0, 20, 20, 20 };
		gbl_bookInfoPanel.columnWeights = new double[] { 0.3, 0.0, 0.0, 0.0 };
		gbl_bookInfoPanel.rowWeights = new double[] { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
		bookInfoPanel.setLayout(gbl_bookInfoPanel);
		{
			// book cover panel
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
				// check if the entered book image is inside the image folder, if not use a
				// default image
				try {
					bookCover.setIcon(
							new ImageIcon(BookRentDialog.class.getResource(bookList.get(bookIndex).getImagePath())));
				} catch (NullPointerException e) {
					bookCover.setIcon(new ImageIcon(BookRentDialog.class.getResource("/images/book2_r.png")));
				}
				panel.add(bookCover);
			}
		}
		{
			// book title label
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
			// book title
			JLabel bookTitle = new JLabel(bookList.get(bookIndex).getTitle());
			GridBagConstraints gbc_bookTitle = new GridBagConstraints();
			gbc_bookTitle.fill = GridBagConstraints.BOTH;
			gbc_bookTitle.insets = new Insets(0, 0, 5, 5);
			gbc_bookTitle.gridx = 3;
			gbc_bookTitle.gridy = 2;
			bookInfoPanel.add(bookTitle, gbc_bookTitle);
		}
		{
			// book author label
			JLabel authorLabel = new JLabel("Author: ");
			GridBagConstraints gbc_authorLabel = new GridBagConstraints();
			gbc_authorLabel.fill = GridBagConstraints.BOTH;
			gbc_authorLabel.insets = new Insets(0, 0, 5, 5);
			gbc_authorLabel.gridx = 2;
			gbc_authorLabel.gridy = 3;
			bookInfoPanel.add(authorLabel, gbc_authorLabel);
		}
		{
			// book author
			JLabel author = new JLabel(bookList.get(bookIndex).getAuthor());
			GridBagConstraints gbc_author = new GridBagConstraints();
			gbc_author.fill = GridBagConstraints.BOTH;
			gbc_author.insets = new Insets(0, 0, 5, 5);
			gbc_author.gridx = 3;
			gbc_author.gridy = 3;
			bookInfoPanel.add(author, gbc_author);
		}
		{
			// book genre label
			JLabel genreLabel = new JLabel("Genre: ");
			GridBagConstraints gbc_genreLabel = new GridBagConstraints();
			gbc_genreLabel.fill = GridBagConstraints.BOTH;
			gbc_genreLabel.insets = new Insets(0, 0, 5, 5);
			gbc_genreLabel.gridx = 2;
			gbc_genreLabel.gridy = 4;
			bookInfoPanel.add(genreLabel, gbc_genreLabel);
		}
		{
			// book genre
			JLabel genre = new JLabel(bookList.get(bookIndex).getGenre());
			genre.setHorizontalAlignment(SwingConstants.LEFT);
			GridBagConstraints gbc_genre = new GridBagConstraints();
			gbc_genre.fill = GridBagConstraints.BOTH;
			gbc_genre.insets = new Insets(0, 0, 5, 5);
			gbc_genre.gridx = 3;
			gbc_genre.gridy = 4;
			bookInfoPanel.add(genre, gbc_genre);
		}
		{
			// book availability label
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
			// book availability
			JLabel avaliability = new JLabel(bookList.get(bookIndex).getAvailableCopies().size() + " copies");
			GridBagConstraints gbc_avaliability = new GridBagConstraints();
			gbc_avaliability.fill = GridBagConstraints.BOTH;
			gbc_avaliability.insets = new Insets(0, 0, 0, 5);
			gbc_avaliability.gridx = 3;
			gbc_avaliability.gridy = 5;
			bookInfoPanel.add(avaliability, gbc_avaliability);
		}
		{
			// panel for buttons
			JPanel buttonPane = new JPanel();
			buttonPane.setLayout(new FlowLayout(FlowLayout.RIGHT));
			getContentPane().add(buttonPane, BorderLayout.SOUTH);
			{
				// borrow/return button
				JButton borrowReturnButton = new JButton("Borrow");
				// if there are no available copies, disable the button
				if (bookList.get(bookIndex).getAvailableCopies().size() == 0) {
					borrowReturnButton.setEnabled(false);
					borrowReturnButton.setText("Not Available");
				}
				// if the book is borrowed by the user, change the button to return
				if (userIndex != -1
						&& bookList.get(bookIndex).isBorrowedByUser(userList.get(userIndex).getUserName())) {
					borrowReturnButton.setEnabled(true);
					borrowReturnButton.setText("Return");
				}

				borrowReturnButton.addActionListener(new ActionListener() {
					public void actionPerformed(ActionEvent e) {
						// if the user is not logged in, or the user is admin, show error message
						try {
							if (userIndex == -1 || userIndex == 0) {
								throw new Exception();
							} else {
								// get the current user
								RegularUser currentUser = (RegularUser) userList.get(userIndex);

								// if the button is borrow
								if (borrowReturnButton.getText().equals("Borrow")) {
									// borrowing book
									bookList.get(bookIndex).borrowBook(currentUser.getUserName());

									// add the book to the user's borrowed book list
									for (HardCopy copy : bookList.get(bookIndex).getCopies()) {
										if (copy.getBorrower().equals(currentUser.getUserName())) {
											currentUser.addBorrowedBook(copy);
										}
									}

									// update the user in the userList
									userList.remove(userIndex);
									userList.add(userIndex, currentUser);

									// set borrowing to success
									isBorrowSuccessful = true;
								}

								// if the button is return
								else if (borrowReturnButton.getText().equals("Return")) {
									// return book from current user's book list
									currentUser.returnBook(bookList.get(bookIndex).getTitle());

									// returning book
									bookList.get(bookIndex).returnBook(currentUser.getUserName());

									// add the updated user to the userList
									userList.remove(userIndex);
									userList.add(userIndex, currentUser);

									// set return to success
									isReturnSuccessful = true;
								}

								// set number of copies remaining
								numCopiesRemaining = bookList.get(bookIndex).getAvailableCopies().size();
								setVisible(false);
							}

						}
						// if the user is not logged in, or the user is admin, show error message
						catch (Exception error) {
							if (userIndex == -1) {
								JOptionPane.showMessageDialog(null, "You must be logged in to borrow a book.", "Error",
										JOptionPane.ERROR_MESSAGE);
							} else {
								JOptionPane.showMessageDialog(null, "Admin cannot borrow books.", "Error",
										JOptionPane.ERROR_MESSAGE);
							}

						}

					}
				});
				borrowReturnButton.setActionCommand("Borrow/Return");
				buttonPane.add(borrowReturnButton);
				getRootPane().setDefaultButton(borrowReturnButton);
			}
			{
				// cancel button
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

	// returns true is the book was borrowed or returned successfully
	public boolean isBookNumChanged() {
		if (isBorrowSuccessful || isReturnSuccessful) {
			return true;
		} else {
			return false;
		}
	}

	// returns the index of the book in the bookList
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

	// returns the number of remaining copies
	public int getNumberofRemainingCopies() {
		return numCopiesRemaining;
	}

}

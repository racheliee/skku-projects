import java.awt.BorderLayout;
import java.awt.FlowLayout;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.border.EmptyBorder;
import java.awt.GridBagLayout;
import javax.swing.JLabel;
import javax.swing.JOptionPane;

import java.awt.GridBagConstraints;
import java.awt.Insets;
import javax.swing.JTextField;
import java.awt.event.ActionListener;
import java.io.FileOutputStream;
import java.io.PrintWriter;
import java.util.List;
import java.util.regex.Pattern;
import java.awt.event.ActionEvent;
import javax.swing.SwingConstants;

/**
 * This class represents the dialog for adding a new book to the library
 * management system. It extends the JDialog class and provides functionality
 * for adding a new book to the book list.
 */
public class AddNewBookDialog extends JDialog {

	private static final long serialVersionUID = 1L;
	private final JPanel contentPanel = new JPanel();
	private JTextField genreTextField;
	private JTextField authorTextField;
	private JTextField numCopiesTextField;
	private JTextField titleTextField;
	private JTextField imagePathTextField;

	boolean isAddBookButtonPressed = false; // to check if add book button is pressed

	/**
	 * Create the dialog for adding a new book
	 * 
	 * @param parentFrame the frame that the dialog is created from
	 * @param bookList    the list of books that the new book will be added to
	 */
	public AddNewBookDialog(JFrame parentFrame, List<Book> bookList) {
		super(parentFrame, true);

		setBounds(100, 100, 450, 300);
		getContentPane().setLayout(new BorderLayout());
		contentPanel.setBorder(new EmptyBorder(5, 5, 5, 5));
		getContentPane().add(contentPanel, BorderLayout.CENTER);
		GridBagLayout gbl_contentPanel = new GridBagLayout();
		gbl_contentPanel.columnWidths = new int[] { 0, 264, 0 };
		gbl_contentPanel.rowHeights = new int[] { 45, 43, 37, 37, 38, 42, 0 };
		gbl_contentPanel.columnWeights = new double[] { 0.0, 1.0, Double.MIN_VALUE };
		gbl_contentPanel.rowWeights = new double[] { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
		contentPanel.setLayout(gbl_contentPanel);
		{

			// title label
			JLabel titleLabel = new JLabel("  Title: ");
			titleLabel.setHorizontalAlignment(SwingConstants.LEFT);
			GridBagConstraints gbc_titleLabel = new GridBagConstraints();
			gbc_titleLabel.fill = GridBagConstraints.BOTH;
			gbc_titleLabel.anchor = GridBagConstraints.EAST;
			gbc_titleLabel.insets = new Insets(0, 0, 5, 5);
			gbc_titleLabel.gridx = 0;
			gbc_titleLabel.gridy = 1;
			contentPanel.add(titleLabel, gbc_titleLabel);
		}
		{
			// title text field for user input
			titleTextField = new JTextField();
			GridBagConstraints gbc_titleTextField = new GridBagConstraints();
			gbc_titleTextField.insets = new Insets(0, 0, 5, 0);
			gbc_titleTextField.fill = GridBagConstraints.HORIZONTAL;
			gbc_titleTextField.gridx = 1;
			gbc_titleTextField.gridy = 1;
			contentPanel.add(titleTextField, gbc_titleTextField);
			titleTextField.setColumns(10);
		}
		{

			// author label
			JLabel lblAuthor = new JLabel("  Author: ");
			GridBagConstraints gbc_lblAuthor = new GridBagConstraints();
			gbc_lblAuthor.fill = GridBagConstraints.BOTH;
			gbc_lblAuthor.anchor = GridBagConstraints.EAST;
			gbc_lblAuthor.insets = new Insets(0, 0, 5, 5);
			gbc_lblAuthor.gridx = 0;
			gbc_lblAuthor.gridy = 2;
			contentPanel.add(lblAuthor, gbc_lblAuthor);
		}
		{

			// author text field for user input
			authorTextField = new JTextField();
			GridBagConstraints gbc_authorTextField = new GridBagConstraints();
			gbc_authorTextField.insets = new Insets(0, 0, 5, 0);
			gbc_authorTextField.fill = GridBagConstraints.HORIZONTAL;
			gbc_authorTextField.gridx = 1;
			gbc_authorTextField.gridy = 2;
			contentPanel.add(authorTextField, gbc_authorTextField);
			authorTextField.setColumns(10);
		}
		{
			// number of copies label
			JLabel numCopiesLabel = new JLabel("  Number of Copies: ");
			GridBagConstraints gbc_numCopiesLabel = new GridBagConstraints();
			gbc_numCopiesLabel.fill = GridBagConstraints.BOTH;
			gbc_numCopiesLabel.anchor = GridBagConstraints.EAST;
			gbc_numCopiesLabel.insets = new Insets(0, 0, 5, 5);
			gbc_numCopiesLabel.gridx = 0;
			gbc_numCopiesLabel.gridy = 3;
			contentPanel.add(numCopiesLabel, gbc_numCopiesLabel);
		}
		{
			// number of copies text field for user input
			numCopiesTextField = new JTextField();
			GridBagConstraints gbc_numCopiesTextField = new GridBagConstraints();
			gbc_numCopiesTextField.insets = new Insets(0, 0, 5, 0);
			gbc_numCopiesTextField.fill = GridBagConstraints.HORIZONTAL;
			gbc_numCopiesTextField.gridx = 1;
			gbc_numCopiesTextField.gridy = 3;
			contentPanel.add(numCopiesTextField, gbc_numCopiesTextField);
			numCopiesTextField.setColumns(10);
		}
		{
			// genre label
			JLabel genreLabel = new JLabel("  Genre: ");
			GridBagConstraints gbc_genreLabel = new GridBagConstraints();
			gbc_genreLabel.fill = GridBagConstraints.BOTH;
			gbc_genreLabel.anchor = GridBagConstraints.EAST;
			gbc_genreLabel.insets = new Insets(0, 0, 5, 5);
			gbc_genreLabel.gridx = 0;
			gbc_genreLabel.gridy = 4;
			contentPanel.add(genreLabel, gbc_genreLabel);
		}
		{
			// genre text field for user input
			genreTextField = new JTextField();
			GridBagConstraints gbc_genreTextField = new GridBagConstraints();
			gbc_genreTextField.insets = new Insets(0, 0, 5, 0);
			gbc_genreTextField.fill = GridBagConstraints.HORIZONTAL;
			gbc_genreTextField.gridx = 1;
			gbc_genreTextField.gridy = 4;
			contentPanel.add(genreTextField, gbc_genreTextField);
			genreTextField.setColumns(10);
		}
		{
			// image path label
			JLabel imagePathLabel = new JLabel("  Book Cover Image Path: ");
			GridBagConstraints gbc_imagePathLabel = new GridBagConstraints();
			gbc_imagePathLabel.fill = GridBagConstraints.BOTH;
			gbc_imagePathLabel.anchor = GridBagConstraints.EAST;
			gbc_imagePathLabel.insets = new Insets(0, 0, 0, 5);
			gbc_imagePathLabel.gridx = 0;
			gbc_imagePathLabel.gridy = 5;
			contentPanel.add(imagePathLabel, gbc_imagePathLabel);
		}
		{
			// image path text field for user input
			imagePathTextField = new JTextField();
			GridBagConstraints gbc_imagePathTextField = new GridBagConstraints();
			gbc_imagePathTextField.fill = GridBagConstraints.HORIZONTAL;
			gbc_imagePathTextField.gridx = 1;
			gbc_imagePathTextField.gridy = 5;
			contentPanel.add(imagePathTextField, gbc_imagePathTextField);
			imagePathTextField.setColumns(10);
		}
		{
			JPanel buttonPane = new JPanel();
			buttonPane.setLayout(new FlowLayout(FlowLayout.RIGHT));
			getContentPane().add(buttonPane, BorderLayout.SOUTH);
			{
				// add button to add new book to book list
				JButton addButton = new JButton("Add");
				addButton.addActionListener(new ActionListener() {

					public void actionPerformed(ActionEvent e) {
						// get user input from text fields about new book
						String title = titleTextField.getText();
						String author = authorTextField.getText();
						String numCopies = numCopiesTextField.getText();
						String genre = genreTextField.getText();
						String imagePath = imagePathTextField.getText();

						// check if user input is valid
						try {
							if (isValidTitle(title) && isValidAuthor(author) && isValidNumCopies(numCopies)
									&& isValidGenre(genre) && isValidImagePath(imagePath)) {
								Book newBook = new Book(title, author, Integer.parseInt(numCopies), genre, imagePath);
								bookList.add(newBook);
								appendBookToFile(newBook);
								isAddBookButtonPressed = true;
								dispose();
							} else {
								throw new Exception();
							}
							// if user input is invalid, display error message
						} catch (Exception error) {
							String errorMsg = "";

							if (!isValidTitle(title)) {
								errorMsg += "Invalid title.\n";
							}
							if (!isValidAuthor(author)) {
								errorMsg += "Invalid author.\n";
							}
							if (!isValidNumCopies(numCopies)) {
								errorMsg += "Invalid number of copies.\n";
							}
							if (!isValidGenre(genre)) {
								errorMsg += "Invalid genre.\n";
							}
							if (!isValidImagePath(imagePath)) {
								errorMsg += "Invalid image path.\n";
							}
							JOptionPane.showMessageDialog(null, errorMsg, "Error", JOptionPane.ERROR_MESSAGE);
						}

					}
				});
				addButton.setActionCommand("Add");
				buttonPane.add(addButton);
				getRootPane().setDefaultButton(addButton);
			}
			{
				// cancel button to close dialog
				JButton cancelButton = new JButton("Cancel");
				cancelButton.addActionListener(new ActionListener() {
					public void actionPerformed(ActionEvent e) {
						dispose();
					}
				});
				cancelButton.setActionCommand("Cancel");
				buttonPane.add(cancelButton);
			}
		}
	}

	// check if the title is valid (only letters, numbers, and spaces)
	public boolean isValidTitle(String title) {
		if (Pattern.matches("^[a-zA-Z0-9\\s]+$", title)) {
			return true;
		}
		return false;
	}

	// check if the author is valid (only letters and spaces)
	public boolean isValidAuthor(String author) {
		if (Pattern.matches("^[a-zA-Z]+\\s[a-zA-Z]+$", author)) {
			return true;
		}
		return false;

	}

	// check if the number of copies is valid (only numbers)
	public boolean isValidNumCopies(String numCopies) {
		if (Pattern.matches("^\\d+$", numCopies)) {
			return true;
		}
		return false;
	}

	// check if the genre is valid (only Fiction, Non-Fiction, Mystery, or Romance)
	public boolean isValidGenre(String genre) {
		if (genre.equals("Fiction") || genre.equals("Non-Fiction") || genre.equals("Mystery")
				|| genre.equals("Romance")) {
			return true;
		}
		return false;
	}

	// check if the image path is valid (only letters, numbers, underscores, and
	// forward slashes)
	public boolean isValidImagePath(String imagePath) {
		if (Pattern.matches("^[a-zA-Z0-9\\/_.]+$", imagePath)) {
			return true;
		}
		return false;
	}

	// append new book to books.txt
	public void appendBookToFile(Book book) {
		FileOutputStream bookFile = null;
		// try to open books.txt
		try {
			bookFile = new FileOutputStream("books.txt", true);
			// if books.txt is not found, display error message
		} catch (Exception e) {
			JOptionPane.showMessageDialog(null, "books.txt not found", "Error", JOptionPane.ERROR_MESSAGE);
			e.printStackTrace();
		}

		// write new book to books.txt
		PrintWriter writer = new PrintWriter(bookFile);
		writer.println(book.getTitle());
		writer.println(book.getAuthor());
		writer.println(book.getAvailableCopies().size());
		writer.println(book.getGenre());
		writer.println(book.getImagePath());
		writer.close();
	}

	// check if add book button is pressed
	public boolean isAddNewBookButtonPressed() {
		return isAddBookButtonPressed;
	}
}

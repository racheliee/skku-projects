import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

/**
 * This class represents a regular user of the library.
 * It extends the User class and contains a list of borrowed books.
 * It also contains methods for adding and removing books from the list of
 * borrowed books.
 * Subclass of User.
 */
public class RegularUser extends User {
	// stores the list of borrowed books
	public List<HardCopy> borrowedBooks; // list of borrowed books of the user

	// constructor
	public RegularUser() {
		super();
		borrowedBooks = new ArrayList<HardCopy>();
	}

	// constructor
	public RegularUser(String username, String password) {
		super(username, password);
		borrowedBooks = new ArrayList<HardCopy>();
	}

	// returns the borrowedBook list
	public List<HardCopy> getBorrowedBooks() {
		return borrowedBooks;
	}

	// add book to borrowedBooks list
	public void addBorrowedBook(HardCopy book) {
		borrowedBooks.add(book);
	}

	// remove book from borrowedBooks list
	public void returnBook(String title) {
		Iterator<HardCopy> iterator = borrowedBooks.iterator();

		while (iterator.hasNext()) {
			HardCopy copy = iterator.next();
			if (copy.getBook().getTitle().equals(title)) {
				iterator.remove(); // Used an iterator to safely remove the element
			}
		}
	}

}

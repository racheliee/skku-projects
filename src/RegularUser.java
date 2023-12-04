import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class RegularUser extends User {
	public List<HardCopy> borrowedBooks;

	public RegularUser() {
		super();
		borrowedBooks = new ArrayList<HardCopy>();
	}

	public RegularUser(String username, String password) {
		super(username, password);
		borrowedBooks = new ArrayList<HardCopy>();
	}

	// returns the borrowedBook list
	public List<HardCopy> getBorrowedBooks() {
		return borrowedBooks;
	}

	// add book to borrowedBooks if user borrows a book
	public void addBorrowedBook(HardCopy book) {
		borrowedBooks.add(book);
	}

	// remove book from borrowedBooks if user returns book
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

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

// regular user is a child class of user. It has a list of borrowed books
public class RegularUser extends User {
	//stores the list of borrowed books
	public List<HardCopy> borrowedBooks;

	//constructor
	public RegularUser() {
		super();
		borrowedBooks = new ArrayList<HardCopy>();
	}
	
	//constructor
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

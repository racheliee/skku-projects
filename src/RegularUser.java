import java.util.ArrayList;
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
		for (HardCopy book1 : borrowedBooks) {
			System.out.println(book1.getBook().getTitle());
		}
	}

	// remove book from borrowedBooks if user returns book
	public void returnBook(String title) {
		for (HardCopy copies : borrowedBooks) {
			if (copies.getBook().getTitle().equals(title)) {
				borrowedBooks.remove(copies);
				break;
			}
		}
	}

}

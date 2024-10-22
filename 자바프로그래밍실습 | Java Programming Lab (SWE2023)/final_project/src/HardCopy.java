import java.time.LocalDate;

/**
 * This class represents a hard copy of a book in the library.
 * It contains information about the book copy such as the book it belongs to,
 * the borrow date, due date, whether it is borrowed or not, and the borrower.
 * It also contains methods for borrowing and returning the book copy.
 * Subclass of Book.
 */
public class HardCopy {
	private Book book; // book that the hard copy belongs to
	private LocalDate borrowDate; // date that the book is borrowed
	private LocalDate dueDate; // date that the book is due
	private boolean isBorrowed; // whether the book is borrowed or not
	private String borrower; // username of the borrower of the book

	public HardCopy(Book book) {
		this.book = book;
		this.borrowDate = null;
		this.dueDate = null;
		this.isBorrowed = false;
		this.borrower = "";
	}

	/**
	 * Borrow the book copy by setting the borrow date, due date, borrower
	 * 
	 * @param date     date that the book is borrowed
	 * @param borrower username of the borrower of the book
	 */
	public void borrowBookCopy(LocalDate date, String borrower) {
		if (!isBorrowed) {
			this.borrowDate = date;
			this.isBorrowed = true;
			this.borrower = borrower;
			this.dueDate = date.plusDays(7);
		}
	}

	/**
	 * Return the book copy by setting the borrow date, due date, borrower to null
	 */
	public void returnBookCopy() {
		this.borrowDate = null;
		this.isBorrowed = false;
		this.borrower = null;
	}

	// get & set methods
	public Book getBook() {
		return book;
	}

	public void setBook(Book book) {
		this.book = book;
	}

	public LocalDate getBorrowDate() {
		return borrowDate;
	}

	public void setBorrowDate(LocalDate borrowDate) {
		this.borrowDate = borrowDate;
	}

	public LocalDate getDueDate() {
		return dueDate;
	}

	public void setDueDate(LocalDate dueDate) {
		this.dueDate = dueDate;
	}

	public boolean isBorrowed() {
		return isBorrowed;
	}

	public void setBorrowed(boolean isBorrowed) {
		this.isBorrowed = isBorrowed;
	}

	public String getBorrower() {
		return borrower;
	}

	public void setBorrower(String borrower) {
		this.borrower = borrower;
	}
}

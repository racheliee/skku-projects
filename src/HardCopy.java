import java.time.LocalDate;

// the hard copy class only stores information on borrowed status and due date
public class HardCopy {
	private Book book;
	private LocalDate borrowDate;
	private LocalDate dueDate;
	private boolean isBorrowed;
	private String borrower;

	public HardCopy(Book book) {
		this.book = book;
		this.borrowDate = null;
		this.dueDate = null;
		this.isBorrowed = false;
	}

	public void borrowBookCopy(LocalDate date, String borrower) {
		if (!isBorrowed) {
			this.borrowDate = date;
			this.isBorrowed = true;
			this.borrower = borrower;
		} else {
			System.out.println("This copy is already borrowed.");
		}
	}

	public void returnBookCopy() {
		this.borrowDate = null;
		this.isBorrowed = false;
	}

	// get set methods
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

	// Additional methods as needed
}

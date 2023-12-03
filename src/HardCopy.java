import java.time.LocalDate;

public class HardCopy {
    private Book book;
    private LocalDate borrowDate;
    private boolean isBorrowed;
    private String borrower;

    public HardCopy(Book book) {
        this.book = book;
        this.borrowDate = null;
        this.isBorrowed = false;
    }

    public void borrowBookCopy(LocalDate date) {
        if (!isBorrowed) {
            this.borrowDate = date;
            this.isBorrowed = true;
        } else {
            System.out.println("This copy is already borrowed.");
        }
    }

    public void returnBookCopy() {
        this.borrowDate = null;
        this.isBorrowed = false;
    }

    public LocalDate getBorrowDate() {
        return borrowDate;
    }

    public boolean isBorrowed() {
        return isBorrowed;
    }

    public Book getBook() {
        return book;
    }

    // Additional methods as needed
}

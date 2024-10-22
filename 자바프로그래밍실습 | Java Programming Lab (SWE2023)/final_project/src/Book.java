
import java.time.LocalDate;
import java.util.ArrayList;
import java.util.List;

/*
 * This class represents a book in the library.
 * It contains information about the book such as the title, author, genre, and
 * the number of copies.
 */
public class Book {
    String title; // title of the book
    String author; // author of the book
    List<HardCopy> copies; // list of copies of the book
    private String genre; // genre of the book
    String imagePath; // path to the image of the book

    // default constructor
    public Book() {
        title = "";
        author = "";
        copies = null;
        genre = "";
    }

    // constructor
    public Book(String title, String author, int numberOfCopies, String genre, String imagePath) {
        this.title = title;
        this.author = author;
        this.copies = new ArrayList<>();
        this.genre = genre;
        this.imagePath = imagePath;

        // add copies of the book to the list of copies
        for (int i = 0; i < numberOfCopies; i++) {
            copies.add(new HardCopy(this));
        }
    }

    // find the copies of the book that are available and return them as a list
    public List<HardCopy> getAvailableCopies() {
        List<HardCopy> availableCopies = new ArrayList<>();
        for (HardCopy copy : copies) {
            if (!copy.isBorrowed()) {
                availableCopies.add(copy);
            }
        }

        return availableCopies;
    }

    /*
     * get username of the borrower of the book as a parameter and borrow the book
     * using HardCopy.borrowBookCopy() method
     * if there are available copies
     */
    public void borrowBook(String username) {
        List<HardCopy> availableCopies = getAvailableCopies();

        if (availableCopies.size() > 0) {
            availableCopies.get(0).borrowBookCopy(LocalDate.now(), username);
        } else {
            System.out.println("No copies available.");
        }
    }

    /*
     * get username of the borrower of the book as a parameter and return the book
     * using HardCopy.returnBookCopy() method
     * if the user has borrowed the book
     */
    public void returnBook(String username) {
        for (HardCopy copy : copies) {
            if (copy.isBorrowed() && copy.getBorrower().equals(username)) {
                copy.returnBookCopy();
                break;
            }
        }
    }

    // get & set methods
    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getAuthor() {
        return author;
    }

    public void setAuthor(String author) {
        this.author = author;
    }

    public List<HardCopy> getCopies() {
        return copies;
    }

    public void setCopies(List<HardCopy> copies) {
        this.copies = copies;
    }

    public String getGenre() {
        return genre;
    }

    public void setGenre(String genre) {
        this.genre = genre;
    }

    public String getImagePath() {
        return imagePath;
    }

    public void setImagePath(String imagePath) {
        this.imagePath = imagePath;
    }

    // check if the book is borrowed by the user
    // we'll use this function for chaning the borrow/return button text
    public boolean isBorrowedByUser(String username) {
        for (HardCopy copy : copies) {
            if (copy.isBorrowed() && copy.getBorrower().equals(username)) {
                return true;
            }
        }
        return false;
    }

}

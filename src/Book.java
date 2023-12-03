

import java.time.LocalDate;
import java.util.ArrayList;
import java.util.List;

public class Book {
    String title;
    String author;
    List<HardCopy> copies;

    public Book(String title, String author, int numberOfCopies) {
        this.title = title;
        this.author = author;
        this.copies = new ArrayList<>();

        for (int i = 0; i < numberOfCopies; i++) {
            copies.add(new HardCopy(this));
        }
    }

    public List<HardCopy> getAvailableCopies() {
        List<HardCopy> availableCopies = new ArrayList<>();

        for (HardCopy copy : copies) {
            if (!copy.isBorrowed()) {
                availableCopies.add(copy);
            }
        }

        return availableCopies;
    }

    // borrowBook() method
    public void borrowBook() {
        List<HardCopy> availableCopies = getAvailableCopies();

        if (availableCopies.size() > 0) {
            availableCopies.get(0).borrowBookCopy(LocalDate.now());
        } else {
            System.out.println("No copies available.");
        }
    }

    // returnBook() method
    public void returnBook() {
        for (HardCopy copy : copies) {
            if (copy.isBorrowed()) {
                copy.returnBookCopy();
                break;
            }
        }
    }

    // additional methods as needed

}

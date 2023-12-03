

import java.time.LocalDate;
import java.util.ArrayList;
import java.util.List;

public class Book {
    String title;
    String author;
    List<HardCopy> copies;
    private String genre;
    
    //default constructor
    public Book() {
    	title = "";
    	author = "";
    	copies = null;
    	genre = "";
    }

    public Book(String title, String author, int numberOfCopies, String genre) {
        this.title = title;
        this.author = author;
        this.copies = new ArrayList<>();
        this.genre = genre;

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

    //get & set methods
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

	

    // additional methods as needed
    
    
  
    

}

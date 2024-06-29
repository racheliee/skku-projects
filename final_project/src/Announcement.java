
/*
 * Announcement class
 * for storing announcement information
 */
public class Announcement {
	String title;
	String contents;

	public Announcement(String title, String contents) {
		this.title = title;
		this.contents = contents;
	}

	public String getTitle() {
		return this.title;
	}

	public String getContents() {
		return contents;
	}

	public void setTitle(String title) {
		this.title = title;
	}

	public void setContents(String contents) {
		this.contents = contents;
	}
}

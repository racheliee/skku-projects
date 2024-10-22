import java.awt.BorderLayout;
import java.util.List;
import javax.swing.JFrame;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JPanel;
import javax.swing.JLabel;
import java.awt.GridBagLayout;
import java.awt.GridBagConstraints;
import java.awt.Insets;
import javax.swing.JTextField;
import javax.swing.BoxLayout;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import java.awt.Font;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

/**
 * This class represents the dialog for adding/editing announcements.
 * It extends the JDialog class and provides functionality for adding/editing
 * announcements.
 */
public class AnnouncementDialog extends JDialog {

	private static final long serialVersionUID = 1L;
	private JLabel announcementTitleLabel;
	private JPanel buttonPannel;
	private JButton editButton;
	private JScrollPane scrollPane;
	private JTextArea textArea;
	private JTextField announcementTitle;
	private JButton applyEditButton;
	private JButton deleteButton;

	boolean isDeleteButtonPressed = false; // checks if the delete button was pressed
	boolean isApplyEditButtonPressed = false; // for checking if applyEditButton is pressed to determine whether to
												// add new announcement table row
	int currentIndex = -1;

	public AnnouncementDialog(List<Announcement> announcementList, int announcementIndex,
			JFrame parentFrame, boolean isAdmin, boolean isNewAnnouncement) {
		super(parentFrame, true);
		currentIndex = announcementIndex;

		setBounds(100, 100, 529, 391);
		getContentPane().setLayout(new BorderLayout());
		{
			// announcement title panel for displaying the title of the announcement
			JPanel panel = new JPanel();
			getContentPane().add(panel, BorderLayout.NORTH);
			GridBagLayout gbl_panel = new GridBagLayout();
			gbl_panel.columnWidths = new int[] { 0, 0, 0 };
			gbl_panel.rowHeights = new int[] { 0, 0 };
			gbl_panel.columnWeights = new double[] { 0.0, 1.0, Double.MIN_VALUE };
			gbl_panel.rowWeights = new double[] { 0.0, Double.MIN_VALUE };
			panel.setLayout(gbl_panel);

			// announcement title label
			announcementTitleLabel = new JLabel(" Title : ");
			announcementTitleLabel.setFont(new Font("Lucida Grande", Font.PLAIN, 21));
			GridBagConstraints gbc_announcementTitleLabel = new GridBagConstraints();
			gbc_announcementTitleLabel.anchor = GridBagConstraints.EAST;
			gbc_announcementTitleLabel.insets = new Insets(0, 0, 0, 5);
			gbc_announcementTitleLabel.gridx = 0;
			gbc_announcementTitleLabel.gridy = 0;
			panel.add(announcementTitleLabel, gbc_announcementTitleLabel);

			// announcement title text field for displaying the title of the announcement
			announcementTitle = new JTextField();
			// if the situation is adding a new announcement, then set the text field to be
			// empty and editable
			if (isNewAnnouncement) {
				announcementTitle.setText("");
				announcementTitle.setEditable(true);
			}
			// if the situation is editing an existing announcement, then set the text field
			// to be the title of the announcement and not editable
			else {
				announcementTitle.setText(announcementList.get(announcementIndex).getTitle());
				announcementTitle.setEditable(false);
			}
			announcementTitle.setFont(new Font("Lucida Grande", Font.PLAIN, 21));
			GridBagConstraints gbc_announcementTitle = new GridBagConstraints();
			gbc_announcementTitle.fill = GridBagConstraints.HORIZONTAL;
			gbc_announcementTitle.gridx = 1;
			gbc_announcementTitle.gridy = 0;
			panel.add(announcementTitle, gbc_announcementTitle);
			announcementTitle.setColumns(10);

		}
		{
			JPanel panel = new JPanel();
			getContentPane().add(panel, BorderLayout.CENTER);
			panel.setLayout(new BoxLayout(panel, BoxLayout.X_AXIS));

			scrollPane = new JScrollPane();
			panel.add(scrollPane);

			// text area for displaying the contents of the announcement
			textArea = new JTextArea();
			textArea.setLineWrap(true);
			// if the situation is adding a new announcement, then set the text area to be
			// empty and editable
			if (isNewAnnouncement) {
				textArea.setText("");
				textArea.setEditable(true);
			}
			// if the situation is editing an existing announcement, then set the text area
			// to be the contents of the announcement and not editable
			else {
				textArea.setText(announcementList.get(announcementIndex).getContents());
				textArea.setEditable(false);
			}
			textArea.setFont(new Font("Lucida Grande", Font.PLAIN, 18));
			scrollPane.setViewportView(textArea);
		}

		// button panel for edit, apply, and delete buttons
		buttonPannel = new JPanel();
		getContentPane().add(buttonPannel, BorderLayout.SOUTH);

		// edit button for editing the announcement
		editButton = new JButton("edit");
		editButton.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseClicked(MouseEvent e) {
				announcementTitle.setEditable(true);
				textArea.setEditable(true);

				// when the edit button is pressed, set the apply button visible and the edit
				// button invisible
				applyEditButton.setVisible(true);
				editButton.setVisible(false);
			}
		});
		buttonPannel.add(editButton);
		// set the edit button visible only if the user is an admin and the announcement
		editButton.setVisible(isAdmin && !isNewAnnouncement);

		// apply button for applying the changes made to the announcement
		applyEditButton = new JButton("apply");
		applyEditButton.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseClicked(MouseEvent e) {
				// if the situation is adding a new announcement, then add the new announcement
				// to the announcement list
				if (isNewAnnouncement) {
					announcementList.add(new Announcement(announcementTitle.getText(), textArea.getText()));
					isApplyEditButtonPressed = true;
				}
				// if the situation is editing an existing announcement, then set the title and
				// contents of the announcement to the new title and contents
				else {
					String newTitle = announcementTitle.getText();
					String newContents = textArea.getText();
					announcementList.get(announcementIndex).setTitle(newTitle);
					announcementList.get(announcementIndex).setContents(newContents);
				}
				setVisible(false);
			}
		});
		buttonPannel.add(applyEditButton);
		applyEditButton.setVisible(false || isNewAnnouncement);

		// delete button for deleting the announcement
		deleteButton = new JButton("delete");
		deleteButton.addMouseListener(new MouseAdapter() {
			public void mouseClicked(MouseEvent e) {
				// System.out.println(announcementIndex);
				announcementList.remove(announcementIndex);
				isDeleteButtonPressed = true;

				setVisible(false);
			}
		});
		buttonPannel.add(deleteButton);
		// set the delete button visible only if the user is an admin and the
		// announcement
		deleteButton.setVisible(isAdmin && !isNewAnnouncement);
	}

	// returns true if the applyEditButton is pressed
	public boolean isApplyEditButtonPressed() {
		return isApplyEditButtonPressed;
	}

	// returns true if the deleteButton is pressed
	public boolean isDeleteButtonPressed() {
		return isDeleteButtonPressed;
	}

	// returns the index of the announcement
	public int getAnnouncementIndex() {
		return currentIndex;
	}
}

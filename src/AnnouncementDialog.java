import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.util.List;
import javax.swing.JFrame;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JPanel;
import javax.swing.border.EmptyBorder;
import javax.swing.JLabel;
import java.awt.GridBagLayout;
import java.awt.GridBagConstraints;
import java.awt.Insets;
import javax.swing.JTextField;
import javax.swing.BoxLayout;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.SwingConstants;
import java.awt.Font;

public class AnnouncementDialog extends JDialog {

	private static final long serialVersionUID = 1L;
	private JLabel announcementTitleLabel;
	private JPanel panel_1;
	private JButton btnNewButton;
	private JScrollPane scrollPane;
	private JTextArea textArea;
	private JLabel announcementTitle;

	public AnnouncementDialog(List<Announcement> announcementList, int announcementIndex,
			JFrame parentFrame) {
		setBounds(100, 100, 645, 590);
		getContentPane().setLayout(new BorderLayout());
		{
			JPanel panel = new JPanel();
			getContentPane().add(panel, BorderLayout.NORTH);
			GridBagLayout gbl_panel = new GridBagLayout();
			gbl_panel.columnWidths = new int[] { 0, 0, 0 };
			gbl_panel.rowHeights = new int[] { 0, 0 };
			gbl_panel.columnWeights = new double[] { 0.0, 1.0, Double.MIN_VALUE };
			gbl_panel.rowWeights = new double[] { 0.0, Double.MIN_VALUE };
			panel.setLayout(gbl_panel);

			announcementTitleLabel = new JLabel(" Title : ");
			announcementTitleLabel.setFont(new Font("Lucida Grande", Font.PLAIN, 21));
			GridBagConstraints gbc_announcementTitleLabel = new GridBagConstraints();
			gbc_announcementTitleLabel.anchor = GridBagConstraints.EAST;
			gbc_announcementTitleLabel.insets = new Insets(0, 0, 0, 5);
			gbc_announcementTitleLabel.gridx = 0;
			gbc_announcementTitleLabel.gridy = 0;
			panel.add(announcementTitleLabel, gbc_announcementTitleLabel);

			announcementTitle = new JLabel(announcementList.get(announcementIndex).getTitle());
			announcementTitle.setFont(new Font("Lucida Grande", Font.PLAIN, 21));
			announcementTitle.setHorizontalAlignment(SwingConstants.LEFT);
			GridBagConstraints gbc_announcementTitle = new GridBagConstraints();
			gbc_announcementTitle.fill = GridBagConstraints.BOTH;
			gbc_announcementTitle.gridx = 1;
			gbc_announcementTitle.gridy = 0;
			panel.add(announcementTitle, gbc_announcementTitle);

		}
		{
			JPanel panel = new JPanel();
			getContentPane().add(panel, BorderLayout.CENTER);
			panel.setLayout(new BoxLayout(panel, BoxLayout.X_AXIS));

			scrollPane = new JScrollPane();
			panel.add(scrollPane);

			textArea = new JTextArea(announcementList.get(announcementIndex).getContents());
			textArea.setEditable(false);
			textArea.setFont(new Font("Lucida Grande", Font.PLAIN, 18));
			scrollPane.setViewportView(textArea);
		}

		panel_1 = new JPanel();
		getContentPane().add(panel_1, BorderLayout.SOUTH);

		btnNewButton = new JButton("New button");
		panel_1.add(btnNewButton);
	}

}

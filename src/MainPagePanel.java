
import javax.swing.JPanel;
import java.awt.GridBagLayout;
import javax.swing.JLabel;
import java.awt.GridBagConstraints;
import java.awt.Insets;
import java.awt.GridLayout;
import java.awt.BorderLayout;
import javax.swing.GroupLayout;
import javax.swing.GroupLayout.Alignment;
import javax.swing.SwingConstants;
import javax.swing.LayoutStyle.ComponentPlacement;

public class MainPagePanel extends JPanel {

	private static final long serialVersionUID = 1L;
	private JLabel lblNewLabel;
	private JLabel weekdayLabel;
	private JLabel satLabel;
	private JLabel sunLabel;
	private JLabel specialEventLabel;
	private JLabel event1Label;
	private JLabel event2Label;
	private JLabel event3Label;


	/**
	 * Create the panel.
	 */
	public MainPagePanel() {
		GridBagLayout gridBagLayout = new GridBagLayout();
		gridBagLayout.columnWidths = new int[] {30, 30, 30, 30, 30};
		gridBagLayout.rowHeights = new int[] {30, 30, 30, 30, 30, 0, 30, 0, 0, 0, 0, 0};
		gridBagLayout.columnWeights = new double[]{0.0, 0.0, 0.0, 0.0, 0.0};
		gridBagLayout.rowWeights = new double[]{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, Double.MIN_VALUE};
		setLayout(gridBagLayout);
		
		lblNewLabel = new JLabel("Library Hours;");
		GridBagConstraints gbc_lblNewLabel = new GridBagConstraints();
		gbc_lblNewLabel.gridwidth = 2;
		gbc_lblNewLabel.insets = new Insets(0, 0, 5, 5);
		gbc_lblNewLabel.gridx = 1;
		gbc_lblNewLabel.gridy = 2;
		add(lblNewLabel, gbc_lblNewLabel);
		
		weekdayLabel = new JLabel("Monday to Friday: 9AM - 6PM");
		GridBagConstraints gbc_weekdayLabel = new GridBagConstraints();
		gbc_weekdayLabel.fill = GridBagConstraints.BOTH;
		gbc_weekdayLabel.insets = new Insets(0, 0, 5, 5);
		gbc_weekdayLabel.gridx = 3;
		gbc_weekdayLabel.gridy = 3;
		add(weekdayLabel, gbc_weekdayLabel);
		
		satLabel = new JLabel("Saturday: 9AM - 4PM");
		GridBagConstraints gbc_satLabel = new GridBagConstraints();
		gbc_satLabel.fill = GridBagConstraints.BOTH;
		gbc_satLabel.insets = new Insets(0, 0, 5, 5);
		gbc_satLabel.gridx = 3;
		gbc_satLabel.gridy = 4;
		add(satLabel, gbc_satLabel);
		
		sunLabel = new JLabel("Sunday: closed");
		GridBagConstraints gbc_sunLabel = new GridBagConstraints();
		gbc_sunLabel.fill = GridBagConstraints.BOTH;
		gbc_sunLabel.insets = new Insets(0, 0, 5, 5);
		gbc_sunLabel.gridx = 3;
		gbc_sunLabel.gridy = 5;
		add(sunLabel, gbc_sunLabel);
		
		specialEventLabel = new JLabel("Special Events This Week:");
		GridBagConstraints gbc_specialEventLabel = new GridBagConstraints();
		gbc_specialEventLabel.gridwidth = 3;
		gbc_specialEventLabel.fill = GridBagConstraints.BOTH;
		gbc_specialEventLabel.insets = new Insets(0, 0, 5, 5);
		gbc_specialEventLabel.gridx = 1;
		gbc_specialEventLabel.gridy = 7;
		add(specialEventLabel, gbc_specialEventLabel);
		
		event1Label = new JLabel("Tech Talk Workshop: Wednesday, 3 PM - 5 PM");
		GridBagConstraints gbc_event1Label = new GridBagConstraints();
		gbc_event1Label.insets = new Insets(0, 0, 5, 5);
		gbc_event1Label.fill = GridBagConstraints.BOTH;
		gbc_event1Label.gridx = 3;
		gbc_event1Label.gridy = 8;
		add(event1Label, gbc_event1Label);
		
		event2Label = new JLabel("Book Club Discussion: Thursday, 6:30 PM - 8 PM");
		GridBagConstraints gbc_event2Label = new GridBagConstraints();
		gbc_event2Label.insets = new Insets(0, 0, 5, 5);
		gbc_event2Label.fill = GridBagConstraints.BOTH;
		gbc_event2Label.gridx = 3;
		gbc_event2Label.gridy = 9;
		add(event2Label, gbc_event2Label);
		
		event3Label = new JLabel("Children's Storytime: Saturday, 10:30 AM - 11 AM");
		GridBagConstraints gbc_event3Label = new GridBagConstraints();
		gbc_event3Label.insets = new Insets(0, 0, 0, 5);
		gbc_event3Label.fill = GridBagConstraints.BOTH;
		gbc_event3Label.gridx = 3;
		gbc_event3Label.gridy = 10;
		add(event3Label, gbc_event3Label);
	
	}

}


import javax.swing.JPanel;
import java.awt.GridBagLayout;
import javax.swing.JLabel;
import java.awt.GridBagConstraints;
import java.awt.Insets;
import javax.swing.SwingConstants;

/**
 * This class represents the intro page panel of the library management system.
 * It extends the JPanel class and provides functionality for displaying the
 * library hours.
 */
public class IntroPagePanel extends JPanel {

	private static final long serialVersionUID = 1L;
	private JLabel lblNewLabel;
	private JLabel weekdayLabel;
	private JLabel satLabel;
	private JLabel sunLabel;

	/**
	 * Create the panel.
	 */
	public IntroPagePanel() {
		GridBagLayout gridBagLayout = new GridBagLayout();
		gridBagLayout.columnWidths = new int[] { 30, 30, 30, 30, 30 };
		gridBagLayout.rowHeights = new int[] { 30, 30, 30, 30, 30, 0, 30, 0, 0, 0, 0, 0 };
		gridBagLayout.columnWeights = new double[] { 0.0, 0.0, 0.0, 0.0, 0.0 };
		gridBagLayout.rowWeights = new double[] { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
				Double.MIN_VALUE };
		setLayout(gridBagLayout);

		// library hours label
		lblNewLabel = new JLabel("Library Hours:");
		GridBagConstraints gbc_lblNewLabel = new GridBagConstraints();
		gbc_lblNewLabel.gridwidth = 3;
		gbc_lblNewLabel.insets = new Insets(0, 0, 5, 5);
		gbc_lblNewLabel.gridx = 1;
		gbc_lblNewLabel.gridy = 2;
		add(lblNewLabel, gbc_lblNewLabel);

		// library hours
		weekdayLabel = new JLabel("Monday to Friday: 9AM - 6PM");
		GridBagConstraints gbc_weekdayLabel = new GridBagConstraints();
		gbc_weekdayLabel.fill = GridBagConstraints.BOTH;
		gbc_weekdayLabel.insets = new Insets(0, 0, 5, 5);
		gbc_weekdayLabel.gridx = 2;
		gbc_weekdayLabel.gridy = 3;
		add(weekdayLabel, gbc_weekdayLabel);

		// library hours
		satLabel = new JLabel("Saturday: 9AM - 4PM");
		satLabel.setHorizontalAlignment(SwingConstants.CENTER);
		GridBagConstraints gbc_satLabel = new GridBagConstraints();
		gbc_satLabel.fill = GridBagConstraints.BOTH;
		gbc_satLabel.insets = new Insets(0, 0, 5, 5);
		gbc_satLabel.gridx = 2;
		gbc_satLabel.gridy = 4;
		add(satLabel, gbc_satLabel);

		// library hours
		sunLabel = new JLabel("Sunday: closed");
		sunLabel.setHorizontalAlignment(SwingConstants.CENTER);
		GridBagConstraints gbc_sunLabel = new GridBagConstraints();
		gbc_sunLabel.fill = GridBagConstraints.BOTH;
		gbc_sunLabel.insets = new Insets(0, 0, 5, 5);
		gbc_sunLabel.gridx = 2;
		gbc_sunLabel.gridy = 5;
		add(sunLabel, gbc_sunLabel);

	}

}

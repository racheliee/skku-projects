package assignment_4;

import java.awt.EventQueue;
import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.SwingWorker;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JButton;
import javax.swing.JProgressBar;
import java.awt.Font;
import java.awt.event.ActionListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.FileOutputStream;
import java.io.PrintWriter;
import java.util.List;
import java.util.regex.Pattern;
import java.awt.event.ActionEvent;
import java.awt.Color;
import java.awt.BorderLayout;
import javax.swing.JPanel;
import java.awt.GridBagLayout;
import java.awt.GridBagConstraints;
import java.awt.Insets;

public class LucasSeries {

	private JFrame lucasSumFrame;
	private JScrollPane scrollPane;
	private JPanel topRowPanel;
	private JPanel BottomRowPanel;
	private JTextArea lucasSumTextArea;
	private JTextField inputNumTextField;
	private JLabel numRowLabel;
	private JButton sumButton;
	private JButton cancelButton;
	private JProgressBar progressBar;
	private JLabel sumLabel;

	private int num; // the number of lucas series we want to calculate
	boolean cancelled; // keeps track if the user cancelled the calculation

	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					LucasSeries window = new LucasSeries();
					window.lucasSumFrame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * Create the application.
	 */
	public LucasSeries() {
		initialize();
	}

	/**
	 * Initialize the contents of the frame.
	 */
	private void initialize() {
		lucasSumFrame = new JFrame();
		lucasSumFrame.setBounds(100, 100, 610, 421);
		lucasSumFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		lucasSumFrame.getContentPane().setLayout(new BorderLayout(0, 0));

		// create the scrollable area for the text where the lucas series is being
		// calculated
		scrollPane = new JScrollPane();
		lucasSumFrame.getContentPane().add(scrollPane);

		lucasSumTextArea = new JTextArea();
		lucasSumTextArea.setFont(new Font("PingFang TC", Font.PLAIN, 12));
		lucasSumTextArea.setWrapStyleWord(true);
		scrollPane.setViewportView(lucasSumTextArea);

		// create the a panel for the top row components
		// (labels, input text fields, getting sum button, cancel button)
		topRowPanel = new JPanel();
		lucasSumFrame.getContentPane().add(topRowPanel, BorderLayout.NORTH);

		numRowLabel = new JLabel("Number of row in Lucas Series: ");
		topRowPanel.add(numRowLabel);
		numRowLabel.setFont(new Font("PingFang TC", Font.PLAIN, 12));

		inputNumTextField = new JTextField();
		topRowPanel.add(inputNumTextField);
		inputNumTextField.setFont(new Font("PingFang TC", Font.PLAIN, 12));
		inputNumTextField.setColumns(10);

		// sumButton initiates the calculation of the Lucas Series
		sumButton = new JButton("Get Sum of Lucas Series");
		topRowPanel.add(sumButton);
		sumButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				// get the input number
				String input = inputNumTextField.getText();

				try {
					// if the input is not valid
					if (Pattern.matches("^[0-9]+$", input)) {
						num = Integer.parseInt(input);
						if (num < 3 || num > 20) {
							throw new Exception();
						}
					} else {
						throw new Exception();
					}
					
					// if there are no issues, reset fields and start the thread to calculate the Lucas Series
					lucasSumTextArea.setText("");
					cancelled = false;
					start();

				} catch (Exception e1) {
					// show error message
					JOptionPane.showMessageDialog(null, "Enter a valid number between 3 and 20.", "Warning",
							JOptionPane.ERROR_MESSAGE);
				}

			}
		});
		sumButton.setFont(new Font("PingFang TC", Font.PLAIN, 12));

		// cancel button
		cancelButton = new JButton("Cancel");
		topRowPanel.add(cancelButton);
		cancelButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				stop();
			}
		});
		cancelButton.setFont(new Font("PingFang TC", Font.PLAIN, 12));

		// make components for the bottom row of the panel (progress bar & sum label)
		BottomRowPanel = new JPanel();
		lucasSumFrame.getContentPane().add(BottomRowPanel, BorderLayout.SOUTH);
		GridBagLayout gbl_BottomRowPanel = new GridBagLayout();
		gbl_BottomRowPanel.columnWidths = new int[] { 450, 50 };
		gbl_BottomRowPanel.rowHeights = new int[] { 23, 0 };
		gbl_BottomRowPanel.columnWeights = new double[] { 0.0, 0.0, 0.0, Double.MIN_VALUE };
		gbl_BottomRowPanel.rowWeights = new double[] { 0.0, Double.MIN_VALUE };
		BottomRowPanel.setLayout(gbl_BottomRowPanel);

		progressBar = new JProgressBar();
		progressBar.setStringPainted(true);
		progressBar.setFont(new Font("PingFang TC", Font.PLAIN, 12));
		progressBar.setBackground(new Color(60, 179, 113));
		progressBar.setMinimum(0);
		progressBar.setMaximum(100);
		GridBagConstraints gbc_progressBar = new GridBagConstraints();
		gbc_progressBar.anchor = GridBagConstraints.NORTH;
		gbc_progressBar.fill = GridBagConstraints.HORIZONTAL;
		gbc_progressBar.insets = new Insets(0, 0, 0, 5);
		gbc_progressBar.gridx = 0;
		gbc_progressBar.gridy = 0;
		BottomRowPanel.add(progressBar, gbc_progressBar);

		sumLabel = new JLabel("Sum = ");
		GridBagConstraints gbc_sumLabel = new GridBagConstraints();
		gbc_sumLabel.anchor = GridBagConstraints.NORTHWEST;
		gbc_sumLabel.gridx = 2;
		gbc_sumLabel.gridy = 0;
		BottomRowPanel.add(sumLabel, gbc_sumLabel);
	}

	// a function which recursively calculates the Lucas Series
	int calcLucasSeries(int num) {
		if (num == 0) {
			return 2;
		} else if (num == 1) {
			return 1;
		} else {
			return calcLucasSeries(num - 1) + calcLucasSeries(num - 2);
		}
	}

	// this function creates a new thread
	// thread result is the sum of the lucas series
	// the GUI is updates integer value on the GUI
	private void start() {
		SwingWorker<Integer, Integer> lucasWorker = new SwingWorker<Integer, Integer>() {
			// calculate the lucas series
			@Override
			protected Integer doInBackground() throws Exception {
				cancelButton.setEnabled(true);
				sumButton.setEnabled(false);
				int currentVal = 0;
				int lucasSeriesSum = 0;

				for (int i = 0; i < num; i++) {
					// if the user presses the cancel button, stop the process
					if (cancelled) {
						break;
					}

					Thread.sleep(100); // update every 0.1 second
					currentVal = calcLucasSeries(i);
					lucasSeriesSum += currentVal;

					publish(currentVal);
					publish(lucasSeriesSum);
					setProgress((i + 1) * 100 / num);
				}

				return lucasSeriesSum;
			}

			// update GUI during the background task
			protected void process(List<Integer> published) {
				// current value that will be added to the string and current sum of the lucas
				// series
				Integer currentVal = published.get(published.size() - 2);
				Integer sum = published.get(published.size() - 1);

				// update the GUI
				lucasSumTextArea.append(currentVal + "\n");
				sumLabel.setText("Sum = " + Integer.toString(sum));
			}

			// when the calculation is done, write the lucas series to a text file called
			// "LucasSeries.txt"
			// Reset the sum text area, enable the calculate sum button, and disable the
			// cancel button
			protected void done() {
				cancelButton.setEnabled(false);
				sumButton.setEnabled(true);

				try {
					// write to output
					FileOutputStream fileObject = new FileOutputStream("LucasSeries.txt", false);
					PrintWriter x = new PrintWriter(fileObject);

					String[] lucasProgress = lucasSumTextArea.getText().split("\r?\n");

					// printing output in [a, b, c, d, e] form
					x.print("[");
					for (int i = 0; i < lucasProgress.length - 1; i++) {
						x.print(lucasProgress[i] + ", ");
					}
					x.println(lucasProgress[lucasProgress.length - 1] + "]");

					x.close();
					fileObject.close();

					// show a message that the lucas series has been saved to a file
					//JOptionPane.showMessageDialog(null, "Lucas Series saves to LucasSeries.txt", "Done",
					//		JOptionPane.INFORMATION_MESSAGE);

				} catch (Exception e) {
					JOptionPane.showMessageDialog(null, "Error while writing output file", "Error",
							JOptionPane.ERROR_MESSAGE);
				}
			}

		};
		
		lucasWorker.addPropertyChangeListener(new PropertyChangeListener() {
			public void propertyChange(PropertyChangeEvent e) {
				// if there was a change, update the progress bar
				if ("progress".equals(e.getPropertyName())) {
					progressBar.setValue((Integer) e.getNewValue());
				}
			}
		});
		
		lucasWorker.execute();
	}

	// if the worker presses the cancel button, cancelled is set as false
	private void stop() {
		cancelled = true;
	}

}

package midterm_proj;

import java.awt.EventQueue;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JButton;
import javax.swing.ImageIcon;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.awt.event.ActionEvent;
import javax.swing.JLabel;
import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.Font;
import javax.swing.JTextArea;

public class TripPlanner extends JFrame {
	//the following are elements in the GUI
	private static final long serialVersionUID = 1L;
	private JLabel PlannerLogoLabel;
	private JLabel tripPlannerLabel;
	private JTextArea userText;
	private JPanel transportPanel;
	private JButton taxiButton;
	private JButton trainButton;
	private JButton busButton;
	private JPanel yesNoPanel;
	private JButton yesButton;
	private JButton noButton;
	private JPanel controlPanel;
	private JButton cancelButton;
	private JButton clearButton;
	private JButton enterButton;
	private JPanel numberPanel;
	private JButton button1;
	private JButton button2;
	private JButton button3;
	private JButton button4;
	private JButton button5;
	private JButton button6;
	private JButton button7;
	private JButton button8;
	private JButton button9;
	private JButton button0;
	
	// the ArrayList below keeps track of the transportations being taken
	private ArrayList<PublicTransport> transportList = new ArrayList<PublicTransport>(); 
	private String currTransport; // tracks what the current transport is at the moment
	private int numTransport = 0; // counts total number of transport taken
	private int state; // keeps track of which question is being prompted (base fare = 0,
						// fare per station/km = 1, and number of stations/distance = 2)

	// Launch the application.
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					TripPlanner frame = new TripPlanner();
					frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	// Create the frame.
	public TripPlanner() {

		setTitle("Trip Planner");
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setBounds(100, 100, 578, 627);
		getContentPane().setLayout(null);

		// creating the "TRIP PLANNER" Label
		tripPlannerLabel = new JLabel("TRIP PLANNER");
		tripPlannerLabel.setBounds(231, 116, 107, 16);
		getContentPane().add(tripPlannerLabel);

		// adding the planner logo image at the top
		PlannerLogoLabel = new JLabel("planner logo");
		PlannerLogoLabel.setBounds(90, 20, 400, 84);
		getContentPane().add(PlannerLogoLabel);
		PlannerLogoLabel.setIcon(new ImageIcon(TripPlanner.class.getResource("/assets/planner_logo.png")));

		// creating the text area to take user input and print the prompts
		userText = new JTextArea();
		userText.setLineWrap(true);
		userText.setText("Choose transport (from left menu):");
		userText.setBounds(136, 135, 298, 186);
		getContentPane().add(userText);

		// the transport panel contains the train, bus, and taxi buttons
		transportPanel = new JPanel();
		transportPanel.setBounds(26, 135, 107, 95);
		getContentPane().add(transportPanel);
		transportPanel.setLayout(null);

		// creating the train button
		trainButton = new JButton("TRAIN");
		trainButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				state = 0;
				currTransport = "train";
				userText.setText("You choose TRAIN\nEnter base fare: ");
			}
		});
		trainButton.setForeground(new Color(0, 0, 0));
		trainButton.setBounds(6, 0, 100, 27);
		trainButton.setIcon(new ImageIcon(TripPlanner.class.getResource("/assets/train.png")));
		transportPanel.add(trainButton);

		// creating the bus button
		busButton = new JButton("BUS");
		busButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				state = 0;
				currTransport = "bus";
				userText.setText("You choose BUS\nEnter base fare: ");
			}
		});
		busButton.setBounds(6, 27, 100, 30);
		busButton.setIcon(new ImageIcon(TripPlanner.class.getResource("/assets/bus.png")));
		transportPanel.add(busButton);

		// creating the taxi button
		taxiButton = new JButton("TAXI");
		taxiButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				state = 0;
				currTransport = "taxi";
				userText.setText("You choose TAXI\nEnter base fare: ");
			}
		});
		taxiButton.setBounds(6, 57, 100, 26);
		taxiButton.setIcon(new ImageIcon(TripPlanner.class.getResource("/assets/taxi.png")));
		transportPanel.add(taxiButton);

		// this panel contains the Yes and No buttons on the right
		yesNoPanel = new JPanel();
		yesNoPanel.setBounds(429, 133, 133, 84);
		getContentPane().add(yesNoPanel);
		yesNoPanel.setLayout(null);

		// creating the yes button
		yesButton = new JButton("Yes");
		yesButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				// re-prompt to add more transportation methods
				userText.setText("Choose transport (from left menu):");
			}
		});
		yesButton.setBounds(6, 0, 121, 34);
		yesButton.setIcon(new ImageIcon(TripPlanner.class.getResource("/assets/enterSmall.png")));
		yesNoPanel.add(yesButton);

		// creating the no button
		noButton = new JButton("No");
		noButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				userText.setText(""); // clear out the text area
				double totalfare = 0;

				// loop through the transport list and print each transportation and its total fare
				for (int i = 0; i < numTransport; i++) {
					int num = i + 1;
					if (transportList.get(i) instanceof Train) {
						userText.append("Transport " + num + ": ");
						userText.append(transportList.get(i).getModel() + "\n");
						transportList.get(i).calculatePayment();
						userText.append("Fare: " + String.format("%.2f", transportList.get(i).getTotalFare()) + "\n");
						totalfare += transportList.get(i).getTotalFare();
					} else if (transportList.get(i) instanceof Bus) {
						userText.append("Transport " + num + ": ");
						userText.append(transportList.get(i).getModel() + "\n");
						transportList.get(i).calculatePayment();
						userText.append("Fare: " + String.format("%.2f", transportList.get(i).getTotalFare()) + "\n");
						totalfare += transportList.get(i).getTotalFare();
					} else if (transportList.get(i) instanceof Taxi) {
						userText.append("Transport " + num + ": ");
						userText.append(transportList.get(i).getModel() + "\n");
						transportList.get(i).calculatePayment();
						userText.append("Fare: " + String.format("%.2f", transportList.get(i).getTotalFare()) + "\n");
						totalfare += transportList.get(i).getTotalFare();
					}

				}
				// print out total fare
				userText.append("=======================\n");
				userText.append("Total Trip Fare: " + String.format("%.2f", totalfare));
				
				numTransport = 0; // reduce the number of transport to 0
				transportList.clear(); // clear out the transportation list
			}
		});
		noButton.setBounds(6, 27, 121, 34);
		noButton.setIcon(new ImageIcon(TripPlanner.class.getResource("/assets/cancelSmall.png")));
		yesNoPanel.add(noButton);

		// this panel contains the cancel, clean, and enter buttons
		controlPanel = new JPanel();
		controlPanel.setBounds(375, 322, 153, 243);
		getContentPane().add(controlPanel);
		controlPanel.setLayout(null);

		// creating the cancel button
		cancelButton = new JButton("CANCEL");
		cancelButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				userText.setText("Cancelled!\nNew plan:\nChoose transport (from left menu): ");
				numTransport = 0; // reduce the number of transport to 0
				transportList.clear(); // clear out the transportation list
			}
		});
		cancelButton.setFont(new Font("Lucida Grande", Font.PLAIN, 14));
		cancelButton.setBounds(6, 0, 129, 66);
		cancelButton.setIcon(new ImageIcon(TripPlanner.class.getResource("/assets/cancel.png")));
		controlPanel.add(cancelButton);

		// creating the clear button
		clearButton = new JButton("CLEAR");
		clearButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				// clear out the text by resetting the text depending on the state
				if (state == 0) {
					if (currTransport.equals("train")) {
						userText.setText("You choose TRAIN\nEnter base fare: ");
					} else if (currTransport.equals("bus")) {
						userText.setText("You choose BUS\nEnter base fare: ");
					} else if (currTransport.equals("taxi")) {
						userText.setText("You choose TAXI\nEnter base fare: ");
					}

				} else if (state == 1) {
					if (currTransport.equals("train") || currTransport.equals("bus")) {
						userText.setText("Enter fare per station (for extra stations): ");
					} else if (currTransport.equals("taxi")) {
						userText.setText("Enter fare per km: ");
					}
				} else if (state == 2) {
					if (currTransport.equals("train") || currTransport.equals("bus")) {
						userText.setText("Enter number of stations: ");
					} else if (currTransport.equals("taxi")) {
						userText.setText("Enter distance (in km): ");
					}
				}
			}
		});
		clearButton.setFont(new Font("Lucida Grande", Font.PLAIN, 15));
		clearButton.setBounds(6, 63, 129, 66);
		clearButton.setIcon(new ImageIcon(TripPlanner.class.getResource("/assets/clear.png")));
		controlPanel.add(clearButton);

		// creating the enter button
		enterButton = new JButton("ENTER");
		enterButton.addActionListener(new ActionListener() {
			// variables to store the necessary information before creating the transportation objects
			double basefare = 0;
			double extrafare = 0;
			int numStations = 0;
			double distance = 0;

			public void actionPerformed(ActionEvent e) {

				if (state == 0) { // set base fare and prompt for fare per station or km
					basefare = Double.parseDouble(userText.getText().replaceAll("[^0-9]", ""));

					state++;

					if (currTransport.equals("train") || currTransport.equals("bus")) {
						userText.setText("Enter fare per station (for extra stations): ");
					} else if (currTransport.equals("taxi")) {
						userText.setText("Enter fare per km: ");
					}
				} else if (state == 1) { // set fare per station or km and prompt for number of stations or distance
					if (currTransport.equals("train") || currTransport.equals("bus")) {
						extrafare = Double.parseDouble(userText.getText().replaceAll("[^0-9]", ""));
						userText.setText("Enter number of stations: ");
					} else if (currTransport.equals("taxi")) {
						extrafare = Double.parseDouble(userText.getText().replaceAll("[^0-9]", ""));
						userText.setText("Enter distance (in km): ");
					}

					state++;
				} else if (state == 2) { // set number of stations or distance
					if (currTransport.equals("train") || currTransport.equals("bus")) {
						numStations = Integer.parseInt(userText.getText().replaceAll("[^0-9]", ""));
					} else if (currTransport.equals("taxi")) {
						distance = Double.parseDouble(userText.getText().replaceAll("[^0-9]", ""));
					}

					// if the transport is completed without cancelling, add it to the list of
					// transportations
					if (currTransport.equals("train")) {
						transportList.add(new Train(basefare, extrafare, numStations));
						numTransport++; // increase the number of transportations
					} else if (currTransport.equals("bus")) {
						transportList.add(new Bus(basefare, extrafare, numStations));
						numTransport++; // increase the number of transportations
					} else if (currTransport.equals("taxi")) {
						transportList.add(new Taxi(basefare, extrafare, distance));
						numTransport++; // increase the number of transportations
					}

					state++;
					userText.setText("Add more transport (from right menu)?"); // prompt for more transit
				}
			}
		});
		enterButton.setFont(new Font("Lucida Grande", Font.PLAIN, 15));
		enterButton.setBounds(6, 126, 129, 66);
		enterButton.setIcon(new ImageIcon(TripPlanner.class.getResource("/assets/enter.png")));
		controlPanel.add(enterButton);

		// this panel contains the number buttons from 0 to 9
		numberPanel = new JPanel();
		FlowLayout flowLayout = (FlowLayout) numberPanel.getLayout();
		flowLayout.setHgap(0);
		flowLayout.setVgap(-2);
		numberPanel.setBounds(146, 323, 267, 259);
		getContentPane().add(numberPanel);

		// creating buttons for numbers 0 through 9
		// pressing each button will add their number on the screen
		// button 1
		button1 = new JButton("");
		button1.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				userText.append("1");
			}
		});
		button1.setIcon(new ImageIcon(TripPlanner.class.getResource("/assets/1.png")));
		numberPanel.add(button1);

		// button2
		button2 = new JButton("");
		button2.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				userText.append("2");
			}
		});
		button2.setIcon(new ImageIcon(TripPlanner.class.getResource("/assets/2.png")));
		numberPanel.add(button2);

		// button3
		button3 = new JButton("");
		button3.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				userText.append("3");
			}
		});
		button3.setIcon(new ImageIcon(TripPlanner.class.getResource("/assets/3.png")));
		numberPanel.add(button3);

		// button 4
		button4 = new JButton("");
		button4.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				userText.append("4");
			}
		});
		button4.setIcon(new ImageIcon(TripPlanner.class.getResource("/assets/4.png")));
		numberPanel.add(button4);

		// button 5
		button5 = new JButton("");
		button5.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				userText.append("5");
			}
		});
		button5.setIcon(new ImageIcon(TripPlanner.class.getResource("/assets/5.png")));
		numberPanel.add(button5);

		// button 6
		button6 = new JButton("");
		button6.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				userText.append("6");
			}
		});
		button6.setIcon(new ImageIcon(TripPlanner.class.getResource("/assets/6.png")));
		numberPanel.add(button6);

		// button 7
		button7 = new JButton("");
		button7.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				userText.append("7");
			}
		});
		button7.setIcon(new ImageIcon(TripPlanner.class.getResource("/assets/7.png")));
		numberPanel.add(button7);

		// button 8
		button8 = new JButton("");
		button8.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				userText.append("8");
			}
		});
		button8.setIcon(new ImageIcon(TripPlanner.class.getResource("/assets/8.png")));
		numberPanel.add(button8);

		// button 9
		button9 = new JButton("");
		button9.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				userText.append("9");
			}
		});
		button9.setIcon(new ImageIcon(TripPlanner.class.getResource("/assets/9.png")));
		numberPanel.add(button9);

		// button 0
		button0 = new JButton("");
		button0.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				userText.append("0");
			}
		});
		button0.setIcon(new ImageIcon(TripPlanner.class.getResource("/assets/0.png")));
		numberPanel.add(button0);
	}
}
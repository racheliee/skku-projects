package midterm_proj;

public abstract class PublicTransport implements Payable {
	protected String model;
	protected double baseFare;
	protected double totalFare;

	// constructor
	public PublicTransport(double baseFare, String model) {
		setBaseFare(baseFare);
		setModel(model);
	}

	// gets the model
	public String getModel() {
		return model;
	}

	// gets the base fare
	public double getBaseFare() {
		return baseFare;
	}

	// gets the total fare
	public double getTotalFare() {
		return totalFare;
	}

	// sets the model
	public void setModel(String model) {
		this.model = model;
	}

	// sets the base fare
	public void setBaseFare(double baseFare) {
		this.baseFare = baseFare;
	}

	// sets the total fare
	public void setTotalFare(double totalFare) {
		this.totalFare = totalFare;
	}

	// calculates the payment (implemented in child classes)
	@Override
	public abstract void calculatePayment();

}

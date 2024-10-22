package midterm_proj;

public class Taxi extends PublicTransport {
	protected double farePerKm;
	protected double distance;

	// constructor
	public Taxi(double basefare, double farePerKm, double distance) {
		super(basefare, "KAKAO TAXI");
		setFarePerKm(farePerKm);
		setDistance(distance);
	}

	// gets the fare per km
	public double getFarePerKm() {
		return farePerKm;
	}

	// gets the distance travelled
	public double getDistance() {
		return distance;
	}

	// sets the fare per km
	public void setFarePerKm(double farePerKm) {
		this.farePerKm = farePerKm;
	}

	// sets the distance travelled
	public void setDistance(double distance) {
		this.distance = distance;
	}

	// calculates the total payment that needs to be made by summing the base fare
	// and the product of distance and farePerKm
	@Override
	public void calculatePayment() {
		super.setTotalFare(super.getBaseFare() + distance * farePerKm);
	}

}

package midterm_proj;

public class Train extends PublicTransport {
	public Station stations;

	// constructor
	public Train(double basefare, double farePerStation, int numStations) {
		super(basefare, "KORAIL");
		stations = new Station(farePerStation, numStations);
	}

	// get the stations variable
	public Station getStation() {
		return stations;
	}

	// sets stations variable
	public void setStation(Station s) {
		stations = s;
	}

	// calculates the total payment that needs to be made
	// if less than 5 stations the total fare is the base fare
	// if there are more than 5, additional cost is added
	@Override
	public void calculatePayment() {
		if (stations.getNStations() < 5) {
			super.setTotalFare(baseFare);
		} else {
			super.setTotalFare(baseFare + (stations.getNStations() - 5) * stations.getFarePerStation());
		}
	}
}

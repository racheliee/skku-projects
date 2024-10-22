package midterm_proj;

public class Station {
	public double farePerStation;
	public int nStations;

	// constructor
	public Station(double farePerStation, int nStations) {
		this.farePerStation = farePerStation;
		this.nStations = nStations;
	}

	// gets the fare per station
	public double getFarePerStation() {
		return farePerStation;
	}

	// gets the number of stations
	public int getNStations() {
		return nStations;
	}

	// sets the fare per stations
	public void setFarePerStation(double farePerStation) {
		this.farePerStation = farePerStation;
	}

	// sets the number of stations
	public void setNStations(int nStations) {
		this.nStations = nStations;
	}
}

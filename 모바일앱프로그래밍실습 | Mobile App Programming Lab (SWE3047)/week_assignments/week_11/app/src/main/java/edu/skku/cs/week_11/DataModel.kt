package edu.skku.cs.week_11

data class DataModel (var location:LocationState, var current:Weather) {
    data class Weather(
        var temp_c: Double? = null,
        var wind_mph: Double? = null,
        var condition: Condition)
    data class Condition(var text: String ?= null)
    data class LocationState(var lat: Double? = null, var lon: Double? = null)
}
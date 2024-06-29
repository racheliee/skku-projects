package edu.skku.cs.week_10

//data class Register(var email: String ?= null, var password: String ?= null)
//data class RegisterResponse(var id: String ?= null, var createdAt : String ?= null)

data class Show(val show: Showinfo?){
    data class Showinfo(val id: Int?)
}

data class Episode(val id: Int?, val name: String?, val season: Int?, val number: Int?, val airdate: String?)


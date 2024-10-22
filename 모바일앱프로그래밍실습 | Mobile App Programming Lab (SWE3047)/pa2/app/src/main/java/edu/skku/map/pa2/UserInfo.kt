package edu.skku.map.pa2

import java.io.Serializable


data class UserInfo(
    val id: String,
    val passwd: String,
    val info: Info,
    val reserved: ArrayList<Reserved>
) : Serializable

data class Info(
    val name: String,
    val age: Int,
    val gender: String
) : Serializable

data class Reserved(
    val reservation_id: Int,
    val restaurant_id: Int,
    val number_of_people: Int,
    val date: String,
    val time: String
) : Serializable

/* json file format
[
    {
      "id": "skku",
      "passwd": "1234",
      "info":
      {
        "name": "James",
        "age": 25,
        "gender": "male"
      },
      "reserved": [
        {
            "reservation_id": 1,
            "restaurant_id" : 1,
            "number_of_people" : 4,
            "date" : "2024-06-01",
            "time" : "18:00"
        }
      ]
    },
    {
      "id": "agent1",
      "passwd": "asdf",
      "info":
      {
        "name": "Catherine",
        "age": 20,
        "gender": "female"
      },
      "reserved": [
      ]
    }
]

 */
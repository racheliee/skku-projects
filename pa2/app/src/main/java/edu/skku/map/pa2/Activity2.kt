package edu.skku.map.pa2

import android.content.Intent
import android.os.Bundle
import android.widget.ListView
import androidx.appcompat.app.AppCompatActivity
import com.google.gson.Gson
import com.google.gson.reflect.TypeToken
import java.io.BufferedReader
import java.io.InputStreamReader

class Activity2 : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_2)

        // read the restaurant_info.txt
        val restaurantList = readRestaurantInfo()

        // get list view
        val listView = findViewById<ListView>(R.id.a2_listView)
        val adapter = RestaurantAdapter(restaurantList, this)
        listView.adapter = adapter

        // move to activity 3 if the item is clicked
        listView.setOnItemClickListener { _, _, position, _ ->
            val restaurant = restaurantList[position]
            val intent = Intent(this, Activity3::class.java).apply{
                putExtra("CLIKED_RESTAURANT", restaurant)
            }
            startActivity(intent)
        }

    }

    override fun onBackPressed() {
        super.onBackPressed()
        finish()
    }

    private fun readRestaurantInfo(): ArrayList<RestaurantInfo> {
        val inputStream = assets.open("restaurant_info.txt")
        val reader = BufferedReader(InputStreamReader(inputStream))
        val jsonText = reader.use { it.readText() }

        val gson = Gson()
        val restaurantInfoType = object : TypeToken<List<RestaurantInfo>>() {}.type
        return gson.fromJson(jsonText, restaurantInfoType)
    }
}
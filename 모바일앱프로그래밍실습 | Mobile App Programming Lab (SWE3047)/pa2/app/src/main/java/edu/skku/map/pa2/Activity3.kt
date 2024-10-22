package edu.skku.map.pa2

import android.content.Intent
import android.content.res.Resources
import android.os.Bundle
import android.widget.ImageView
import android.widget.ListView
import android.widget.TextView
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat

class Activity3 : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_3)

        // get restaurant
        val restaurant = intent.getSerializableExtra("CLIKED_RESTAURANT") as? RestaurantInfo

        restaurant?.let{
            // set thumbnail image
            val imageView = findViewById<ImageView>(R.id.a3_imageView)
            val resourceId = if (restaurant.image.isNotEmpty() && resources.getIdentifier(restaurant.image, "drawable", packageName) != 0) {
                resources.getIdentifier(restaurant.image, "drawable", packageName)
            } else {
                R.drawable.default_img
            }
            imageView.setImageResource(resourceId)

            // set restaurant details
            val detailTextView = findViewById<TextView>(R.id.a3_detailtextView)
            val details = """
                Name: ${restaurant.restaurant}
                Type: ${restaurant.type}
                Location: ${restaurant.location}
                Rating: ${restaurant.rating}
                Description: ${restaurant.description}
                Opening Hours: ${restaurant.openingHours.open} - ${restaurant.openingHours.close}
            """.trimIndent()
            detailTextView.text = details

            // set menulist
            val menuTextView = findViewById<ListView>(R.id.a3_menuListView)
            val menuList = restaurant.menu?: listOf() // Check for null and provide a default empty list
            val adapter = MenuAdapter(menuList, this)
            menuTextView.adapter = adapter

            // reserve button
            val reserveButton = findViewById<TextView>(R.id.a3_reserveButton)
            reserveButton.setOnClickListener {
                val intent = Intent(this, Activity4::class.java).apply {
                    putExtra("restaurant", restaurant)
                }
                startActivity(intent)
            }
        }

    }

    override fun onBackPressed() {
        super.onBackPressed()
        finish()
    }
}
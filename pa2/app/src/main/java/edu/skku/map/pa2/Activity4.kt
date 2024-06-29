package edu.skku.map.pa2

import android.os.Bundle
import android.widget.Button
import android.widget.EditText
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat

class Activity4 : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_4)

        // get restaurant
        val restaurant = intent.getSerializableExtra("restaurant") as? RestaurantInfo

        val button = findViewById<Button>(R.id.a4_confirmbutton)

    }
}
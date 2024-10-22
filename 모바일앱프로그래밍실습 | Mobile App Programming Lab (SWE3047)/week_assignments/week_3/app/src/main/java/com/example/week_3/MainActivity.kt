package com.example.week_3

import android.os.Bundle
import android.widget.TextView
import android.widget.Toast
import android.widget.Button
import android.widget.ImageView
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.activity_main)
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main)) { v, insets ->
            val systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom)
            insets
        }

        var counter = 0
        val textview = findViewById<TextView>(R.id.textView)
        val imageview = findViewById<ImageView>(R.id.imageView)
        val left_btn = findViewById<Button>(R.id.button)
        val right_btn = findViewById<Button>(R.id.button2)

        textview.text = "pizza"
        imageview.setImageResource(R.drawable.pizza)

        right_btn.setOnClickListener {
            if (textview.text == "pizza") {
                textview.text = "chicken"
                imageview.setImageResource(R.drawable.chicken)
            }
            else if (textview.text == "chicken"){
                textview.text = "hamburger"
                imageview.setImageResource(R.drawable.hamburger)
            }
            else if (textview.text == "hamburger"){
                textview.text = "ramen"
                imageview.setImageResource(R.drawable.ramen)
            }
            else { //ramen
                Toast.makeText(this@MainActivity, "Last Image", Toast.LENGTH_LONG).show()
            }
        }

        left_btn.setOnClickListener{
            if (textview.text == "pizza") {
                Toast.makeText(this@MainActivity, "First Image", Toast.LENGTH_LONG).show()
            }
            else if (textview.text == "chicken"){
                textview.text = "pizza"
                imageview.setImageResource(R.drawable.pizza)
            }
            else if (textview.text == "hamburger"){
                textview.text = "chicken"
                imageview.setImageResource(R.drawable.chicken)
            }
            else { //ramen
                textview.text = "hamburger"
                imageview.setImageResource(R.drawable.hamburger)
            }
        }
    }
}
package com.example.week_6

import android.content.Intent
import android.os.Bundle
import android.widget.Button
import android.widget.EditText
import androidx.appcompat.app.ActionBar
import androidx.appcompat.app.AppCompatActivity

class MainActivity : AppCompatActivity() {
    companion object{
        const val EXT_NAME = "extra_key_name"
        const val EXT_AGE = "extra_key_age"
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

//        // calling the action bar
//        var actionBar = getSupportActionBar()
//
//        // showing the back button in action bar
//        if (actionBar != null) {
//            actionBar.setDisplayHomeAsUpEnabled(true)
//        }

        val btnNewActivity = findViewById<Button>(R.id.buttonNewActivity)

        btnNewActivity.setOnClickListener {
            val editTextName = findViewById<EditText>(R.id.editTextName)
            val editTextAge = findViewById<EditText>(R.id.editTextAge)

            val name = editTextName.text.toString()
            val age = editTextAge.text.toString().toInt()

            // create an intent to start the new activity
            val intent = Intent(this, Activity_2::class.java).apply {
                putExtra(EXT_NAME, name)
                putExtra(EXT_AGE, age)
            }
            finish() // finish the current activity
            startActivity(intent) // start the new activity
        }
    }
}
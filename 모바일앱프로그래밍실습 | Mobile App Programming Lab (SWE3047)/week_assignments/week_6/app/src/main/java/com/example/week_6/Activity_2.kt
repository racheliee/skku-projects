package com.example.week_6

import android.content.Intent
import android.os.Bundle
import android.view.View
import android.view.ViewGroup
import android.widget.BaseAdapter
import android.widget.Button
import android.widget.EditText
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity

class Activity_2: AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_2)

        val name = intent.getStringExtra(MainActivity.EXT_NAME)
        val age = intent.getIntExtra(MainActivity.EXT_AGE, -1)

        val question = findViewById<TextView>(R.id.textViewQuestion)

        question.setText("You typed ${name} and age ${age}, is that right?")

        //yes button
        val yesButtonActivity = findViewById<Button>(R.id.buttonYes)
        yesButtonActivity.setOnClickListener() {
            val intent = Intent(this, Activity_3::class.java).apply {
                putExtra(MainActivity.EXT_NAME, name)
                putExtra(MainActivity.EXT_AGE, age)
            }
            startActivity(intent) // if yes, go to the next activity (Activity_3)
        }

        // no button
        val noButtonActivity = findViewById<Button>(R.id.buttonNo)
        noButtonActivity.setOnClickListener(){
            val intent = Intent(this, MainActivity::class.java)
            finish() // if no, finish the current activity
            startActivity(intent) // go back to the previous activity (main)
        }
    }
}
package edu.skku.cs.week_7

import android.content.Intent
import android.os.Bundle
import android.provider.AlarmClock
import android.widget.Button
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity

class Activity2: AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_2)

        // get the data passed from MainActivity
        val hour = intent.getIntExtra("hour", -1)
        val minute = intent.getIntExtra("minute", -1)
        val desc = intent.getStringExtra("desc")

        val question = findViewById<TextView>(R.id.textViewQuestion)
        question.text = "Do you want to set an alarm for ${hour}:${minute} with the description '${desc}'?"

        val okButton = findViewById<Button>(R.id.buttonOk)
        val cancelButton = findViewById<Button>(R.id.buttonCancel)

        okButton.setOnClickListener(){
            // create an intent to set an alarm (implicit intent)
            val alarmIntent = Intent(AlarmClock.ACTION_SET_ALARM).apply {
                putExtra(AlarmClock.EXTRA_HOUR, hour)
                putExtra(AlarmClock.EXTRA_MINUTES, minute)
                putExtra(AlarmClock.EXTRA_MESSAGE, desc)
            }

            // check if there is an app that can handle the intent
            if (alarmIntent.resolveActivity(packageManager) != null) {
                finish() // close Activity2
                startActivity(alarmIntent) // start the alarm app
            }
        }

        cancelButton.setOnClickListener(){
            val intent = Intent(this, MainActivity::class.java)
            finish() // close Activity2
            startActivity(intent) // start MainActivity
        }
    }
}
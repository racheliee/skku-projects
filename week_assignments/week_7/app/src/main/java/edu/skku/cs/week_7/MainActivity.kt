package edu.skku.cs.week_7

import android.content.Intent
import android.net.Uri
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.Button
import android.widget.EditText

class MainActivity : AppCompatActivity() {
    // used for passing data between activities
    companion object{
        const val EXT_HOUR = "hour"
        const val EXT_MINUTE = "minute"
        const val EXT_DESC = "desc"
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val time = findViewById<EditText>(R.id.editTextTime)
        val description = findViewById<EditText>(R.id.editTextDescription)

        val btn = findViewById<Button>(R.id.button)
        btn.setOnClickListener {
            val hour = time.text.toString().split(":")[0].toInt()
            val minute = time.text.toString().split(":")[1].toInt()
            val desc = description.text.toString()

            // create an intent to start Activity2 (explicit intent)
            val intent = Intent(this, Activity2::class.java).apply{
                // pass the data to Activity2
                putExtra(EXT_HOUR, hour)
                putExtra(EXT_MINUTE, minute)
                putExtra(EXT_DESC, desc)
            }

            // start Activity2
            startActivity(intent)
        }

//        val btnStart = findViewById<Button>(R.id.buttonBrowse)
//        btnStart.setOnClickListener {
//            val urlEditText = findViewById<EditText>(R.id.editTextUrl)
//            val uri = Uri.parse("https://" + urlEditText.text.toString())
//            val webIntent = Intent(Intent.ACTION_VIEW, uri)
//            startActivity(webIntent)
//        }
    }
}
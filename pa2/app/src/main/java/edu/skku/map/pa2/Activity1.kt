package edu.skku.map.pa2

import android.content.Intent
import android.os.Bundle
import android.widget.Button
import android.widget.ImageView
import android.widget.ListView
import android.widget.TextView
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat

class Activity1 : AppCompatActivity() {
    companion object {
        const val USER_INFO = "USER_INFO"
    }
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_1)

        // get the user info from the intent
        val user = intent.getSerializableExtra(USER_INFO) as UserInfo?

        // set image
        val a1_image = findViewById<ImageView>(R.id.a1imageView)
        a1_image.setImageResource((R.drawable.title_figure))

        // format in ID: Name (Age/Gender)
        val userinfo_text = findViewById<TextView>(R.id.a1_userInfoTextView)
        userinfo_text.text = "${user?.id}: ${user?.info?.name} (${user?.info?.age}/${user?.info?.gender})"

        // enter the reservations in the listView
        val listView = findViewById<ListView>(R.id.a1_reservationListView)
        val adapter = ReservationAdapter(user?.reserved?: arrayListOf(), this)
        listView.adapter = adapter

        // if reservation button pressed, move to activity 2
        val reservation_button = findViewById<Button>(R.id.a1_reservationButton)
        reservation_button.setOnClickListener {
            val intent = Intent(this, Activity2::class.java)
            // finish?
            startActivity(intent)
        }

    }

    override fun onBackPressed() {
        super.onBackPressed()
        finish()
    }
}
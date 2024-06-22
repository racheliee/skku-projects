package edu.skku.map.pa3

import android.os.Build
import android.os.Bundle
import android.view.View
import android.view.WindowInsets
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import edu.skku.map.pa3.models.UserInfo

class MainPageActivity : AppCompatActivity() {
    companion object{
        // const val USER_INFO = "USER_INFO"
        const val SESSION_ID = "SESSION_ID"
    }

    private lateinit var sessionID: String

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.mainpage_layout)

        // Hide the status bar
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            window.setDecorFitsSystemWindows(false)
            window.insetsController!!.hide(WindowInsets.Type.statusBars())
        } else {
            window.decorView.systemUiVisibility = View.SYSTEM_UI_FLAG_FULLSCREEN
        }

        // get the session ID
        sessionID = intent.getStringExtra(SESSION_ID) ?: ""
        Toast.makeText(this, "Session ID: $sessionID", Toast.LENGTH_SHORT).show()






    }
}
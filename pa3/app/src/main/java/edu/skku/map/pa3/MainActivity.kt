package edu.skku.map.pa3

import android.os.Build
import android.os.Bundle
import android.view.View
import android.view.WindowInsets
import android.widget.ImageButton
import androidx.appcompat.app.AppCompatActivity
import androidx.fragment.app.Fragment
import edu.skku.map.pa3.fragments.FavouriteFragment
import edu.skku.map.pa3.fragments.HomeFragment
import edu.skku.map.pa3.fragments.ProfileFragment
import edu.skku.map.pa3.fragments.SearchFragment
import edu.skku.map.pa3.models.*

class MainActivity : AppCompatActivity() {
    companion object {
        const val SESSION_ID = "SESSION_ID"
        const val ACCOUNT_DETAILS = "ACCOUNT_DETAILS"
    }

    private lateinit var sessionId: String
    private lateinit var accountDetails: AccountDetail

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // Hide the status bar
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            window.setDecorFitsSystemWindows(false)
            window.insetsController!!.hide(WindowInsets.Type.statusBars())
        } else {
            window.decorView.systemUiVisibility = View.SYSTEM_UI_FLAG_FULLSCREEN
        }

        // Get the session ID and account details
        sessionId = intent.getStringExtra(SESSION_ID)!!
        accountDetails = intent.getSerializableExtra(ACCOUNT_DETAILS) as AccountDetail

        // Set default fragment
        loadFragment(HomeFragment())

        // Set click listeners for buttons
        findViewById<ImageButton>(R.id.home_button).setOnClickListener {
            loadFragment(HomeFragment())
        }

        findViewById<ImageButton>(R.id.profile_button).setOnClickListener {
            loadFragment(ProfileFragment.newInstance(accountDetails))
        }

        findViewById<ImageButton>(R.id.search_button).setOnClickListener {
            loadFragment(SearchFragment())
        }

        findViewById<ImageButton>(R.id.favourite_button).setOnClickListener {
            loadFragment(FavouriteFragment.newInstance(sessionId, accountDetails.id))
        }
    }

    private fun loadFragment(fragment: Fragment) {
        supportFragmentManager.beginTransaction()
            .replace(R.id.fragment_container, fragment)
            .commit()
    }

    override fun onResume() {
        super.onResume()
        hideStatusBar()
    }

    private fun hideStatusBar() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            window.setDecorFitsSystemWindows(false)
            window.insetsController?.hide(WindowInsets.Type.statusBars())
        } else {
            window.decorView.systemUiVisibility = View.SYSTEM_UI_FLAG_FULLSCREEN
        }
    }
}

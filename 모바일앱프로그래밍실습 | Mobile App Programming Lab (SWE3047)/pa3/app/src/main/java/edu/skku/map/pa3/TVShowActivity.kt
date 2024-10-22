package edu.skku.map.pa3

import android.os.Build
import android.os.Bundle
import android.view.View
import android.view.WindowInsets
import android.widget.Button
import android.widget.ImageView
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.bumptech.glide.Glide
import edu.skku.map.pa3.models.*
import edu.skku.map.pa3.network.AccountMediaNetworkUtils

class TVShowActivity : AppCompatActivity() {

    private lateinit var tvShow: TVShow
    private lateinit var watchlistButton: Button
    private lateinit var favoriteButton: Button
    val sessionId = GlobalData.sessionId
    val accountId = GlobalData.accountDetails!!.id

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_tvshow)

        // Initialize views
        val tvShowTitle = findViewById<TextView>(R.id.activity_tvshow_title)
        val tvShowOverview = findViewById<TextView>(R.id.activity_tvshow_overview)
        val tvShowPoster = findViewById<ImageView>(R.id.activity_tvshow_poster)
        watchlistButton = findViewById(R.id.tvshow_watchlist_button)
        favoriteButton = findViewById(R.id.tvshow_favorite_button)

        // Get TV show data from intent
        tvShow = intent.getSerializableExtra("tvShow") as TVShow

        // Set TV show data to views
        tvShowTitle.text = tvShow.name
        tvShowOverview.text = tvShow.overview
        Glide.with(this)
            .load("https://image.tmdb.org/t/p/w500${tvShow.poster_path}")
            .into(tvShowPoster)

        // Set up button click listeners
        watchlistButton.setOnClickListener {
            addToWatchlist()
        }

        favoriteButton.setOnClickListener {
            addToFavorite()
        }
    }

    private fun addToWatchlist() {
        if (sessionId != null) {
            AccountMediaNetworkUtils.addToWatchlist(tvShow.id, "tv", sessionId,
                accountId.toString()
            ) { success, e ->
                runOnUiThread {
                    if (success) {
                        Toast.makeText(this, "${tvShow.name} added to Watchlist", Toast.LENGTH_SHORT).show()
                    } else {
                        e?.printStackTrace()
                        Toast.makeText(this, "Failed to add ${tvShow.name} to Watchlist", Toast.LENGTH_SHORT).show()
                    }
                }
            }
        }
    }

    private fun addToFavorite() {
        if (sessionId != null) {
            AccountMediaNetworkUtils.addToFavorites(tvShow.id, "tv", sessionId,
                accountId.toString()
            ) { success, e ->
                runOnUiThread {
                    if (success) {
                        Toast.makeText(this, "${tvShow.name} added to Favorites", Toast.LENGTH_SHORT).show()
                    } else {
                        e?.printStackTrace()
                        Toast.makeText(this, "Failed to add ${tvShow.name} to Favorites", Toast.LENGTH_SHORT).show()
                    }
                }
            }
        }
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

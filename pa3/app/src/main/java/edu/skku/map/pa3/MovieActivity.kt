package edu.skku.map.pa3

import android.os.Build
import android.os.Bundle
import android.view.View
import android.view.WindowInsets
import android.widget.Button
import android.widget.ImageView
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import com.bumptech.glide.Glide
import edu.skku.map.pa3.models.Movie

class MovieActivity : AppCompatActivity() {

    private lateinit var movie: Movie
    private lateinit var watchlistButton: Button
    private lateinit var favoriteButton: Button

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_movie)

        // Initialize views
        val movieTitle = findViewById<TextView>(R.id.activity_movie_title)
        val movieOverview = findViewById<TextView>(R.id.activity_movie_overview)
        val moviePoster = findViewById<ImageView>(R.id.activity_movie_poster)
        watchlistButton = findViewById(R.id.watchlist_button)
        favoriteButton = findViewById(R.id.favorite_button)

        // Get movie data from intent
        movie = intent.getSerializableExtra("movie") as Movie

        // Set movie data to views
        movieTitle.text = movie.title
        movieOverview.text = movie.overview
        Glide.with(this)
            .load("https://image.tmdb.org/t/p/w500${movie.poster_path}")
            .into(moviePoster)

        // Set up button click listeners
//        watchlistButton.setOnClickListener {
//            addToWatchlist()
//        }
//
//        favoriteButton.setOnClickListener {
//            addToFavorite()
//        }
    }

//    private fun addToWatchlist() {
//        AccountMediaNetworkUtils.addToWatchlist(movie.id) { success, e ->
//            runOnUiThread {
//                if (success) {
//                    Toast.makeText(this, "${movie.title} added to Watchlist", Toast.LENGTH_SHORT).show()
//                } else {
//                    e?.printStackTrace()
//                    Toast.makeText(this, "Failed to add ${movie.title} to Watchlist", Toast.LENGTH_SHORT).show()
//                }
//            }
//        }
//    }
//
//    private fun addToFavorite() {
//        AccountMediaNetworkUtils.addToFavorites(movie.id) { success, e ->
//            runOnUiThread {
//                if (success) {
//                    Toast.makeText(this, "${movie.title} added to Favorites", Toast.LENGTH_SHORT).show()
//                } else {
//                    e?.printStackTrace()
//                    Toast.makeText(this, "Failed to add ${movie.title} to Favorites", Toast.LENGTH_SHORT).show()
//                }
//            }
//        }
//    }

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

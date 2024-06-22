package edu.skku.map.pa3

import android.os.Build
import android.os.Bundle
import android.view.View
import android.view.WindowInsets
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import edu.skku.map.pa3.models.PopularMovies
import edu.skku.map.pa3.models.PopularTVShows
import edu.skku.map.pa3.network.MediaNetworkUtils
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import java.io.IOException

class HomePageActivity : AppCompatActivity() {
    companion object{
        // const val USER_INFO = "USER_INFO"
        const val SESSION_ID = "SESSION_ID"
    }

    private lateinit var sessionID: String
    private lateinit var popularMoviesAdapter: MediaAdapter
    private lateinit var popularTVShowsAdapter: MediaAdapter

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.homepage_layout)

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

        // Setup RecyclerView
        val popularMoviesList = findViewById<RecyclerView>(R.id.popular_movies_list)
        popularMoviesList.layoutManager = LinearLayoutManager(this, LinearLayoutManager.HORIZONTAL, false)
        popularMoviesAdapter = MediaAdapter(listOf())
        popularMoviesList.adapter = popularMoviesAdapter

        val popularTVShowsList = findViewById<RecyclerView>(R.id.popular_tv_shows_list)
        popularTVShowsList.layoutManager = LinearLayoutManager(this, LinearLayoutManager.HORIZONTAL, false)
        popularTVShowsAdapter = MediaAdapter(listOf())
        popularTVShowsList.adapter = popularTVShowsAdapter

        // Fetch popular movies
        fetchPopularMovies()

        // Fetch popular TV shows
        fetchPopularTVShows()
    }

    private fun fetchPopularMovies() {
        val url = "https://api.themoviedb.org/3/movie/popular?language=en-US&page=1"
        CoroutineScope(Dispatchers.IO).launch {
            try {
                val popularMovies: List<PopularMovies> = MediaNetworkUtils.getPopularMovies(url)
                withContext(Dispatchers.Main) {
                    popularMoviesAdapter.updateMovies(popularMovies)
                }
            } catch (e: IOException) {
                withContext(Dispatchers.Main) {
                    Toast.makeText(this@HomePageActivity, "Network error: ${e.message}", Toast.LENGTH_SHORT).show()
                }
            }
        }
    }

    private fun fetchPopularTVShows() {
        val url = "https://api.themoviedb.org/3/tv/popular?language=en-US&page=1"
        CoroutineScope(Dispatchers.IO).launch {
            try {
                val popularTVShows = MediaNetworkUtils.getPopularTVShows(url)
                withContext(Dispatchers.Main) {
                    popularTVShowsAdapter.updateTVShows(popularTVShows)
                }
            } catch (e: IOException) {
                withContext(Dispatchers.Main) {
                    Toast.makeText(this@HomePageActivity, "Network error: ${e.message}", Toast.LENGTH_SHORT).show()
                }
            }
        }
    }
}
package edu.skku.map.pa3.fragments

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.Toast
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import edu.skku.map.pa3.MediaAdapter
import edu.skku.map.pa3.R
import edu.skku.map.pa3.network.*
import edu.skku.map.pa3.models.*
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import java.io.IOException
import kotlin.random.Random

class HomeFragment : Fragment() { // OnItemClickListener {

    private lateinit var popularMoviesAdapter: MediaAdapter<Movie>
    private lateinit var popularTVShowsAdapter: MediaAdapter<TVShow>
    private lateinit var upcomingMoviesAdapter: MediaAdapter<Movie>
    private lateinit var upcomingTVShowsAdapter: MediaAdapter<TVShow>

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.homepage_layout, container, false)

        // Initialize adapters
        popularMoviesAdapter = MediaAdapter()
        popularTVShowsAdapter = MediaAdapter()
        upcomingMoviesAdapter = MediaAdapter()
        upcomingTVShowsAdapter = MediaAdapter()

        // Set up RecyclerViews
        val popularMoviesList = view.findViewById<RecyclerView>(R.id.popular_movies_list)
        popularMoviesList.layoutManager = LinearLayoutManager(context, LinearLayoutManager.HORIZONTAL, false)
        popularMoviesList.adapter = popularMoviesAdapter

        val popularTVShowsList = view.findViewById<RecyclerView>(R.id.popular_tv_shows_list)
        popularTVShowsList.layoutManager = LinearLayoutManager(context, LinearLayoutManager.HORIZONTAL, false)
        popularTVShowsList.adapter = popularTVShowsAdapter

        val upcomingMoviesList = view.findViewById<RecyclerView>(R.id.upcoming_movies_list)
        upcomingMoviesList.layoutManager = LinearLayoutManager(context, LinearLayoutManager.HORIZONTAL, false)
        upcomingMoviesList.adapter = upcomingMoviesAdapter

        val upcomingTVShowsList = view.findViewById<RecyclerView>(R.id.upcoming_tv_shows_list)
        upcomingTVShowsList.layoutManager = LinearLayoutManager(context, LinearLayoutManager.HORIZONTAL, false)
        upcomingTVShowsList.adapter = upcomingTVShowsAdapter

        // Fetch and update data
        fetchPopularMovies()
        fetchPopularTVShows()
        fetchUpcomingMovies()
        fetchUpcomingTVShows()

        val randomMovieButton = view.findViewById<Button>(R.id.random_movie_button)
        randomMovieButton.setOnClickListener{
            fetchRandomMovie()
        }

        return view
    }

    private fun fetchPopularMovies() {
        HomeMediaNetworkUtils.getPopularMovies("https://api.themoviedb.org/3/movie/popular?language=en-US&page=1", object : HomeMediaNetworkUtils.MovieCallback {
            override fun onSuccess(movies: List<Movie>) {
                CoroutineScope(Dispatchers.Main).launch {
                    popularMoviesAdapter.updateData(movies)
                }
            }

            override fun onFailure(e: IOException) {
                CoroutineScope(Dispatchers.Main).launch {
                    e.printStackTrace()
                    Toast.makeText(context, "Failed to fetch popular movies", Toast.LENGTH_SHORT).show()
                }
            }
        })
    }


    private fun fetchPopularTVShows() {
        HomeMediaNetworkUtils.getPopularTVShows("https://api.themoviedb.org/3/tv/popular?language=en-US&page=1") { popularTVShows, e ->
            CoroutineScope(Dispatchers.Main).launch {
                if (e != null) {
                    e.printStackTrace()
                } else {
                    popularTVShowsAdapter.updateData(popularTVShows!!)
                }
            }
        }
    }

    private fun fetchUpcomingMovies() {
        HomeMediaNetworkUtils.getUpcomingMovies("https://api.themoviedb.org/3/movie/upcoming?language=en-US&page=1") { upcomingMovies, e ->
            CoroutineScope(Dispatchers.Main).launch {
                if (e != null) {
                    e.printStackTrace()
                } else {
                    upcomingMoviesAdapter.updateData(upcomingMovies!!)
                }
            }
        }
    }

    private fun fetchUpcomingTVShows() {
        HomeMediaNetworkUtils.getAiringTodayShows("https://api.themoviedb.org/3/tv/on_the_air?language=en-US&page=1") { airingTodayShows, e ->
            CoroutineScope(Dispatchers.Main).launch {
                if (e != null) {
                    e.printStackTrace()
                } else {
                    upcomingTVShowsAdapter.updateData(airingTodayShows!!)
                }
            }
        }
    }

    private fun fetchRandomMovie() {
        // Generate a random page number (1 to 500)
        val randomPage = Random.nextInt(1, 501)
        val url = "https://api.themoviedb.org/3/movie/popular?language=en-US&page=$randomPage"

        HomeMediaNetworkUtils.getPopularMovies(url, object : HomeMediaNetworkUtils.MovieCallback {
            override fun onSuccess(movies: List<Movie>) {
                CoroutineScope(Dispatchers.Main).launch {
                    // Select a random movie from the list
                    val randomMovie = movies.random()

                    // Navigate to the MovieInfoFragment
                    val fragment = RandMovieInfoFragment.newInstance(randomMovie)
                    fragmentManager?.beginTransaction()?.replace(R.id.fragment_container, fragment)?.addToBackStack(null)?.commit()
                }
            }

            override fun onFailure(e: IOException) {
                CoroutineScope(Dispatchers.Main).launch {
                    e.printStackTrace()
                }
            }
        })
    }

//    override fun onItemClick(item: Any) {
//        when (item) {
//            is PopularMovies -> {
//                val intent = Intent(activity, MovieActivity::class.java)
//                intent.putExtra("movie", item)
//                startActivity(intent)
//            }
//            is PopularTVShows -> {
//                val intent = Intent(activity, MovieActivity::class.java)
//                intent.putExtra("tvShow", item)
//                startActivity(intent)
//            }
//            // Handle other item types if needed
//        }
//    }
}

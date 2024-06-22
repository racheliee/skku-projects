package edu.skku.map.pa3

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import edu.skku.map.pa3.network.*
import edu.skku.map.pa3.models.*
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import java.io.IOException

class HomeFragment : Fragment() {

    private lateinit var popularMoviesAdapter: MediaAdapter<PopularMovies>
    private lateinit var popularTVShowsAdapter: MediaAdapter<PopularTVShows>
    private lateinit var upcomingMoviesAdapter: MediaAdapter<UpcomingMovie>
    private lateinit var upcomingTVShowsAdapter: MediaAdapter<AiringTodayShow>

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

        return view
    }

    private fun fetchPopularMovies() {
        CoroutineScope(Dispatchers.IO).launch {
            try {
                val popularMovies = HomeMediaNetworkUtils.getPopularMovies("https://api.themoviedb.org/3/movie/popular?language=en-US&page=1")
                CoroutineScope(Dispatchers.Main).launch {
                    popularMoviesAdapter.updateData(popularMovies)
                }
            } catch (e: IOException) {
                e.printStackTrace()
            }
        }
    }

    private fun fetchPopularTVShows() {
        CoroutineScope(Dispatchers.IO).launch {
            try {
                val popularTVShows = HomeMediaNetworkUtils.getPopularTVShows("https://api.themoviedb.org/3/tv/popular?language=en-US&page=1")
                CoroutineScope(Dispatchers.Main).launch {
                    popularTVShowsAdapter.updateData(popularTVShows)
                }
            } catch (e: IOException) {
                e.printStackTrace()
            }
        }
    }

    private fun fetchUpcomingMovies() {
        CoroutineScope(Dispatchers.IO).launch {
            try {
                val upcomingMovies = HomeMediaNetworkUtils.getUpcomingMovies("https://api.themoviedb.org/3/movie/upcoming?language=en-US&page=1")
                CoroutineScope(Dispatchers.Main).launch {
                    upcomingMoviesAdapter.updateData(upcomingMovies)
                }
            } catch (e: IOException) {
                e.printStackTrace()
            }
        }
    }

    private fun fetchUpcomingTVShows() {
        CoroutineScope(Dispatchers.IO).launch {
            try {
                val upcomingTVShows = HomeMediaNetworkUtils.getAiringTodayShows("https://api.themoviedb.org/3/tv/on_the_air?language=en-US&page=1")
                CoroutineScope(Dispatchers.Main).launch {
                    upcomingTVShowsAdapter.updateData(upcomingTVShows)
                }
            } catch (e: IOException) {
                e.printStackTrace()
            }
        }
    }
}

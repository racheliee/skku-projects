package edu.skku.map.pa3.ui

import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
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

class FavouriteFragment: Fragment() {

    private lateinit var sessionID: String
    private var accountID: Int = 21341108

    private lateinit var favoriteMoviesAdapter: MediaAdapter<FavoriteMovie>
    private lateinit var favoriteTVShowsAdapter: MediaAdapter<FavoriteTVShow>
    private lateinit var movieWatchlistAdapter: MediaAdapter<WatchlistMovie>
    private lateinit var tvWatchlistAdapter: MediaAdapter<WatchlistTVShow>

    companion object{
        private const val ARG_SESSION_ID = "session_id"
        private const val ARG_ACCOUNT_ID = "account_id"

        fun newInstance(sessionID: String, accountID: Int) = FavouriteFragment().apply {
            arguments = Bundle().apply {
                putString(ARG_SESSION_ID, sessionID)
                putInt(ARG_ACCOUNT_ID, accountID)
            }
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        arguments?.let {
            sessionID = it.getString(ARG_SESSION_ID)!!
            accountID = it.getInt(ARG_ACCOUNT_ID)
        }
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.favourite_layout, container, false)

        // Initialize adapters
        favoriteMoviesAdapter = MediaAdapter()
        favoriteTVShowsAdapter = MediaAdapter()
        tvWatchlistAdapter = MediaAdapter()
        movieWatchlistAdapter = MediaAdapter()

        // Set up RecyclerViews
        val favoriteMoviesList = view.findViewById<RecyclerView>(R.id.favorite_movies_list)
        favoriteMoviesList.layoutManager = LinearLayoutManager(context, LinearLayoutManager.HORIZONTAL, false)
        favoriteMoviesList.adapter = favoriteMoviesAdapter

        val favoriteTVShowsList = view.findViewById<RecyclerView>(R.id.favorite_tv_shows_list)
        favoriteTVShowsList.layoutManager = LinearLayoutManager(context, LinearLayoutManager.HORIZONTAL, false)
        favoriteTVShowsList.adapter = favoriteTVShowsAdapter

        val tvWatchlist = view.findViewById<RecyclerView>(R.id.tv_watchlist)
        tvWatchlist.layoutManager = LinearLayoutManager(context, LinearLayoutManager.HORIZONTAL, false)
        tvWatchlist.adapter = tvWatchlistAdapter

        val movieWatchlist = view.findViewById<RecyclerView>(R.id.movie_watchlist)
        movieWatchlist.layoutManager = LinearLayoutManager(context, LinearLayoutManager.HORIZONTAL, false)
        movieWatchlist.adapter = movieWatchlistAdapter

        // Fetch and update data
        fetchFavoriteMovies()
        fetchFavoriteTVShows()
        fetchTVWatchlist()
        fetchMovieWatchlist()

        return view
    }

    private fun fetchFavoriteMovies() {
        val url = "https://api.themoviedb.org/3/account/$accountID/favorite/movies?language=en-US&page=1&session_id=$sessionID&sort_by=created_at.asc"
        Log.d("FavouriteFragment", "URL: $url")

        CoroutineScope(Dispatchers.IO).launch {
            try {
                val favoriteMovies = FavouriteMediaNetworkUtils.getFavoriteMovies(url)
                CoroutineScope(Dispatchers.Main).launch {
                    favoriteMoviesAdapter.updateData(favoriteMovies)
                }
            } catch (e: IOException) {
                e.printStackTrace()
            }
        }
    }

    private fun fetchFavoriteTVShows() {
        val url = "https://api.themoviedb.org/3/account/$accountID/favorite/tv?language=en-US&page=1&session_id=$sessionID&sort_by=created_at.asc"
        CoroutineScope(Dispatchers.IO).launch {
            try {
                val favoriteTVShows = FavouriteMediaNetworkUtils.getFavoriteTVShows(url)
                CoroutineScope(Dispatchers.Main).launch {
                    favoriteTVShowsAdapter.updateData(favoriteTVShows)
                }
            } catch (e: IOException) {
                e.printStackTrace()
            }
        }
    }

    private fun fetchTVWatchlist() {
        val url = "https://api.themoviedb.org/3/account/$accountID/watchlist/tv?language=en-US&page=1&session_id=$sessionID&sort_by=created_at.asc"
        CoroutineScope(Dispatchers.IO).launch {
            try {
                val tvWatchlist = FavouriteMediaNetworkUtils.getTVShowWatchlist(url)
                CoroutineScope(Dispatchers.Main).launch {
                    tvWatchlistAdapter.updateData(tvWatchlist)
                }
            } catch (e: IOException) {
                e.printStackTrace()
            }
        }
    }

    private fun fetchMovieWatchlist() {
        val url = "https://api.themoviedb.org/3/account/$accountID/watchlist/movies?language=en-US&page=1&session_id=$sessionID&sort_by=created_at.asc"

        CoroutineScope(Dispatchers.IO).launch {
            try {
                val movieWatchlist = FavouriteMediaNetworkUtils.getMovieWatchlist(url)
                CoroutineScope(Dispatchers.Main).launch {
                    movieWatchlistAdapter.updateData(movieWatchlist)
                }
            } catch (e: IOException) {
                e.printStackTrace()
            }
        }
    }
}

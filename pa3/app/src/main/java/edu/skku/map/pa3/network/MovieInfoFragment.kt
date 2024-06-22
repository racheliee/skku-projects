package edu.skku.map.pa3.ui

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageView
import android.widget.TextView
import androidx.fragment.app.Fragment
import com.bumptech.glide.Glide
import edu.skku.map.pa3.R
import edu.skku.map.pa3.models.PopularMovies
import edu.skku.map.pa3.network.HomeMediaNetworkUtils
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import java.io.IOException

class MovieInfoFragment : Fragment() {

    companion object {
        private const val ARG_MOVIE = "movie"

        fun newInstance(movie: PopularMovies) = MovieInfoFragment().apply {
            arguments = Bundle().apply {
                putSerializable(ARG_MOVIE, movie)
            }
        }
    }

    private lateinit var movie: PopularMovies

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        arguments?.let {
            movie = it.getSerializable(ARG_MOVIE) as PopularMovies
        }
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.movie_info_layout, container, false)

        val moviePoster = view.findViewById<ImageView>(R.id.movie_poster)
        val movieTitle = view.findViewById<TextView>(R.id.movie_title)
        val movieReleaseDate = view.findViewById<TextView>(R.id.movie_release_date)
        val movieRating = view.findViewById<TextView>(R.id.movie_rating)
        val movieOverview = view.findViewById<TextView>(R.id.movie_overview)
        val generateAnotherButton = view.findViewById<TextView>(R.id.generate_another_button)

        movieTitle.text = movie.title
        movieReleaseDate.text = "Release Date: ${movie.release_date}"
        movieRating.text = "Rating: ${movie.vote_average}/10"
        movieOverview.text = movie.overview

        Glide.with(this)
            .load("https://image.tmdb.org/t/p/w500${movie.poster_path}")
            .into(moviePoster)

        generateAnotherButton.setOnClickListener {
            fetchRandomMovie()
        }

        return view
    }

    private fun fetchRandomMovie() {
        CoroutineScope(Dispatchers.IO).launch {
            try {
                val randomPage = (1..100).random() // Randomly select a page from 1 to 100
                val url = "https://api.themoviedb.org/3/movie/popular?api_key=<YOUR_API_KEY>&language=en-US&page=$randomPage"
                val movies = HomeMediaNetworkUtils.getPopularMovies(url)
                if (movies.isNotEmpty()) {
                    val randomMovie = movies.random() // Randomly select a movie from the list
                    CoroutineScope(Dispatchers.Main).launch {
                        updateMovieInfo(randomMovie)
                    }
                }
            } catch (e: IOException) {
                e.printStackTrace()
            }
        }
    }

    private fun updateMovieInfo(movie: PopularMovies) {
        val moviePoster = view?.findViewById<ImageView>(R.id.movie_poster)
        val movieTitle = view?.findViewById<TextView>(R.id.movie_title)
        val movieReleaseDate = view?.findViewById<TextView>(R.id.movie_release_date)
        val movieRating = view?.findViewById<TextView>(R.id.movie_rating)
        val movieOverview = view?.findViewById<TextView>(R.id.movie_overview)

        movieTitle?.text = movie.title
        movieReleaseDate?.text = "Release Date: ${movie.release_date}"
        movieRating?.text = "Rating: ${movie.vote_average}/10"
        movieOverview?.text = movie.overview

        if (moviePoster != null) {
            Glide.with(this)
                .load("https://image.tmdb.org/t/p/w500${movie.poster_path}").into(moviePoster)
        }
    }
}

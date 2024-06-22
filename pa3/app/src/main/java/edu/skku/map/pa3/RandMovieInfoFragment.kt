import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
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

class RandMovieInfoFragment : Fragment() {

    companion object {
        private const val ARG_MOVIE = "movie"

        fun newInstance(movie: PopularMovies) = RandMovieInfoFragment().apply {
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
        val view = inflater.inflate(R.layout.movie_info_rand_layout, container, false)

        // Initialize your views and set movie data
        val movieTitle = view.findViewById<TextView>(R.id.movie_title)
        val movieOverview = view.findViewById<TextView>(R.id.movie_overview)
        val moviePoster = view.findViewById<ImageView>(R.id.movie_poster)
        val generateAgainButton = view.findViewById<Button>(R.id.generate_another_button)

        movieTitle.text = movie.title
        movieOverview.text = movie.overview
        Glide.with(this)
            .load("https://image.tmdb.org/t/p/w500${movie.poster_path}")
            .into(moviePoster)

        // Set click listeners
        generateAgainButton.setOnClickListener {
            fetchRandomMovie()
        }

        return view
    }

    private fun fetchRandomMovie() {
        val randomPage = (1..100).random() // Randomly select a page from 1 to 100
        val url = "https://api.themoviedb.org/3/movie/popular?api_key=<YOUR_API_KEY>&language=en-US&page=$randomPage"

        HomeMediaNetworkUtils.getPopularMovies(url, object : HomeMediaNetworkUtils.MovieCallback {
            override fun onSuccess(movies: List<PopularMovies>) {
                if (movies.isNotEmpty()) {
                    val randomMovie = movies.random() // Randomly select a movie from the list
                    CoroutineScope(Dispatchers.Main).launch {
                        updateMovieInfo(randomMovie)
                    }
                }
            }

            override fun onFailure(e: IOException) {
                e.printStackTrace()
            }
        })
    }


    private fun updateMovieInfo(movie: PopularMovies) {
        view?.findViewById<TextView>(R.id.movie_title)?.text = movie.title
        view?.findViewById<TextView>(R.id.movie_overview)?.text = movie.overview
        view?.findViewById<ImageView>(R.id.movie_poster)?.let {
            Glide.with(this)
                .load("https://image.tmdb.org/t/p/w500${movie.poster_path}")
                .into(it)
        }
    }
}

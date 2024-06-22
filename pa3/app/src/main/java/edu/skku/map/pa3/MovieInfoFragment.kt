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
        val view = inflater.inflate(R.layout.movie_info_rand_layout, container, false)

        // Initialize your views and set movie data
        val movieTitle = view.findViewById<TextView>(R.id.movie_title)
        val movieOverview = view.findViewById<TextView>(R.id.movie_overview)
        val moviePoster = view.findViewById<ImageView>(R.id.movie_poster)

        movieTitle.text = movie.title
        movieOverview.text = movie.overview
        Glide.with(this)
            .load("https://image.tmdb.org/t/p/w500${movie.poster_path}")
            .into(moviePoster)

        return view
    }
}

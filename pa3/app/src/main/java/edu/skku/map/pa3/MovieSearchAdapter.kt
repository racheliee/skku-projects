package edu.skku.map.pa3

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageView
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import com.bumptech.glide.Glide
import edu.skku.map.pa3.models.Movie

class MovieSearchAdapter (
    private var movies: List<Movie> = listOf()
) : RecyclerView.Adapter<MovieSearchAdapter.MovieViewHolder>() {

    class MovieViewHolder(view: View) : RecyclerView.ViewHolder(view) {
        val moviePoster: ImageView = view.findViewById(R.id.movie_poster)
        val movieTitle: TextView = view.findViewById(R.id.movie_title)
        val movieReleaseDate: TextView = view.findViewById(R.id.movie_release_date)
        val movieOverview: TextView = view.findViewById(R.id.movie_overview)
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): MovieViewHolder {
        val view = LayoutInflater.from(parent.context).inflate(R.layout.item_movie_search, parent, false)
        return MovieViewHolder(view)
    }

    override fun onBindViewHolder(holder: MovieViewHolder, position: Int) {
        val movie = movies[position]
        holder.apply {
            movieTitle.text = movie.title
            movieReleaseDate.text = movie.release_date
            movieOverview.text = movie.overview
            Glide.with(itemView.context)
                .load("https://image.tmdb.org/t/p/w500${movie.poster_path}")
                .into(moviePoster)
        }
    }

    override fun getItemCount(): Int {
        return movies.size
    }

    fun updateData(newMovies: List<Movie>) {
        movies = newMovies
        notifyDataSetChanged()
    }
}
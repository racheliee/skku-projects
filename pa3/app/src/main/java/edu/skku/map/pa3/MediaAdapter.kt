package edu.skku.map.pa3

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageView
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import com.bumptech.glide.Glide
import edu.skku.map.pa3.models.PopularMovies
import edu.skku.map.pa3.models.PopularTVShows

class MediaAdapter(
    private var popularMovies: List<PopularMovies> = listOf(),
    private var popularTVShows: List<PopularTVShows> = listOf()
) : RecyclerView.Adapter<RecyclerView.ViewHolder>() {

    companion object {
        private const val VIEW_TYPE_MOVIE = 0
        private const val VIEW_TYPE_TV_SHOW = 1
    }

    class MovieViewHolder(view: View) : RecyclerView.ViewHolder(view) {
        val moviePoster: ImageView = view.findViewById(R.id.movie_poster)
        val movieTitle: TextView = view.findViewById(R.id.movie_title)
        val movieRating: TextView = view.findViewById(R.id.movie_rating)
    }

    class TVShowViewHolder(view: View) : RecyclerView.ViewHolder(view) {
        val tvShowPoster: ImageView = view.findViewById(R.id.movie_poster)
        val tvShowTitle: TextView = view.findViewById(R.id.movie_title)
        val tvShowRating: TextView = view.findViewById(R.id.movie_rating)
    }

    override fun getItemViewType(position: Int): Int {
        return if (position < popularMovies.size) VIEW_TYPE_MOVIE else VIEW_TYPE_TV_SHOW
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): RecyclerView.ViewHolder {
        return if (viewType == VIEW_TYPE_MOVIE) {
            val view = LayoutInflater.from(parent.context).inflate(R.layout.item_movie, parent, false)
            MovieViewHolder(view)
        } else {
            val view = LayoutInflater.from(parent.context).inflate(R.layout.item_movie, parent, false)
            TVShowViewHolder(view)
        }
    }

    override fun onBindViewHolder(holder: RecyclerView.ViewHolder, position: Int) {
        if (getItemViewType(position) == VIEW_TYPE_MOVIE) {
            val movie = popularMovies[position]
            (holder as MovieViewHolder).apply {
                movieTitle.text = movie.title
                movieRating.text = String.format("★ Rating: %.1f/10", movie.vote_average)
                Glide.with(itemView.context)
                    .load("https://image.tmdb.org/t/p/w500${movie.poster_path}")
                    .into(moviePoster)
            }
        } else {
            val tvShow = popularTVShows[position - popularMovies.size]
            (holder as TVShowViewHolder).apply {
                tvShowTitle.text = tvShow.name
                tvShowRating.text = String.format("★ Rating: %.1f/10", tvShow.vote_average)
                Glide.with(itemView.context)
                    .load("https://image.tmdb.org/t/p/w500${tvShow.poster_path}")
                    .into(tvShowPoster)
            }
        }
    }

    override fun getItemCount(): Int {
        return popularMovies.size + popularTVShows.size
    }

    fun updateMovies(newPopularMovies: List<PopularMovies>) {
        popularMovies = newPopularMovies
        notifyDataSetChanged()
    }

    fun updateTVShows(newPopularTVShows: List<PopularTVShows>) {
        popularTVShows = newPopularTVShows
        notifyDataSetChanged()
    }
}

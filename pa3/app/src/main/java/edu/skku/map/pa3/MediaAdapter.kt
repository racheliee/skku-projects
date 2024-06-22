package edu.skku.map.pa3

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageView
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import com.bumptech.glide.Glide
import edu.skku.map.pa3.models.*

class MediaAdapter<T>(
    private var items: List<T> = listOf()
) : RecyclerView.Adapter<RecyclerView.ViewHolder>() {

    companion object {
        private const val VIEW_TYPE_MOVIE = 0
        private const val VIEW_TYPE_TV_SHOW = 1
        private const val VIEW_TYPE_MOVIE_UPCOMING = 2
        private const val VIEW_TYPE_TV_SHOW_AIRING = 3
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

    class UpcomingMovieViewHolder(view: View) : RecyclerView.ViewHolder(view) {
        val moviePoster: ImageView = view.findViewById(R.id.movie_poster)
        val movieTitle: TextView = view.findViewById(R.id.movie_title)
        val movieReleaseDate: TextView = view.findViewById(R.id.movie_rating)
    }

    class AiringTodayShowViewHolder(view: View) : RecyclerView.ViewHolder(view) {
        val tvShowPoster: ImageView = view.findViewById(R.id.movie_poster)
        val tvShowTitle: TextView = view.findViewById(R.id.movie_title)
        val tvShowRating: TextView = view.findViewById(R.id.movie_rating)
    }

    override fun getItemViewType(position: Int): Int {
        return when (items[position]) {
            is PopularMovies -> VIEW_TYPE_MOVIE
            is WatchlistMovie -> VIEW_TYPE_MOVIE
            is FavoriteMovie -> VIEW_TYPE_MOVIE
            is PopularTVShows -> VIEW_TYPE_TV_SHOW
            is WatchlistTVShow -> VIEW_TYPE_TV_SHOW
            is FavoriteTVShow -> VIEW_TYPE_TV_SHOW
            is UpcomingMovie -> VIEW_TYPE_MOVIE_UPCOMING
            is AiringTodayShow -> VIEW_TYPE_TV_SHOW_AIRING
            else -> throw IllegalArgumentException("Invalid item type")
        }
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): RecyclerView.ViewHolder {
        return when (viewType) {
            VIEW_TYPE_MOVIE -> {
                val view = LayoutInflater.from(parent.context).inflate(R.layout.item_movie, parent, false)
                MovieViewHolder(view)
            }
            VIEW_TYPE_TV_SHOW -> {
                val view = LayoutInflater.from(parent.context).inflate(R.layout.item_movie, parent, false)
                TVShowViewHolder(view)
            }
            VIEW_TYPE_MOVIE_UPCOMING -> {
                val view = LayoutInflater.from(parent.context).inflate(R.layout.item_movie, parent, false)
                UpcomingMovieViewHolder(view)
            }
            VIEW_TYPE_TV_SHOW_AIRING -> {
                val view = LayoutInflater.from(parent.context).inflate(R.layout.item_movie, parent, false)
                AiringTodayShowViewHolder(view)
            }
            else -> throw IllegalArgumentException("Invalid view type")
        }
    }

    override fun onBindViewHolder(holder: RecyclerView.ViewHolder, position: Int) {
        when (holder) {
            is MovieViewHolder -> {
                val movie = items[position] as PopularMovies
                holder.apply {
                    movieTitle.text = movie.title
                    movieRating.text = String.format("★ Rating: %.1f/10", movie.vote_average)
                    Glide.with(itemView.context)
                        .load("https://image.tmdb.org/t/p/w500${movie.poster_path}")
                        .into(moviePoster)
                }
            }
            is TVShowViewHolder -> {
                val tvShow = items[position] as PopularTVShows
                holder.apply {
                    tvShowTitle.text = tvShow.name
                    tvShowRating.text = String.format("★ Rating: %.1f/10", tvShow.vote_average)
                    Glide.with(itemView.context)
                        .load("https://image.tmdb.org/t/p/w500${tvShow.poster_path}")
                        .into(tvShowPoster)
                }
            }
            is UpcomingMovieViewHolder -> {
                val upcomingMovie = items[position] as UpcomingMovie
                holder.apply {
                    movieTitle.text = upcomingMovie.title
                    movieReleaseDate.text = "Release Date: ${upcomingMovie.release_date}"
                    Glide.with(itemView.context)
                        .load("https://image.tmdb.org/t/p/w500${upcomingMovie.poster_path}")
                        .into(moviePoster)
                }
            }
            is AiringTodayShowViewHolder -> {
                val airingTodayShow = items[position] as AiringTodayShow
                holder.apply {
                    tvShowTitle.text = airingTodayShow.name
                    tvShowRating.text = String.format("★ Rating: %.1f/10", airingTodayShow.vote_average)
                    Glide.with(itemView.context)
                        .load("https://image.tmdb.org/t/p/w500${airingTodayShow.poster_path}")
                        .into(tvShowPoster)
                }
            }
        }
    }

    override fun getItemCount(): Int {
        return items.size
    }

    fun updateData(newItems: List<T>) {
        items = newItems
        notifyDataSetChanged()
    }
}

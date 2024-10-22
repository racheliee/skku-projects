package edu.skku.map.pa3

import android.content.Intent
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageView
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import com.bumptech.glide.Glide
import edu.skku.map.pa3.models.*

class MovieSearchAdapter (
    private var items: List<Any> = listOf()
) : RecyclerView.Adapter<RecyclerView.ViewHolder>() {

    companion object {
        private const val VIEW_TYPE_MOVIE = 0
        private const val VIEW_TYPE_TV_SHOW = 1
    }

    class MovieViewHolder(view: View) : RecyclerView.ViewHolder(view) {
        val moviePoster: ImageView = view.findViewById(R.id.movie_poster)
        val movieTitle: TextView = view.findViewById(R.id.movie_title)
        val movieReleaseDate: TextView = view.findViewById(R.id.movie_release_date)
        val movieOverview: TextView = view.findViewById(R.id.movie_overview)
    }

    class TVShowViewHolder(view: View) : RecyclerView.ViewHolder(view) {
        val tvShowPoster: ImageView = view.findViewById(R.id.movie_poster)
        val tvShowTitle: TextView = view.findViewById(R.id.movie_title)
        val tvShowReleaseDate: TextView = view.findViewById(R.id.movie_release_date)
        val tvShowOverview: TextView = view.findViewById(R.id.movie_overview)
    }

    override fun getItemViewType(position: Int): Int {
        return when (items[position]) {
            is Movie -> VIEW_TYPE_MOVIE
            is TVShow -> VIEW_TYPE_TV_SHOW
            else -> throw IllegalArgumentException("Invalid item type")
        }
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): RecyclerView.ViewHolder {
        return when (viewType) {
            VIEW_TYPE_MOVIE -> {
                val view = LayoutInflater.from(parent.context).inflate(R.layout.item_movie_search, parent, false)
                MovieViewHolder(view).apply{
                    itemView.setOnClickListener {
                        val movie = items[adapterPosition] as Movie
                        val intent = Intent(itemView.context, MovieActivity::class.java)
                        intent.putExtra("movie", movie)
                        itemView.context.startActivity(intent)
                    }
                }
            }
            VIEW_TYPE_TV_SHOW -> {
                val view = LayoutInflater.from(parent.context).inflate(R.layout.item_movie_search, parent, false)
                TVShowViewHolder(view).apply{
                    itemView.setOnClickListener {
                        val tvShow = items[adapterPosition] as TVShow
                        val intent = Intent(itemView.context, TVShowActivity::class.java)
                        intent.putExtra("tvShow", tvShow)
                        itemView.context.startActivity(intent)
                    }
                }
            }
            else -> throw IllegalArgumentException("Invalid view type")
        }
    }

    override fun onBindViewHolder(holder: RecyclerView.ViewHolder, position: Int) {
        when (holder) {
            is MovieViewHolder -> {
                val movie = items[position] as Movie
                holder.apply {
                    movieTitle.text = movie.title
                    movieReleaseDate.text = movie.release_date
                    movieOverview.text = movie.overview
                    Glide.with(itemView.context)
                        .load("https://image.tmdb.org/t/p/w500${movie.poster_path}")
                        .into(moviePoster)
                }
            }
            is TVShowViewHolder -> {
                val tvShow = items[position] as TVShow
                holder.apply {
                    tvShowTitle.text = tvShow.name
                    tvShowReleaseDate.text = tvShow.first_air_date
                    tvShowOverview.text = tvShow.overview
                    Glide.with(itemView.context)
                        .load("https://image.tmdb.org/t/p/w500${tvShow.poster_path}")
                        .into(tvShowPoster)
                }
            }
        }
    }

    override fun getItemCount(): Int {
        return items.size
    }

    fun updateData(newItems: List<Any>) {
        items = newItems
        notifyDataSetChanged()
    }
}
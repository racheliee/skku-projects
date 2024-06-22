package edu.skku.map.pa3.network

import android.util.Log
import com.google.gson.Gson
import com.google.gson.reflect.TypeToken
import edu.skku.map.pa3.models.*
import okhttp3.OkHttpClient
import okhttp3.Request
import okhttp3.logging.HttpLoggingInterceptor
import java.io.IOException

object FavouriteMediaNetworkUtils {
    private val client: OkHttpClient
    private val gson: Gson = Gson()
    private val tmdbAccessToken = "eyJhbGciOiJIUzI1NiJ9.eyJhdWQiOiI1ZTllMGFmNjhjNTllYWZlYjcxNzIzZjU2YTM4NjJiYyIsInN1YiI6IjY2NzVkYTkyZGU3MmZkMzI3N2IwNTIyYSIsInNjb3BlcyI6WyJhcGlfcmVhZCJdLCJ2ZXJzaW9uIjoxfQ.QLYG3b_OYmMs6zYxSj_KaahIUM8jcCkssz1AUxWaZmA"

    init {
        val logging = HttpLoggingInterceptor()
        logging.setLevel(HttpLoggingInterceptor.Level.BODY)

        client = OkHttpClient.Builder()
            .addInterceptor(logging)
            .build()
    }

    // Fetch favorite movies from the TMDB API
    @Throws(IOException::class)
    fun getFavoriteMovies(url: String): List<FavoriteMovie> {
        val request = Request.Builder()
            .url(url)
            .addHeader("accept", "application/json")
            .addHeader("Authorization", "Bearer $tmdbAccessToken")
            .build()

        client.newCall(request).execute().use { response ->
            if (!response.isSuccessful) throw IOException("Unexpected code $response")
            val json = response.body!!.string()

            val type = object : TypeToken<FavoriteMoviesResponse>() {}.type
            val tmdbResponse: FavoriteMoviesResponse = gson.fromJson(json, type)
            Log.d("MediaNetworkUtils", "Fetched ${tmdbResponse.results.size} favorite movies")

            return tmdbResponse.results
        }
    }

    @Throws(IOException::class)
    fun getFavoriteTVShows(url: String): List<FavoriteTVShow> {
        val request = Request.Builder()
            .url(url)
            .addHeader("accept", "application/json")
            .addHeader("Authorization", "Bearer $tmdbAccessToken")
            .build()

        client.newCall(request).execute().use { response ->
            if (!response.isSuccessful) throw IOException("Unexpected code $response")
            val json = response.body!!.string()

            val type = object : TypeToken<FavoriteTVShowsResponse>() {}.type
            val tmdbResponse: FavoriteTVShowsResponse = gson.fromJson(json, type)
            Log.d("MediaNetworkUtils", "Fetched ${tmdbResponse.results.size} favorite TV shows")

            return tmdbResponse.results
        }
    }

    // Fetch movie watchlist from the TMDB API
    @Throws(IOException::class)
    fun getMovieWatchlist(url: String): List<WatchlistMovie> {
        val request = Request.Builder()
            .url(url)
            .addHeader("accept", "application/json")
            .addHeader("Authorization", "Bearer $tmdbAccessToken")
            .build()

        client.newCall(request).execute().use { response ->
            if (!response.isSuccessful) throw IOException("Unexpected code $response")
            val json = response.body!!.string()

            val type = object : TypeToken<MovieWatchlistResponse>() {}.type
            val tmdbResponse: MovieWatchlistResponse = gson.fromJson(json, type)
            Log.d("MediaNetworkUtils", "Fetched ${tmdbResponse.results.size} movie watchlist")

            return tmdbResponse.results
        }
    }

    // Fetch TV show watchlist from the TMDB API
    @Throws(IOException::class)
    fun getTVShowWatchlist(url: String): List<WatchlistTVShow> {
        val request = Request.Builder()
            .url(url)
            .addHeader("accept", "application/json")
            .addHeader("Authorization", "Bearer $tmdbAccessToken")
            .build()

        client.newCall(request).execute().use { response ->
            if (!response.isSuccessful) throw IOException("Unexpected code $response")
            val json = response.body!!.string()

            val type = object : TypeToken<TVShowWatchlistResponse>() {}.type
            val tmdbResponse: TVShowWatchlistResponse = gson.fromJson(json, type)
            Log.d("MediaNetworkUtils", "Fetched ${tmdbResponse.results.size} TV show watchlist")

            return tmdbResponse.results
        }
    }
}

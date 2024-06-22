package edu.skku.map.pa3.network

import android.util.Log
import com.google.gson.Gson
import com.google.gson.reflect.TypeToken
import edu.skku.map.pa3.models.MovieResponses
import edu.skku.map.pa3.models.*
import edu.skku.map.pa3.models.TVShowResponses
import okhttp3.OkHttpClient
import okhttp3.Request
import okhttp3.logging.HttpLoggingInterceptor
import java.io.IOException

object SearchMediaNetworkUtils {
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

    // Search movies by name and genre
    @Throws(IOException::class)
    fun searchMovies(query: String, selectedGenre: String?): List<PopularMovies> {
        val genreFilter = if (selectedGenre.isNullOrEmpty()) "" else "&with_genres=$selectedGenre"
        val url = "https://api.themoviedb.org/3/search/movie?query=$query$genreFilter&language=en-US&page=1"

        val request = Request.Builder()
            .url(url)
            .addHeader("accept", "application/json")
            .addHeader("Authorization", "Bearer $tmdbAccessToken")
            .build()

        client.newCall(request).execute().use { response ->
            if (!response.isSuccessful) throw IOException("Unexpected code $response")
            val json = response.body!!.string()

            val type = object : TypeToken<MovieResponses>() {}.type
            val tmdbResponse: MovieResponses = gson.fromJson(json, type)
            Log.d("SearchMediaNetworkUtils", "Fetched ${tmdbResponse.results.size} movies")

            return tmdbResponse.results
        }
    }

    // Search TV shows by name and genre
    @Throws(IOException::class)
    fun searchTVShows(query: String, selectedGenre: String?): List<PopularTVShows> {
        val genreFilter = if (selectedGenre.isNullOrEmpty()) "" else "&with_genres=$selectedGenre"
        val url = "https://api.themoviedb.org/3/search/tv?query=$query$genreFilter&language=en-US&page=1"

        val request = Request.Builder()
            .url(url)
            .addHeader("accept", "application/json")
            .addHeader("Authorization", "Bearer $tmdbAccessToken")
            .build()

        client.newCall(request).execute().use { response ->
            if (!response.isSuccessful) throw IOException("Unexpected code $response")
            val json = response.body!!.string()

            val type = object : TypeToken<TVShowResponses>() {}.type
            val tmdbResponse: TVShowResponses = gson.fromJson(json, type)
            Log.d("SearchMediaNetworkUtils", "Fetched ${tmdbResponse.results.size} TV shows")

            return tmdbResponse.results
        }
    }
}

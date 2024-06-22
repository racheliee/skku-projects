package edu.skku.map.pa3.network

import android.util.Log
import com.google.gson.Gson
import com.google.gson.reflect.TypeToken
import edu.skku.map.pa3.models.MovieResponses
import edu.skku.map.pa3.models.PopularMovies
import edu.skku.map.pa3.models.PopularTVShows
import edu.skku.map.pa3.models.TVShowResponses
import okhttp3.OkHttpClient
import okhttp3.Request
import okhttp3.logging.HttpLoggingInterceptor
import java.io.IOException

object MediaNetworkUtils {
    private val client: OkHttpClient
    private val gson: Gson = Gson()
    private val tmdbAcessToken = "eyJhbGciOiJIUzI1NiJ9.eyJhdWQiOiI1ZTllMGFmNjhjNTllYWZlYjcxNzIzZjU2YTM4NjJiYyIsInN1YiI6IjY2NzVkYTkyZGU3MmZkMzI3N2IwNTIyYSIsInNjb3BlcyI6WyJhcGlfcmVhZCJdLCJ2ZXJzaW9uIjoxfQ.QLYG3b_OYmMs6zYxSj_KaahIUM8jcCkssz1AUxWaZmA"

    init {
        val logging = HttpLoggingInterceptor()
        logging.setLevel(HttpLoggingInterceptor.Level.BODY)

        client = OkHttpClient.Builder()
            .addInterceptor(logging)
            .build()
    }

    // This function is used to get popular movies from the TMDB API
    @Throws(IOException::class)
    fun getPopularMovies(url: String): List<PopularMovies> {
        val request = Request.Builder()
            .url(url)
            .addHeader("accept", "application/json")
            .addHeader("Authorization", "Bearer $tmdbAcessToken")
            .build()

        client.newCall(request).execute().use { response ->
            if (!response.isSuccessful) throw IOException("Unexpected code $response")
            val json = response.body!!.string()

            val type = object : TypeToken<MovieResponses>() {}.type
            val tmdbResponse: MovieResponses = gson.fromJson(json, type)
            Log.d("MediaNetworkUtils", "Fetched ${tmdbResponse.results.size} popular movies")

            return tmdbResponse.results
        }
    }

    // This function is used to get popular TV shows from the TMDB API
    @Throws(IOException::class)
    fun getPopularTVShows(url: String): List<PopularTVShows> {
        val request = Request.Builder()
            .url(url)
            .addHeader("accept", "application/json")
            .addHeader("Authorization", "Bearer $tmdbAcessToken")
            .build()

        client.newCall(request).execute().use { response ->
            if (!response.isSuccessful) throw IOException("Unexpected code $response")
            val json = response.body!!.string()

            val type = object : TypeToken<TVShowResponses>() {}.type
            val tmdbResponse: TVShowResponses = gson.fromJson(json, type)
            Log.d("MediaNetworkUtils", "Fetched ${tmdbResponse.results.size} popular TV shows")

            return tmdbResponse.results
        }
    }
}

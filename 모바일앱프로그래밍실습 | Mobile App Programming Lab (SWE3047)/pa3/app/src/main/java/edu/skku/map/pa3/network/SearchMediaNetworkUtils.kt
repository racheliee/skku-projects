package edu.skku.map.pa3.network

import android.util.Log
import com.google.gson.Gson
import com.google.gson.reflect.TypeToken
import edu.skku.map.pa3.models.PopularMovieResponses
import edu.skku.map.pa3.models.*
import edu.skku.map.pa3.models.PopularTVShowResponse
import okhttp3.OkHttpClient
import okhttp3.Request
import okhttp3.logging.HttpLoggingInterceptor
import java.io.IOException

object SearchMediaNetworkUtils {
    private val client: OkHttpClient
    private val gson: Gson = Gson()
    private val tmdbAccessToken = "YOUR ACCESS TOKEN"

    init {
        val logging = HttpLoggingInterceptor()
        logging.setLevel(HttpLoggingInterceptor.Level.BODY)

        client = OkHttpClient.Builder()
            .addInterceptor(logging)
            .build()
    }

    @Throws(IOException::class)
    fun searchMovies(url: String): List<Movie> {
        val request = Request.Builder()
            .url(url)
            .addHeader("accept", "application/json")
            .addHeader("Authorization", "Bearer $tmdbAccessToken")
            .build()

        client.newCall(request).execute().use { response ->
            if (!response.isSuccessful) throw IOException("Unexpected code $response")
            val json = response.body!!.string()

            val type = object : TypeToken<PopularMovieResponses>() {}.type
            val tmdbResponse: PopularMovieResponses = gson.fromJson(json, type)
            Log.d("MediaNetworkUtils", "Fetched ${tmdbResponse.results.size} movies")

            return tmdbResponse.results
        }
    }

    @Throws(IOException::class)
    fun searchTVShows(url: String): List<TVShow> {
        val request = Request.Builder()
            .url(url)
            .addHeader("accept", "application/json")
            .addHeader("Authorization", "Bearer $tmdbAccessToken")
            .build()

        client.newCall(request).execute().use { response ->
            if (!response.isSuccessful) throw IOException("Unexpected code $response")
            val json = response.body!!.string()

            val type = object : TypeToken<PopularTVShowResponse>() {}.type
            val tmdbResponse: PopularTVShowResponse = gson.fromJson(json, type)
            Log.d("MediaNetworkUtils", "Fetched ${tmdbResponse.results.size} TV shows")

            return tmdbResponse.results
        }
    }
}

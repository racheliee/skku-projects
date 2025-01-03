package edu.skku.map.pa3.network

import android.util.Log
import com.google.gson.Gson
import com.google.gson.reflect.TypeToken
import edu.skku.map.pa3.models.*
import okhttp3.*
import okhttp3.logging.HttpLoggingInterceptor
import java.io.IOException

object HomeMediaNetworkUtils {
    private val client: OkHttpClient
    private val gson: Gson = Gson()
    private val tmdbAcessToken = "YOUR ACCESS TOKEN"

    init {
        val logging = HttpLoggingInterceptor()
        logging.setLevel(HttpLoggingInterceptor.Level.BODY)

        client = OkHttpClient.Builder()
            .addInterceptor(logging)
            .build()
    }

    interface MovieCallback {
        fun onSuccess(movies: List<Movie>)
        fun onFailure(e: IOException)
    }

    fun getPopularMovies(url: String, callback: MovieCallback) {
        val request = Request.Builder()
            .url(url)
            .addHeader("accept", "application/json")
            .addHeader("Authorization", "Bearer $tmdbAcessToken")
            .build()

        client.newCall(request).enqueue(object : Callback {
            override fun onFailure(call: Call, e: IOException) {
                callback.onFailure(e)
            }

            override fun onResponse(call: Call, response: Response) {
                if (!response.isSuccessful) {
                    callback.onFailure(IOException("Unexpected code $response"))
                    return
                }

                val json = response.body!!.string()
                val type = object : TypeToken<PopularMovieResponses>() {}.type
                val tmdbResponse: PopularMovieResponses = gson.fromJson(json, type)
                Log.d("MediaNetworkUtils", "Fetched ${tmdbResponse.results.size} popular movies")

                callback.onSuccess(tmdbResponse.results)
            }
        })
    }

    fun getPopularTVShows(url: String, callback: (List<TVShow>?, IOException?) -> Unit) {
        val request = Request.Builder()
            .url(url)
            .addHeader("accept", "application/json")
            .addHeader("Authorization", "Bearer $tmdbAcessToken")
            .build()

        client.newCall(request).enqueue(object : Callback {
            override fun onFailure(call: Call, e: IOException) {
                Log.e("HomeMediaNetworkUtils", "Error fetching popular TV shows", e)
                callback(null, e)
            }

            override fun onResponse(call: Call, response: Response) {
                response.use {
                    if (!it.isSuccessful) {
                        callback(null, IOException("Unexpected code $response"))
                        return
                    }

                    val json = it.body!!.string()
                    val type = object : TypeToken<PopularTVShowResponse>() {}.type
                    val tmdbResponse: PopularTVShowResponse = gson.fromJson(json, type)
                    Log.d("HomeMediaNetworkUtils", "Fetched ${tmdbResponse.results.size} popular TV shows")
                    callback(tmdbResponse.results, null)
                }
            }
        })
    }

    fun getUpcomingMovies(url: String, callback: (List<Movie>?, IOException?) -> Unit) {
        val request = Request.Builder()
            .url(url)
            .addHeader("accept", "application/json")
            .addHeader("Authorization", "Bearer $tmdbAcessToken")
            .build()

        client.newCall(request).enqueue(object : Callback {
            override fun onFailure(call: Call, e: IOException) {
                Log.e("HomeMediaNetworkUtils", "Error fetching upcoming movies", e)
                callback(null, e)
            }

            override fun onResponse(call: Call, response: Response) {
                response.use {
                    if (!it.isSuccessful) {
                        callback(null, IOException("Unexpected code $response"))
                        return
                    }

                    val json = it.body!!.string()
                    val type = object : TypeToken<UpcomingMoviesResponse>() {}.type
                    val tmdbResponse: UpcomingMoviesResponse = gson.fromJson(json, type)
                    Log.d("HomeMediaNetworkUtils", "Fetched ${tmdbResponse.results.size} upcoming movies")
                    callback(tmdbResponse.results, null)
                }
            }
        })
    }

    fun getAiringTodayShows(url: String, callback: (List<TVShow>?, IOException?) -> Unit) {
        val request = Request.Builder()
            .url(url)
            .addHeader("accept", "application/json")
            .addHeader("Authorization", "Bearer $tmdbAcessToken")
            .build()

        client.newCall(request).enqueue(object : Callback {
            override fun onFailure(call: Call, e: IOException) {
                Log.e("HomeMediaNetworkUtils", "Error fetching shows airing today", e)
                callback(null, e)
            }

            override fun onResponse(call: Call, response: Response) {
                response.use {
                    if (!it.isSuccessful) {
                        callback(null, IOException("Unexpected code $response"))
                        return
                    }

                    val json = it.body!!.string()
                    val type = object : TypeToken<AiringTodayShowResponse>() {}.type
                    val tmdbResponse: AiringTodayShowResponse = gson.fromJson(json, type)
                    Log.d("HomeMediaNetworkUtils", "Fetched ${tmdbResponse.results.size} shows airing today")
                    callback(tmdbResponse.results, null)
                }
            }
        })
    }
}

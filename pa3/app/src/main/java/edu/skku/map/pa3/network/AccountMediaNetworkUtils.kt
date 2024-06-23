package edu.skku.map.pa3.network

import okhttp3.*
import com.google.gson.Gson
import okhttp3.MediaType.Companion.toMediaTypeOrNull
import okhttp3.logging.HttpLoggingInterceptor
import java.io.IOException

object AccountMediaNetworkUtils {
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

    fun addToFavorites(mediaId: Int, mediaType: String, sessionId: String, accountId: String, callback: (Boolean, Exception?) -> Unit) {
        val url = "https://api.themoviedb.org/3/account/$accountId/favorite?session_id=$sessionId"
        val jsonBody = """
            {
                "media_type": "$mediaType",
                "media_id": $mediaId,
                "favorite": true
            }
        """.trimIndent()

        val mediaType = "application/json; charset=utf-8".toMediaTypeOrNull()
        val requestBody = RequestBody.create(mediaType, jsonBody)
        val request = Request.Builder()
            .url(url)
            .post(requestBody)
            .addHeader("accept", "application/json")
            .addHeader("content-type", "application/json")
            .addHeader("Authorization", "Bearer $tmdbAccessToken")
            .build()

        client.newCall(request).enqueue(object : Callback {
            override fun onFailure(call: Call, e: IOException) {
                callback(false, e)
            }

            override fun onResponse(call: Call, response: Response) {
                if (!response.isSuccessful) {
                    callback(false, IOException("Unexpected code $response"))
                } else {
                    callback(true, null)
                }
            }
        })
    }

    fun addToWatchlist(mediaId: Int, mediaType: String, sessionId: String, accountId: String, callback: (Boolean, Exception?) -> Unit) {
        val url = "https://api.themoviedb.org/3/account/$accountId/watchlist?session_id=$sessionId"
        val jsonBody = """
            {
                "media_type": "$mediaType",
                "media_id": $mediaId,
                "watchlist": true
            }
        """.trimIndent()

        val mediaType = "application/json; charset=utf-8".toMediaTypeOrNull()
        val requestBody = RequestBody.create(mediaType, jsonBody)
        val request = Request.Builder()
            .url(url)
            .post(requestBody)
            .addHeader("accept", "application/json")
            .addHeader("content-type", "application/json")
            .addHeader("Authorization", "Bearer $tmdbAccessToken")
            .build()

        client.newCall(request).enqueue(object : Callback {
            override fun onFailure(call: Call, e: IOException) {
                callback(false, e)
            }

            override fun onResponse(call: Call, response: Response) {
                if (!response.isSuccessful) {
                    callback(false, IOException("Unexpected code $response"))
                } else {
                    callback(true, null)
                }
            }
        })
    }
}

package edu.skku.map.pa3.network

import okhttp3.*
import com.google.gson.Gson
import okhttp3.logging.HttpLoggingInterceptor
import java.io.IOException

object AccountMediaNetworkUtils {
    private val client: OkHttpClient
    private val gson: Gson = Gson()
    private val tmdbAccessToken = "YOUR_ACCESS_TOKEN" // Replace with your actual token
    private val accountId = "21341108" // Replace with your actual account ID

    init {
        val logging = HttpLoggingInterceptor()
        logging.setLevel(HttpLoggingInterceptor.Level.BODY)

        client = OkHttpClient.Builder()
            .addInterceptor(logging)
            .build()
    }

//    fun addToFavorites(mediaId: Int, callback: (Boolean, Exception?) -> Unit) {
//        val url = "https://api.themoviedb.org/3/account/$accountId/favorite?api_key=$tmdbAccessToken"
//        val jsonBody = """
//            {
//                "media_type": "movie",
//                "media_id": $mediaId,
//                "favorite": true
//            }
//        """.trimIndent()
//
//        val requestBody = RequestBody.create(MediaType.get("application/json; charset=utf-8"), jsonBody)
//        val request = Request.Builder()
//            .url(url)
//            .post(requestBody)
//            .addHeader("accept", "application/json")
//            .addHeader("content-type", "application/json")
//            .addHeader("Authorization", "Bearer $tmdbAccessToken")
//            .build()
//
//        client.newCall(request).enqueue(object : Callback {
//            override fun onFailure(call: Call, e: IOException) {
//                callback(false, e)
//            }
//
//            override fun onResponse(call: Call, response: Response) {
//                if (!response.isSuccessful) {
//                    callback(false, IOException("Unexpected code $response"))
//                } else {
//                    callback(true, null)
//                }
//            }
//        })
//    }
//
//    fun addToWatchlist(mediaId: Int, callback: (Boolean, Exception?) -> Unit) {
//        val url = "https://api.themoviedb.org/3/account/$accountId/watchlist?api_key=$tmdbAccessToken"
//        val jsonBody = """
//            {
//                "media_type": "movie",
//                "media_id": $mediaId,
//                "watchlist": true
//            }
//        """.trimIndent()
//
//        val requestBody = RequestBody.create(MediaType.get("application/json; charset=utf-8"), jsonBody)
//        val request = Request.Builder()
//            .url(url)
//            .post(requestBody)
//            .addHeader("accept", "application/json")
//            .addHeader("content-type", "application/json")
//            .addHeader("Authorization", "Bearer $tmdbAccessToken")
//            .build()
//
//        client.newCall(request).enqueue(object : Callback {
//            override fun onFailure(call: Call, e: IOException) {
//                callback(false, e)
//            }
//
//            override fun onResponse(call: Call, response: Response) {
//                if (!response.isSuccessful) {
//                    callback(false, IOException("Unexpected code $response"))
//                } else {
//                    callback(true, null)
//                }
//            }
//        })
//    }
}

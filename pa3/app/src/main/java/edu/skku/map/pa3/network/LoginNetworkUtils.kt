package edu.skku.map.pa3.network

import android.util.Log
import com.google.gson.Gson
import com.google.gson.reflect.TypeToken
import edu.skku.map.pa3.models.*
import okhttp3.MediaType.Companion.toMediaTypeOrNull
import okhttp3.OkHttpClient
import okhttp3.Request
import okhttp3.RequestBody.Companion.toRequestBody
import okhttp3.logging.HttpLoggingInterceptor
import java.io.IOException

object LoginNetworkUtils {
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

    // get request token
    @Throws(IOException::class)
    fun getRequestToken(url: String): RequestTokenResponse {
        val request = Request.Builder()
            .url(url)
            .addHeader("accept", "application/json")
            .addHeader("Authorization", "Bearer $tmdbAcessToken")
            .build()

        client.newCall(request).execute().use { response ->
            if (!response.isSuccessful) throw IOException("Unexpected code $response")
            val json = response.body!!.string()

            val type = object : TypeToken<RequestTokenResponse>() {}.type
            val requestTokenResponse: RequestTokenResponse = gson.fromJson(json, type)
            Log.d("NetworkUtils", "Request Token: ${requestTokenResponse.request_token}")

            return requestTokenResponse
        }
    }


    // get authentication via login method
    @Throws(IOException::class)
    fun postRequest(url: String, jsonBody: String, username: String, password: String, requestToken: String): LoginResponse {
        val body = jsonBody.toRequestBody("application/json; charset=utf-8".toMediaTypeOrNull())

        val request = Request.Builder()
            .url(url)
            .post(body)
            .addHeader("accept", "application/json")
            .addHeader("content-type", "application/json")
            .addHeader("Authorization", "Bearer $tmdbAcessToken")
            .build()

        client.newCall(request).execute().use { response ->
            if (!response.isSuccessful) throw IOException("Unexpected code $response")
            return gson.fromJson(response.body!!.string(), LoginResponse::class.java)
        }
    }

    // get session id
    @Throws(IOException::class)
    fun getSessionID(url: String, jsonBody: String): SessionResponse {
        val body = jsonBody.toRequestBody("application/json; charset=utf-8".toMediaTypeOrNull())
        val request = Request.Builder()
            .url(url)
            .post(body)
            .addHeader("accept", "application/json")
            .addHeader("content-type", "application/json")
            .addHeader("Authorization", "Bearer $tmdbAcessToken")
            .build()

        client.newCall(request).execute().use { response ->
            if (!response.isSuccessful) throw IOException("Unexpected code $response")
            return gson.fromJson(response.body!!.string(), SessionResponse::class.java)
        }
    }

    // get account details
    @Throws(IOException::class)
    fun getAccountDetails(url: String): AccountDetail{
        val request = Request.Builder()
            .url(url)
            .addHeader("accept", "application/json")
            .addHeader("Authorization", "Bearer $tmdbAcessToken")
            .build()

        client.newCall(request).execute().use { response ->
            if (!response.isSuccessful) throw IOException("Unexpected code $response")
            return gson.fromJson(response.body!!.string(), AccountDetail::class.java)
        }
    }
}

package edu.skku.cs.week_10

import android.os.Bundle
import android.widget.Button
import android.widget.ListView
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import okhttp3.*
import com.google.gson.Gson
import com.google.gson.reflect.TypeToken
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import okhttp3.MediaType.Companion.toMediaType
import okhttp3.RequestBody.Companion.toRequestBody
import okio.IOException

class MainActivity : AppCompatActivity() {
    // use OkHttpClient class defined in Okhttp API
    private val client = OkHttpClient()
    private val gson = Gson()
    private val host = "https://api.tvmaze.com"
    private val path = "/search/shows?q="
    private val url = "/shows/:id/episodes"

    // list adapter used to update the actual screen based on the parsed result
    private lateinit var adapter: ListAdapter

    // get the search query from the user and search for the show
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val btn = findViewById<Button>(R.id.searchButton)
        val input = findViewById<TextView>(R.id.searchBar)
        val listView = findViewById<ListView>(R.id.listView)
        adapter = ListAdapter(this, arrayListOf())
        listView.adapter = adapter

        btn.setOnClickListener {
            val target = input.text.toString()
            searchShows(target)  // search for the show
        }
    }

    private fun searchShows(query: String) {
        // use Request class defined in Okhttp API
        val request = Request.Builder().url("$host$path$query").build()

        // Put request into queue for asynchronous networking
        client.newCall(request).enqueue(object : Callback {
            override fun onFailure(call: Call, e: IOException) {
                e.printStackTrace()
            }

            override fun onResponse(call: Call, response: Response) {
                response.use {
                    if (!response.isSuccessful) throw IOException("Unexpected code $response")

                    // response.body is a ResponseBody object, convert it to string
                    val str = response.body!!.string()

                    // Use Gson to parse JSON string to object
                    // TypeToken is used to get type of List<Show>
                    val showListType = object : TypeToken<List<Show>>() {}.type
                    // parse JSON string to List<Show>
                    val shows = gson.fromJson<List<Show>>(str, showListType)

                    if (shows.isNotEmpty()) {
                        val showId = shows[0].show?.id ?: return
                        fetchEpisodes(showId)
                    }
                }
            }
        })
    }

    private fun fetchEpisodes(showId: Int) {
        val request = Request.Builder().url("$host$url".replace(":id", showId.toString())).build()

        client.newCall(request).enqueue(object : Callback {
            override fun onFailure(call: Call, e: IOException) {
                e.printStackTrace()
            }

            override fun onResponse(call: Call, response: Response) {
                response.use {
                    if (!response.isSuccessful) throw IOException("Unexpected code $response")

                    val str = response.body!!.string()

                    // Use Gson to parse JSON string to object
                    // TypeToken is used to get type of List<Episode>
                    val episodeListType = object : TypeToken<List<Episode>>() {}.type
                    // parse JSON string to List<Episode>
                    val episodes = gson.fromJson<List<Episode>>(str, episodeListType)

                    // When changing textView, you must run it in Main(UI) Thread
                    CoroutineScope(Dispatchers.Main).launch {
                        adapter.data.clear()
                        adapter.data.addAll(episodes)
                        adapter.notifyDataSetChanged() // notify adapter that data has changed
                    }
                }
            }
        })
    }
}
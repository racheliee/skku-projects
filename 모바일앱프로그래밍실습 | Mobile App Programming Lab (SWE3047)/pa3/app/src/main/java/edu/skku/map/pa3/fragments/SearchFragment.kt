package edu.skku.map.pa3.fragments

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.*
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import edu.skku.map.pa3.MovieSearchAdapter
import edu.skku.map.pa3.R
import edu.skku.map.pa3.models.*
import edu.skku.map.pa3.network.*
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import java.io.IOException

class SearchFragment : Fragment() {

    private lateinit var searchEditText: EditText
    private lateinit var searchButton: Button
    private lateinit var searchResultsAdapter: MovieSearchAdapter

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.search_layout, container, false)

        searchEditText = view.findViewById(R.id.search_edit_text)
        searchButton = view.findViewById(R.id.search_button)
        val searchResultsList = view.findViewById<RecyclerView>(R.id.search_results_list)

        searchResultsAdapter = MovieSearchAdapter()
        searchResultsList.layoutManager = LinearLayoutManager(context)
        searchResultsList.adapter = searchResultsAdapter

        searchButton.setOnClickListener {
            performSearch()
        }

        return view
    }

    private fun performSearch() {
        val query = searchEditText.text.toString()

        if (query.isNotEmpty()) {
            val movieUrl = "https://api.themoviedb.org/3/search/movie?api_key=<YOUR_API_KEY>&query=$query"
            val tvShowUrl = "https://api.themoviedb.org/3/search/tv?api_key=<YOUR_API_KEY>&query=$query"

            CoroutineScope(Dispatchers.IO).launch {
                try {
                    val movies = SearchMediaNetworkUtils.searchMovies(movieUrl)
                    val tvShows = SearchMediaNetworkUtils.searchTVShows(tvShowUrl)

                    val searchResults = mutableListOf<Any>()
                    searchResults.addAll(movies)
                    searchResults.addAll(tvShows)

                    CoroutineScope(Dispatchers.Main).launch {
                        searchResultsAdapter.updateData(searchResults)
                    }
                } catch (e: IOException) {
                    CoroutineScope(Dispatchers.Main).launch {
                        e.printStackTrace()
                        Toast.makeText(context, "Failed to fetch search results", Toast.LENGTH_SHORT).show()
                    }
                }
            }
        } else {
            Toast.makeText(context, "Please enter a search query", Toast.LENGTH_SHORT).show()
        }
    }
}

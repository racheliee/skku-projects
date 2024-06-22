package edu.skku.map.pa3.ui

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.*
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import edu.skku.map.pa3.MediaAdapter
import edu.skku.map.pa3.MovieSearchAdapter
import edu.skku.map.pa3.R
import edu.skku.map.pa3.models.PopularMovies
import edu.skku.map.pa3.network.HomeMediaNetworkUtils
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import java.io.IOException

class SearchFragment : Fragment() {

    private lateinit var searchEditText: EditText
    private lateinit var searchButton: Button
    private lateinit var genreSpinner: Spinner
    private lateinit var searchResultsAdapter: MovieSearchAdapter

    private val genres = mapOf(
        "All" to "",
        "Action" to 28,
        "Comedy" to 35,
        "Drama" to 18,
        "Fantasy" to 14,
        "Horror" to 27,
        "Romance" to 10749,
        "Sci-Fi" to 878
    )

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.search_layout, container, false)

        searchEditText = view.findViewById(R.id.search_edit_text)
        searchButton = view.findViewById(R.id.search_button)
        genreSpinner = view.findViewById(R.id.genre_spinner)
        val searchResultsList = view.findViewById<RecyclerView>(R.id.search_results_list)

        searchResultsAdapter = MovieSearchAdapter()
        searchResultsList.layoutManager = LinearLayoutManager(context)
        searchResultsList.adapter = searchResultsAdapter

        val genreAdapter = ArrayAdapter(
            requireContext(),
            android.R.layout.simple_spinner_item,
            genres.keys.toList()
        )
        genreAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
        genreSpinner.adapter = genreAdapter

        searchButton.setOnClickListener {
            performSearch()
        }

        return view
    }


    private fun performSearch() {
        val query = searchEditText.text.toString()
        val selectedGenre = genres[genreSpinner.selectedItem.toString()]

        if (query.isNotEmpty()) {
            val genreFilter = selectedGenre?.let { "&with_genres=$it" } ?: ""
            val url = "https://api.themoviedb.org/3/search/movie?api_key=<YOUR_API_KEY>&query=$query$genreFilter"

            HomeMediaNetworkUtils.getPopularMovies(url) { searchResults, e ->
                CoroutineScope(Dispatchers.Main).launch {
                    if (e != null) {
                        e.printStackTrace()
                    } else {
                        searchResultsAdapter.updateData(searchResults ?: listOf())
                    }
                }
            }
        } else {
            Toast.makeText(context, "Please enter a search query", Toast.LENGTH_SHORT).show()
        }
    }


}

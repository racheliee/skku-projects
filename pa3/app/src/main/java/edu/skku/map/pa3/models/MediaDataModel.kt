package edu.skku.map.pa3.models

import com.google.gson.annotations.SerializedName
import java.io.Serializable

data class PopularMovies(
    val adult: Boolean,
    val backdrop_path: String?,
    val genre_ids: List<Int>,
    val id: Int,
    val original_language: String,
    val original_title: String,
    val overview: String,
    val popularity: Double,
    val poster_path: String,
    val release_date: String,
    val title: String,
    val video: Boolean,
    val vote_average: Double,
    val vote_count: Int
) : Serializable

data class MovieResponses(
    val page: Int,
    val results: List<PopularMovies>,
    val total_pages: Int,
    val total_results: Int
) : Serializable


data class PopularTVShows(
    val adult: Boolean,
    val backdrop_path: String,
    val genre_ids: List<Int>,
    val id: Int,
    val original_language: String,
    val original_name: String,
    val overview: String,
    val popularity: Double,
    val poster_path: String,
    val first_air_date: String,
    val name: String,
    val vote_average: Double,
    val vote_count: Int,
): Serializable

data class TVShowResponses(
    val page: Int,
    val results: List<PopularTVShows>,
    val total_pages: Int,
    val total_results: Int
): Serializable

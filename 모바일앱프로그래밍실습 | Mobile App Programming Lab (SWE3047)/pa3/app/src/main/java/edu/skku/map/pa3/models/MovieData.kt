package edu.skku.map.pa3.models

import java.io.Serializable

data class Movie(
    val adult: Boolean,
    val backdrop_path: String,
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
    val vote_count: Int,
) : Serializable


data class PopularMovieResponses(
    val page: Int,
    val results: List<Movie>,
    val total_pages: Int,
    val total_results: Int
) : Serializable


data class Dates(
    val maximum: String,
    val minimum: String
)

data class UpcomingMoviesResponse(
    val dates: Dates,
    val page: Int,
    val results: List<Movie>,
    val total_pages: Int,
    val total_results: Int
)

data class FavoriteMoviesResponse(
    val page: Int,
    val results: List<Movie>,
    val total_pages: Int,
    val total_results: Int
) : Serializable

data class MovieWatchlistResponse(
    val page: Int,
    val results: List<Movie>,
    val total_pages: Int,
    val total_results: Int
) : Serializable
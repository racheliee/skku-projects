package edu.skku.map.pa3.models

import java.io.Serializable

data class TVShow(
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

data class PopularTVShowResponse(
    val page: Int,
    val results: List<TVShow>,
    val total_pages: Int,
    val total_results: Int
): Serializable

data class AiringTodayShowResponse(
    val page: Int,
    val results: List<TVShow>,
    val total_pages: Int,
    val total_results: Int
)

data class FavoriteTVShowsResponse(
    val page: Int,
    val results: List<TVShow>,
    val total_pages: Int,
    val total_results: Int
) : Serializable

data class TVShowWatchlistResponse(
    val page: Int,
    val results: List<TVShow>,
    val total_pages: Int,
    val total_results: Int
) : Serializable


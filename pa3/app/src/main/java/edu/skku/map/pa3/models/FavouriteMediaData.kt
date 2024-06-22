
package edu.skku.map.pa3.models

import java.io.Serializable

data class FavoriteMovie(
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

data class FavoriteMoviesResponse(
    val page: Int,
    val results: List<FavoriteMovie>,
    val total_pages: Int,
    val total_results: Int
) : Serializable


data class FavoriteTVShow(
    val adult: Boolean,
    val backdrop_path: String,
    val genre_ids: List<Int>,
    val id: Int,
    val origin_country: List<String>,
    val original_language: String,
    val original_name: String,
    val overview: String,
    val popularity: Double,
    val poster_path: String,
    val first_air_date: String,
    val name: String,
    val vote_average: Double,
    val vote_count: Int,
) : Serializable

data class FavoriteTVShowsResponse(
    val page: Int,
    val results: List<FavoriteTVShow>,
    val total_pages: Int,
    val total_results: Int
) : Serializable
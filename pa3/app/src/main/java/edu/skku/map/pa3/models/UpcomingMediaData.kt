package edu.skku.map.pa3.models

data class UpcomingMovie(
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
    val vote_count: Int
)

data class Dates(
    val maximum: String,
    val minimum: String
)

data class UpcomingMoviesResponse(
    val dates: Dates,
    val page: Int,
    val results: List<UpcomingMovie>,
    val total_pages: Int,
    val total_results: Int
)

data class AiringTodayShow(
    val adult: Boolean,
    val backdrop_path: String?,
    val genre_ids: List<Int>,
    val id: Int,
    val origin_country: List<String>,
    val original_language: String,
    val original_name: String,
    val overview: String,
    val popularity: Double,
    val poster_path: String?,
    val first_air_date: String,
    val name: String,
    val vote_average: Double,
    val vote_count: Int
)

data class AiringTodayShowResponse(
    val page: Int,
    val results: List<AiringTodayShow>,
    val total_pages: Int,
    val total_results: Int
)
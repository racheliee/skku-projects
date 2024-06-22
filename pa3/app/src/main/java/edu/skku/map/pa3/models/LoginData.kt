package edu.skku.map.pa3.models

import java.io.Serializable

data class RequestTokenResponse(
    val success: Boolean,
    val expires_at: String,
    val request_token: String
)

data class LoginResponse(
    val success: Boolean,
    val expires_at: String,
    val request_token: String
)

data class SessionResponse(
    val success: Boolean,
    val session_id: String
)

data class AccountDetail(
    val id: Int,
    val iso_639_1: String,
    val iso_3166_1: String,
    val name: String,
    val include_adult: Boolean,
    val username: String
): Serializable
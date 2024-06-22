package edu.skku.map.pa3.models

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


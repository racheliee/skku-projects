package edu.skku.map.pa3

import android.content.Intent
import android.net.Uri
import android.os.Build
import android.os.Bundle
import android.util.Log
import android.view.View
import android.view.WindowInsets
import android.widget.Button
import android.widget.EditText
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.google.gson.Gson
import com.google.gson.reflect.TypeToken
import edu.skku.map.pa3.models.*
import edu.skku.map.pa3.network.LoginNetworkUtils
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import java.io.BufferedReader
import java.io.IOException

class LoginActivity : AppCompatActivity() {

    private val tmdbApiKey = "5e9e0af68c59eafeb71723f56a3862bc"
    private lateinit var requestToken: String
    private lateinit var sessionID: String
    private lateinit var currentUser: UserInfo

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.login_layout)

        // Hide the status bar
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            window.setDecorFitsSystemWindows(false)
            window.insetsController!!.hide(WindowInsets.Type.statusBars())
        } else {
            window.decorView.systemUiVisibility = View.SYSTEM_UI_FLAG_FULLSCREEN
        }

        // Initialize views after setting the content view
        val usernameEditText = findViewById<EditText>(R.id.username)
        val passwordEditText = findViewById<EditText>(R.id.password)
        val loginButton = findViewById<Button>(R.id.login_button)
        val errorMessageTextView = findViewById<TextView>(R.id.error_message)

        // Set up login logic
        loginButton.setOnClickListener {
            val username = usernameEditText.text.toString()
            val password = passwordEditText.text.toString()

            if (username.isEmpty() || password.isEmpty()) {
                errorMessageTextView.text = "Username or Password cannot be empty"
                errorMessageTextView.visibility = View.VISIBLE
            } else {
                CoroutineScope(Dispatchers.IO).launch {
                    createRequestToken(username, password, errorMessageTextView)
                }

//                val isLoginSuccessful = login(username, password, errorMessageTextView)
//                Toast.makeText(this, if(isLoginSuccessful) "Login Success" else "Login Failed", Toast.LENGTH_SHORT).show()
//                if(isLoginSuccessful) {
//                    val intent = Intent(this, MainPageActivity::class.java).apply {
//                        putExtra(MainPageActivity.USER_INFO, currentUser)
//                    }
//                    startActivity(intent)
//                }else{
//                    // if the login is failed, clear the id and password
//                    usernameEditText.setText("")
//                    passwordEditText.setText("")
//                }
            }
        }
    }

    private fun login(username: String, password: String, errorMessageTextView: TextView): Boolean {
        // read the user_info.txt
        val reader: BufferedReader = assets.open("user_info.txt").bufferedReader()
        val user_read = reader.use { it.readText() }

        // parse the user_info.txt
        val gson = Gson()
        val user_info_type = object : TypeToken<List<UserInfo>>() {}.type
        val user_info = gson.fromJson<List<UserInfo>>(user_read, user_info_type)

        // check if the user exists
        for(user in user_info) {
            // if the user exists, go to the next page
            if(user.username == username && user.password == password) {
                currentUser = user
                return true
            }
        }

        errorMessageTextView.text = "Invalid username or password. If you don't have an account, please sign up."
        errorMessageTextView.visibility = View.VISIBLE

        return false
    }


    // Create a request token for user authentication
    // these functions are not used in the final version (they connected to the tmdb user authentication)
    // Create a request token for user authentication
    private suspend fun createRequestToken(username: String, password: String, errorMessageTextView: TextView) {
        val url = "https://api.themoviedb.org/3/authentication/token/new?api_key=$tmdbApiKey"

        try {
            val requestTokenResponse: RequestTokenResponse = LoginNetworkUtils.getRequestToken(url)
            requestToken = requestTokenResponse.request_token
            Log.d("LoginActivity", "Request Token: $requestToken")

            withContext(Dispatchers.Main) {
                Toast.makeText(applicationContext, "Request Token: $requestToken", Toast.LENGTH_SHORT).show()
                createSessionWithLogin(username, password, errorMessageTextView)
            }
        } catch (e: IOException) {
            withContext(Dispatchers.Main) {
                errorMessageTextView.text = "Network error: ${e.message}"
                errorMessageTextView.visibility = View.VISIBLE
            }
        }
    }

    private fun createSessionWithLogin(username: String, password: String, errorMessageTextView: TextView) {
        val url = "https://api.themoviedb.org/3/authentication/token/validate_with_login"

        CoroutineScope(Dispatchers.IO).launch {
            try {
                val jsonBody = """
                    {
                        "username": "$username",
                        "password": "$password",
                        "request_token": "$requestToken"
                    }
                """.trimIndent()
                val response = LoginNetworkUtils.postRequest(url, jsonBody, username, password, requestToken)
                val sessionResponse: LoginResponse = response

                withContext(Dispatchers.Main) {
                    if (sessionResponse.success) { // successfully authenticated
                        Toast.makeText(this@LoginActivity, "Login verified!", Toast.LENGTH_SHORT).show()
                        createSession(errorMessageTextView)
                    } else {
                        errorMessageTextView.text = "Wrong username or password. Please try again."
                        errorMessageTextView.visibility = View.VISIBLE
                    }
                }
            } catch (e: IOException) {
                withContext(Dispatchers.Main) {
//                    errorMessageTextView.text = "Network error: ${e.message}"
                    errorMessageTextView.text = "Network error: Wrong username or password. Please try again."
                    errorMessageTextView.visibility = View.VISIBLE
                }
            }
        }
    }

    private fun createSession(errorMessageTextView: TextView) {
        val url = "https://api.themoviedb.org/3/authentication/session/new?api_key=$tmdbApiKey"

        CoroutineScope(Dispatchers.IO).launch {
            try {
                val jsonBody = """
                    {
                        "request_token": "$requestToken"
                    }
                """.trimIndent()
                val response = LoginNetworkUtils.getSessionID(url, jsonBody)
                val sessionResponse: SessionResponse = response

                withContext(Dispatchers.Main) {
                    if (sessionResponse.success) {
                        sessionID = sessionResponse.session_id
                        Log.d("LoginActivity", "Session ID: $sessionID")
                        Toast.makeText(this@LoginActivity, "Session ID: $sessionID", Toast.LENGTH_SHORT).show()

                        // go to the next page
                        val intent = Intent(this@LoginActivity, HomePageActivity::class.java).apply {
                            putExtra(HomePageActivity.SESSION_ID, sessionID)
                        }
                        startActivity(intent)
                    } else {
                        errorMessageTextView.text = "Failed to create session. Please try again."
                        errorMessageTextView.visibility = View.VISIBLE
                    }
                }
            } catch (e: IOException) {
                withContext(Dispatchers.Main) {
                    errorMessageTextView.text = "Network error: ${e.message}"
                    errorMessageTextView.visibility = View.VISIBLE
                }
            }
        }

    }

    // previously used to create session via web
    override fun onNewIntent(intent: Intent) {
        super.onNewIntent(intent)
        val data: Uri? = intent.data
        if (data != null && data.scheme == "yourappscheme" && data.host == "auth") {
            val errorMessageTextView = findViewById<TextView>(R.id.error_message)
//            createSessionWithLogin(errorMessageTextView)
        }
    }
}

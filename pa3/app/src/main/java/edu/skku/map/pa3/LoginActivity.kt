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
import edu.skku.map.pa3.models.*
import edu.skku.map.pa3.network.LoginNetworkUtils
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import java.io.IOException

class LoginActivity : AppCompatActivity() {
    private val tmdbApiKey = "5e9e0af68c59eafeb71723f56a3862bc"
    private lateinit var requestToken: String
    private lateinit var sessionID: String
    private lateinit var accountDetail: AccountDetail

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
            }
        }

        // set up links
        val signUpLink = findViewById<TextView>(R.id.sign_up_link)
        val forgotPasswordLink = findViewById<TextView>(R.id.forgot_password)
        val privacyPolicyLink = findViewById<TextView>(R.id.privacy_policy)
        val termsOfServiceLink = findViewById<TextView>(R.id.terms_of_service)

        // Set up link click listeners
        signUpLink.setOnClickListener(openWebPage("https://www.themoviedb.org/signup"))
        forgotPasswordLink.setOnClickListener(openWebPage("https://www.themoviedb.org/reset-password"))
        privacyPolicyLink.setOnClickListener(openWebPage("https://www.themoviedb.org/privacy-policy"))
        termsOfServiceLink.setOnClickListener(openWebPage("https://www.themoviedb.org/terms-of-use"))
    }

    private fun openWebPage(url: String): View.OnClickListener {
        return View.OnClickListener {
            val webpage: Uri = Uri.parse(url)
            val intent = Intent(Intent.ACTION_VIEW, webpage)
            if (intent.resolveActivity(packageManager) != null) {
                startActivity(intent)
            }
        }
    }

    // Create a request token for user authentication
    private suspend fun createRequestToken(username: String, password: String, errorMessageTextView: TextView) {
        val url = "https://api.themoviedb.org/3/authentication/token/new?api_key=$tmdbApiKey"

        try {
            val requestTokenResponse: RequestTokenResponse = LoginNetworkUtils.getRequestToken(url)
            requestToken = requestTokenResponse.request_token
            Log.d("LoginActivity", "Request Token: $requestToken")

            withContext(Dispatchers.Main) {
//                Toast.makeText(applicationContext, "Request Token: $requestToken", Toast.LENGTH_SHORT).show()
                createSessionWithLogin(username, password, errorMessageTextView)
            }
        } catch (e: IOException) {
            withContext(Dispatchers.Main) {
                errorMessageTextView.text = "Network error: ${e.message}"
                errorMessageTextView.visibility = View.VISIBLE
            }
        }
    }

    // Create a session with the request token and user credentials
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
//                        Toast.makeText(this@LoginActivity, "Login verified!", Toast.LENGTH_SHORT).show()
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

    // Create a session with the request token that has been authenticated
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
//                        Toast.makeText(this@LoginActivity, "Session ID: $sessionID", Toast.LENGTH_SHORT).show()

                        fetchAccountDetails() // get the account ID
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

    // Fetch the account details after creating the session
    private fun fetchAccountDetails() {
        val url = "https://api.themoviedb.org/3/account?api_key=$tmdbApiKey&session_id=$sessionID"

        CoroutineScope(Dispatchers.IO).launch {
            try {
                val response = LoginNetworkUtils.getAccountDetails(url)
                val accountResponse: AccountDetail = response

                withContext(Dispatchers.Main) {
                    if (accountResponse.id != -1) {
                        accountDetail = accountResponse
                        Log.d("LoginActivity", "Account ID: $accountDetail")
//                        Toast.makeText(this@LoginActivity, "Account ID: $accountDetail", Toast.LENGTH_SHORT).show()

                        // go to the next page
                        val intent = Intent(this@LoginActivity, MainActivity::class.java).apply {
                            putExtra(MainActivity.SESSION_ID, sessionID)
                            putExtra(MainActivity.ACCOUNT_DETAILS, accountDetail)
                        }
                        startActivity(intent)
                        finish()
                    } else {
                        Log.d("LoginActivity", "Failed to get account ID")
                    }
                }
            } catch (e: IOException) {
                withContext(Dispatchers.Main) {
                    Log.d("LoginActivity", "Network error: ${e.message}")
                }
            }
        }
    }

    override fun onResume() {
        super.onResume()
        hideStatusBar()
    }

    private fun hideStatusBar() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            window.setDecorFitsSystemWindows(false)
            window.insetsController?.hide(WindowInsets.Type.statusBars())
        } else {
            window.decorView.systemUiVisibility = View.SYSTEM_UI_FLAG_FULLSCREEN
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

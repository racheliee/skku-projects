package edu.skku.map.pa2

import android.content.Intent
import android.os.Bundle
import android.os.Parcelable
import android.widget.Button
import android.widget.EditText
import android.widget.ImageView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.google.gson.Gson
import com.google.gson.reflect.TypeToken
import java.io.BufferedReader

// this is the first page
// login the user
class MainActivity : AppCompatActivity() {
    private lateinit var currentUser: UserInfo

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // set the image for the title figure
        val title_figure = findViewById<ImageView>(R.id.titleFigureImageView)
        title_figure.setImageResource((R.drawable.title_figure))

        // get the id and password from the user if the button is clicked
        val login_button = findViewById<Button>(R.id.loginButton)
        val id_editText = findViewById<EditText>(R.id.idEditText)
        val password_editText = findViewById<EditText>(R.id.passwordEditText)

        login_button.setOnClickListener() {
            val success = login(id_editText.text.toString(), password_editText.text.toString())
            Toast.makeText(this, if(success) "Login Success" else "Login Failed", Toast.LENGTH_SHORT).show()

            // if the login is successful, go to the next page
            if(success){
                val intent = Intent(this, Activity1::class.java).apply {
                    putExtra(Activity1.USER_INFO,  currentUser)
                }
//                finish() // finish the current activity
                startActivity(intent)
            }else{
                // if the login is failed, clear the id and password
                id_editText.setText("")
                password_editText.setText("")
            }
        }
    }

    override fun onBackPressed() {
        // If the Main activity is the root activity, exit the application
        if (isTaskRoot) {
            finishAffinity()
        } else {
            super.onBackPressed() // Return to the previous activity
        }
    }

    fun login(id: String, password: String): Boolean {
        // read the user_info.txt
        val reader: BufferedReader = assets.open("user_info.txt").bufferedReader()
        val user_read = reader.use { it.readText() }

        // parse the user_info.txt
        // the user_info.txt is in json format
        val gson = Gson()
        val user_info_type = object : TypeToken<List<UserInfo>>() {}.type
        val user_info = gson.fromJson<List<UserInfo>>(user_read, user_info_type)

        // check if the user exists
        for(user in user_info) {
            if(user.id == id && user.passwd == password) {
                // if the user exists, go to the next page
                currentUser = user
                return true
            }
        }

        return false
    }
}
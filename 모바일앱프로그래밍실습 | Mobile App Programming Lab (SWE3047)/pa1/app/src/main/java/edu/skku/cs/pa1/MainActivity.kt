package edu.skku.cs.pa1

import android.os.Bundle
import android.widget.Button
import android.widget.EditText
import android.widget.ListView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import java.io.BufferedReader

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main)) { v, insets ->
            val systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom)
            insets
        }

        //read the word dictionary file
        val wordDict = ArrayList<String>()
        val reader: BufferedReader = assets.open("wordle_words.txt").bufferedReader()

        reader.useLines { lines -> lines.forEach { wordDict.add(it) } }

        // randomly select a word from the dictionary
        val random_index = (0..wordDict.size).random()
        val todaysWordle = wordDict[random_index].uppercase()
        Toast.makeText(
            applicationContext,
            "Today's Wordle: ${todaysWordle}",
            Toast.LENGTH_SHORT
        ).show()

        // keep track of the wordle guesses
        val wordleGuessList = ArrayList<String>()

        // keep track of the letters
        val greyList = ArrayList<String>()
        val greenList = ArrayList<String>()
        val yellowList = ArrayList<String>()

        val submitButton = findViewById<Button>(R.id.buttonSubmit)
        val wordleGuessEditText = findViewById<EditText>(R.id.editTextWordleGuess)

        //list adapters
        val greenListView = findViewById<ListView>(R.id.greenLetterListView)
        val greenLetterListAdapter = LetterListAdapter(applicationContext, greenList, 3)
        greenListView.adapter = greenLetterListAdapter

        val yellowListView = findViewById<ListView>(R.id.yellowLetterListView)
        val yellowLetterListAdapter = LetterListAdapter(applicationContext, yellowList, 2)
        yellowListView.adapter = yellowLetterListAdapter

        val greyListView = findViewById<ListView>(R.id.greyLetterListView)
        val greyLetterListAdapter = LetterListAdapter(applicationContext, greyList, 1)
        greyListView.adapter = greyLetterListAdapter

        // wordle list view adapter
        val wordleGuessListView = findViewById<ListView>(R.id.guessedWordlesListView)
        val wordleGuessAdapter = WordleGuessAdapter(applicationContext, wordleGuessList, todaysWordle, greenList, yellowList, greyList, greenLetterListAdapter, yellowLetterListAdapter, greyLetterListAdapter)
        wordleGuessListView.adapter = wordleGuessAdapter


        submitButton.setOnClickListener {
            val guessedWordle = wordleGuessEditText.text.toString()

            // the word exists in the dictionary
            if(guessedWordle in wordDict){
                wordleGuessList.add(guessedWordle.uppercase()) // add the new word to the list
                wordleGuessAdapter.notifyDataSetChanged()

                greenLetterListAdapter.notifyDataSetChanged()
                yellowLetterListAdapter.notifyDataSetChanged()
                greyLetterListAdapter.notifyDataSetChanged()

                wordleGuessEditText.text.clear() // clear the edit text
            }else{ // the word doesn't exist in the dictionary
                // show a toast message saying the word doesn't exist
                Toast.makeText(
                    applicationContext,
                    "Word '${guessedWordle}' not in dictionary ",
                    Toast.LENGTH_SHORT
                ).show()
            }
        }

    }
}
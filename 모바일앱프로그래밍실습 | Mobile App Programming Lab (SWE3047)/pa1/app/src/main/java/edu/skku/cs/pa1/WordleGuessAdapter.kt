package edu.skku.cs.pa1

import android.content.Context
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.BaseAdapter
import android.widget.TextView
import android.widget.Toast

// colors
val GREEN = "FF99F691".toLong(16).toInt()
val YELLOW = "FFFFE46F".toLong(16).toInt()
val GREY = "FF787C7E".toLong(16).toInt()
val BLACK = "FF000000".toLong(16).toInt()
val WHITE = "FFFFFFFF".toLong(16).toInt()

class WordleGuessAdapter(val context: Context, val wordle_guess: ArrayList<String>, val todays_wordle: String, val greenList: ArrayList<String>, val yellowList: ArrayList<String>, val greyList: ArrayList<String>, val greenLetterListAdapter: LetterListAdapter, val yellowLetterListAdapter: LetterListAdapter, val greyLetterListAdapter: LetterListAdapter): BaseAdapter() {
    override fun getCount(): Int {
        return wordle_guess.size
    }

    override fun getItem(position: Int): Any {
        return wordle_guess[position]
    }

    override fun getItemId(position: Int): Long {
        return 0
    }

    override fun getView(position: Int, convertView: View?, parent: ViewGroup?): View {
        val inflater = context.getSystemService(Context.LAYOUT_INFLATER_SERVICE) as LayoutInflater
        val generatedView = inflater.inflate(R.layout.item_wordle_guess, null)

        // convert today's wordle into a list
        val todaysWordleList = todays_wordle.toMutableList()
        // initialize a case list to 1 of length 5
        // green = 3, yellow = 2, grey = 1
        val caseList = MutableList(5) { 1 }

        // check for correct letters in correct position
        for(i in wordle_guess[position].indices){
            if(wordle_guess[position][i] == todaysWordleList[i]){
                caseList[i] = 3
                todaysWordleList[i] = '*' // mark the letter as used
            }
        }

        // check for correct letters in wrong position
        for(i in wordle_guess[position].indices){
            if(caseList[i] != 3 && todaysWordleList.contains(wordle_guess[position][i])){
                caseList[i] = 2
                todaysWordleList[todaysWordleList.indexOf(wordle_guess[position][i])] = '*'
            }
        }

        // add letters to their corresponding list
        for(i in wordle_guess[position].indices){
            var letter = wordle_guess[position][i].toString()

            var textViewName = "textViewLetter" + (i+1).toString()
            val wordleGuessLetterEditText = generatedView.findViewById<TextView>(context.resources.getIdentifier(textViewName, "id", context.packageName))

            wordleGuessLetterEditText.setText(wordle_guess[position][i].toString())

            if(caseList[i] == 3){
                wordleGuessLetterEditText.setBackgroundColor(GREEN)
                wordleGuessLetterEditText.setTextColor(BLACK)

                if(!greenList.contains(letter)){
                    greenList.add(wordle_guess[position][i].toString())
                    greenLetterListAdapter.notifyDataSetChanged()
                }

                if(yellowList.contains(letter)){
                    yellowList.remove(letter)
                    yellowLetterListAdapter.notifyDataSetChanged()
                }
            } else if(caseList[i] == 2){
                wordleGuessLetterEditText.setBackgroundColor(YELLOW)
                wordleGuessLetterEditText.setTextColor(BLACK)

                if(!yellowList.contains(letter) && !greenList.contains(letter)){
                    yellowList.add(wordle_guess[position][i].toString())
                    yellowLetterListAdapter.notifyDataSetChanged()
                }

            } else if (caseList[i] == 1){
                wordleGuessLetterEditText.setBackgroundColor(GREY)
                wordleGuessLetterEditText.setTextColor(WHITE)

                if(!greyList.contains(letter) && !greenList.contains(letter) && !yellowList.contains(letter)){
                    greyList.add(wordle_guess[position][i].toString())
                    greyLetterListAdapter.notifyDataSetChanged()
                }
            }
        }

        return generatedView
    }
}
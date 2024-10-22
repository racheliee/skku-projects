package edu.skku.cs.pa1

import android.content.Context
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.BaseAdapter
import android.widget.TextView

class LetterListAdapter(val context: Context, val letterList: ArrayList<String>, val case: Int): BaseAdapter() {
    override fun getCount(): Int {
        return letterList.size
    }

    override fun getItem(position: Int): Any {
        return 0
    }

    override fun getItemId(position: Int): Long {
        return 0
    }

    override fun getView(position: Int, convertView: View?, parent: ViewGroup?): View {
        val inflater = context.getSystemService(Context.LAYOUT_INFLATER_SERVICE) as LayoutInflater
        val generatedView = inflater.inflate(R.layout.item_letter, null)

        var textView = generatedView.findViewById<TextView>(R.id.textView)
        textView.text = letterList[position]

        if(case == 1){
            textView.setBackgroundColor(GREY)
            textView.setTextColor(WHITE)
        } else if(case == 2){
            textView.setBackgroundColor(YELLOW)
            textView.setTextColor(BLACK)
        } else if(case == 3){
            textView.setBackgroundColor(GREEN)
            textView.setTextColor(BLACK)
        }

        return generatedView
    }

}
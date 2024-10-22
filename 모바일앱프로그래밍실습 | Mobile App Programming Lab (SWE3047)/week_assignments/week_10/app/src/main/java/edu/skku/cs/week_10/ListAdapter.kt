package edu.skku.cs.week_10

import android.content.Context
import android.view.LayoutInflater
import android.widget.BaseAdapter
import android.view.View
import android.view.ViewGroup
import android.widget.TextView

class ListAdapter(val context: Context, val data: ArrayList<Episode>): BaseAdapter() {
    override fun getCount(): Int {
        return data.size
    }

    override fun getItem(position: Int): Any {
        return data.get(position)
    }

    override fun getItemId(position: Int): Long {
        return 0
    }

    override fun getView(position: Int, convertView: View?, parent: ViewGroup?): View {
        val inflater: LayoutInflater = LayoutInflater.from(context)
        val view: View = inflater.inflate(R.layout.episode_item, null)

        var seasonText = view.findViewById<TextView>(R.id.seasonText)
        var numberText = view.findViewById<TextView>(R.id.numberText)
        var nameText = view.findViewById<TextView>(R.id.nameText)
        var airdateText = view.findViewById<TextView>(R.id.airdateText)

        seasonText.setText(data[position].season.toString())
        numberText.setText(data[position].number.toString())
        nameText.setText(data[position].name)
        airdateText.setText(data[position].airdate)

        return view
    }
}
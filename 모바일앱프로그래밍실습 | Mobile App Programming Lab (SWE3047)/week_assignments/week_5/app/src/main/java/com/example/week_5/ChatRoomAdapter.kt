package com.example.week_5

import android.content.Context
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.BaseAdapter
import android.widget.ImageView
import android.widget.TextView
import org.w3c.dom.Text

class ChatRoomAdapter (val data: ArrayList<ChatRoom>, val context: Context): BaseAdapter() {
    override fun getCount(): Int {
        return data.size
    }

    override fun getItem(p0: Int): Any {
        return data[p0]
    }

    override fun getItemId(p0: Int): Long {
        return 0
    }

    override fun getView(p0: Int, p1: View?, p2: ViewGroup?): View {
        val inflater = context.getSystemService(Context.LAYOUT_INFLATER_SERVICE) as LayoutInflater
        val generatedView = inflater.inflate(R.layout.item_chatroom,null)

        val textViewName = generatedView.findViewById<TextView>(R.id.textViewRoomName)
        val textViewChat = generatedView.findViewById<TextView>(R.id.textViewRecentChat)
        val textViewGroupNumber = generatedView.findViewById<TextView>(R.id.textViewGroupSize)
        val textViewTime = generatedView.findViewById<TextView>(R.id.textViewLastTime)
        val imageViewThumbnail = generatedView.findViewById<ImageView>(R.id.imageViewProfile)
        val textViewUnreadMsgs = generatedView.findViewById<TextView>(R.id.textViewMsgCount)

        textViewName.text = data[p0].name
        textViewChat.text = data[p0].lastChat
        textViewTime.text = data[p0].lastTime
        textViewGroupNumber.text = "" + data[p0].groupNumber
        imageViewThumbnail.setImageResource(data[p0].thumbnail)
        textViewUnreadMsgs.text = "" + data[p0].unreadMessages

        if(data[p0].unreadMessages > 0 && data[p0].groupNumber == 1){
            textViewGroupNumber.text = ""
        }

        return generatedView
    }

}
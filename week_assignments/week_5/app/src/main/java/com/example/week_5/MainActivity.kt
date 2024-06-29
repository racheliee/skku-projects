package com.example.week_5

import android.os.Bundle
import android.widget.ListView
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.activity_main)
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main)) { v, insets ->
            val systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom)
            insets
        }

        val items = ArrayList<ChatRoom>()

        items.add(ChatRoom("Kakao T", "Please leave review.", R.drawable.ic_launcher_background, 1, "8:24 p.m.", 1))
        items.add(ChatRoom("SKKU Software", "Anybody taking MAP lecture? This week lab session is too hard. Can anybody give me the hint? I will be very happy if you help. If nobody help me, I will be very sad.", R.drawable.ic_launcher_background, 512, "7:15 p.m.", 128))
        items.add(ChatRoom("Brother", "Hey.", R.drawable.ic_launcher_background, 1, "4:21 p.m.", 12))
        items.add(ChatRoom("Family", "Emoji", R.drawable.ic_launcher_background, 4, "4:05 p.m.", 5))
        items.add(ChatRoom("Study group", "See you tomorrow!", R.drawable.ic_launcher_background, 5, "4:01 p.m.", 2))
        items.add(ChatRoom("Yogiyo", "How was the food?", R.drawable.ic_launcher_background, 1, "3:24 p.m.", 1))
        items.add(ChatRoom("lorem ipsum", "dolor", R.drawable.ic_launcher_background, 6, "2:22 p.m.", 123))
        items.add(ChatRoom("Placeholder", "Placeholder", R.drawable.ic_launcher_background, 12, "11:58 a.m.", 5))

        val myAdapter = ChatRoomAdapter(items, applicationContext)
        val listView = findViewById<ListView>(R.id.listViewChatRoom)
        listView.adapter = myAdapter

    }
}
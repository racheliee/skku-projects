package com.example.week_4

import android.content.Context
import android.os.Bundle
import android.view.LayoutInflater
import android.widget.Button
import android.widget.ImageView
import android.widget.LinearLayout
import android.widget.ListView
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.constraintlayout.widget.ConstraintSet.Layout
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // functionality one
        var linear_layout_1 = findViewById<LinearLayout>(R.id.linear_layout_1)
        var btn_top = findViewById<Button>(R.id.button)

        btn_top.setOnClickListener(){
            val layoutInflater: LayoutInflater = applicationContext.getSystemService(Context.LAYOUT_INFLATER_SERVICE) as LayoutInflater
            layoutInflater.inflate(R.layout.sub_layout, linear_layout_1, true)

            var img1 = findViewById<ImageView>(R.id.imageView)
            var img2 = findViewById<ImageView>(R.id.imageView2)

            img1.setImageResource(R.drawable.bbq)
            img2.setImageResource(R.drawable.bhc)
        }

        var pizza_btn = findViewById<Button>(R.id.pizza_btn)
        var hamburger_btn = findViewById<Button>(R.id.hamburg_btn)
        var chicken_btn = findViewById<Button>(R.id.chicken_btn)

        pizza_btn.setOnClickListener(){
            var items = arrayListOf<Restaurant>()
            items.add(Restaurant(R.drawable.domino, "domino's pizza"))
            items.add(Restaurant(R.drawable.pizzahut, "pizza hut"))
            items.add(Restaurant(R.drawable.pizzanarachickengongju, "pizza nara chicken gongju"))


            val listAdaptor = CustomAdapter(this, items)
            var mainList = findViewById<ListView>(R.id.listview)
            mainList.adapter = listAdaptor
        }

        hamburger_btn.setOnClickListener(){
            var items = arrayListOf<Restaurant>()
            items.add(Restaurant(R.drawable.mcdonalds, "mcdonalds"))
            items.add(Restaurant(R.drawable.burgerking, "burger king"))
            items.add(Restaurant(R.drawable.lotteria, "lotteria"))
            items.add(Restaurant(R.drawable.momstouch, "mom's touch"))


            val listAdaptor = CustomAdapter(this, items)
            var mainList = findViewById<ListView>(R.id.listview)
            mainList.adapter = listAdaptor
        }

        chicken_btn.setOnClickListener(){
            var items = arrayListOf<Restaurant>()
            items.add(Restaurant(R.drawable.bhc, "BHC"))
            items.add(Restaurant(R.drawable.bbq, "BBQ king"))
            items.add(Restaurant(R.drawable.pizzanarachickengongju, "pizza nara chicken gongju"))
            items.add(Restaurant(R.drawable.goobne, "goobne"))


            val listAdaptor = CustomAdapter(this, items)
            var mainList = findViewById<ListView>(R.id.listview)
            mainList.adapter = listAdaptor
        }


    }
}
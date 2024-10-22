package edu.skku.map.pa2

import android.content.Context
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.BaseAdapter
import android.widget.ImageView
import android.widget.TextView

class RestaurantAdapter(val restaurants: ArrayList<RestaurantInfo>, val context: Context): BaseAdapter() {
    override fun getCount(): Int {
        return restaurants.size
    }

    override fun getItem(position: Int): Any {
        return restaurants[position]
    }

    override fun getItemId(position: Int): Long {
        return 0
    }

    override fun getView(position: Int, convertView: View?, parent: ViewGroup?): View {
        val inflater = context.getSystemService(Context.LAYOUT_INFLATER_SERVICE) as LayoutInflater

        val view = inflater.inflate(R.layout.restaurant_info_layout, null)

        val restaurant = restaurants[position]

        val imageView = view.findViewById<ImageView>(R.id.menuImageView)
        val nameTextView = view.findViewById<TextView>(R.id.menu_menuNameTextView)
        val locRateTextView = view.findViewById<TextView>(R.id.restaurant_locRateTextView)
        val hoursTextView = view.findViewById<TextView>(R.id.menu_priceTextView)

        // Populate views with restaurant information
        nameTextView.text = restaurant.restaurant
        locRateTextView.text = "${restaurant.location}/${restaurant.rating}"
        hoursTextView.text = "${restaurant.openingHours.open} - ${restaurant.openingHours.close}"

        // Set image
        val resourceId = if (restaurant.image.isNotEmpty() && context.resources.getIdentifier(restaurant.image, "drawable", context.packageName) != 0) {
            context.resources.getIdentifier(restaurant.image, "drawable", context.packageName)
        } else {
            R.drawable.default_img
        }
        imageView.setImageResource(resourceId)

        return view
    }
}
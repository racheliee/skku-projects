package edu.skku.map.pa2

import android.view.View
import android.view.ViewGroup
import android.widget.BaseAdapter
import android.content.Context
import android.util.Log
import android.view.LayoutInflater
import android.widget.ImageView
import android.widget.TextView
import com.google.gson.Gson
import com.google.gson.reflect.TypeToken
import java.io.BufferedReader

class ReservationAdapter(val reservations: ArrayList<Reserved>, val context: Context): BaseAdapter() {
    override fun getCount(): Int {
        return reservations.size
    }

    override fun getItem(position: Int): Any {
        return reservations[position]
    }

    override fun getItemId(position: Int): Long {
        return 0
    }

    override fun getView(p0: Int, convertView: View?, parent: ViewGroup?): View {
        val inflater = context.getSystemService(Context.LAYOUT_INFLATER_SERVICE) as LayoutInflater
        val view = inflater.inflate(R.layout.reservation_info_layout, null)

        // get the reservation item
        val reservation_item = getItem(p0) as Reserved

        // get the current restaurant
        var curr_restaurant = getRestaurant(reservation_item.restaurant_id)
        if(curr_restaurant == null){ // return empty view if the restaurant is not found
            Log.d("ReservationAdapter", "Restaurant not found for reservation: $reservation_item")
            return view
        }

        // set image
        val image = view.findViewById<ImageView>(R.id.menuImageView)
        val resourceId = if (curr_restaurant.image.isNotEmpty() && context.resources.getIdentifier(curr_restaurant.image, "drawable", context.packageName) != 0){
            context.resources.getIdentifier(curr_restaurant.image, "drawable", context.packageName)
        } else {
            // Use default image if restaurant image is not provided
            R.drawable.default_img
        }
        image.setImageResource(resourceId)

        // set text
        val restaurant_name_text = view.findViewById<TextView>(R.id.menu_menuNameTextView)
        val number_of_people_text = view.findViewById<TextView>(R.id.restaurant_locRateTextView)
        val date_text = view.findViewById<TextView>(R.id.reservation_dateTextView)
        val time_text = view.findViewById<TextView>(R.id.menu_priceTextView)

        restaurant_name_text.text = curr_restaurant.restaurant
        number_of_people_text.text = reservation_item.number_of_people.toString()
        date_text.text = reservation_item.date.toString()
        time_text.text = reservation_item.time.toString()


        return view
    }

    fun getRestaurant(id: Int): RestaurantInfo? {
        val reader : BufferedReader = context.assets.open("restaurant_info.txt").bufferedReader()
        val res_read = reader.use { it.readText() }

        val gson = Gson()
        val restaurant_info_type = object: TypeToken<List<RestaurantInfo>>() {}.type
        val restaurant_info = gson.fromJson<List<RestaurantInfo>>(res_read, restaurant_info_type)

        for(restaurant in restaurant_info){
            if(restaurant.id == id){
//                Toast.makeText(context, "Restaurant found: ${restaurant.name}", Toast.LENGTH_SHORT).show()
                return restaurant
            }
        }

        return null
    }
}
package edu.skku.map.pa2

import android.content.Context
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.BaseAdapter

class MenuAdapter(private val menuList: List<MenuInfo>, private val context: Context) : BaseAdapter() {
    override fun getCount(): Int {
        return menuList.size
    }

    override fun getItem(position: Int): Any {
        return menuList[position]
    }

    override fun getItemId(position: Int): Long {
        return 0
    }

    override fun getView(position: Int, convertView: View?, parent: ViewGroup?): View {
        val inflater = context.getSystemService(Context.LAYOUT_INFLATER_SERVICE) as LayoutInflater
        val view = inflater.inflate(R.layout.menu_info_layout, null)

        val menu = menuList[position]

        val imageView = view.findViewById<android.widget.ImageView>(R.id.menuImageView)
        val nameTextView = view.findViewById<android.widget.TextView>(R.id.menu_menuNameTextView)
        val priceTextView = view.findViewById<android.widget.TextView>(R.id.menu_priceTextView)

        val resourceId = if (menu.image.isNotEmpty() && context.resources.getIdentifier(menu.image, "drawable", context.packageName) != 0) {
            context.resources.getIdentifier(menu.image, "drawable", context.packageName)
        } else {
            R.drawable.default_img
        }
        imageView.setImageResource(resourceId)

        nameTextView.text = menu.name
        priceTextView.text = menu.price.toString()

        return view
    }
}
package edu.skku.map.pa2

import java.io.Serializable

data class RestaurantInfo(
    val id: Int,
    val restaurant: String,
    val type: String,
    val location: String,
    val rating: String,
    val image: String,
    val description: String,
    val openingHours: OpeningHoursInfo,
    val menu: List<MenuInfo>
) : Serializable

data class OpeningHoursInfo(
    val open: String,
    val close: String
) : Serializable

data class MenuInfo(
    val id: Int,
    val name: String,
    val price: Int,
    val image: String
) : Serializable


/**
 *  {
 *         "id": 1,
 *         "restaurant": "McDonald's",
 *         "type": "Fast Food",
 *         "location": "New York",
 *         "rating": "4.5/5",
 *         "image": "mcdonalds_thumbnail",
 *         "description": "McDonald",
 *         "openingHours" : {
 *             "open" : "10:00",
 *             "close" : "22:00"
 *         },
 *         "Menu": [
 *             {
 *                 "id": 1,
 *                 "name": "burger",
 *                 "price": 10,
 *                 "image": "menu_burger"
 *             },
 *             {
 *                 "id": 2,
 *                 "name": "fries",
 *                 "price": 5,
 *                 "image": "menu_fries"
 *             },
 *             {
 *                 "id": 3,
 *                 "name": "coke",
 *                 "price": 3,
 *                 "image": "menu_coke"
 *             }
 *         ]
 *     }
 */
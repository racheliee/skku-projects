package edu.skku.cs.week_9

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.Button
import android.widget.TextView
import kotlinx.coroutines.*
import java.text.SimpleDateFormat
import java.util.Calendar

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val button = findViewById<Button>(R.id.button)
        val textView = findViewById<TextView>(R.id.textView)
        button.setOnClickListener {
            // create a new coroutine to run the long task
            CoroutineScope(Dispatchers.Main).launch {
                textView.text = longTask().await()
            }
        }
    }

    fun longTask() = CoroutineScope(Dispatchers.Default).async{
        Thread.sleep(5000)
        var total_points = 0
        var inner_points = 0
        val textView = findViewById<TextView>(R.id.textView)

        for(i in 1..100){
            for(j in 1..1000000) {
                val x = Math.random()
                val y = Math.random()
                if(x*x + y*y <= 1){
                    inner_points++
                }
                total_points++
            }

            // use a coroutine to update the UI (Dispatchers.Main)
            CoroutineScope(Dispatchers.Main).launch(){
                val pi = 4.0 * inner_points.toDouble() / total_points.toDouble()
                textView.setText("Done ${i}%... \ncurrent estimation: ${String.format("%.6f", pi)}")
            }
        }

        val pi = 4.0 * inner_points.toDouble() / total_points.toDouble()
        return@async "Done! \nfinal estimation: ${String.format("%.6f", pi)}" // return the final estimation
    }
}
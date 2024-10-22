package edu.skku.map.pa3.fragments

import android.content.Intent
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.ImageView
import android.widget.TextView
import androidx.fragment.app.Fragment
import edu.skku.map.pa3.LoginActivity
import edu.skku.map.pa3.R
import edu.skku.map.pa3.models.AccountDetail

class ProfileFragment : Fragment() {

    companion object {
        private const val ARG_ACCOUNT_DETAILS = "account_details"

        fun newInstance(accountDetails: AccountDetail) = ProfileFragment().apply {
            arguments = Bundle().apply {
                putSerializable(ARG_ACCOUNT_DETAILS, accountDetails)
            }
        }
    }

    private lateinit var accountDetails: AccountDetail

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        arguments?.let {
            accountDetails = it.getSerializable(ARG_ACCOUNT_DETAILS) as AccountDetail
        }
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.profile_layout, container, false)

        // Set up profile information
        val profileName = view.findViewById<TextView>(R.id.profile_name)
        val profileUsername = view.findViewById<TextView>(R.id.profile_username)
        val profileImage = view.findViewById<ImageView>(R.id.profile_image)
        val logoutButton = view.findViewById<Button>(R.id.logout_button)

        profileName.text = accountDetails.name
        profileUsername.text = accountDetails.username
        profileImage.setImageResource(R.drawable.profile) // Replace with your default profile picture resource

        // Set up logout button action
        logoutButton.setOnClickListener {
            // Handle logout action
            activity?.let {
                val intent = Intent(it, LoginActivity::class.java)
                intent.flags = Intent.FLAG_ACTIVITY_NEW_TASK or Intent.FLAG_ACTIVITY_CLEAR_TASK
                startActivity(intent)
                it.finish()
            }
        }

        return view
    }
}

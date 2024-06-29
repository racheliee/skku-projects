# Project Assignment 2: Filmdr

## Overview
Filmdr is a mobile application developed for the Mobile App Programming personal project. This app allows users to browse, search, and view detailed information about popular movies and TV shows using The Movie Database (TMDB) API. Users can also add movies and TV shows to their favorites or watchlist for easy access.

## Objective
- **Implement Movie and TV Show Browsing**: Create a mobile application where users can browse and search for movies and TV shows, view detailed information, and manage their favorites and watchlist.
- **UI Requirements**:
  - Use modern Android UI components like RecyclerView and ConstraintLayout.
  - Ensure a user-friendly interface with clear navigation and intuitive interactions.

## Key Features

### Functionalities
- **Browse Popular Movies and TV Shows**: Display lists of popular movies and TV shows fetched from the TMDB API.
- **Detailed Information**: Allow users to view detailed information about selected movies and TV shows.
- **Search Functionality**: Enable users to search for movies and TV shows by name.
- **Favorites and Watchlist Management**: Allow users to add or remove movies and TV shows to/from their favorites and watchlist.

## Activities
- **MainActivity**: Hosts the primary navigation and fragments for Home, Profile, Search, and Favorites.
- **MovieActivity**: Displays detailed information about a selected movie, allowing users to add it from favorites or watchlist.
- **TVShowActivity**: Displays detailed information about a selected TV show, allowing users to add it from favorites or watchlist.
- **MovieActivity**: Displays detailed information about a selected movie, allowing users to add it from favorites or watchlist.

## API Integration
The app communicates with the TMDB API to fetch data about movies and TV shows. The API requests are handled using OkHttp, and responses are parsed with Gson.

The API Keys are stored securely and are not exposed in the codebase. To use this app, you need to 
1. provide your own TMDB API key in `LoginActivity.kt` where it says `YOUR API KEY`
2. provide your access token to in all the files under the `network` package where it says `YOUR ACCESS TOKEN`

## How to Use
1. **Browse Popular Content**: Launch the app and navigate to the Home tab to see popular movies and TV shows.
2. **View Details**: Click on any movie or TV show to view detailed information.
3. **Search**: Navigate to the Search tab, enter a movie or TV show name, and view the search results.
4. **Manage Favorites/Watchlist**: In the details view, use buttons to add or remove the movie or TV show from your favorites or watchlist.
5. **Random Movie Recommendation**: Click the random movie button to get a random movie recommendation.
6. **Profile**: View your profile and manage your favorites and watchlist.
7. **Connect with TMDB Account**: Uses your TMDB account to manage favorites and watchlist.

### Additional Notes on Login feature
- Login works as follows:
1. Make a request to the TMDB API to get a request token.
2. Instead of opening the WebView, user credential is used to authenticate the user.
3. The request token is used to get the session ID.
4. Once the session ID is obtained, the user information such as user ID can be fetched.

## Screenshots
![Login](readme_images/filmdr_1.png)
![Main Page](readme_images/filmdr_2.png)
![Random Movie](readme_images/filmdr_3.png)
![Search](readme_images/filmdr_4.png)
![Detail](readme_images/filmdr_5.png)
![Favourites](readme_images/filmdr_6.png)
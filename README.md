Music System Overview This project is a music system written in C that uses the SDL (Simple DirectMedia Layer) library to play music files. It allows users to manage songs, albums, artists, playlists, and more.

in the Folder text files you can find some default data, with 3 songs just for the demonstration.

Features Play music files. Manage songs, albums, artists, and playlists. User management. Repository patterns for song and playlist storage. Installation To run this project, you'll need to have SDL installed on your system. Follow these steps to set up the project:

Install SDL(3):

On Linux: sudo apt-get install libsdl3-dev On MacOS: brew install sdl3 On Windows: Download the SDL development libraries from the SDL website and follow the installation instructions.

Download Songs: Ensure you have songs downloaded and named according to the code specified in the Song class.

Usage Prepare the Files Folder: Ensure you have songs downloaded and placed in a folder named files. This folder should also contain example data for songs, users, artists, playlists, and more.

User Interface:

The application will prompt you with various options to manage songs, albums, artists, playlists, and users.

Classes Song Represents a music track. Contains attributes such as title, artist, and Song code.

Album Represents a collection of songs. Contains attributes like album name, artist, and list of songs.

AlbumManager Manages multiple albums. Provides functionalities to add, remove, and retrieve albums.

SongRepository Handles storage and retrieval of songs. Provides methods to add, remove, and search for songs.

Playlist Represents a playlist containing multiple songs. Includes methods to add and remove songs from the playlist.

PlaylistRepo Manages multiple playlists. Provides functionalities to add, remove, and retrieve playlists.

Artist Represents an artist with attributes such as name and a short bio.

User Represents a user of the music system. Contains attributes like username, and user-specific playlists, songs.

Dependencies SDL3: Simple DirectMedia Layer library for multimedia handling. SDL3 MIXER. Contributing Contributions are welcome! Please fork the repository and submit pull requests for any enhancements or bug fixes.

Fork the Project Create your Feature Branch (git checkout -b feature/AmazingFeature) Commit your Changes (git commit -m 'Add some AmazingFeature') Push to the Branch (git push origin feature/AmazingFeature) Open a Pull Request

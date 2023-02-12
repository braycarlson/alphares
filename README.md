## alphares
alphares is a tool for Windows to save custom settings in Fortnite.

I made this tool when it was possible to customize the resolution via a configuration file, however, Fortnite disallowed it sometime after the release of this project. I have noticed alphares appearing in YouTube and TikTok videos, so it still might be possible to change the resolution of Fortnite, but I am unable confirm it as I no longer play the game. If you want to download this tool, then please do so from GitHub as it is the safest option. Go to the "Download" section below for instructions.

![alphares](https://raw.githubusercontent.com/braycarlson/alphares/master/alphares/gfx/alphares.png)

## Download
* Go to [Releases](https://github.com/braycarlson/alphares/releases/tag/1.2.1)
* Under **Assets**, click "alphares_1.2.1_x64.exe"
* Rename the executable to "alphares"
* Move it to an accessible location (i.e., Documents, Desktop, etc.)

## How to use
* Open the "alphares" executable
* Change the `Width` and `Height` to a desired resolution
* Click on "Read-only" so the box is checked
* Press "Apply"
* You *might* also have to change settings in the Nvidia or AMD control panel

## Popular resolution(s)
* 1600x1080
* 1440x1080
* 1280x1024
* 1154x1080
* 1080x1080

## Note
* You must have launched **Fortnite** at least once, and allowed the game to automatically detect the best settings or have saved custom settings in-game before running alphares.
* You need to set a custom resolution in your Nvidia or AMD Control Panel as well as in alphares to use a custom resolution in Fortnite.
* alphares can set/unset the read-only attribute for the Fortnite configuration file. It is set by default, however, you will not be allowed to make changes to settings in-game while it is set. If it is unset, Fortnite **might** overwrite your custom settings when you update the game.
* You must set the FPS field to `0` in alphares, if you want to have a **Unlimited** framerate in Fortnite.

## To-do
* Draw an outline on the `Apply` button when focused
* Customize and change the color of the `BS_GROUPBOX` rectangle
* Redraw the radio buttons

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details

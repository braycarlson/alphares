## alphares
alphares is a tool for Windows to save custom settings in Fortnite.

I made this tool when it was possible to customize Fortnite's resolution via a configuration file, however, Fortnite changed that sometime after the release of this project. As of late, I have noticed alphares appearing in YouTube and TikTok videos, so it might be possible to change the resolution of Fortnite again, but I am unable confirm it, as I no longer play the game. If you want to download this tool, then please do so from [here](https://github.com/braycarlson/alphares/releases/tag/1.2.3) as it is the safest option.

![alphares](https://raw.githubusercontent.com/braycarlson/alphares/master/alphares/asset/alphares.png)

## Download
* Go to [Releases](https://github.com/braycarlson/alphares/releases/tag/1.2.3)
* Under **Assets**, click "alphares.exe"
* Rename the executable to "alphares"
* Move it to an accessible location (i.e., Documents, Desktop, etc.)

## How to use
* Open the "alphares" executable
* Change the `Width` and `Height` to a desired resolution
* Click on "Read-only" so the box is checked
* Press "Apply"
* You *might* also have to change the settings in the NVIDIA or AMD Control Panel

## Uninstall

1. Close Fortnite.
2. Press: <kbd>Win</kbd> + <kbd>R</kbd> to open a "Run" window.
3. In the box, type: `%localappdata%` and press <kbd>Enter</kbd>.
4. From there, you will have to navigate to: "FortniteGame/Saved/Config/WindowsClient/GameUserSettings.ini".
5. Right-click on "GameUserSettings.ini", then select "Properties".
6. A dialog box will open where you can uncheck the "Read-only" flag, and then press "Apply".
7. Delete the `alphares` folder and executable.

## Popular resolution(s)
* 1600x1080
* 1440x1080
* 1280x1024
* 1154x1080
* 1080x1080

## Support
If you want to support what I do, consider making a donation:
* [PayPal](https://www.paypal.com/donate/?business=4RVQB7W25BNJA&no_recurring=0&currency_code=CAD)
* [BuyMeACoffee](https://www.buymeacoffee.com/braycarlson)
* [Ko-fi](https://ko-fi.com/braycarlson)

## Note
* You must have launched Fortnite at least once, and allowed the game to detect the best settings or saved custom settings in-game before running alphares.
* alphares can set/unset the configuration file's read-only attribute. This is to prevent Fortnite from updating the configuration file and resetting the custom resolution. As such, you will be unable to save settings in-game while it is set. You must uncheck the read-only box and press "Apply" to save changes.
* If you want to have an uncapped framerate in Fortnite, you must set the FPS field to `0` in alphares.

## To-do
* Draw an outline on the `Apply` button when focused
* Customize and change the color of the `BS_GROUPBOX` rectangle
* Redraw the radio buttons

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details

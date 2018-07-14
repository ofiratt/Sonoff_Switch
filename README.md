# Sonoff_Switch

This is a very simple application to control Sonoff Basic, here you can learn how to:
1. Connect to WIFI
2. Connect to MQTT server
3. Subscribe to MQTT topic and active/deactivate the switch
4. Publish MQTT topic - send the switch status
5. Control ESP led on/off and show the switch status

## Installation

I use [Platformio](https://platformio.org) with [VSCode](https://code.visualstudio.com) but you can use [Arduino IDE](https://www.arduino.cc/en/Main/Software) if you like.
[you can learn how to use VSCode with Platformio here](http://docs.platformio.org/en/latest/ide/vscode.html)
Then do the following:
 - Fill the user_config.h with your data
 - Build your code (ctrl + shift + p / command + shift + p) then type "Platformio: Build"
 - Uplode to Sonoff device  (ctrl + shift + p / command + shift + p) then type "Platformio: Upload"
 - Check the log  (ctrl + shift + p / command + shift + p) then type "Platformio: Serial Monitor"
 
 
## License

Sonoff_Switch is freely distributable under the terms of the MIT license

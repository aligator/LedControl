# LED Control

This is a firmware to control a led matrix using an esp8266 over MQTT with auto discovery for HomeAssistant.

The base topic and all other connection confiurations can be configured in the `Config.h`.
Just copy over the `Config.example.h`.

The available topics are:  
`<MQTT_TOPIC>/set/brightness`  `0` - `255`
`<MQTT_TOPIC>/set/fps`  `-1` - `500` (`-1` means no restriction -> as fast as the main loop runs)  
`<MQTT_TOPIC>/set/switch` `ON` / `OFF`
`<MQTT_TOPIC>/set/text/text` any string  
`<MQTT_TOPIC>/set/text/scroll` `ON` / `OFF` If the text should scroll. The text is just cut off, if it is too long.
`<MQTT_TOPIC>/set/blink` `ON` / `OFF` If the blink mode is enabled  

The color can be changed using these topics:  
`<MQTT_TOPIC>/add/color`  
  Add a color to the current color array  
`<MQTT_TOPIC>/set/color`  
  Set and replace the current colors with a specific color (array)  

The colors can have the following formats:
* Numeric `0 - 16777215` (int representation of 0x000000 - 0xFFFFFF)
* JSON array: `[255, 255, 255]`  
* HomeAssistant default format: `rrr,ggg,bbb` e.g. `0,100,255`

The `<MQTT_TOPIC>/set/color` also supports json arrays:
* Numeric `[0, 5555454, 16777215]`
* JSON array: `[[255, 255, 255], [255, 255, 255]]`  
* HomeAssistant default format: `["rrr,ggg,bbb", "rrr,ggg,bbb"]` e.g. `["0,100,255", "255,0,0"]`

# LED Control

This is a firmware to control a led matrix using an esp8266 over MQTT with auto discovery for HomeAssistant.

The base topic and all other connection confiurations can be configured in the `Config.h`.
Just copy over the `Config.example.h`.

The available topics are:  
`<MQTT_TOPIC>/set/brightness`  `0` - `255`
`<MQTT_TOPIC>/set/fps`  `-1` - `500` (`-1` means no restriction -> as fast as the main loop runs)  
`<MQTT_TOPIC>/set/switch` `ON` / `OFF`

To get the current status:  
`<MQTT_TOPIC>/get/brightness` `0` - `255`  
`<MQTT_TOPIC>/get/switch` `ON` / `OFF`  

The color can be changed using these topics:  

`<MQTT_TOPIC>/add/color`  
  Add a color to the current color array  
* Numeric 0 - 16777215 (int representation of 0x000000 - 0xFFFFFF)
* JSON array: `[255, 255, 255]`  


`<MQTT_TOPIC>/set/color`   
  Set and replace the current colors with a specific color (array)  
* Numeric `0 - 16777215` (int representation of 0x000000 - 0xFFFFFF)
* JSON array: `[[255, 255, 255], [255, 255, 255]]`  
  Use this to set more than one color.
* To support the home assistant default for lamp entities: `rrr,ggg,bbb` e.g. `0,100,255`



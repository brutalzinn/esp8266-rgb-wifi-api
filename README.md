This repository is to be used with esp 8266 or generic micro controllers with wifi integration.

With this project, you can send post requests with hue and saturation data and control rgb led state.

This project will be used with [zoom-monitor-googlesheet](https://github.com/brutalzinn/zoom-monitor-googlesheets) 

The api url is http://192.168.0.25/LED (POST REQUEST)

Example of json request:

~~~json
{
    "on":true, //led state
    "hue":270, // hue to control led color
    "sat":250 //saturation
}
~~~
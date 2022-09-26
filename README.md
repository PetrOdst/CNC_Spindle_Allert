# CNC-Spindle-Allert

Pause your GRBL Arduino CNCShield task once spindle is colliding or malfuncioning

When I was testing and tuning my first CNC I realized that even tiny mistake, malfunction or misalignment sooner or later leads to disaster damaging entire machine if its not spotted soon enough. I also observe that absolute majority of thees “catastrophic scenarios” can be either diagnosed be extremely high current on spindle or caused by extremely low. High current means that it costs more effert for spindle to move then was expected. That shows ist probably on wrong place, it can by caused by missed steppes, axis malfunction, defective G-code , ect.. anyway Its reason to pause the task before something worse happens. on the ather hand, when current is lower then when spindle goes idle it probably not spinning at all and it is just meter of time when it crash in to the stock and again, time to hit the pause button.

I used my hull current sensor to get readings from/on the spindle , couple of relays to start/stop the spindle and emulate button pushing and ESP32 based Lilygo T4 with TFT display and couple of buttons to control it all. I am shure that there more elegant ways to get this idea working, but I had to go out with what I got lying around my workshop.

## Parts list:
  * [LilyGO TTGO T-4 ESP32 2.2 TFT ](https://www.laskakit.cz/lilygo-ttgo-t-4-esp32-2-2-tft-psarm-8m-ip5306--vyvojova-deska/)
  * [2 channel relay module](https://www.laskakit.cz/2-kanaly-rele-modul--5vdc-250vac-10a/) or 2x [single channel relay module](https://www.laskakit.cz/1-kanal-5v-rele-modul--low-level--250vac-10a/)
  * [hall effect current sensor](https://www.laskakit.cz/arduino-proudovy-senzor-20a-acs712/)

## Wiring:
Current sensor goes to ESPs pin 35 Spindle operating relay to pin 19 and pause button emulating relay to pin 24.  

Made by [Hodinový Maker](http://www.hodinovy-maker.cz)

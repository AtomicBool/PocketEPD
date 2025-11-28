<img src="./images/img1.jpg" width="200"/>
<img src="./images/img2.png" width="300"/>

# Software
located in `./POKEINK/` 
open it via `STM32CubeIDE`
# PCB 
`./PCB/ProPrj_PocketEPD.epro` was made with EDA provided `lceda.cn`

`./PCB/Export_Altium.zip` was exported from LCEDA, but please do check if it(both schematic and layout) matchs `./PCB/Schematic.png` if you use AltiumDesigner

# Features
right now, it supports 8x16 english character, lines, progress bar(circular), and circle(reuse the progress function)

# TODO
1. function that interpret serial input and draw things see `currentPlan.txt`
2. drawRectangle()

# Parts
EPD https://www.good-display.cn/product/515.html
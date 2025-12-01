名字受到了PocketLCD的启发，不过估计知道稚晖君的大多是国人就没写到另一个Readme里

# Software
在 `./POKEINK/`， 用 `STM32CubeIDE` 打开

# PCB 
`./PCB/ProPrj_PocketEPD.epro` 用的lceda

`./PCB/Export_Altium.zip` 是导出给 AltiumDesigner的，但是还务必确保和原理图图片一致，我电脑没装AD所以看不了

# Features
8x16字符， 线段, 圆环进度条（如果内径设置为0就是画实心圆）
通过一种类似脚本语言的方式绘制 `guide.txt`是通讯协议，`testCMDs`是实例（也是python脚本默认调用的）
具体写在英文ReadME里面了因为修改实在是太不方便 这个README用于大体描述

# TODO
不在此README更新

# Parts
墨水屏淘宝上买的瑕疵屏 10元一个 官方的文档链接 https://www.good-display.cn/product/515.html
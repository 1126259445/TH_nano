# 关于TH_nano开源软件
本项目是一个桌面级的微信时钟可以图形形式显示当前天气状态以及时间和本地的温湿度情况，整体机身小巧精致，成本低廉，软硬件在此开源与大家分享
![222](https://user-images.githubusercontent.com/63451966/166114332-369f18e3-8f99-441e-8acf-1c1610f73f7c.png)

## 硬件准备
硬件材料 ： ESP8266-12F; 0.96寸OLED屏（I2C总线）；DHT11温湿度传感器；按键一个
连接如下：
| ESP8266_IO | 功能       |             |
|------------|----------|-------------|
| GPIO0      | 按键检测     | 和boot下载按键复用 |
| GPIO5      | DHT11    | 单总线温湿度传感器  |
| GPIO14     | I2C_SCL | oled屏时钟线    |
| GPIO2      | I2C_SDA  | oled屏数据线    |

![444](https://user-images.githubusercontent.com/63451966/166114370-e0205312-bff2-491d-857f-667aad72d7e2.png)
## 软件方案
TH_nano 具备本地的 **温湿度采集，时间显示，当前天气** 情况获取，所有数据实时显示在OLED屏上，时间和天气情况有网络通过HTTP请求获取；温湿度由DHT11 1s采集一次
1. 天气情况向知心天气获取 [天气示例](http://api.seniverse.com/v3/weather/now.json?key=SJG1TE67YcBqU7D9X&location=shenzhen&language=en&unit=c) 具体可到知心天气官网查看更多使用方法[之心天气官网](http://www.seniverse.com/login)
2. 时间向网络请求 [时间示例](http://quan.suning.com/getSysTime.do)
3. 按键接在了GPIO0上和BOOT功能复用，下载程序时需要按住按键再下载程序，本硬件方案以搭载一键下载电路，可直接运行下载命令一键下载。运行时通过长按直到指示灯快闪时可进入smartconfig智能配网，通过安信可官房小程序即可完成配网（当然也可以通过网页来配网后边会介绍）

## 开发环境
基础工程仓库地址： https://gitee.com/xuhongv/AiThinkerProjectForESP.git

使用UBUNTU1804虚拟机搭建开发环境具体可参考安信可官方文档
[LINUX下SDK开发环境搭建](http://aiclouds3.0-docs.aithinker.com/#/2.device/configuration_linux)
本方案基于ESP8266的RTOS SDK3.0版本开发而来,在xClouds开源架构下进行二次开发，下边时关于xClouds 开源架构的介绍

-------------------------------------

# 关于  xClouds 开源架构

&nbsp;&nbsp;&nbsp;&nbsp; 当我们日复一日年复一年的搬砖的时候，你是否曾想过提升一下开发效率，如果一个  **IOT智能家具物联网**  模板的项目摆在你的面前，你还会选择自己搭架构么？

 &nbsp;&nbsp;&nbsp;&nbsp; 但是搭建出一个好的架构并非易事，有多少人愿意选择去做，还有多少人选择努力去做好，可能寥寥无几，但是你今天看到的，正是你所想要的，一个真正能解决你开发新项目时最大痛点的架构工程，你不需要再麻木 ``Copy`` 原有旧项目的代码，只需改动少量代码就能得到想要的效果，你会发现开发新项目其实是一件很快乐的事；

&nbsp;&nbsp;&nbsp;&nbsp; 很幸运的是，在2018年底时候，我已经自学打通了天猫精灵音箱和小爱同学音箱同时控制一个 ESP8266，那时候我就开始专研服务器开发，自学攻破了数据库、php语言，再到了微信公众号的生态，我打通了这一整个的控制；

&nbsp;&nbsp;&nbsp;&nbsp;  直到目前为止，我依然在走这条道路，但是随着开源其趋势不断影响，很多组织或个人奋力地做一些开源，源源不断的架构和代码封装，加快了我们程序员开发的效率，比如前端的三大框架的 **Angular**，**React**，**Vue**：


&nbsp;&nbsp;&nbsp;&nbsp;  国内AI智能音箱一大崛起，也有很多优秀的企业一步一步崛起，像涂鸦就是典型的例子，一站式开发，对于很多小型家电传统公司接入AI音箱等智能控制，无疑就是一个 “菩萨” 般存在；

&nbsp;&nbsp;&nbsp;&nbsp;  话说回来，类似这种平台，对于我们开发者有什么值得学习的地方呢？ 最熟悉宣传语就是：**全球部署，一接入可对接多语音平台。**

&nbsp;&nbsp;&nbsp;&nbsp;  于是乎，我有个大胆的想法能否单独做这样的架构出来并开源出来：

- **跨平台、开发快、全开源**的宗旨；
- 微信公众号内 `airkiss` 配网和 `MQTT` 控制；
- 微信小程序内 `smartConfig` 配网和 `MQTT` 控制；
- 设备端要求：支持 `airkiss` 配网和 `MQTT` 协议即可；
- 服务器端：有微信公众号业务和各云平台对接的业务，支持但不限于 天猫精灵、小爱同学、小度音箱、Alexa音箱等；
- 扩展性强，支持用户二次开发，可私定义协议；
- 三端开源：设备端、服务器端、前端；

&nbsp;&nbsp;&nbsp;&nbsp;  现在，我有能力地公开这个架构，我想对于那些想学系统化的，或者电子diy爱好者，抑或是在校大学生，可参考学习，我的愿景就是：

&nbsp;&nbsp;&nbsp;&nbsp; **xClouds能在国内物联网一枝独秀，助国内物联网发展；**

&nbsp;&nbsp;&nbsp;&nbsp; **但愿人长久，搬砖不再有！**



# xClouds-devices for  ESP8266  开发框架


&nbsp;&nbsp;&nbsp;&nbsp;  认识我的人都知道，我和乐鑫 ESP8266 芯片有一个解不开的缘分，我通过此芯片认识很多志同道合的的小伙伴，也让我逐渐地找到了自己的人生价值，所以，我会以自己擅长的 Wi-Fi模块二次开发，带领大家快速体验和商业化使用此物美价廉的芯片；


&nbsp;&nbsp;&nbsp;&nbsp;  本框架基于**ESP8266_RTOS_SDK v3.0** 上二次开发，采用C语言开发，并非 **arduino** 开发；

## 例程简介

* [1.SmartConfig_AirKiss_To_MQTT](./Ai-examples/1.SmartConfig_AirKiss_To_MQTT) : 简单模板，实现 smartconfig 一键配网 和 微信公众号 airkiss 配网，并实现连接 MQTT 服务器的通讯实现上下发数据的例子；

---

## 开发入门

### 环境搭建

&nbsp;&nbsp;&nbsp;&nbsp;  因为此为基于 RTOS 系统上开发，需要一定的C基础或 freeRtos 开发基础，强烈建议使用 Linux 开发；

* [Windows](https://aithinker.blog.csdn.net/article/details/104390508)
* [Linux](https://xuhong.blog.csdn.net/article/details/104736261)

&nbsp;&nbsp;&nbsp;&nbsp;然后跑起 demo， 比如 [1.SmartConfig_AirKiss_To_MQTT](./Ai-examples/1.SmartConfig_AirKiss_To_MQTT)

```
cd /Ai-examples/1.SmartConfig_AirKiss_To_MQTT
make menuconfig
make -j8 flash 
make monitor
```

# 感谢：

- PHP微信对接：https://github.com/zoujingli/WeChatDeveloper
- PHP Oauth2.0：https://github.com/bshaffer/oauth2-server-php
- PHP 框架：http://www.thinkphp.cn
- 乐鑫物联网操作系统：https://github.com/espressif/esp-idf

项目遵循协议： [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)

服务器开源地址：https://github.com/xuhongv/xClouds-php

设备端开源地址：https://github.com/xuhongv/xClouds-device

**1、额外说明，架构中提到的对公司或组织的观点，如有争议，请联系我；**

**2、架构中涉及到的技术点，我会一一公布出来以表感谢；**

**3、同时，也欢迎大家支持我，或一起壮大这个框架，奉献你代码项目；**

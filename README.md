# MAA-Remote-Console
一个基于 ESP8266 的《明日方舟》MAA 助手远程控制面板。带有UI、Token 登录验证、Webhook 通知接收，以及状态实时反馈功能。

## ✨ 功能特性标识符前缀

- 📱 **局域网网页控制**：手机/电脑浏览器直接访问 ESP8266 即可控制 MAA 开启/停止。
- 🔐 **多用户 Token 验证**：内置轻量级登录系统，防止局域网内其他人乱点。
- 🐱 **高颜值 UI**：二次元风格磨砂玻璃（Glassmorphism）卡片、动态背景、左下角萌宠小猫。
- 📡 **端点自动生成**：网页直接显示 `getTask` 和 `reportStatus` 的完整地址，一键复制。
- 📬 **Webhook 通知接收**：支持接收 MAA 的外部通知并显示在网页日志中。
- 🖼️ **自定义头像**：使用 LittleFS 文件系统，可随时更换控制台头像。

## 🛠️ 硬件与依赖

- **开发板**：ESP8266 (NodeMCU, Wemos D1 mini 等)
- **开发环境**：[Arduino IDE](https://www.arduino.cc/en/software) (建议 2.x 版本)
- **依赖库**：
  - `ESP8266WiFi` (板载包自带)
  - `ESP8266WebServer` (板载包自带)
  - `LittleFS` (需要自行下载)
  - [ArduinoJson](https://arduinojson.org/) (通过库管理器安装 6.x 或 7.x)
- **插件**：[arduino-littlefs-upload](https://github.com/earlephilhower/arduino-littlefs-upload/releases) (用于上传头像图片到 ESP8266)

## 🚀 快速开始

### 1. 编译与烧录

1. 克隆或下载本项目。
2. 用 Arduino IDE 打开 `MAA_Controller.ino`。
3. 头像需要看LittleFS的使用说明（如果没有上传会变成E）
4. 找到代码中的 **配置区**，修改你的 WiFi 信息和登录账号等：
   ```cpp
   const char* WIFI_SSID = "你的WiFi名称";
   const char* WIFI_PASSWORD = "你的WiFi密码";
   ---- 服务器端口 ----
   const int SERVER_PORT = 80（可以更改但是maa也需要调整）;
   ---- 串口波特率 ----
   const long SERIAL_BAUD = 115200;（一般情况不要更改）
   ---- 会话配置 ----
   const int MAX_SESSIONS = 3;（同时登入人数）
   const unsigned long SESSION_TIMEOUT_MS = 30UL * 60UL * 1000UL;（登入验证超时设置）
   // ---- MAA 标识符前缀 ----
   const char* MAA_ID_PREFIX = "Enpty-";（标识符前缀可以随意更改）
   // ...
   User users[] = {
     {"admin", "admin123", "", true } // 用户名, 密码
   };

### 2. 上传 LittleFS (头像)

1. 在项目目录下确保有 data 文件夹，里面放一张名为 avatar.jpg 的图片。

2. 在 Arduino IDE 中，点击 工具 -> ESP8266 LittleFS Data Upload。(注意：上传时请关闭串口监视器)

### 3. 访问控制台

1. 打开串口监视器（波特率 115200），查看 ESP8266 获取的局域网 IP。

2. 浏览器访问该 IP，输入账号密码登录。

### 3. MAA 配置指南

登录网页后，你会看到两串地址。

1. MAA 远程控制：在 MAA 设置 -> 远程控制中，填入网页上显示的两个地址，并填写网页上生成的 用户标识符。

2. MAA 外部通知 (Webhook)：在 MAA 设置 -> 通知设置 -> 添加推送 -> 自定义 Webhook：

-URL 填入：http://你的ESP8266_IP/webhook
-消息体模板填入：{"title": "{title}", "content": "{content}"}

###声明：
请遵守知识共享 署名-非商业性使用-相同方式共享 4.0 国际Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License (CC BY-NC-ND 4.0)
不允许商业用 如果有人商业用可以联系ENPTY@qq.com
###你可以自由地：

分享 - 在任何媒介或格式下复制并分发本素材。

改编 - 对本素材进行重混、转换或依据本素材进行再创作。

只要遵守下述条款的规定，授权人将不能撤回你使用本创作的自由。

###唯须遵循以以下条件：

署名 - 你必须予以明确说明，并提供指向本许可证的链接，同时明确说明相较于原始版本创作是否经过修改。你可以采用恰当的形式表述相关信息，但不得以任何方式暗示授权人已为你的使用行为背书。

非商业性使用 - 你不得将本素材用于商业用途。

相同方式共享 - 如果你对本素材进行了重混、转换，或依据本素材进行再创作，你必须采用与源素材相同的许可协议来分发你的创作。

不得增加额外限制 - 你不能增设任何法律限制或是技术限制，来限制他人进行本许可证已经允许的行为。

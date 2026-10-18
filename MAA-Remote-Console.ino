#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include "maa_page.h"

// ===== 配置区 =====

// ---- WiFi 配置 ----
const char* WIFI_SSID     = "你的WiFi名称";
const char* WIFI_PASSWORD = "你的WiFi密码";

// ---- 服务器端口 ----
const int SERVER_PORT = 80;

// ---- 串口波特率 ----
const long SERIAL_BAUD = 115200;

// ---- 会话配置 ----
const int MAX_SESSIONS = 3;
const unsigned long SESSION_TIMEOUT_MS = 30UL * 60UL * 1000UL;

// ---- MAA 标识符前缀 ----
const char* MAA_ID_PREFIX = "Enpty-";

// ---- 用户表（用户名 / 密码 / 是否管理员）----
struct User {
  String username;
  String password;
  String maaUser;
  bool   isAdmin;
};

User users[] = {
  {"admin", "admin123", "", true } ,
  {"usERWDFW", "paFSFSSEF", "", false}
};

const int USER_COUNT = sizeof(users) / sizeof(users[0]);

// ============================================================
// ===================== 配置区结束 ===========================
// ============================================================

ESP8266WebServer server(SERVER_PORT);

struct UserSession {
  String token;
  int    userIndex;
  unsigned long createdAt;
  bool   active;
};
UserSession sessions[MAX_SESSIONS];

bool maaOn = false;
bool hasPendingCommand = false;
String pendingCommandType = "";
String lastCommandType = "";
int activeUserIndex = -1;

String lastTaskId = "";
unsigned long taskCounter = 0;

String lastReportTask = "";
String lastReportStatus = "";
unsigned long lastReportTime = 0;

// 普通的系统日志
String logMsg1 = "等待操作...";
String logMsg2 = "";
String logMsg3 = "";

unsigned long lastSessionClean = 0;

// ---- Webhook 通知相关 ----
String lastWebhookTitle = "";
String lastWebhookContent = "";
unsigned long lastWebhookTime = 0;
int webhookCount = 0;

// ===================== 工具函数声明 =====================
void addLog(String msg);

// ===================== 工具函数 =====================

String genTaskId() {
  taskCounter++;
  return String(ESP.getChipId(), HEX) + "-" + String(taskCounter) + "-" +
         String(random(0xFFFF), HEX) + "-" + String(millis(), HEX);
}

String genToken() {
  String t = "";
  t.reserve(40);
  for (int i = 0; i < 4; i++) {
    t += String(random(0x1000000), HEX);
    if (i < 3) t += "-";
  }
  return t;
}

void generateMaaUsers() {
  for (int i = 0; i < USER_COUNT; i++) {
    users[i].maaUser = String(MAA_ID_PREFIX) + users[i].username + "-" + String(ESP.getChipId(), HEX);
  }
}

void addLog(String msg) {
  logMsg3 = logMsg2;
  logMsg2 = logMsg1;
  unsigned long sec = millis() / 1000;
  char t[20];
  snprintf(t, sizeof(t), "%02u:%02u:%02u",
           (unsigned)((sec / 3600) % 24), (unsigned)((sec / 60) % 60), (unsigned)(sec % 60));
  logMsg1 = "[" + String(t) + "] " + msg;
}

void sendJson(int code, const String& body) {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(code, "application/json", body);
}

int findSession(const String& tk) {
  if (tk == "") return -1;
  for (int i = 0; i < MAX_SESSIONS; i++) {
    if (!sessions[i].active || sessions[i].token != tk) continue;
    if (millis() - sessions[i].createdAt > SESSION_TIMEOUT_MS) {
      sessions[i].active = false;
      return -1;
    }
    return i;
  }
  return -1;
}

int createSession(int ui) {
  for (int i = 0; i < MAX_SESSIONS; i++) {
    if (!sessions[i].active) {
      sessions[i].token = genToken();
      sessions[i].userIndex = ui;
      sessions[i].createdAt = millis();
      sessions[i].active = true;
      return i;
    }
  }
  int o = 0;
  for (int i = 1; i < MAX_SESSIONS; i++)
    if (sessions[i].createdAt < sessions[o].createdAt) o = i;
  sessions[o].token = genToken();
  sessions[o].userIndex = ui;
  sessions[o].createdAt = millis();
  sessions[o].active = true;
  return o;
}

void destroySession(const String& tk) {
  for (int i = 0; i < MAX_SESSIONS; i++)
    if (sessions[i].active && sessions[i].token == tk) {
      sessions[i].active = false;
      return;
    }
}

void cleanExpiredSessions() {
  unsigned long now = millis();
  for (int i = 0; i < MAX_SESSIONS; i++) {
    if (sessions[i].active && (now - sessions[i].createdAt > SESSION_TIMEOUT_MS)) {
      sessions[i].active = false;
    }
  }
}

int findUserByName(const String& n) {
  for (int i = 0; i < USER_COUNT; i++)
    if (users[i].username == n) return i;
  return -1;
}
int findUserByMaaUser(const String& m) {
  for (int i = 0; i < USER_COUNT; i++)
    if (users[i].maaUser == m) return i;
  return -1;
}

int currentUser() {
  String tk = server.arg("token");
  int s = findSession(tk);
  return s < 0 ? -1 : sessions[s].userIndex;
}

// ===================== 静态文件 =====================

void handleStaticFile(const String& path, const String& contentType) {
  if (!LittleFS.exists(path)) {
    server.send(404, "text/plain", "File not found");
    return;
  }
  File file = LittleFS.open(path, "r");
  server.streamFile(file, contentType);
  file.close();
}

// ===================== Webhook 通知处理 =====================

void handleWebhook() {
  if (!server.hasArg("plain")) {
    server.send(400, "text/plain", "No body");
    return;
  }

  String body = server.arg("plain");
  Serial.println("[Webhook] 收到 MAA 通知: " + body);

  DynamicJsonDocument doc(4096);
  DeserializationError err = deserializeJson(doc, body);

  if (err) {
    Serial.println("[Webhook] JSON 解析失败: " + String(err.c_str()));
    server.send(200, "text/plain", "OK (parse error)");
    return;
  }

  String title = doc["title"] | "MAA 通知";
  String content = doc["content"] | "";

  if (content == "") {
    if (doc.containsKey("body")) content = doc["body"].as<String>();
    else if (doc.containsKey("message")) content = doc["message"].as<String>();
    else if (doc.containsKey("text")) content = doc["text"].as<String>();
  }

  lastWebhookTitle = title;
  lastWebhookContent = content;
  lastWebhookTime = millis();
  webhookCount++;

  addLog("MAA 通知: " + title);
  if (content.length() > 0) {
    if (content.length() > 80) {
      addLog("  " + content.substring(0, 80) + "...");
    } else {
      addLog("  " + content);
    }
  }

  server.send(200, "text/plain", "OK");
}

// ===================== 页面路由 =====================

void handleRoot() {
  server.send_P(200, "text/html; charset=utf-8", PAGE_HTML);
}

void handleLoginPost() {
  String u = server.arg("username");
  String p = server.arg("password");
  int ui = findUserByName(u);
  if (ui < 0 || users[ui].password != p) {
    addLog("登录失败: " + u);
    server.send(401, "text/plain", "登录失败");
    return;
  }
  int si = createSession(ui);
  addLog("用户 " + u + " 已登录");
  server.send(200, "text/plain", sessions[si].token);
}

void handleLogout() {
  String tk = server.arg("token");
  int s = findSession(tk);
  if (s >= 0) addLog("用户 " + users[sessions[s].userIndex].username + " 已退出");
  destroySession(tk);
  server.send(200, "text/plain", "OK");
}

void handleMaaState() {
  int ui = currentUser();
  if (ui < 0) { server.send(401, "text/plain", "未登录"); return; }
  DynamicJsonDocument doc(1024);
  doc["on"]       = maaOn;
  doc["log1"]     = logMsg1;
  doc["log2"]     = logMsg2;
  doc["log3"]     = logMsg3;

  doc["maaUser"]  = users[ui].maaUser;
  doc["username"] = users[ui].username;
  doc["isAdmin"]  = users[ui].isAdmin;
  doc["ip"]       = WiFi.localIP().toString();
  String out; serializeJson(doc, out);
  sendJson(200, out);
}

void handleMaaSet() {
  int ui = currentUser();
  if (ui < 0) { server.send(401, "text/plain", "未登录"); return; }
  String s = server.arg("state");
  if (s != "on" && s != "off") { server.send(400, "text/plain", "参数错误"); return; }
  activeUserIndex = ui;
  if (s == "on") {
    maaOn = true; pendingCommandType = "LinkStart"; hasPendingCommand = true;
    addLog("用户 " + users[ui].username + " 请求开启一键长草");
  } else {
    maaOn = false; pendingCommandType = "StopTask"; hasPendingCommand = true;
    addLog("用户 " + users[ui].username + " 请求停止当前任务");
  }
  server.send(200, "text/plain", "OK");
}

void handleAdminData() {
  int ui = currentUser();
  if (ui < 0 || !users[ui].isAdmin) { server.send(403, "text/plain", "无权限"); return; }

  DynamicJsonDocument doc(3072);
  unsigned long sec = millis() / 1000;
  doc["ip"]     = WiFi.localIP().toString();
  doc["uptime"] = String(sec / 3600) + "h " + String((sec / 60) % 60) + "m " + String(sec % 60) + "s";
  doc["heap"]   = String(ESP.getFreeHeap() / 1024) + " KB";

  String u = "";
  for (int i = 0; i < USER_COUNT; i++) {
    u += "• " + users[i].username + (users[i].isAdmin ? " [管理员]" : "") + "\n";
    u += "  标识符: " + users[i].maaUser + "\n";
  }
  doc["users"] = u;

  int online = 0; String ss = "";
  for (int i = 0; i < MAX_SESSIONS; i++) {
    if (sessions[i].active && millis() - sessions[i].createdAt < SESSION_TIMEOUT_MS) {
      online++;
      ss += "• " + users[sessions[i].userIndex].username + "\n";
    }
  }
  if (ss == "") ss = "无\n";
  doc["online"]   = online;
  doc["sessions"] = ss;

  String au = "无";
  if (activeUserIndex >= 0) au = users[activeUserIndex].username;
  doc["activeUser"] = au;
  doc["on"] = maaOn;
  doc["logs"] = logMsg1 + "\n" + logMsg2 + "\n" + logMsg3 + "\n";
  doc["webhookCount"] = webhookCount;
  doc["lastWebhookTitle"] = lastWebhookTitle;
  doc["lastWebhookContent"] = lastWebhookContent;

  String out; serializeJson(doc, out);
  sendJson(200, out);
}

// ===================== MAA 端点 =====================

void handleGetTask() {
  String reqUser = "";
  if (server.hasArg("plain")) {
    DynamicJsonDocument d(512);
    if (!deserializeJson(d, server.arg("plain"))) reqUser = d["user"] | "";
  }
  int ui = findUserByMaaUser(reqUser);
  DynamicJsonDocument doc(512);
  JsonArray tasks = doc.createNestedArray("tasks");

  if (ui >= 0 && hasPendingCommand) {
    if (activeUserIndex != -1 && activeUserIndex != ui) {
      // 有其他用户正在控制，静默忽略
    } else {
      String id = genTaskId();
      JsonObject t = tasks.createNestedObject();
      t["id"] = id; t["type"] = pendingCommandType;
      lastTaskId = id; lastCommandType = pendingCommandType;
      hasPendingCommand = false; pendingCommandType = "";
    }
  }

  String out; serializeJson(doc, out);
  sendJson(200, out);
}

void handleReportStatus() {
  if (server.hasArg("plain")) {
    String body = server.arg("plain");
    DynamicJsonDocument d(1024);
    if (!deserializeJson(d, body)) {
      lastReportTask   = d["task"]   | "";
      lastReportStatus = d["status"] | "";
      lastReportTime   = millis();
      if (lastReportStatus == "SUCCESS") {
        if (lastCommandType == "LinkStart") {
          addLog("MAA 汇报：一键长草已开始执行");
          maaOn = false;
        }
        else if (lastCommandType == "StopTask") {
          addLog("MAA 汇报：已停止当前任务");
          activeUserIndex = -1;
          maaOn = false;
        }
        else {
          addLog("MAA 汇报：任务已完成");
          maaOn = false;
        }
      } else {
        addLog("MAA 汇报：异常 (" + lastReportStatus + ")");
        maaOn = false;
      }
    }
  }
  server.send(200, "text/plain", "ok");
}

// ===================== setup / loop =====================

void setup() {
  Serial.begin(SERIAL_BAUD);
  delay(500);
  randomSeed(ESP.getChipId() * micros());
  generateMaaUsers();

  if (!LittleFS.begin()) {
    Serial.println("LittleFS 挂载失败，尝试格式化...");
    LittleFS.format();
    if (!LittleFS.begin()) {
      Serial.println("LittleFS 初始化失败！头像功能不可用。");
    } else {
      Serial.println("LittleFS 格式化并初始化成功。");
    }
  } else {
    Serial.println("LittleFS 初始化成功。");
  }

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("连接 WiFi");
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println();
  String ip = WiFi.localIP().toString();
  Serial.println("-------------------------------------");
  if (SERVER_PORT == 80) {
    Serial.println("控制台: http://" + ip + "/");
    Serial.println("Webhook 地址: http://" + ip + "/webhook");
  } else {
    Serial.println("控制台: http://" + ip + ":" + String(SERVER_PORT) + "/");
    Serial.println("Webhook 地址: http://" + ip + ":" + String(SERVER_PORT) + "/webhook");
  }
  Serial.println("-------------------------------------");
  Serial.println("账号信息（MAA 标识符请复制到 MAA）：");
  for (int i = 0; i < USER_COUNT; i++) {
    Serial.println("  " + users[i].username + " / " + users[i].password +
                   (users[i].isAdmin ? " [管理员]" : ""));
    Serial.println("    MAA 标识符: " + users[i].maaUser);
  }
  Serial.println("-------------------------------------");

  server.on("/",                 HTTP_GET,  handleRoot);
  server.on("/login",            HTTP_POST, handleLoginPost);
  server.on("/logout",           HTTP_GET,  handleLogout);
  server.on("/maa/state",        HTTP_GET,  handleMaaState);
  server.on("/maa/set",          HTTP_GET,  handleMaaSet);
  server.on("/admin/data",       HTTP_GET,  handleAdminData);
  server.on("/maa/getTask",      HTTP_POST, handleGetTask);
  server.on("/maa/reportStatus", HTTP_POST, handleReportStatus);

  // ---- Webhook 路由 ----
  server.on("/webhook",          HTTP_POST, handleWebhook);

  server.on("/avatar.jpg", HTTP_GET, []() { handleStaticFile("/avatar.jpg", "image/jpeg"); });
  server.on("/avatar.png", HTTP_GET, []() { handleStaticFile("/avatar.png", "image/png"); });

  server.begin();
  Serial.println("HTTP 服务器已启动");
}

void loop() {
  server.handleClient();

  if (millis() - lastSessionClean > 60000UL) {
    lastSessionClean = millis();
    cleanExpiredSessions();
  }
}

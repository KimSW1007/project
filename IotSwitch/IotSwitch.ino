#include <WiFi.h>
#include <FirebaseESP32.h>

// Wi-Fi 설정
#define WIFI_SSID "iPhone 13 Pro"
#define WIFI_PASSWORD "1q2w3e4r"

// Firebase 설정
#define FIREBASE_HOST "esp32-iot-654c9-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "AIzaSyAqXK8aItt8fz2IfhyeEdnWQcvzFtxFYbs"

// Firebase 객체 초기화
FirebaseData firebaseData;
FirebaseConfig firebaseConfig;

// 스위치 핀 정의
#define SWITCH_1_PIN 26
#define SWITCH_2_PIN 27
#define SWITCH_3_PIN 32

// Wi-Fi 연결 함수
void setupWiFi() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int retryCount = 0;
  while (WiFi.status() != WL_CONNECTED && retryCount < 30) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
    retryCount++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to WiFi");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("WiFi connection failed. Restarting...");
    delay(5000);
    ESP.restart();
  }
}

// Firebase 연결 함수
void setupFirebase() {
  firebaseConfig.host = FIREBASE_HOST;
  firebaseConfig.signer.tokens.legacy_token = FIREBASE_AUTH; // Legacy Token 설정

  Firebase.begin(&firebaseConfig, nullptr); // Firebase 시작
  Firebase.reconnectWiFi(true);
}

// 스위치 제어 함수
void controlSwitch(int switchPin, int state) {
  digitalWrite(switchPin, state);
}

void setup() {
  pinMode(SWITCH_1_PIN, OUTPUT);
  pinMode(SWITCH_2_PIN, OUTPUT);
  pinMode(SWITCH_3_PIN, OUTPUT);

  setupWiFi();
  setupFirebase();
}

void loop() {
  // led1 상태 가져오기
  if (Firebase.getString(firebaseData, "/LightControl/led1")) {
    String led1State = firebaseData.stringData();
    Serial.print("led1 state: ");
    Serial.println(led1State);
    controlSwitch(SWITCH_1_PIN, led1State == "1" ? HIGH : LOW);
  } else {
    Serial.print("Failed to read LightControl/led1: ");
    Serial.println(firebaseData.errorReason());
  }

  // led2 상태 가져오기
  if (Firebase.getString(firebaseData, "/LightControl/led2")) {
    String led2State = firebaseData.stringData();
    Serial.print("led2 state: ");
    Serial.println(led2State);
    controlSwitch(SWITCH_2_PIN, led2State == "1" ? HIGH : LOW);
  } else {
    Serial.print("Failed to read LightControl/led2: ");
    Serial.println(firebaseData.errorReason());
  }

  // led3 상태 가져오기
  if (Firebase.getString(firebaseData, "/LightControl/led3")) {
    String led3State = firebaseData.stringData();
    Serial.print("led3 state: ");
    Serial.println(led3State);
    controlSwitch(SWITCH_3_PIN, led3State == "1" ? HIGH : LOW);
  } else {
    Serial.print("Failed to read LightControl/led3: ");
    Serial.println(firebaseData.errorReason());
  }

  delay(3000); // 3초마다 데이터 확인
}

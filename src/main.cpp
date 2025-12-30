/*
 * ESP32 pH Sensor Monitor with MQTT
 * 
 * Датчик: PH-4502C
 * Плата: ESP32 DevKit
 * Framework: Arduino (PlatformIO)
 * 
 * Подключение:
 * - PH-4502C Po -> GPIO 34 (ADC1_CH6)
 * - PH-4502C VCC -> 5V
 * - PH-4502C GND -> GND
 */

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// ==================== НАСТРОЙКИ WiFi ====================
const char* WIFI_SSID = "YOUR_WIFI_SSID";           // Замените на ваш SSID
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";   // Замените на ваш пароль

// ==================== НАСТРОЙКИ MQTT ====================
const char* MQTT_BROKER = "192.168.1.173";
const int MQTT_PORT = 1883;
const char* MQTT_CLIENT_ID = "ESP32_pH_Sensor";
const char* MQTT_TOPIC_PH = "sensor/ph/value";
const char* MQTT_TOPIC_STATUS = "sensor/ph/status";

// ==================== НАСТРОЙКИ ДАТЧИКА PH ====================
const int PH_SENSOR_PIN = 34;           // GPIO 34 (ADC1_CH6)
const int ADC_RESOLUTION = 4095;        // 12-bit ADC (0-4095)
const float ADC_VOLTAGE = 3.3;          // Опорное напряжение ESP32
const int NUM_SAMPLES = 10;             // Количество измерений для усреднения
const unsigned long PUBLISH_INTERVAL = 30000;  // Интервал публикации (30 секунд)

// ==================== КАЛИБРОВКА pH ====================
// Калибровка для PH-4502C с ESP32 (3.3V)
// При pH 7.0: напряжение около 1.5V (середина диапазона)
// При pH 4.0: напряжение около 2.0V
// При pH 10.0: напряжение около 1.0V
// Формула: pH = 7.0 - ((voltage - acidVoltage) / slope)

const float ACID_VOLTAGE = 2.032;       // Напряжение при pH 4.0
const float PH_SLOPE = 0.1773;          // Наклон (mV/pH) для калибровки

// ==================== ГЛОБАЛЬНЫЕ ОБЪЕКТЫ ====================
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// ==================== ПЕРЕМЕННЫЕ ====================
unsigned long lastPublishTime = 0;
float lastPhValue = 0.0;
float lastVoltage = 0.0;
bool wifiConnected = false;
bool mqttConnected = false;

// ==================== ПРОТОТИПЫ ФУНКЦИЙ ====================
void setupWiFi();
void reconnectWiFi();
void setupMQTT();
void reconnectMQTT();
float readPhSensor();
float readAverageVoltage();
float voltageToADC(int adcValue);
float voltageToPh(float voltage);
void publishPhData(float pH, float voltage);
void publishStatus(const char* status);

// ==================== SETUP ====================
void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n\n======================================");
    Serial.println("ESP32 pH Sensor Monitor");
    Serial.println("======================================");
    
    // Настройка пина датчика
    pinMode(PH_SENSOR_PIN, INPUT);
    analogSetAttenuation(ADC_11db);  // Полный диапазон 0-3.3V
    
    // Подключение к WiFi
    setupWiFi();
    
    // Настройка MQTT
    setupMQTT();
    
    Serial.println("Система готова к работе!");
    Serial.println("======================================\n");
    
    // Публикация статуса при запуске
    publishStatus("online");
}

// ==================== MAIN LOOP ====================
void loop() {
    // Проверка подключения WiFi
    if (WiFi.status() != WL_CONNECTED) {
        wifiConnected = false;
        reconnectWiFi();
    } else {
        wifiConnected = true;
    }
    
    // Проверка подключения MQTT
    if (!mqttClient.connected()) {
        mqttConnected = false;
        reconnectMQTT();
    } else {
        mqttConnected = true;
    }
    
    mqttClient.loop();
    
    // Публикация данных с заданным интервалом
    unsigned long currentTime = millis();
    if (currentTime - lastPublishTime >= PUBLISH_INTERVAL) {
        lastPublishTime = currentTime;
        
        // Чтение данных с датчика
        float voltage = readAverageVoltage();
        float pH = voltageToPh(voltage);
        
        lastPhValue = pH;
        lastVoltage = voltage;
        
        // Вывод в Serial Monitor
        Serial.println("\n--- Измерение pH ---");
        Serial.printf("Напряжение: %.3f V\n", voltage);
        Serial.printf("pH: %.2f\n", pH);
        Serial.println("--------------------");
        
        // Публикация данных в MQTT
        publishPhData(pH, voltage);
    }
    
    delay(100);
}

// ==================== WiFi ФУНКЦИИ ====================
void setupWiFi() {
    Serial.println("\n[WiFi] Подключение к WiFi...");
    Serial.printf("[WiFi] SSID: %s\n", WIFI_SSID);
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        wifiConnected = true;
        Serial.println("\n[WiFi] Подключено!");
        Serial.printf("[WiFi] IP адрес: %s\n", WiFi.localIP().toString().c_str());
        Serial.printf("[WiFi] Уровень сигнала: %d dBm\n", WiFi.RSSI());
    } else {
        wifiConnected = false;
        Serial.println("\n[WiFi] Не удалось подключиться!");
    }
}

void reconnectWiFi() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[WiFi] Переподключение к WiFi...");
        WiFi.disconnect();
        delay(1000);
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        
        int attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < 20) {
            delay(500);
            Serial.print(".");
            attempts++;
        }
        
        if (WiFi.status() == WL_CONNECTED) {
            wifiConnected = true;
            Serial.println("\n[WiFi] Переподключено!");
            Serial.printf("[WiFi] IP адрес: %s\n", WiFi.localIP().toString().c_str());
        } else {
            wifiConnected = false;
            Serial.println("\n[WiFi] Не удалось переподключиться!");
        }
    }
}

// ==================== MQTT ФУНКЦИИ ====================
void setupMQTT() {
    mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
    Serial.printf("\n[MQTT] Настройка MQTT брокера: %s:%d\n", MQTT_BROKER, MQTT_PORT);
    reconnectMQTT();
}

void reconnectMQTT() {
    if (!wifiConnected) {
        return;
    }
    
    int attempts = 0;
    while (!mqttClient.connected() && attempts < 5) {
        Serial.printf("[MQTT] Попытка подключения (%d/5)...\n", attempts + 1);
        
        if (mqttClient.connect(MQTT_CLIENT_ID)) {
            mqttConnected = true;
            Serial.println("[MQTT] Подключено к брокеру!");
            Serial.printf("[MQTT] Client ID: %s\n", MQTT_CLIENT_ID);
            
            // Публикация статуса при подключении
            publishStatus("online");
        } else {
            mqttConnected = false;
            Serial.printf("[MQTT] Ошибка подключения, код: %d\n", mqttClient.state());
            Serial.println("[MQTT] Повтор через 5 секунд...");
            delay(5000);
        }
        attempts++;
    }
    
    if (!mqttClient.connected()) {
        Serial.println("[MQTT] Не удалось подключиться после 5 попыток!");
    }
}

// ==================== ФУНКЦИИ ДАТЧИКА pH ====================
float readAverageVoltage() {
    float voltageSum = 0.0;
    
    // Собираем NUM_SAMPLES измерений
    for (int i = 0; i < NUM_SAMPLES; i++) {
        int adcValue = analogRead(PH_SENSOR_PIN);
        float voltage = voltageToADC(adcValue);
        voltageSum += voltage;
        delay(10);  // Небольшая задержка между измерениями
    }
    
    // Возвращаем среднее значение
    return voltageSum / NUM_SAMPLES;
}

float voltageToADC(int adcValue) {
    // Преобразование ADC значения в напряжение
    return (adcValue / (float)ADC_RESOLUTION) * ADC_VOLTAGE;
}

float voltageToPh(float voltage) {
    // Преобразование напряжения в значение pH
    // Формула на основе калибровки датчика PH-4502C
    float pH = 7.0 - ((voltage - ACID_VOLTAGE) / PH_SLOPE);
    
    // Ограничение значения pH в диапазоне 0-14
    if (pH < 0.0) pH = 0.0;
    if (pH > 14.0) pH = 14.0;
    
    return pH;
}

// ==================== ФУНКЦИИ ПУБЛИКАЦИИ MQTT ====================
void publishPhData(float pH, float voltage) {
    if (!mqttConnected) {
        Serial.println("[MQTT] Не подключено к брокеру, пропуск публикации...");
        return;
    }
    
    // Создание JSON документа
    StaticJsonDocument<200> doc;
    doc["pH"] = round(pH * 100) / 100.0;  // Округление до 2 знаков
    doc["voltage"] = round(voltage * 1000) / 1000.0;  // Округление до 3 знаков
    doc["timestamp"] = millis() / 1000;
    
    // Сериализация JSON
    char jsonBuffer[200];
    serializeJson(doc, jsonBuffer);
    
    // Публикация в MQTT
    if (mqttClient.publish(MQTT_TOPIC_PH, jsonBuffer, true)) {
        Serial.printf("[MQTT] Данные опубликованы в топик: %s\n", MQTT_TOPIC_PH);
        Serial.printf("[MQTT] Payload: %s\n", jsonBuffer);
    } else {
        Serial.println("[MQTT] Ошибка публикации данных!");
    }
}

void publishStatus(const char* status) {
    if (!mqttConnected) {
        return;
    }
    
    StaticJsonDocument<100> doc;
    doc["status"] = status;
    doc["device"] = "ESP32_pH_Sensor";
    doc["ip"] = WiFi.localIP().toString();
    
    char jsonBuffer[100];
    serializeJson(doc, jsonBuffer);
    
    mqttClient.publish(MQTT_TOPIC_STATUS, jsonBuffer, true);
    Serial.printf("[MQTT] Статус опубликован: %s\n", status);
}


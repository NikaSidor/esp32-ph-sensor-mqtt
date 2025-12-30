/*
 * Простой тест для проверки подключения датчика pH
 * 
 * Этот код НЕ требует Wi-Fi и MQTT
 * Используйте для быстрой проверки оборудования
 * 
 * Как использовать:
 * 1. Переименуйте src/main.cpp в src/main.cpp.backup
 * 2. Скопируйте этот файл в src/main.cpp
 * 3. Загрузите в ESP32
 * 4. Откройте Serial Monitor (115200 baud)
 */

#include <Arduino.h>

// Настройки датчика
const int PH_SENSOR_PIN = 34;
const int ADC_RESOLUTION = 4095;
const float ADC_VOLTAGE = 3.3;
const int NUM_SAMPLES = 10;

// Калибровка (стандартные значения)
const float ACID_VOLTAGE = 2.032;
const float PH_SLOPE = 0.1773;

// Функции
float readAverageVoltage();
float voltageToADC(int adcValue);
float voltageToPh(float voltage);

void setup() {
    Serial.begin(115200);
    delay(2000);
    
    Serial.println("\n\n=====================================");
    Serial.println("   ESP32 pH Sensor - Simple Test");
    Serial.println("=====================================\n");
    
    pinMode(PH_SENSOR_PIN, INPUT);
    analogSetAttenuation(ADC_11db);
    
    Serial.println("Система запущена!");
    Serial.println("Погрузите датчик в раствор и наблюдайте за показаниями.\n");
}

void loop() {
    // Чтение напряжения
    float voltage = readAverageVoltage();
    
    // Преобразование в pH
    float pH = voltageToPh(voltage);
    
    // Вывод результатов
    Serial.println("┌─────────────────────────────────┐");
    Serial.printf("│ Напряжение: %6.3f V        │\n", voltage);
    Serial.printf("│ pH:         %6.2f           │\n", pH);
    Serial.println("└─────────────────────────────────┘");
    
    // Классификация pH
    Serial.print("Состояние: ");
    if (pH < 6.5) {
        Serial.println("🔵 КИСЛАЯ");
    } else if (pH > 7.5) {
        Serial.println("🟢 ЩЕЛОЧНАЯ");
    } else {
        Serial.println("🟡 НЕЙТРАЛЬНАЯ");
    }
    
    Serial.println();
    
    delay(2000);  // Задержка 2 секунды
}

float readAverageVoltage() {
    float voltageSum = 0.0;
    
    for (int i = 0; i < NUM_SAMPLES; i++) {
        int adcValue = analogRead(PH_SENSOR_PIN);
        float voltage = voltageToADC(adcValue);
        voltageSum += voltage;
        delay(10);
    }
    
    return voltageSum / NUM_SAMPLES;
}

float voltageToADC(int adcValue) {
    return (adcValue / (float)ADC_RESOLUTION) * ADC_VOLTAGE;
}

float voltageToPh(float voltage) {
    float pH = 7.0 - ((voltage - ACID_VOLTAGE) / PH_SLOPE);
    
    if (pH < 0.0) pH = 0.0;
    if (pH > 14.0) pH = 14.0;
    
    return pH;
}


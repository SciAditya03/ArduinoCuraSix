#include <Wire.h>
#include <SPI.h>
#include "MAX30105.h"  // MAX30102 Library
#include "SparkFun_MLX90632.h"  // MLX90632 Library
#include "MAX30001.h"  // MAX30001 Library

// Sensor Objects
MAX30105 particleSensor;  // MAX30102 HRV + SpO2
SparkFun_MLX90632 mlx;  // MLX90632 Skin Temperature
MAX30001 bioSensor;  // MAX30001 Bioelectrical Impedance

#define GSR_PIN 34  // GSR Sensor on GPIO34

void setup() {
    Serial.begin(9600);
    Wire.begin(21, 22); // SDA, SCL for I2C Sensors

    // ✅ Initialize MAX30102
    if (!particleSensor.begin(Wire, I2C_SPEED_STANDARD)) {
        Serial.println("❌ MAX30102 not found!");
    } else {
        particleSensor.setup();
        particleSensor.setPulseAmplitudeRed(0x0A);
        particleSensor.setPulseAmplitudeIR(0x0A);
        Serial.println("✅ MAX30102 Initialized!");
    }

    // ✅ Initialize MLX90632
    if (!mlx.begin()) {
        Serial.println("❌ MLX90632 not detected!");
    } else {
        Serial.println("✅ MLX90632 Initialized!");
    }

    // ✅ Initialize MAX30001
    if (!bioSensor.begin()) {
        Serial.println("❌ MAX30001 not found!");
    } else {
        Serial.println("✅ MAX30001 Initialized!");
    }

    // ✅ Configure GSR Sensor (Optional)
    pinMode(GSR_PIN, INPUT);
}

void loop() {
    // ✅ Read MAX30102 (HRV + SpO2)
    long irValue = particleSensor.getIR();
    long redValue = particleSensor.getRed();

    // ✅ Read MLX90632 (Temperature)
    float temp = mlx.getObjectTemp();
    if (isnan(temp)) temp = -1;  // Handle invalid readings

    // ✅ Read GSR (Skin Conductance)
    int gsrValue = analogRead(GSR_PIN);

    // ✅ Read MAX30001 (Bioelectrical Impedance)
    int bioZ = bioSensor.readBioZ();

    // ✅ Print Sensor Data
    Serial.println("----- Sensor Readings -----");
    Serial.print("HRV/IR: "); Serial.print(irValue);
    Serial.print(" | SpO2/Red: "); Serial.println(redValue);

    Serial.print("Temperature: "); Serial.print(temp); Serial.println(" C");

    Serial.print("GSR Value: "); Serial.println(gsrValue);

    Serial.print("Bioelectrical Impedance: "); Serial.println(bioZ);
    Serial.println("--------------------------");

    delay(2000);  // Increased delay for stable HRV readings
}
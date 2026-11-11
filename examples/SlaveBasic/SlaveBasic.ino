// DroidParadeSlave - Beispiel: liest alle Parameter und gibt sie bei neuem Paket aus.
#include <DroidParadeSlave.h>

DroidParadeSlave droid;

void setup() {
  Serial.begin(115200);
  if (!droid.begin(1)) Serial.println("ESP-NOW Init fehlgeschlagen");  // Kanal 1 wie Master
}

void loop() {
  if (!droid.isConnected()) {
    // Getter liefern jetzt Fail-safe-Werte (alles aus, Rotation 0, Licht OFF)
    static uint32_t last = 0;
    if (millis() - last > 1000) { last = millis(); Serial.println("Kein Master"); }
    delay(10);
    return;
  }

  if (droid.hasNewData()) {
    Serial.printf("seq=%u\n", droid.getSequence());

    Serial.print("left:");   for (uint8_t i = 1; i <= 3;  i++) Serial.print(droid.getBodyLeft(i));
    Serial.print(" right:"); for (uint8_t i = 1; i <= 3;  i++) Serial.print(droid.getBodyRight(i));
    Serial.print(" center:");for (uint8_t i = 1; i <= 12; i++) Serial.print(droid.getBodyCenter(i));
    Serial.printf(" arms:%d\n", droid.getBodyArms());

    Serial.printf("dome_rotation:%d dome:", droid.getDomeRotation());
    for (uint8_t i = 1; i <= 6; i++) Serial.print(droid.getDome(i));
    Serial.print(" special:");
    for (uint8_t i = 1; i <= 3; i++) Serial.print(droid.getSpecialAction(i));

    const char *names[] = {"off", "color", "rainbow"};
    Serial.printf("\nlight:%s color:%06X\n", names[droid.getLightProp()], (unsigned)droid.getLightColor());
  }
  delay(10);
}

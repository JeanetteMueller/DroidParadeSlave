# DroidParadeSlave

Empfänger-Bibliothek des **Droid Parade**-Systems. Läuft auf dem Controller eines Droiden, empfängt das Paket des Masters per ESP-NOW und stellt die Werte über Getter bereit.

**Autorin:** Jeanette Müller · **Version:** 1.0.0 · **Plattform:** ESP32 (Arduino Core 2.x und 3.x) · **Abhängigkeit:** `DroidParadeShared`

## Benutzung
```cpp
#include <DroidParadeSlave.h>
DroidParadeSlave droid;

void setup() { droid.begin(1, 1); }  // Droiden-ID 1, WLAN-Kanal 1 (wie Master)

void loop() {
  if (droid.hasNewData()) {
    int rot = droid.getDomeRotation();
    bool l1 = droid.getBodyLeft(1);
  }
}
```

## Verhalten
- Ungültige Pakete (Magic, Version, CRC, Länge) werden verworfen.
- Bricht die Verbindung ab (`DROID_PARADE_TIMEOUT_MS`), liefern alle Getter aus/0 und `getLightProp()` OFF (Fail-safe).

## API
`begin(channel)`, `isConnected`, `hasNewData`, `onReceive(callback)`, `msSinceLastPacket`, `packetsReceived`, `getBodyLeft(n)`, `getBodyRight(n)`, `getBodyCenter(n)`, `getBodyArms()`, `getDomeRotation()`, `getDome(n)`, `getSpecialAction(n)`, `getLightProp()`, `getLightColor()`, `getSequence()`, `getPacket()`. Beispiel: `examples/SlaveBasic`.

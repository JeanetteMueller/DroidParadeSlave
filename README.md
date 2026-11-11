# DroidParadeSlave

Empfänger-Bibliothek des **Droid Parade**-Systems. Läuft auf dem Controller eines Droiden, empfängt das Paket des Masters per ESP-NOW und stellt die Werte über Getter bereit.

**Autorin:** Jeanette Müller · **Version:** 1.0.0 · **Plattform:** ESP32 (Arduino Core 2.x und 3.x) · **Abhängigkeit:** `DroidParadeShared`

## Benutzung
```cpp
#include <DroidParadeSlave.h>
DroidParadeSlave droid;

void setup() { droid.begin(paradeConfig); }  // Config wie beim Master (siehe unten)

void loop() {
  if (droid.hasNewData()) {
    int rot = droid.getDomeRotation();
    bool l1 = droid.getBodyLeft(1);
  }
}
```

## Verhalten
- Ungültige Pakete (Länge, Magic, Version, Signatur) werden verworfen.
- Bricht die Verbindung ab (`DROID_PARADE_TIMEOUT_MS`), liefern alle Getter aus/0 und `getLightProp()` OFF (Fail-safe).

## Zugehörigkeit & Sicherheit
Master und Droiden teilen eine `DroidParadeConfig`:

```cpp
const DroidParadeConfig paradeConfig = {
  /* paradeId */ 0x0001,                     // öffentliche Kennung der Parade
  /* secret   */ "mein-geheimes-passwort",   // mind. 8 Zeichen, geheim halten
  /* channel  */ 1                           // WLAN-Kanal
};
```

- Jedes Paket trägt die `paradeId` und ist mit einem gekürzten HMAC-SHA256 über das `secret` signiert.
- Pakete mit anderer Parade-ID, falscher Signatur oder alter Sequenznummer (Replay) werden verworfen (`packetsRejected()`).
- `begin()` liefert `false`, wenn das Secret fehlt oder kürzer als 8 Zeichen ist.
- Die Pakete sind signiert, aber **nicht verschlüsselt**; das Secret darf nicht ins öffentliche Repo.

## API
`begin(config)`, `isConnected`, `hasNewData`, `onReceive(callback)`, `msSinceLastPacket`, `packetsReceived`, `packetsRejected`, `getBodyLeft(n)`, `getBodyRight(n)`, `getBodyCenter(n)`, `getBodyArms()`, `getDomeRotation()`, `getDome(n)`, `getSpecialAction(n)`, `getLightProp()`, `getLightColor()`, `getSequence()`, `getPacket()`. Beispiel: `examples/SlaveBasic`.

/*
 * DroidParadeSlave - empfängt das gemeinsame Paket per ESP-NOW
 * Autorin: Jeanette Müller
 */
#pragma once
#include <Arduino.h>
#include <DroidParadeShared.h>

class DroidParadeSlave {
public:
  bool begin(uint8_t channel = 1);

  // Verbindung
  bool isConnected() const;                  // Paket innerhalb DROID_PARADE_TIMEOUT_MS erhalten
  uint32_t msSinceLastPacket() const;
  uint32_t packetsReceived() const { return _count; }
  bool hasNewData();                         // true seit letztem Aufruf neues Paket; setzt zurück
  void onReceive(void (*cb)(const DroidParadePacket &)) { _cb = cb; }

  // Getter (Nummern 1-basiert). Bei getrennter Verbindung: alles aus / 0 (Fail-safe).
  bool     getBodyLeft(uint8_t n) const   { return isConnected() && dpGetBit(_p.bodyLeft, n, DP_BODY_LEFT_COUNT); }
  bool     getBodyRight(uint8_t n) const  { return isConnected() && dpGetBit(_p.bodyRight, n, DP_BODY_RIGHT_COUNT); }
  bool     getBodyCenter(uint8_t n) const { return isConnected() && dpGetBit(_p.bodyCenter, n, DP_BODY_CENTER_COUNT); }
  bool     getBodyArms() const            { return isConnected() && _p.bodyArms; }
  int16_t  getDomeRotation() const        { return isConnected() ? _p.domeRotation : 0; }
  bool     getDome(uint8_t n) const       { return isConnected() && dpGetBit(_p.dome, n, DP_DOME_COUNT); }
  bool     getSpecialAction(uint8_t n) const { return isConnected() && dpGetBit(_p.specialAction, n, DP_SPECIAL_COUNT); }
  DroidParadeLightProp getLightProp() const { return isConnected() ? (DroidParadeLightProp)_p.lightProp : DP_LIGHT_OFF; }
  uint32_t getLightColor() const          { return _p.lightColor; }
  uint16_t getSequence() const            { return _p.sequence; }
  DroidParadePacket getPacket() const     { return _p; }

  void _handle(const uint8_t *data, int len);  // intern (Callback)

private:
  DroidParadePacket _p = {};
  volatile uint32_t _lastMs = 0;
  volatile uint32_t _count = 0;
  volatile bool _new = false;
  volatile bool _everReceived = false;
  void (*_cb)(const DroidParadePacket &) = nullptr;
};

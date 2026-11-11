#include "DroidParadeSlave.h"
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

static DroidParadeSlave *g_instance = nullptr;

#if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
static void onRecv(const esp_now_recv_info_t *, const uint8_t *data, int len) {
#else
static void onRecv(const uint8_t *, const uint8_t *data, int len) {
#endif
  if (g_instance) g_instance->_handle(data, len);
}

bool DroidParadeSlave::begin(uint8_t channel) {
  g_instance = this;
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  if (esp_now_init() != ESP_OK) return false;
  esp_now_register_recv_cb(onRecv);
  return true;
}

void DroidParadeSlave::_handle(const uint8_t *data, int len) {
  if (len != (int)sizeof(DroidParadePacket)) return;
  DroidParadePacket tmp;
  memcpy(&tmp, data, sizeof(tmp));
  if (!droidParadeValid(tmp)) return;
  _p = tmp;
  _lastMs = millis();
  _everReceived = true;
  _count = _count + 1;
  _new = true;
  if (_cb) _cb(_p);
}

bool DroidParadeSlave::isConnected() const {
  return _everReceived && (millis() - _lastMs) < DROID_PARADE_TIMEOUT_MS;
}

uint32_t DroidParadeSlave::msSinceLastPacket() const {
  return _everReceived ? millis() - _lastMs : UINT32_MAX;
}

bool DroidParadeSlave::hasNewData() {
  bool n = _new;
  _new = false;
  return n;
}

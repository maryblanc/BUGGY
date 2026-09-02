#pragma once

#include <cstdint>

// ==============================
// SD MODULE PINS
// ==============================

constexpr uint8_t SD_CS_PIN   = 5;
constexpr uint8_t SD_MOSI_PIN = 19;
constexpr uint8_t SD_MISO_PIN = 23;
constexpr uint8_t SD_SCLK_PIN = 18;


// ==============================
// KELLY UART
// ==============================

constexpr uint32_t UART_BAUDRATE = 19200;

// Kelly is moved away from UART0.
// UART0 (GPIO1/3) remains available for USB Serial Monitor.

constexpr uint8_t KELLY_RX_PIN = 32;
constexpr uint8_t KELLY_TX_PIN = 33;


// ==============================
// RTC DS1302
// ==============================

constexpr uint8_t RTC_RST_PIN = 27;
constexpr uint8_t RTC_DAT_PIN = 26;
constexpr uint8_t RTC_CLK_PIN = 25;


// ==============================
// UART NEXTION
// ==============================

constexpr uint8_t NEXTION_RX_PIN = 16;
constexpr uint8_t NEXTION_TX_PIN = 17;
constexpr uint32_t NEXTION_BAUDRATE = 9600;
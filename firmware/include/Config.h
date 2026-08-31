#pragma once
#include <cstdint>


// MODUL SD PINY
constexpr uint8_t SD_CS_PIN   = 5; // zolty
constexpr uint8_t SD_MOSI_PIN = 19; // fioletowy
constexpr uint8_t SD_MISO_PIN = 23; // pomaranczowy
constexpr uint8_t SD_SCLK_PIN  = 18; // zielony

// Kelly UART
constexpr uint32_t UART_BAUDRATE = 19200;


// RTC DS1302
constexpr uint8_t RTC_RST_PIN  = 27; // brazowy
constexpr uint8_t RTC_DAT_PIN  = 26; // zolty
constexpr uint8_t RTC_CLK_PIN  = 25; // zielony

// UART KELLY
constexpr uint8_t KELLY_RX_PIN = 3;
constexpr uint8_t KELLY_TX_PIN = 1;

// UART NEXTION
constexpr uint8_t NEXTION_RX_PIN = 16;
constexpr uint8_t NEXTION_TX_PIN = 17;
constexpr uint32_t NEXTION_BAUDRATE = 9600;
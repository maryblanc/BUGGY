# BUGGY

**System telemetrii pojazdu elektrycznego oparty na ESP32 i kontrolerze silnika Kelly KLS**

BUGGY to projekt systemu zbierania, przetwarzania i prezentowania danych eksploatacyjnych pojazdu elektrycznego.

Głównym zadaniem oprogramowania jest komunikacja z kontrolerem silnika Kelly KLS przez RS232, odczytywanie parametrów pracy, weryfikowanie poprawności otrzymanych ramek oraz przekształcanie danych w uporządkowaną strukturę telemetrii.

Projekt zawiera również moduły obsługi karty SD, zegara RTC, symulatora kontrolera oraz wyświetlacza Nextion.

**Aktualny etap projektu:** firmware zostało skompilowane dla ESP32 i przeszło testy programowe. Komunikacja z rzeczywistym kontrolerem Kelly wymaga jeszcze weryfikacji sprzętowej.

---

## 1. Architektura systemu

Główny przepływ danych wygląda następująco:

```text
       KONTROLER KELLY KLS
               |
          RS232 / UART
               |
              ESP32
               |
         KellyProtocol
               |
       KellyFrameReceiver
               |
         KellyFrameCodec
               |
          KellyParser
               |
         TelemetryFrame
               |
           main.cpp
               |
        Serial Monitor
```

### Najważniejsze moduły

| Moduł | Funkcja |
|---|---|
| `KellyController` | Zarządza odczytem telemetrii z kontrolera |
| `KellyProtocol` | Obsługuje wysyłanie zapytań i odbieranie odpowiedzi przez UART |
| `KellyFrameCodec` | Buduje zapytania, oblicza checksum i sprawdza ramki |
| `KellyFrameReceiver` | Odbiera bajty i odnajduje prawidłowe ramki |
| `KellyParser` | Interpretuje dane odczytane z kontrolera |
| `KellyLinkStatus` | Przechowuje stan komunikacji i liczbę odebranych bajtów |
| `KellySimulator` | Generuje przykładowe dane kontrolera do testów |
| `UARTPacketBuilder` | Buduje dane monitorów na podstawie struktury telemetrii |
| `TelemetryFrame` | Przechowuje parametry pracy kontrolera |

W aktualnym `main.cpp` program cyklicznie odpytuje kontroler i wypisuje dane oraz status komunikacji przez USB Serial Monitor.

Moduły Nextion, RTC i zapisu na SD istnieją w projekcie, ale nie są obecnie połączone z główną pętlą programu.

---

## 2. Konfiguracja sprzętowa

Projekt wykorzystuje płytkę ESP32.

### Komunikacja Kelly

| Parametr | Wartość |
|---|---|
| Interfejs | RS232 przez odpowiedni konwerter poziomów |
| UART ESP32 | UART1 |
| Prędkość transmisji | 19200 baud |
| Format transmisji w firmware | 8N1 |
| RX ESP32 | GPIO32 |
| TX ESP32 | GPIO33 |
| Serial Monitor | 115200 baud |

Konfigurację pinów można znaleźć w:

`firmware/include/Config.h`

**Uwaga:** GPIO ESP32 pracują z poziomami logicznymi 3,3 V. Nie należy podłączać ich bezpośrednio do sygnałów o rzeczywistych poziomach RS232.

Poprzedni układ ESP32 lub konwerter mógł ulec uszkodzeniu. Przed kolejnymi próbami komunikacji konieczne jest sprawdzenie zasilania, masy, poziomów napięć oraz poprawności połączeń.

---

## 3. Protokół komunikacyjny Kelly KLS

Implementacja opiera się na dokumentacji:

**Kelly KLS Controller RS232 Commands List — Version 1.1**

Dokumentacja została przekazana przez dział techniczny Kelly po konsultacji dotyczącej modelu kontrolera używanego w projekcie.

### Obsługiwane komendy

| Komenda | HEX | Przeznaczenie |
|---|---|---|
| `ETS_A2D_BATCH_READ` | `0x1B` | Odczyt danych analogowych |
| `ETS_USER_MONITOR1` | `0x3A` | Odczyt wejść, przełączników i wybranych parametrów |
| `ETS_USER_MONITOR2` | `0x3B` | Odczyt błędów i prędkości obrotowej silnika |

Aktualny program cyklicznie wykorzystuje komendy `0x3A` i `0x3B`. Komenda `0x1B` jest obsługiwana przez kodek ramek, ale nie jest jeszcze wykorzystywana w głównej pętli telemetrii.

Komenda `0x3C`, występująca we wcześniejszym kodzie, została usunięta, ponieważ nie jest opisana w otrzymanej dokumentacji.

### Format zapytania

W implementacji przyjęto trzybajtowe zapytanie:

```text
[KOMENDA] [DŁUGOŚĆ] [CHECKSUM]
```

Przykład dla `MONITOR1`:

```text
3A 00 3A
```

### Format odpowiedzi

Odpowiedź kontrolera ma przyjętą strukturę 19 bajtów:

```text
[CMD] [LEN] [DATA 0..15] [CHECKSUM]
```

- Bajt 0 — kod komendy.
- Bajt 1 — liczba bajtów danych.
- Bajty 2–17 — dane kontrolera.
- Bajt 18 — suma kontrolna.

Checksum jest obliczany jako suma poprzednich bajtów z zachowaniem najmłodszych 8 bitów.

Format został zaimplementowany na podstawie dokumentacji i przetestowany programowo. Ostateczne potwierdzenie długości zapytań oraz odpowiedzi wymaga komunikacji z rzeczywistym kontrolerem.

---

## 4. Odczytywane parametry

### MONITOR1 — `0x3A`

Parser obsługuje między innymi:

- Pozycję pedału przyspieszenia (wartość A/D).
- Sygnał hamulca (wartość A/D).
- Stany przełączników hamulca, jazdy do przodu i cofania.
- Sygnały czujników Halla.
- Pole związane z napięciem akumulatora.
- Pola temperatury silnika i kontrolera.
- Zadany i rzeczywisty kierunek obrotów.

Wartości gazu i hamulca są dodatkowo przeliczane z zakresu 0–255 na procenty.

### MONITOR2 — `0x3B`

| Bajty danych | Znaczenie |
|---|---|
| 0–1 | Flagi błędów kontrolera |
| 2–3 | Mechaniczna prędkość silnika w RPM |
| 4–15 | Znaczenie nieokreślone w dostarczonej dokumentacji |

Wartości 16-bitowe są interpretowane w kolejności starszy bajt, następnie młodszy bajt.

### Ograniczenia interpretacji

Przeliczanie napięcia akumulatora i temperatur na jednostki fizyczne nie zostało jeszcze w pełni zweryfikowane.

Pola te przechowują obecnie wartości pochodzące bezpośrednio z odpowiedzi kontrolera.

Pole `phaseCurrent` nie ma obecnie potwierdzonego źródła pomiaru w `MONITOR2`. W aktualnym parserze przyjmuje wartość zastępczą zero, której nie należy interpretować jako rzeczywistego pomiaru prądu.

---

## 5. Diagnostyka komunikacji

Firmware pozwala rozróżnić kilka stanów komunikacji:

| Status | Znaczenie |
|---|---|
| `NOT_STARTED` | Nie rozpoczęto zapytania |
| `OK` | Odebrano poprawną odpowiedź |
| `NO_RESPONSE` | Nie odebrano żadnych bajtów |
| `INVALID_FRAME` | Odebrano bajty, ale nie znaleziono poprawnej ramki |
| `SEND_ERROR` | Nie udało się wysłać zapytania |

Statusy `MONITOR1` i `MONITOR2` są przechowywane oddzielnie.

Prawidłowa telemetria jest udostępniana przez `KellyController` tylko wtedy, gdy oba odczyty zakończą się sukcesem.

W przypadku nieudanej komunikacji główny program zgłasza brak aktualnych danych zamiast prezentować wyzerowaną strukturę jako rzeczywisty odczyt.

Odbiornik `KellyFrameReceiver` potrafi również pomijać niepasujące bajty i odzyskiwać synchronizację po nieprawidłowej ramce.

---

## 6. Kompilacja firmware

Projekt wykorzystuje **PlatformIO** i framework Arduino dla ESP32.

### Wymagane narzędzia

- Visual Studio Code.
- Rozszerzenie PlatformIO IDE.
- PlatformIO Core.
- Kompilator GCC/G++ dla testów Native na Windows.

### Kompilacja ESP32

Otwórz folder `firmware` w VS Code i uruchom:

```powershell
platformio run -e esp32dev
```

Można również skorzystać z:

`PlatformIO → Project Tasks → esp32dev → General → Build`

Firmware można skompilować bez podłączonego ESP32.

### Serial Monitor

Prędkość monitora szeregowego:

```text
115200 baud
```

---

## 7. Testy automatyczne

Projekt posiada testy jednostkowe uruchamiane na komputerze z Windows bez konieczności podłączania ESP32.

Testy korzystają z PlatformIO Native i frameworka Unity.

### Dostępne zestawy

| Zestaw | Zakres | Liczba testów |
|---|---|---:|
| `test_kelly` | Parser i generator telemetrii | 3 |
| `test_kelly_frame` | Budowanie, walidacja i checksum ramek | 8 |
| `test_kelly_receiver` | Odbiór ramek i synchronizacja | 6 |
| `test_kelly_status` | Statusy komunikacji | 6 |
| `test_kelly_simulator` | Symulator i interpretacja danych | 2 |
| **Razem** | | **25** |

Ostatni zgłoszony wynik testów:

**25 testów zaliczonych, 0 błędów.**

### Uruchamianie testów

```powershell
platformio test -e native -v
```

Na Windows wymagany jest dostępny kompilator GCC/G++.

Wykonane testy potwierdzają działanie sprawdzonych modułów programowych. Nie stanowią potwierdzenia komunikacji z fizycznym kontrolerem Kelly.

---

## 8. Dodatkowe moduły

### Symulator Kelly

`KellySimulator` generuje przykładowe dane `MONITOR1` i `MONITOR2`, umożliwiając sprawdzanie parsera bez podłączonego kontrolera.

Symulator jest objęty testami Native.

### Karta SD

Projekt zawiera moduły związane z obsługą karty SD i zapisem telemetrii w formacie CSV.

Nie są one obecnie wykorzystywane przez aktywną pętlę `main.cpp`.

### RTC

W projekcie znajduje się obsługa zegara DS1302.

Integracja zegara z aktualnym odczytem telemetrii pozostaje do wykonania.

### Nextion

Repozytorium zawiera podstawowy moduł komunikacji z wyświetlaczem Nextion.

Rozwój interfejsu graficznego jest osobnym etapem projektu.

---

## 9. Aktualny stan prac

### Zaimplementowane

- Komunikacja UART dla Kelly KLS.
- Generowanie zapytań odczytu.
- Walidacja odpowiedzi i checksum.
- Odbiornik z mechanizmem synchronizacji ramek.
- Parser danych `MONITOR1` i `MONITOR2`.
- Osobna diagnostyka obu monitorów.
- Symulator danych kontrolera.
- Automatyczne testy jednostkowe.
- Główna pętla cyklicznego odczytu telemetrii.
- Obsługa błędów komunikacji bez prezentowania nieaktualnych pomiarów jako aktualnych.

### Pozostałe zadania

- Test integracyjny całej ścieżki komunikacji z symulowanymi odpowiedziami.
- Weryfikacja jednostek fizycznych parametrów.
- Końcowa kontrola i uproszczenie modelu telemetrii.
- Potwierdzenie działania protokołu na rzeczywistym Kelly KLS.
- Diagnostyka i naprawa interfejsu elektrycznego RS232/UART.
- Integracja logowania SD i zegara RTC.
- Dalszy rozwój interfejsu Nextion.

---

## 10. Bezpieczeństwo i ograniczenia

Aktualne oprogramowanie wykonuje wyłącznie zapytania służące do odczytywania parametrów kontrolera.

Nie implementuje sterowania momentem obrotowym, przepustnicą, hamowaniem ani zapisu parametrów konfiguracyjnych Kelly.

Przed próbami połączenia ze sterownikiem należy:

1. Sprawdzić zasilanie ESP32 i konwertera.
2. Potwierdzić rodzaj i poziomy napięć interfejsu kontrolera.
3. Zweryfikować zgodność połączeń TX, RX i masy sygnałowej.
4. Sprawdzić działanie konwertera i ESP32 niezależnie od kontrolera.
5. Dopiero po weryfikacji elektrycznej rozpocząć odczyt danych.

Poprawna kompilacja i zaliczone testy programowe nie gwarantują prawidłowego działania z fizycznym kontrolerem.

---

## 11. Dokumentacja

Podstawowy dokument protokołu:

`docs/kelly/Kelly KLS Controller RS232 Commands List version 1.1.pdf`

Dokument stanowi punkt odniesienia dla implementacji komend odczytu i interpretacji ramek.

---

## 12. Podsumowanie

BUGGY znajduje się na etapie zakończenia podstawowej implementacji firmware oraz przygotowania do testów sprzętowych.

Podstawowe moduły komunikacji, odbioru i interpretacji danych zostały zaimplementowane i przeszły dostępne testy programowe.

Najważniejszym kolejnym krokiem jest potwierdzenie poprawności elektrycznego połączenia ESP32 z kontrolerem Kelly i sprawdzenie rzeczywistych odpowiedzi RS232.

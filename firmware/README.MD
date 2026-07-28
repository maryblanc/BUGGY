#### PLAN:

## TASK 1
ESP32 --> karta SD --> telemetry.csv

więc:
1. Zaprojektować TelemetryFrame
2. Rozpisać klasy i odpowiedzialności
3. Napisać main.cpp które tylko zrobić inicjalizacje systemu
4. Puscić przez ESP32
5. Podłączyć moduł SD
6. telemetry.csv --> do rozwinięcia jak tam dojdę



TelemetryFrame

DOKUMENTACJA:
1. PROTOCOL.md - z projektu z githuba, opis przesyłanych ramek

co sprawdzić?
- sekcja opisujaca pakiety monitorujące - 3A,3B i 3C
- mapa offsetów, z której wyciągniemy:
    - RPM
    - Battery Voltage
    - Motor Current
    - TPS (gas pedal)
    - Brake
    - Hall Sensors
    - Temperatures
    - Error Status
    itd.

2. CAN Broadcast Protocol --> nie uzywamy CAN ale tam są n
opisane te ramki a wyglądają jak te co widziałam w monitorze

czyli jak interpretować liczby


3. klsreader (projekt z GitHuba)



PRZEPŁYW DANYCH

UART --> KellyProtocol --tworzy-> RawPacket --tworzy-> PelemetryParser --tworzy-> TelemetryFrame --tworzy-> CSVLogger --tworzy-> telemetry.csv

KellyProtocol
------------
Odpowiada za komunikację UART z kontrolerem Kelly.
Nie interpretuje danych.
Nie zapisuje danych.

RawPacket
---------
Przechowuje surową odpowiedź otrzymaną od Kelly.

TelemetryParser
---------------
Zamienia RawPacket na TelemetryFrame.

TelemetryFrame
--------------
Reprezentuje aktualny stan pojazdu.

CSVLogger
---------
Zapisuje TelemetryFrame do pliku CSV.



MODELE DANYCH

1. RawTelemetry --> kopia z kelly
2. TelemetryFrame --> Throttle = 73%, Battery = 51.2 V itd
3. TelemetryRecord --> to, co zapisujemy do CSV


|PLIK| ODPOWIEDZIALNOŚĆ|
|---|---|
|main.cpp| inicjalizacja i główna pętla |
| KellyProtocol | komunikacja UART |
| RawPacker | surowe dane |
| TelemetryParser | dekodowanie pakietów |
| TelemetryFrame | model danych |
| CSVLogger | zapis CSV |

UART
      │
      │ bytes
      ▼
KellyProtocol
      │
      │ RawPacket
      ▼
TelemetryParser
      │
      │ TelemetryFrame
      ▼
CSVLogger
      │
      │ CSV
      ▼
telemetry.csv
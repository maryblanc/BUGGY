# KONTROLER KELLY

**KELLY** - sinusoidal brushless DC motor controller (BLDC)

1. must be used eith Hall sensors
2. doesnt support sensorless burshless motors (?)


**MOSFET** - element wykonawczy, np. fizyczny "przełącznik" prądu

**SVPWM - space vector PWM** - sposób generowania napięć dla silnika
PWM - pulse with modulation

**FOC - field oriented control** - metoda sterowania momentem i prędkością silnika


**PIPELINE PRACY:**
FOC
↓
wylicza jakie napięcia/prądy są potrzebne
↓
SVPWM
↓
zamienia to na sygnały PWM
↓
MOSFET-y
↓
fizycznie przełączają prąd
↓
Silnik
↓
obraca się



<ins>programmed by:</ins>
- PC software
- ***android app?***


## GENERAL FUNCTIONS ##
**1. fault detection & protection**
- wykrywanie błędów i funkcje ochronne
- błędy można podejrzeć:
    - przez PC software
    - Android app

**2. battery voltage monitoring**
- ciągły monitoring napięcia baterii, w tym 'low battery threshold'
- jeśli napięcie:
    - za wysokie --> stop driving
    - za niskie --> stopniowe ograniczanie mocy
    - bardzo za niskie --> shutdown

**3. current protection**
- built-in current loop
- overcurrent protection
- ograniczanie prądu żeby nie uszkodzić układu

**4. temperature protection**
- monitoring temperatury silnika i sterownika
- configurable temperature thresholds
- przy wysokiej temperaturze:
    - current reduction
    - warning
    - shutdown

**5. regenerative braking**
- obsługa rekuperacji
- podczas regen:
    - monitoring napięcia baterii
    - ochrona przed przeładowaniem
**IMPORTANT:** - regen should be used together with mechanical brakes

**6. configurable limits**
- możliwość konfiguracji:
    - max forward speed
    - max reverse speed
    - current limits
    - protection thresholds

**7. programming & configuration**
- kontroler można programować przez:
    - PC software
    - USB cable
    - USB-RS232 converter
    - Android app using designated Bluetooth adapter

**8. power outputs**
- kontroler udostępnia:
    - +5V
    - +12V

- do zasilania:
    - Hall sensors
    - switchy
    - małych układów pomocniczych

**9. analog inputs**
- 3 analogowe wejścia:
    - throttle input
    - brake input
    - motor temperature input
- zakres:
    - 0-5V

**10. Hall sensors** --> <ins>controller requires Hall sensors</ins>
- only supports:
    - three-phase Hall position sensors
    - automatic Hall sensor identification

**11. speed signal** - controller can replicate Hall-A pulse signal
- can be used for:
    - speedometer
    - RPM/speed measurement

**12. boost mode** - configurable boost function
- po aktywacji:
    - full power independent of throttle position

**13. joystick throttle support**
- support for dual 0-5V joystick signals
- both:
    - forward
    - reverse

**14. cruise control**
- cruise control supported
    - only in forward direction

**15. Bluetooth support**
- optional Bluetooth communication
- requires dedicated Kelly adapter --> mamy adapter, gorzej z androidem? haha
- potential use:
    - debugging
    - telemetry
    - monitoring
    - configuration

**16. serial communication**
- support for user-defined serial communication
- potentially useful for:
    - telemetry
    - debugging
    - external communication with ESP32

**17. CAN bus**
- optional CAN bus support
- default:
    - 250 kHz (25000 baud w Arduino IDE)
***IMPORTANT:***
***- KLS-N controllers do NOT include CAN by default*** --> !DO SPRAWDZENIA!
- potentially:
    - telemetry source for ESP32

**OPTIONAL**
- field weakening?
- built-in DC/AC module?
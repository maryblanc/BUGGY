definicja danych systemowych:
- jakie dane istnieją
- jak są reprezentowane
- jakie mają jednostki
- jakie zakresy
- jak wyglada glowny telemetry pack



ROZKMINKI 16.05

## Jakie dane istnieją?
### 1. dane z sensorów
![Proponowane czujniki z prezentacji](assets/proponowane_czujniki.jpeg)

z Kelly pewnie:
- RPM
- poziom baterii akumulatorów 
- prąd i napięcie akumulatorów
- przebieg
- napięcie i prąd silnika

### 2. dane wypadkowe
- moc chwilowa (P = U · I)
- zużycie energii
- maksymalne wartości prądu
- spadki napięcia



**Czy to przypadkiem nie są wszystkie parametry?**
    **--> czy nie będzie trzeba czytać nic bezpośrednio z czujników tylko wszystko z CANa?**



## Jakie mają typym jednostki itd?
| lp. |Zmienna | Typ | Jednostka |
| --- | --- | --- | --- |
| 1 | natęzenie silnika | float | A |
| 2 | napięcie silnika | float | V |
| 3 |RPM | int | obroty/s (? to wtedy chyba rps?) |
| 4 | przebieg | int | km |
| 5 | prędkość | int | km/h |
| 6 | poziom baterii akumulatorów | int | % |
| 7 | natęzenie akumulatorów | float | A |
| 8 | napięcie akumulatorów | float | V |
| 9 | moc | float | W |
| 10 | zuzycie energii | float| kWh |
| 11 | max wartości prądu | float | A? |
| 12 | spadki napięcia | flaot | V? |

## Które mogą być uint16_t?
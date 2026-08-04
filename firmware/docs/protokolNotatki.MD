github mowi:

PC
  │
  ├── 0x3A 00 3A  → Monitor A
  │
  └── 0x3B 00 3B  → Monitor B

  Kelly odpowiada
  19-byte package

  ##PLAN:

  WARSTWA 1:

  KellyTelemetry - stan sterownika:

  KellyTelemetry
{
    throttle

    rpm

    phaseCurrent

    batteryVoltage

    controllerTemp

    motorTemp

    hallA
    hallB
    hallC

    brakeSwitch
    footSwitch
    forwardSwitch
    reverseSwitch

    commandDirection
    feedbackDirection

    errorFlags
}


WARSTWA 2 - Kelly ma dwie opcje - UART i CAN --> bierzemy UART

KellyTelemetry

↓

UART Packet A

↓

UART Packet B


potem Parser UART:

Packet A

↓

KellyTelemetry

i 

Packet B

↓

KellyTelemetry





                KellySimulator
                      │
                      ▼
              TelemetryFrame
                      │
                      ▼
            UARTPacketBuilder
              │            │
              ▼            ▼
         Monitor1      Monitor2
              │            │
              └──────┬─────┘
                     ▼
                KellyParser
                     │
                     ▼
              TelemetryFrame
                     │
                     ▼
         CSV / SD / Dashboard




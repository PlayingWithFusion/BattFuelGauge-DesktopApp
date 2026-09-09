# BattFuelGauge-DesktopApp

Windows desktop application for monitoring [Playing With Fusion Battery Fuel Gauge (BFG)](https://www.playingwithfusion.com/products/264) devices over CAN.

This project is a desktop port of the Playing With Fusion FRC RoboRIO library. Logic used to control Venom motors and to update firmware on Playing With Fusion devices was removed. It is provided to the FRC community free of charge as a starting point for team data collection, pit displays, or other custom tools.

The BFG is a precision current/voltage sensor integrated into an FRC battery cable assembly. It tracks state of charge, effective capacity, match statistics, battery age, and related health metrics. Product details: [playingwithfusion.com/products/264](https://www.playingwithfusion.com/products/264).

## Features

- Talks to BFG devices on a CAN bus from a Windows PC (no roboRIO required)
- Starts a local web server on port **5812** for live data and device management
- Serves a BFG dashboard at `/bfg.html`
- Exposes current BFG state as JSON at `/bfg_state.json`
- Compatible with **CANable** USB-CAN adapters running candlelight firmware
- Also works with most **PEAK PCAN** adapters

Typical uses:

- Pit-side battery checkout before a match
- Collecting BFG JSON for logging, dashboards, or scouting tools
- A starting point for team-specific data collection or display apps

## Requirements

- Windows (x64 recommended)
- [Visual Studio 2022](https://visualstudio.microsoft.com/) to build from source
- A supported USB-CAN adapter:
  - CANable (or compatible) running **candlelight** firmware
  - Most PEAK PCAN adapters
- One or more Playing With Fusion BFG devices on the CAN bus

WPILib code included in this repository was copied from the **2026 WPILib** release.

> **Safety:** If you connect a BFG while it is also attached to a battery charger, use a galvanically isolated CAN adapter. A non-isolated adapter can put charger voltage across your laptop.

## Building

1. Clone this repository.
2. Open `BatteryFuelGaugeMonitor.sln` in **Visual Studio 2022**.
3. Select a configuration (Release | x64 is recommended).
4. Build the solution.

The solution contains two projects:

| Project | Purpose |
| --- | --- |
| `BatteryFuelGaugeMonitor` | Main application and HTTP server |
| `candle` | candlelight USB-CAN driver support |

## Running

1. Connect the USB-CAN adapter to the PC and to the BFG CAN pigtail.
2. Run the built executable.
3. A console window should report that a compatible CAN device was found and that the web UI is available.
4. Point a browser at [http://127.0.0.1:5812](http://127.0.0.1:5812).

Press **Escape** in the console window to exit.

The root page lists Playing With Fusion CAN devices seen on the bus. Live BFG data is shown by opening:

```
http://127.0.0.1:5812/bfg.html?srcaddr=1
```

Replace `1` with the CAN ID (`srcaddr`) of the BFG you want to view.

## HTTP API

The application hosts a small HTTP server on **port 5812**.

| Method | Path | Description |
| --- | --- | --- |
| GET | `/` or `/index.html` | Device management / device list page |
| GET | `/bfg.html?srcaddr=<id>` | Live BFG dashboard for the given CAN ID |
| GET | `/bfg_state.json?srcaddr=<id>` | Current BFG data as JSON |
| POST | `/bfg.cgi` | BFG actions used by the dashboard (name, manufacturer, calibration, display orientation, reset, etc.) |

### Live dashboard

```
http://127.0.0.1:5812/bfg.html?srcaddr=1
```

The page polls `/bfg_state.json` about once per second and shows:

- Live voltage, current, state of charge, depth of discharge, and effective capacity
- Current charge/discharge cycle statistics
- Last robot-match statistics
- Battery name, manufacturer, age, capacity, serial number, and firmware version

### JSON query

Send a GET request to:

```
http://127.0.0.1:5812/bfg_state.json?srcaddr=1
```

Replace `srcaddr` with the CAN ID of the desired BFG. The response is suitable for custom dashboards, logging scripts, or pit-display software.

## Project layout

```
.
├── BatteryFuelGaugeMonitor.sln
├── LICENSE
├── README.md
└── src/
    ├── main.cpp                          # Application entry point
    ├── BatteryFuelGaugeMonitor.vcxproj
    ├── html/                             # Web UI assets (bfg.html, CSS, JS)
    ├── libPlayingWithFusion/
    │   ├── cpp/
    │   │   ├── FusionHttpServerConnection.cpp   # Web server routes and responses
    │   │   └── BattFuelGaugeImpl.cpp            # BFG CAN message decode
    │   └── include/
    ├── candle/                           # candlelight CANable driver
    ├── PCAN/                             # PEAK PCAN SDK bits
    ├── NI/                               # Wrappers for native roboRIO NI libraries
    └── wpilib/                           # Copied 2026 WPILib sources (HAL / wpinet / wpiutil)
```


## Origin

This codebase started as the Playing With Fusion FRC RoboRIO vendor library. On the roboRIO, instantiating a Playing With Fusion device class starts the same style of web server on port 5812. This desktop port keeps the BFG monitoring and HTTP pieces so teams can read gauges from a laptop instead of booting a roboRIO.

Venom motor control and device firmware-update flows were removed from this tree. 

## License

This project is released under the [MIT License](LICENSE). Copyright (c) 2026 Playing With Fusion, Inc.

It is provided as-is, with the hope that FRC teams can use it as a starting point. There is no warranty.

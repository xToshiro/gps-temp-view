# AirTemp_View
# Arduino Project for Urban Heat Island Monitoring with ESP32

This is an Arduino project developed to monitor urban heat islands in the city. It uses an ESP32 with 30 pins, a BME280 temperature, pressure, and humidity sensor, a u-blox NEO-6M GPS module, and an SD Card module to collect georeferenced data of temperature, pressure, and humidity along the route and save this data to a memory card.

## Objective

The objective of this project is to install the device in a vehicle to collect environmental data such as temperature, pressure, and humidity while the vehicle moves through the city. The data is recorded along with GPS location information, allowing for the mapping of urban heat islands in the city.

## Required Components

- ESP32 with 30 pins
- BME280 temperature, pressure, and humidity sensor
- u-blox NEO-6M GPS module
- SD Card module
- Prototyping board (protoboard)
- Power supply compatible with the ESP32
- Cables and wires for connections
- MicroSD card for data storage

## Assembly

1. Connect the ESP32, BME280 sensor, NEO-6M GPS module, and SD Card module to the prototyping board according to the appropriate connection scheme.

2. Power the ESP32 with the power supply and ensure all components are properly connected.

## Configuration

Before uploading the Arduino code to the ESP32, make sure to set up the Arduino IDE development environment with the necessary libraries for the components used (BME280, NEO-6M, SD Card, etc.). You will also need to configure the GPS module's network settings, such as baud rate, protocol, etc.

## Usage

1. Upload the Arduino code to the ESP32 using the Arduino IDE.

2. Place the device in the vehicle according to the necessary guidelines to ensure good GPS reception and accurate environmental data collection.

3. Power on the device and wait for it to start collecting data.

4. The collected data will be stored on the microSD card in a specific format, including date, time, latitude, longitude, temperature, pressure, and humidity information.

## Data Visualization

You can use data visualization software such as spreadsheets or geographic mapping software to analyze the collected data and map urban heat islands in the city.

# Project Update Log

## Version 4.0 - AirTemp View (Current Version)

### Release Date: 08/10/2023

#### Summary:

In the latest update, Version 4.0, named "AirTemp View," we have introduced significant improvements to the project's data collection process. Now, data is collected every second, providing more granular information for urban heat island monitoring.

#### What's New:

1. **Real-Time Clock (RTC) Synchronization:** With Version 4.0, we have implemented real-time clock (RTC) synchronization. The ESP32's internal RTC is now synchronized with the date and time data from the GPS module. This synchronization ensures accurate timekeeping during data collection.

2. **Second-Level Data Collection:** Data is now collected at the second level of granularity. This increased frequency allows for more precise tracking of environmental conditions. The ESP32's internal RTC is used as a synchronization factor for the seconds, ensuring that data is consistently collected every second.

3. **Backup GPS Synchronization:** Users now have the flexibility to use a backup GPS, such as a mobile application, for synchronization. Data can be synchronized later by aligning the date and time from the backup GPS with the collected data.

#### How to Upgrade:

To take advantage of these new features, please update your project code to Version 4.0. Refer to the updated documentation for instructions on how to implement RTC synchronization and utilize the enhanced data collection capabilities.

#### Notes:

Ensure that you are familiar with the changes introduced in this update and make any necessary adjustments to your project setup for optimal data collection and analysis.

---

**Note:** Always ensure the accuracy of GPS data for reliable synchronization, and consider privacy and data security when handling collected information.


## Contributions

Contributions to this project are welcome! Feel free to create issues or send pull requests with improvements.

## License

This project is distributed under the [GNU General Public License v3.0 (GPL-3.0)](LICENSE).

---

**Note:** Ensure that you are familiar with local regulations and privacy laws when using this monitoring device. It is essential to respect the privacy and rights of citizens.

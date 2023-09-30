# gps_temp_view
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

## Contributions

Contributions to this project are welcome! Feel free to create issues or send pull requests with improvements.

## License

This project is distributed under the [GNU General Public License v3.0 (GPL-3.0)](LICENSE).

---

**Note:** Ensure that you are familiar with local regulations and privacy laws when using this monitoring device. It is essential to respect the privacy and rights of citizens.

This is an Arduino Nano RP2040 Connect implementation of the
Friend wearable [protocol](https://docs.basedhardware.com/developer/Protocol/)
(with some simple extensions)

Known issues:

- The audio is streaming, but the Friend app is not getting transcription, likely mic gain issue

# Update:

```
arduino-cli update
arduino-cli upgrade
```

# Boards:

```
arduino-cli core install arduino:mbed_nano
arduino-cli core install arduino:mbed_rp2040
```

# Libraries:

```
arduino-cli lib install "Arduino_BuiltIn"
arduino-cli lib install "ArduinoBLE"
arduino-cli lib install "LittleFS_Mbed_RP2040"
arduino-cli lib install "WiFiNINA"
```

# Build

```
arduino-cli compile -b arduino:mbed_nano:nanorp2040connect -v
arduino-cli upload -b arduino:mbed_nano:nanorp2040connect -p COM5 -v
```
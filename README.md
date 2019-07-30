# Arduino-MQTT-IoT-
Collection of code i created to use arduinos as IoT devices to control relays and send information from optocouplers through MQTT.

## subscribe
some of arduinos will only receive data, and control lights accordingly

## publish
some of arduinos will only publish to various MQTT topics

## timestamping
in an effort of improving stability and reliability of these devices, i decided not to use millis() function at all, so Arduinos aren't aware of time, and all timestamps are created in the other end.

## usage

I created these codes to control lights with home assistant, but it can be used with varying setups

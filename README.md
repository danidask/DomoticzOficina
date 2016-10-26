# DomoticzOficina
Office domotics with Domoticz and Mysensors

Domoticz runs in a Raspberry Pi, connected to a mysensors serial gateway.

There are several nodes:
- Intercom_Interface
Fiscally connected to an ordinary intercom, manage the input from the doorbell and the output to the open door signal.

- Entrance_Node
It manages 4 fiscal switches and 4 relay outputs. 3 first switch direct drives the 3 entrance lights allowing to also controlling them remotely. The other switch report the status to the main controller (domoticz) to trigger macro actions (like turn on all lights) and the other output dives a socket with an air freshener

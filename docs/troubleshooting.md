# Troubleshooting

Symptoms, causes and fixes, cheapest check first.

| Symptom | Cause | Fix |
| --- | --- | --- |
| `sdoDownloadExpedited()` returns false, nothing on the bus | `size` is not 1, 2 or 4 | Use the size of the object's type |
| Every call returns false | Your `send()` fails: controller not started, bus-off, TX queue full | Check the controller is initialised, termination (60 Ω between CAN_H and CAN_L with power off), bit rate |
| Frames on the bus, error frames follow | No other node acknowledges, or bit rates differ | Power the node; set the same bit rate everywhere (500 kbit/s on Vortex V1) |
| Node ignores NMT | Wrong node ID, or the node has not finished booting | Wait for its boot-up message (`0x700 + node`, one byte `0x00`); check the ID |
| SDO abort from the node (`0x580 + node`, byte 0 `0x80`) | Object missing, read-only, wrong size or value out of range | Bytes 4 to 7 hold the CiA 301 abort code; check the EDS |
| CiA 402 drive does not reach Operation enabled | A command was sent before the previous state was reached, or the drive is in Fault | Read the statusword (`0x6041`) between steps; send Fault reset (`0x0080`) first |
| Motor does not move although enabled | Mode of operation not set, or no target written | Write `0x6060` and the target object for that mode |

Next: [API reference](api-reference.md).

"""The layers of the Vortex driver: session, transport interface, your transport, the bus and the device.

Reads: From top to bottom. Your application calls a VortexHostSession (its public methods are listed from
inc/vortex/VortexHostSession.hpp). The session encodes CANopen NMT and expedited SDO download frames as
VortexCanFrame values and hands each to the VortexCanTransport interface (its pure-virtual methods are listed from
inc/vortex/VortexCanTransport.hpp). Your transport class implements that interface with BaseCan, SocketCAN or a
test loopback, and puts the frame on a classic CAN bus. The Vortex device on the bus is the CANopen server.
"""

import re
from pathlib import Path

from hardfoc_mkdocs_theme.figkit import Fig

ROOT = Path(__file__).resolve().parents[3]
SESSION = "inc/vortex/VortexHostSession.hpp"
TRANSPORT = "inc/vortex/VortexCanTransport.hpp"
PROTOCOL = "inc/vortex/VortexProtocol.hpp"


def data():
    session = (ROOT / SESSION).read_text()
    methods = [m for m in re.findall(r"^\s+bool (\w+)\(", session, re.M)]
    transport = (ROOT / TRANSPORT).read_text()
    hooks = re.findall(r"virtual bool (\w+)\([^)]*\)[^;]*= 0;", transport)
    protocol = (ROOT / PROTOCOL).read_text()
    nmt = re.findall(r"^\s+(\w+) = 0x[0-9A-Fa-f]+U,", protocol, re.M)
    return {
        "methods": methods,
        "hooks": hooks,
        "nmt": nmt,
        "sources": [f"{SESSION}:VortexHostSession", f"{TRANSPORT}:VortexCanTransport", f"{PROTOCOL}:NmtCommand"],
    }


def build(d):
    f = Fig(
        "driver-layers",
        "Three layers, one of them yours",
        alt="Layer diagram: the application calls VortexHostSession, which encodes NMT and expedited SDO frames and "
        "passes them to the VortexCanTransport interface, which your transport implements with BaseCan, SocketCAN or "
        "a loopback, which sends them on the CAN bus to the Vortex device.",
        width=1200,
        kicker="Architecture",
        subtitle="The session and the frame encoding are the same everywhere. The transport is one method you write.",
        profile="overview",
        legend=[("brand", "this library"), ("teal", "you write"), ("slate", "bus and device")],
    )
    W = f.body_w
    app = f.card(0, 0, W, "Your application", ["host controller, test rig, Flux middleware ..."], tone="slate")
    top = app.b + 40
    chips_h = f.chips(16, f.region_inner_top(top), W - 32, [m + "()" for m in d["methods"]] +
                      ["NmtCommand::" + n for n in d["nmt"]], tone="brand")
    reg = f.region(0, top, W, f.region_inner_top(top) - top + chips_h + 18, "VortexHostSession", tone="brand",
                   sub="NMT and expedited SDO download, CiA 301 / 402 constants")
    tr = f.card(0, reg.b + 40, W, "VortexCanTransport", ["  ".join(h + "(const VortexCanFrame&)" for h in d["hooks"])],
                tone="brand", accent=True, mono=True)
    port = f.card(0, tr.b + 40, W, "Your transport", ["BaseCan (ESP32 TWAI, STM32 FDCAN), SocketCAN, loopback"],
                  tone="teal", accent=True)
    bus = f.card(0, port.b + 40, W, "CAN bus and Vortex device",
                 ["classic CAN, 11-bit COB-IDs: NMT 0x000, SDO request 0x600 + node; the device is the CANopen server"], tone="slate")
    for a, b in ((app, reg), (reg, tr), (tr, port), (port, bus)):
        f.connect(a, b, "bottom", "top", tone="rail")
    return f

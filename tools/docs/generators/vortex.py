"""Generated regions of the Vortex driver documentation, read from the code.

``<!-- generated: object-constants -->``: the object-dictionary constants of ``inc/vortex/VortexProtocol.hpp`` with
their comments. ``<!-- generated: nmt-commands -->``: the ``NmtCommand`` enum. ``<!-- generated: quickstart-code -->``:
the README's copy of the ``quickstart`` section of ``tests/docs/quickstart.cpp`` (compiled and checked by the host
build). ``hardfoc-docs gen --check`` fails when a region no longer matches the code.
"""

from __future__ import annotations

import re

from hardfoc_mkdocs_theme.paths import repo_root

PROTOCOL = "inc/vortex/VortexProtocol.hpp"


def _section(path: str, name: str) -> str:
    text = (repo_root() / path).read_text()
    m = re.search(rf"--8<-- \[start:{name}\]\n(.*?)\n[^\n]*--8<-- \[end:{name}\]", text, re.S)
    if not m:
        raise SystemExit(f"[gen] no section {name!r} in {path}")
    return m.group(1)


def object_constants(page) -> str:
    text = (repo_root() / PROTOCOL).read_text()
    rows = []
    comment = ""
    for line in text.splitlines():
        s = line.strip()
        if s.startswith("///"):
            comment = s[3:].strip()
            continue
        if s.startswith(("/**", "*")):
            part = s.lstrip("/*").rstrip("*/").replace("@brief", "").replace("**", "").strip()
            comment = (comment + " " + part).strip() if part else comment
            if s.startswith("/**"):
                comment = part
            continue
        m = re.match(r"inline constexpr std::uint(8|16)_t (\w+) = (0x[0-9A-Fa-f]+)U;", s)
        if m:
            bits, name, value = m.groups()
            kind = "sub-index" if bits == "8" else "index"
            rows.append(f"| `{name}` | `{value}` | {kind} | {comment.replace('|', '/')} |")
            comment = ""
        elif s:
            comment = ""
    head = ["| Constant | Value | Kind | Header comment |", "| --- | --- | --- | --- |"]
    src = f"Source: [`{PROTOCOL}`]({page.rel(PROTOCOL)})."
    return "\n".join(head + rows) + "\n\n" + src + "\n"


def nmt_commands() -> str:
    text = (repo_root() / PROTOCOL).read_text()
    body = re.search(r"enum class NmtCommand : std::uint8_t \{(.*?)\};", text, re.S).group(1)
    rows = [f"| `NmtCommand::{n}` | `{v}` |" for n, v in re.findall(r"(\w+) = (0x[0-9A-Fa-f]+)U,", body)]
    return "\n".join(["| Command | Byte 0 |", "| --- | --- |"] + rows) + "\n"


def quickstart_code() -> str:
    return "```cpp\n" + _section("tests/docs/quickstart.cpp", "quickstart") + "\n```\n"


REGIONS = {"object-constants": object_constants, "nmt-commands": nmt_commands, "quickstart-code": quickstart_code}

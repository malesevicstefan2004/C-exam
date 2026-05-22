# vCard Generator

A command-line tool that creates vCard 3.0 contact files (`.vcf`).
Written in C++ for the DHBW TI25 programming exam.

## Build

Requires **CMake ≥ 3.10** and a C++17-compatible compiler (GCC / MinGW).

```bash
mkdir build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

The executable is placed at `build/bin/vcard`.

## Usage

```
vcard [OPTIONS] [OUTPUTFILE]
```

| Option | Description |
|---|---|
| `-h`, `--help` | Show help and exit |
| `-f`, `--firstname NAME` | First name (repeat for additional names) |
| `-s`, `--surname NAME` | Last name (**required**) |
| `--org NAME` | Organization (repeat for multiple; joined with `;`) |
| `--phone-work NUM` | Work phone number (repeat for multiple) |
| `-p`, `--phone-home NUM` | Home phone number (repeat for multiple) |
| `-m`, `--email ADDR` | E-mail address (repeat for multiple) |
| `--programmer-info` | Print the author's vCard and exit |

`OUTPUTFILE` is optional. When omitted the vCard is written to **stdout**.

Both `-f`/`--firstname` and `-s`/`--surname` are **mandatory**.

## Examples

**Print to stdout:**
```bash
vcard -f Thomas -s Staudacher
```

**Multiple first names:**
```bash
vcard -f Erika -f Elisabeth -f Maria -s Mustermann --org DHBW
```

**All fields, save to file:**
```bash
vcard -f Thomas -s Staudacher \
      --org TIA \
      --phone-work +49754177961003 \
      -p +49754144640 \
      -m thomas@example.com \
      contact.vcf
```

**Programmer identification:**
```bash
vcard --programmer-info
```

## Sample Output

```
BEGIN:VCARD
VERSION:3.0
N:Staudacher;Thomas;;;
FN:Thomas Staudacher
ORG:TIA
TEL;TYPE=WORK,VOICE:+49754177961003
TEL;TYPE=HOME,VOICE:+49754144640
EMAIL;TYPE=PREF,INTERNET:thomas_staudacher@yahoo.de
REV:20260402T133340Z
END:VCARD
```

## Project Structure

```
.
├── CMakeLists.txt          Top-level build configuration
├── main.cpp                Entry point, option parsing (getopt_long)
├── list/
│   ├── CMakeLists.txt
│   ├── include/list.h      Linked-list declarations (VcardNode, VcardOrder)
│   └── src/list.cpp        Linked-list implementation
└── vcard/
    ├── CMakeLists.txt
    ├── include/vcard.h     vCard builder declarations
    └── src/vcard.cpp       vCard builder, help, programmer-info
```

## Requirements Coverage

| Requirement | Description | Status |
|---|---|---|
| ReqFunc01/02 | Help text via `-h` / `--help` | ✓ |
| ReqFunc03 | `--programmer-info` vCard output | ✓ |
| ReqFunc04 | REV field with ISO 8601 timestamp | ✓ |
| ReqFunc05–08 | `-f`/`--firstname`, `-s`/`--surname`, mandatory check | ✓ |
| ReqFunc09 | `--org` multi-value, semicolon-joined | ✓ |
| ReqFunc10/11 | `--phone-work`, `-p`/`--phone-home`, multiple | ✓ |
| ReqFunc12 | `-m`/`--email`, multiple | ✓ |
| ReqFunc13–19 | Custom sorted linked list with heap memory | ✓ |
| ReqFunc20/21 | List code in `list.h` / `list.cpp` | ✓ |
| ReqFunc22 | Elements sorted by field order in list | ✓ |
| ReqFunc23/24 | Output to file or stdout | ✓ |
| ReqNonFunc01–04 | GNU g++, WSL Linux, CMake, binary named `vcard` | ✓ |
| ReqNonFunc05 | `getopt` / `getopt_long` for option parsing | ✓ |
| ReqNonFunc06/07 | Headers/sources in separate dirs, include via path | ✓ |
| ReqNonFunc08 | `#ifndef` header guards (MinGW + GNU compatible) | ✓ |
| ReqNonFunc09 | All heap memory freed after use | ✓ |

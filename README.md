# vCard Generator

Command-line tool for creating vCard 3.0 contact files.
DHBW TI25 programming exam – written in C++.

## Build

CMake ≥ 3.10 and GCC (or MinGW) required.

```bash
mkdir build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Executable: `build/bin/vcard`

## Options

| Option | Description |
|---|---|
| `-h`, `--help` | Show help and exit |
| `-f`, `--firstname NAME` | First name (can be repeated for additional names) |
| `-s`, `--surname NAME` | Last name (**required**) |
| `--org NAME` | Organization (can be repeated, joined with `;`) |
| `--phone-work NUM` | Work phone (can be repeated) |
| `-p`, `--phone-home NUM` | Home phone (can be repeated) |
| `-m`, `--email ADDR` | E-mail address (can be repeated) |
| `--programmer-info` | Print author vCard and exit |

`OUTPUTFILE` can be added at the end to write to a file instead of stdout.

## Examples

```bash
# basic
vcard -f Thomas -s Staudacher

# multiple first names
vcard -f Erika -f Elisabeth -f Maria -s Mustermann

# all fields, output to file
vcard -f Thomas -s Staudacher --org TIA --phone-work +49754177961003 -p +49754144640 -m t@example.com contact.vcf

# author info
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
├── CMakeLists.txt
├── main.cpp
├── list/
│   ├── include/list.h
│   └── src/list.cpp
└── vcard/
    ├── include/vcard.h
    └── src/vcard.cpp
```

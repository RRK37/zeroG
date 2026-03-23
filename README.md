# ZeroG

A custom wireless communication protocol for Arduino Uno microcontrollers. ZeroG enables packet-based data transmission between Arduino boards using pulse-width modulation, with support for addressing, acknowledgments, and parity-based error detection.

## Hardware Requirements

- Arduino Uno (ATmega328P)
- 16x2 LCD display module
- Wireless transceiver hardware

### Pin Configuration

| Component | Pin |
|-----------|-----|
| LCD RS | 12 |
| LCD EN | 10 |
| LCD D4–D7 | 5, 4, 3, 2 |
| Transmitter (PWM) | 11 |
| Receiver (analog) | A4 |

## Protocol Overview

ZeroG uses a synchronous, packet-based protocol with a 40ms base timing unit.

**Packet structure** (16 bits):

| Field | Bits | Description |
|-------|------|-------------|
| ADR | 3 | Destination address (0 = ACK) |
| PID | 4 | Packet ID |
| DAT | 8 | Data payload |
| PAR | 1 | Parity bit (XOR of all bits) |

**Key features:**
- "Hello" handshake sequence for synchronization
- Passive and active listening modes
- ACK-based reliable delivery
- 3-bit addressing (up to 7 nodes)
- Multi-byte message sending (up to 16 characters)

## Building and Uploading

Requires [PlatformIO](https://platformio.org/).

```bash
# Build
platformio run -e uno

# Upload to Arduino
platformio run -e uno -t upload
```

## Project Structure

```
src/
├── main.cpp              # Application entry point, LCD setup, receive loop
├── transceiver_uno.cpp   # Protocol implementation (send, listen, ACK)
└── transceiver_uno.h     # Function declarations and pin definitions
```

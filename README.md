# CYBT_test_task
# Bluetooth UART GUI Application
This is a cross-platform Qt-based GUI application that allows basic interaction with a Bluetooth® module (e.g., CYBT-353027-EVAL) over UART.

## Features
✅ Connect and disconnect from a serial port (UART)

✅ Send raw HCI commands to the Bluetooth module

❌ Response parsing is not yet implemented

## Purpose
This application serves as a starting point for communicating with Bluetooth® modules using HCI commands via UART. It can be used for development, debugging, or educational purposes.

Current Functionality
Sends HCI Reset command:
01 03 0C 00

Sends Read BD_ADDR command:
01 09 10 00

Sends Write BD_ADDR command (example for address 0x112233445566):
01 01 FC 06 66 55 44 33 22 11

**Note:** The commands are sent correctly, but the application does not yet handle or display responses from the module.

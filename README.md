# MachineRX
This project is under development and is not ready for use.

The purpose of this experimental project, is to create a cross=platform C++ wrapper and message to maximize code development and reuse back and forth between a PC and a microcontroller.

The is intended to support integration with Micro-ROS, for that the library intends to mirror ROS2 messages to internal thread "blocks".

I make no promises, but check back later for updates. 


### MachineRPX
A lightweight wrapper for POSIX threads with FreeRTOS+POSIX and inter-process communication targeted for machine control applications.


The host (linux) example works fine, except that POSIX doesn't allow for read-only (over-write) queues used to publish topics to other threads, so a shared memspace is used instead with mutex access locks. 
Issues:

### MachineRFX
A lightweight wrapper for FreeRTOS with FreeRTOS PC Emulator and inter-process communication targeted for machine control applications.

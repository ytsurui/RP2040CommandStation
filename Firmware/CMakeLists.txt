# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(RP2040_DCC_CommandStation C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(
    RP2040_DCC_CommandStation
    main.cpp
    peripheral/dccsignalport.cpp
    peripheral/eventTimer.cpp
    peripheral/adcReader.c
    peripheral/powerButton.cpp
    peripheral/i2c_ctrl.c
    peripheral/fail_led.cpp
    peripheral/led_comm.cpp
    peripheral/uart_ctrl.cpp
    throttle/peripheral/lcd.c
    throttle/peripheral/direction.cpp
    throttle/peripheral/keypad.cpp
    app/dccsignal.cpp
    app/current_monitor.cpp
    app/voltage_monitor.cpp
    app/throttle.cpp
    app/trainpacket.cpp
    app/acc_ctrl.cpp
    app/progtrak.cpp

    app/loconet/packetRouter.cpp
    app/loconet/responser.cpp
    app/loconet/exec.cpp
    app/loconet/train_slot.cpp
    app/loconet/train_slot_global.cpp

    app/mt40bus/comparestr.cpp
    app/mt40bus/datareceiver.cpp
    app/mt40bus/datasender.cpp
    app/mt40bus/cmdexec.cpp

    app/trainctrl.cpp

    peripheral/uarttest.cpp

    app/throttle/th_main.cpp
    app/throttle/th_top.cpp
    app/throttle/th_trainctrl.cpp
    app/throttle/th_switchctrl.cpp
    app/throttle/th_prog.cpp
)

pico_set_program_name(RP2040_DCC_CommandStation "RP2040_DCC_CommandStation")
pico_set_program_version(RP2040_DCC_CommandStation "0.1")

pico_enable_stdio_uart(RP2040_DCC_CommandStation 0)
pico_enable_stdio_usb(RP2040_DCC_CommandStation 1)

# Add the standard library to the build
target_link_libraries(
    RP2040_DCC_CommandStation
    pico_stdlib
    pico_multicore
    hardware_timer
    hardware_adc
    hardware_i2c
    )

pico_add_extra_outputs(RP2040_DCC_CommandStation)


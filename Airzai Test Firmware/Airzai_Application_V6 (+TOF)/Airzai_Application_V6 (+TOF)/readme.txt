The system consists from two projects:
    1. Airzai Prototype (airzai_prototype_CC3220SF_LAUNCHXL_tirtos_ccs)
    2. TI-RTOS (tirtos_builds_CC3220SF_LAUNCHXL_release_ccs)

The TI-RTOS project contains prebuilt RTOS files. These files are generated automatically
and linked to Airzai Prototype project.



The Airzai Prototype project has the following structure:
    modules/
            Humidity/   - functions for SHT31 sensor
            LED/        - driver for lp55231 chip
            Proximitry/ - driver and functions for VL53L0x chip
            PWM/        - PWM control functions
            TOUCH/      - IQS213A sensor functions

            CC3220SF_LAUNCHXL.c - HW settings for the board
            i2c_api.c           - I2C common functions
            Board.h             - the board specific defines

    main_thread.c       - a thread with main device logic
    main_tirtos.c       - HW initalize and RTOS run procedures
    uart_parser.c       - LEDs commands parser and control functions
    uart_term.c         - UART initilization and I/O functionality
    uart_thread.c       - UART commands receiving
    conf.h              - configuration parameters



The project contains the following threads:

    mainThread()
        - setup peripheral
        - wait events from touch sensor
        - read and print temperature and humidity from SHT31 sensor

    UARTCommands_Task()
        - wait commands from UART
        - run the commands handlers

    VL53L0X_Task()
        - setup VL53L0X chip to continuous or single measurement mode
        - wait edge/range detection or print current range

    VL53L0X_Interrupts_Handler_Task()
        - used in VL53L0X continuous measurement mode
        - setup edge/range parameters
        - wait event from VL53L0X chip
        - print distance



The lp55231 driver contains structure "ledState" for 6 leds control. By default all leds have zero values in
all fields, so to see a led you need to setup a color, a brightness and switch-on the led. Available commands are:
    on #led                 - swith-on a led with specified number
    off #led                - swith-off a led with specified number
    dim #led #brightness    - set a brightness for a led with specified number
    color #led #color       - set a color for a led with specified number
    ?                       - print commands help message

Possible values:
    #led        - 1...6
    #brightness - 0...254
    #color      - blue, green, skyblue, red, purple, yellow, white



The file "conf.h" contains configuration parameters for debug, threads modes and etc. The parameters which
could be useful for evaluating are:

    LED_DEMO_ENABLE     - switch-on LED work demonstration in mainThread()
    SHT31_DEMO_ENABLE   - continuously read temperature and humidity in main mainThread()

    TOF_ENABLE_CONTUNIOUS_MEAS_INT  - switch TOF sensor mode: continuous or single measurement
    VL53L0X_THRESHOLD_MODE          - TOF interrupt mode (in continuous mode)
    VL53L0X_THRESHOLD_LOW           - low edge
    VL53L0X_THRESHOLD_HIGH          - high edge


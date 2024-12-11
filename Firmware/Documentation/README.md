- # STM32 Node Management System with JSON over UART

  ## Project Overview
  This project implements a node management system on an STM32 microcontroller. It leverages FreeRTOS for task management, enabling efficient handling of multiple nodes such as sensors and actuators. Communication with the system is facilitated through JSON messages sent and received over UART, allowing dynamic initialization, configuration, and control of nodes.

  ## Development Important Notes !!
  
  Initially, the project was planned to be developed and tested using Proteus simulation software. However, after spending three days troubleshooting numerous software bugs in Proteus, which turned out to be a global issue with the software, I had to move to real hardware to meet the project deadline. This decision allowed for seamless integration and testing of the system in a real-world environment, ensuring the robustness and reliability of the implementation.
  
  ## System Requirements
  - **Hardware:**
    - STM32 Microcontroller
    - LM35DZ Temperature Sensor
    - LDR Light Sensor
    - Relay Actuator
    - BLE Module (for wireless communication)
    - UART Monitor (e.g., serial terminal software)
  - **Software:**
    - STM32CubeIDE
    - FreeRTOS
    - Custom Drivers (USART, ADC, GPIO)
    - Third-party Libraries: JSON Parsing Library
  
  ## Architecture and Design
  The project is structured into modular components:
  - **Drivers:** Custom drivers for USART, ADC, and GPIO were developed to ensure precise control and integration with hardware.
  - **Middleware:** A JSON parsing library is used to handle incoming and outgoing JSON messages.
  - **RTOS Tasks:**
    - Task to handle UART communication.
    - Task to handle sensor data collection
    - Task for light sensor data collection
    - Task to manage relay control
    - Task for processing JSON data
  
  ### JSON Communication Protocol
  #### Commands Sent to STM32
  - **Enable Node:** `{"command":"ENA", "nodeID": , "data":}`
  - **Disable Node:** `{"command":"DIS", "nodeID": , "data":}`
  - **Activate Node:** `{"command":"ACT", "nodeID": , "data":}`
  - **Request Status:** `{"command":"STA", "nodeID": , "data":}`
  - **Set Duration:** `{"command":"DUR", "nodeID": , "data":}`
  
  #### Responses from STM32
  - **Sensor Node:** `{"nodeType":"NS", "nodeID": , "data":}`
  - **Actuator Node:** `{"nodeType":"NA", "nodeID": , "data":}`
  
  ### Test Case Example
  1. **Enable Temperature Sensor:**
     - Tx: `{"command":"ENA", "nodeID":128, "data":NULL}`
     - Rx: `{"nodeType":"NS", "nodeID":128, "data":"DONE"}`

  2. **Set Sensor Duration:**
     - Tx: `{"command":"DUR", "nodeID":128, "data":"5"}`
  
  3. **Activate Relay Actuator:**
     - Tx: `{"command":"ACT", "nodeID":80, "data":"1"}`
  
  4. **Request Relay Status:**
     - Tx: `{"command":"STA", "nodeID":80, "data":NULL}`
     - Rx: `{"nodeType":"NA", "nodeID":80, "data":"1"}`
  
  ## Functional Specifications
  1. **Temperature Sensor Node:** Reads temperature values using the LM35DZ sensor and reports them periodically or upon request.
  2. **Light Sensor Node:** Monitors light intensity using the LDR sensor and reports values.
  3. **Relay Actuator Node:** Responds to activation and deactivation commands and reports status.
  
  ## Setup and Installation
  1. Clone the repository from GitHub.
  2. Open the project in STM32CubeIDE.
  3. Compile and flash the firmware onto the STM32 microcontroller.
  4. Connect the hardware components as per the schematic.
  5. Use a UART monitor to send and receive JSON messages.
  
  

  ## Testing and Validation
  - Extensive tests were conducted to validate JSON command processing, node state management, and UART communication.
  - The system was tested using a BLE module to send commands wirelessly and verify node responses.
  - Real-time data acquisition from sensors and control of actuators were verified using the UART monitor.
  
  ## Acknowledgment
  This project demonstrates expertise in embedded systems, FreeRTOS, and peripheral driver development, providing a scalable solution for node management in IoT and automation applications.

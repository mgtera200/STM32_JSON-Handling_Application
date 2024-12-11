
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "STM32F103x8.h"
#include "GPIO_DRIVER.h"
#include "USART_DRIVER.h"
#include "cJSON.h"
#include "ADC.h"

// Declare handles for the UART and sensor tasks, as well as a notification value
TaskHandle_t uartTaskHandle = NULL;
TaskHandle_t sensorTaskHandle = NULL;

// Define node IDs for sensors and actuators to be used in the system
#define TEMP_SENSOR_NODE_ID 128
#define LIGHT_SENSOR_NODE_ID 129
#define RELAY_ACTUATOR_NODE_ID 80
#define QUEUE_LENGTH 10 // Define the maximum number of items that the queue can hold
#define QUEUE_ITEM_SIZE sizeof(JsonMessage) // Define the size of each queue item (a JsonMessage)

// Structure to represent JSON message data, including command, node ID, and data payload
typedef struct {
	char command[64];  // Command to be executed by the system
	int nodeID;        // Unique identifier for the node (sensor/actuator)
	char data[32];     // Additional data associated with the command
} JsonMessage;

// Enum to represent possible GPIO ports for relay control
typedef enum {
	PORTA,
	PORTB,
	PORTC
} RELAY_GPIO_PORT_t;

// Enum to represent USART peripherals available for communication
typedef enum {
	USART_1,
	USART_2
} USART_NUM_t;

// RTOS Handlers and synchronization objects (semaphores and queues)
xQueueHandle xQueueHandel = NULL;    // Queue handle for communication between tasks
SemaphoreHandle_t xJsonSemaphore;    // Semaphore for JSON message processing synchronization
SemaphoreHandle_t USARTSemaphore = NULL; // Semaphore for USART access synchronization
SemaphoreHandle_t relaySemaphore = NULL; // Semaphore for relay task synchronization
SemaphoreHandle_t tempSensorSemaphore = NULL; // Semaphore for temperature sensor task synchronization
SemaphoreHandle_t lightSensorSemaphore = NULL; // Semaphore for light sensor task synchronization

QueueHandle_t xJsonQueue;  // Queue to store JSON messages
TaskHandle_t xTempTaskHandle = NULL; // Handle for temperature sensor task
TaskHandle_t xUartTaskHandle = NULL; // Handle for UART command task
SemaphoreHandle_t xUartMutex; // Mutex for UART communication

// Global variables for node control and sensor data
static int counter = 0;      // Counter for processing UART data

char jsonBuffer[64];         // Buffer to store incoming JSON data
int tempSensorDuration = 2;  // Duration (in seconds) between temperature sensor readings
int lightSensorDuration = 2; // Duration (in seconds) between light sensor readings
uint8_t relayStatus = 0;     // Relay status: 0 = OFF, 1 = ON
uint8_t relayLastStatus = 0; // Last known relay status for state tracking
char num[10];                // Buffer for ADC result
int analog_rx_temperature = 0; // Variable to store the temperature reading
int analog_rx_light = 0;     // Variable to store the light sensor reading

// Function Prototypes for initialization and task handling
void RELAY_Init(RELAY_GPIO_PORT_t port, char pin_num_signal);
void RELAY_DeInit(RELAY_GPIO_PORT_t port, char pin_num_signal);
void UART_Init(USART_NUM_t uart_num);
void Usart_callback(interrupts_Bits *);

// FreeRTOS task functions
void uartTask(void *pvParameters); // Task to handle UART communication
void sensorTask(void *pvParameters); // Task to handle sensor data collection
void lightsensorTask(void *pvParameters); // Task for light sensor data collection
void relayTask(void *pvParameters); // Task to manage relay control
void JsonProcessingTask(void *pvParameters); // Task for processing JSON data

// Main entry point for the application
int main(void) {
	AFIO_CLOCK_EN(); // Enable AFIO clock for alternate function I/O
	USART1_CLOCK_EN(); // Enable USART1 clock

	// Initialize UART for communication
	UART_Init(USART_1);

	// Create binary semaphores for synchronization between tasks
	xJsonSemaphore = xSemaphoreCreateBinary();
	USARTSemaphore = xSemaphoreCreateBinary();
	relaySemaphore = xSemaphoreCreateBinary();
	tempSensorSemaphore = xSemaphoreCreateBinary();
	lightSensorSemaphore = xSemaphoreCreateBinary();

	xSemaphoreGive(USARTSemaphore); // Give the USART semaphore to allow UART communication

	// Create a queue to hold JSON messages with specified length and item size
	xJsonQueue = xQueueCreate(QUEUE_LENGTH, QUEUE_ITEM_SIZE);

	// Create tasks for UART communication, JSON processing, and sensor reading
	xTaskCreate(uartTask, "UART_Task", 450, NULL, 3, &xUartTaskHandle);
	xTaskCreate(JsonProcessingTask, "JSON Processor", 400, NULL, 3, NULL);
	xTaskCreate(sensorTask, "Sensor_Task", 200, NULL, 2, &xTempTaskHandle);
	xTaskCreate(lightsensorTask, "Light_Sensor_Task", 200, NULL, 2, NULL);
	xTaskCreate(relayTask, "Relay_Task", 128, NULL, 1, NULL); // Reduced stack size for relay task

	// Start the FreeRTOS scheduler to begin task execution
	vTaskStartScheduler();

	// Main loop should never be reached if FreeRTOS scheduler is working correctly
	while (1) { /* Infinite loop to keep the main function alive */ }
}

// USART callback function for handling incoming data
void Usart_callback(interrupts_Bits * irq) {
	// Receive a character from USART and store it in the buffer
	MCAL_USART_ReceiveChar(USART1, jsonBuffer + counter);

	// Handle the case when the received buffer is not a valid JSON message
	if (jsonBuffer[0] != '{') {
		counter = 0; // Reset counter if not a JSON start character
	}
	// If a complete JSON message is received
	else if (jsonBuffer[counter] == '}') {
		jsonBuffer[counter + 1] = '\0'; // Null-terminate the string
		counter = 0; // Reset counter for next message

		// Signal the JSON processing task that a new message is available
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		xSemaphoreGiveFromISR(xJsonSemaphore, &xHigherPriorityTaskWoken);

		// Request a context switch if a higher priority task is woken up
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
	else {
		counter++; // Increment counter to store the next character in the buffer
	}
}

// UART Initialization function
void UART_Init(USART_NUM_t uart_num) {
	USART_Config_t UART_CNFG_s;
	UART_CNFG_s.Async_EN = USART_Enable;
	UART_CNFG_s.Async_Config_s.Baud_Rate = 9600; // Set baud rate to 9600
	UART_CNFG_s.Async_Config_s.Stop_Bits = Stop_1; // 1 stop bit
	UART_CNFG_s.Async_Config_s.Word_Length = Eight_bits; // 8-bit word length
	UART_CNFG_s.interrupts_CNFG.RX_Interrupt_Enable_Or_Disable = USART_Enable; // Enable RX interrupt
	UART_CNFG_s.CallBack_FN = Usart_callback; // Set callback function for USART interrupts

	// Initialize USART1 or USART2 depending on the selected USART
	if (uart_num == USART_1) {
		MCAL_USART_Init(USART1, &UART_CNFG_s);
		MCAL_USART_GPIO_Pins_Config(USART1); // Configure USART1 GPIO pins
	}
	else if (uart_num == USART_2) {
		MCAL_USART_Init(USART2, &UART_CNFG_s);
		MCAL_USART_GPIO_Pins_Config(USART2); // Configure USART2 GPIO pins
	}
}

// Relay Initialization function
void RELAY_Init(RELAY_GPIO_PORT_t port, char pin_num_signal) {
	Pin_Config_t GPIO_Pin_CNFG;
	GPIO_Pin_CNFG.mode = Output_Open_drain; // Set the mode to open-drain for relay control
	GPIO_Pin_CNFG.Speed_Output = speed_10; // Set output speed to low
	GPIO_Pin_CNFG.Pin_Num = pin_num_signal; // Set the pin number for relay control

	// Initialize GPIO port A, B, or C based on the selected port
	if (port == PORTA) {
		MCAL_GPIO_Init(GPIOA, &GPIO_Pin_CNFG);
	}
	else if (port == PORTB) {
		MCAL_GPIO_Init(GPIOB, &GPIO_Pin_CNFG);
	}
	else if (port == PORTC) {
		MCAL_GPIO_Init(GPIOC, &GPIO_Pin_CNFG);
	}
}

// Relay De-Initialization function
void RELAY_DeInit(RELAY_GPIO_PORT_t port, char pin_num_signal) {
	// Deinitialize the relay pin on the selected GPIO port
	if (port == PORTA) {
		MCAL_GPIO_DeInit(GPIOA, pin_num_signal);
	}
	else if (port == PORTB) {
		MCAL_GPIO_DeInit(GPIOB, pin_num_signal);
	}
	else if (port == PORTC) {
		MCAL_GPIO_DeInit(GPIOC, pin_num_signal);
	}
}

// Sensor Task for reading temperature and light sensor data
void sensorTask(void *pvParameters) {
	while (1) {
		// Wait for the semaphore indicating that the sensor is enabled
		if (xSemaphoreTake(tempSensorSemaphore, portMAX_DELAY) == pdTRUE) {
			// Perform temperature sensor reading
			if (adc_check(ADC1)) {
				int data = 0;
				float temperature = 0.0f;
				const float Vref_measured = 3.27f; // Measured Vref for ADC
				const float offset = -10.0f; // Calibration offset for temperature
				data = adc_rx(ADC1, PA, 0); // Read ADC value from temperature sensor

				// Convert ADC value to voltage
				float voltage = (float)data * Vref_measured / 4095.0f;

				// Convert voltage to temperature
				temperature = (voltage / 0.01f) + offset;
				analog_rx_temperature = (int)temperature;

				// Send the temperature data as a JSON message over UART
				if (xSemaphoreTake(USARTSemaphore, portMAX_DELAY) == pdTRUE) {
					char jsonString[100];
					snprintf(jsonString, sizeof(jsonString), "{\"nodeType\":\"NS\", \"nodeID\": %d, \"data\": \"%d°C\"}", TEMP_SENSOR_NODE_ID, analog_rx_temperature);

					// Send the JSON string byte by byte over USART
					for (int i = 0; i < strlen(jsonString); i++) {
						MCAL_USART_SendChar(USART1, jsonString[i]);
					}

					xSemaphoreGive(USARTSemaphore); // Release the USART semaphore
				}
			}
			vTaskDelay(tempSensorDuration * 1000); // Delay for the next reading
			xSemaphoreGive(tempSensorSemaphore); // Release the semaphore after the task is complete
		}
	}
}
// Light Sensor Task
void lightsensorTask(void *pvParameters) {
    // Infinite loop to continuously monitor light sensor status
    while (1) {
        // Wait for the sensor to be enabled (sensorEnabled flag is set)
        if (xSemaphoreTake(lightSensorSemaphore, portMAX_DELAY) == pdTRUE) {
            // Perform sensor reading if enabled
            if(adc_check(ADC2)) {
                // Read light sensor data from ADC2
                analog_rx_light = adc_rx(ADC2, PA, 1);

                // Check if USART semaphore is available for communication
                if (xSemaphoreTake(USARTSemaphore, portMAX_DELAY) == pdTRUE) {
                    // Prepare a JSON string to send light sensor data
                    char jsonString[100];
                    snprintf(jsonString, sizeof(jsonString), "{\"nodeType\":\"NS\", \"nodeID\": %d, \"data\": \"%d\"}", LIGHT_SENSOR_NODE_ID, analog_rx_light);

                    // Send the JSON string character by character via USART
                    for (int i = 0; i < strlen(jsonString); i++) {
                        MCAL_USART_SendChar(USART1, jsonString[i]);
                    }

                    // Release USART semaphore after transmission
                    xSemaphoreGive(USARTSemaphore);
                }
            }
            // Delay for the light sensor reading interval (in seconds)
            vTaskDelay(lightSensorDuration * 1000);
            // Release light sensor semaphore
            xSemaphoreGive(lightSensorSemaphore);
        }
    }
}

// UART Task to handle receiving and processing commands
void uartTask(void *pvParameters) {
    JsonMessage jsonMsg;

    // Infinite loop to continuously receive commands from UART
    while (1) {
        // Wait for a message in the queue
        if (xQueueReceive(xJsonQueue, &jsonMsg, portMAX_DELAY) == pdTRUE) {
            // Command handling for enabling sensors or actuators
            if (strcmp(jsonMsg.command, "ENA") == 0) {
                // Enable temperature sensor if node ID matches
                if(jsonMsg.nodeID == TEMP_SENSOR_NODE_ID) {
                    // Prepare and send "DONE" message in JSON format
                    char jsonString[100];
                    snprintf(jsonString, sizeof(jsonString), "{\"nodeType\":\"NS\", \"nodeID\": %d, \"data\": \"DONE\"}", jsonMsg.nodeID);
                    if (xSemaphoreTake(USARTSemaphore, portMAX_DELAY) == pdTRUE) {
                        // Send JSON string over USART
                        for (int i = 0; i < strlen(jsonString); i++) {
                            MCAL_USART_SendChar(USART1, jsonString[i]);
                        }
                        xSemaphoreGive(USARTSemaphore);
                    }
                    // Initialize ADC for temperature sensor
                    adc_init(ADC1, PA, 0);
                }
                // Enable light sensor if node ID matches
                else if(jsonMsg.nodeID == LIGHT_SENSOR_NODE_ID) {
                    // Prepare and send "DONE" message in JSON format
                    char jsonString[100];
                    snprintf(jsonString, sizeof(jsonString), "{\"nodeType\":\"NS\", \"nodeID\": %d, \"data\": \"DONE\"}", jsonMsg.nodeID);
                    if (xSemaphoreTake(USARTSemaphore, portMAX_DELAY) == pdTRUE) {
                        // Send JSON string over USART
                        for (int i = 0; i < strlen(jsonString); i++) {
                            MCAL_USART_SendChar(USART1, jsonString[i]);
                        }
                        xSemaphoreGive(USARTSemaphore);
                    }
                    // Initialize ADC for light sensor
                    adc_init(ADC2, PA, 1);
                }
                // Enable relay actuator if node ID matches
                else if(jsonMsg.nodeID == RELAY_ACTUATOR_NODE_ID) {
                    // Prepare and send "DONE" message in JSON format
                    char jsonString[100];
                    snprintf(jsonString, sizeof(jsonString), "{\"nodeType\":\"NA\", \"nodeID\": %d, \"data\": \"DONE\"}", jsonMsg.nodeID);
                    if (xSemaphoreTake(USARTSemaphore, portMAX_DELAY) == pdTRUE) {
                        // Send JSON string over USART
                        for (int i = 0; i < strlen(jsonString); i++) {
                            MCAL_USART_SendChar(USART1, jsonString[i]);
                        }
                        xSemaphoreGive(USARTSemaphore);
                    }
                    // Initialize relay actuator
                    RELAY_Init(PORTB, 5);
                    relayStatus = relayLastStatus;
                    xSemaphoreGive(relaySemaphore);
                }
            }
            // Command handling for disabling sensors or actuators
            else if (strcmp(jsonMsg.command, "DIS") == 0) {
                // Disable temperature sensor if node ID matches
                if(jsonMsg.nodeID == TEMP_SENSOR_NODE_ID) {
                    // Prepare and send "DONE" message in JSON format
                    char jsonString[100];
                    snprintf(jsonString, sizeof(jsonString), "{\"nodeType\":\"NS\", \"nodeID\": %d, \"data\": \"DONE\"}", jsonMsg.nodeID);
                    if (xSemaphoreTake(USARTSemaphore, portMAX_DELAY) == pdTRUE) {
                        // Send JSON string over USART
                        for (int i = 0; i < strlen(jsonString); i++) {
                            MCAL_USART_SendChar(USART1, jsonString[i]);
                        }
                        xSemaphoreGive(USARTSemaphore);
                    }
                    // De-initialize ADC for temperature sensor
                    if (xSemaphoreTake(tempSensorSemaphore, portMAX_DELAY) == pdTRUE) {
                        adc_Deinit(ADC1, PA, 0);
                    }
                }
                // Disable light sensor if node ID matches
                else if(jsonMsg.nodeID == LIGHT_SENSOR_NODE_ID) {
                    // Prepare and send "DONE" message in JSON format
                    char jsonString[100];
                    snprintf(jsonString, sizeof(jsonString), "{\"nodeType\":\"NS\", \"nodeID\": %d, \"data\": \"DONE\"}", jsonMsg.nodeID);
                    if (xSemaphoreTake(USARTSemaphore, portMAX_DELAY) == pdTRUE) {
                        // Send JSON string over USART
                        for (int i = 0; i < strlen(jsonString); i++) {
                            MCAL_USART_SendChar(USART1, jsonString[i]);
                        }
                        xSemaphoreGive(USARTSemaphore);
                    }
                    // De-initialize ADC for light sensor
                    if (xSemaphoreTake(lightSensorSemaphore, portMAX_DELAY) == pdTRUE) {
                        adc_Deinit(ADC2, PA, 1);
                    }
                }
                // Disable relay actuator if node ID matches
                else if(jsonMsg.nodeID == RELAY_ACTUATOR_NODE_ID) {
                    // Prepare and send "DONE" message in JSON format
                    char jsonString[100];
                    snprintf(jsonString, sizeof(jsonString), "{\"nodeType\":\"NA\", \"nodeID\": %d, \"data\": \"DONE\"}", jsonMsg.nodeID);
                    if (xSemaphoreTake(USARTSemaphore, portMAX_DELAY) == pdTRUE) {
                        // Send JSON string over USART
                        for (int i = 0; i < strlen(jsonString); i++) {
                            MCAL_USART_SendChar(USART1, jsonString[i]);
                        }
                        xSemaphoreGive(USARTSemaphore);
                    }
                    // De-initialize relay actuator
                    if (xSemaphoreTake(relaySemaphore, portMAX_DELAY) == pdTRUE) {
                        relayLastStatus = relayStatus;
                        RELAY_DeInit(PORTB, 5);
                    }
                }
            }
            // Command handling for activating or deactivating relays
            else if (strcmp(jsonMsg.command, "ACT") == 0) {
                if (strcmp(jsonMsg.data, "1") == 0) {
                    relayStatus = 1; // Activate relay
                }
                if (strcmp(jsonMsg.data, "0") == 0) {
                    relayStatus = 0; // Deactivate relay
                }
            }
            // Command handling for status reporting
            else if (strcmp(jsonMsg.command, "STA") == 0) {
                // Prepare JSON response with relay status
                char jsonString[100];
                snprintf(jsonString, sizeof(jsonString), "{\"nodeType\":\"NA\", \"nodeID\": %d, \"data\": \"%d\"}", jsonMsg.nodeID, relayStatus);
                if (xSemaphoreTake(USARTSemaphore, portMAX_DELAY) == pdTRUE) {
                    // Send status information via USART
                    for (int i = 0; i < strlen(jsonString); i++) {
                        MCAL_USART_SendChar(USART1, jsonString[i]);
                    }
                    xSemaphoreGive(USARTSemaphore);
                }
            }
            // Command handling for setting durations
            else if (strcmp(jsonMsg.command, "DUR") == 0) {
                if(jsonMsg.nodeID == TEMP_SENSOR_NODE_ID) {
                    tempSensorDuration = atoi(jsonMsg.data);  // Set temperature sensor duration in seconds
                    xSemaphoreGive(tempSensorSemaphore);
                } else if(jsonMsg.nodeID == LIGHT_SENSOR_NODE_ID) {
                    lightSensorDuration = atoi(jsonMsg.data);  // Set light sensor duration in seconds
                    xSemaphoreGive(lightSensorSemaphore);
                }
            }
        }
    }
}


// Relay Control Task
void relayTask(void *pvParameters) {

	while (1) {
		// Wait for the relay semaphore to ensure exclusive access to relay control
		if (xSemaphoreTake(relaySemaphore, portMAX_DELAY) == pdTRUE)
		{

			// Check relay status and turn the relay on or off accordingly
			if (relayStatus == 1) {
				MCAL_GPIO_WritePin(GPIOB, 5, 0);  // Turn relay on
			} else {
				MCAL_GPIO_WritePin(GPIOB, 5, 1);  // Turn relay off
			}

			// Release the relay semaphore after completing the operation
			xSemaphoreGive(relaySemaphore);

			// Delay for 100 ms before checking relay status again
			vTaskDelay(100);  // Check relay status every 100 ms
		}
	}
}

// Data Processing Task
void JsonProcessingTask(void *pvParameters) {

	JsonMessage jsonMsg;

	while (1) {
		// Wait for the JSON semaphore to signal that a new JSON message is available
		if (xSemaphoreTake(xJsonSemaphore, portMAX_DELAY) == pdTRUE) {

			// Parse the JSON buffer to extract command data
			cJSON *json = cJSON_Parse(jsonBuffer);

			// Extract "command" from JSON and copy it into jsonMsg
			cJSON *command = cJSON_GetObjectItemCaseSensitive(json, "command");
			if (cJSON_IsString(command) && (command->valuestring != NULL)) {
				strncpy(jsonMsg.command, command->valuestring, sizeof(jsonMsg.command) - 1);
				jsonMsg.command[sizeof(jsonMsg.command) - 1] = '\0';  // Ensure null termination
			}

			// Extract "nodeID" from JSON and store it in jsonMsg
			cJSON *nodeID = cJSON_GetObjectItemCaseSensitive(json, "nodeID");
			if (cJSON_IsNumber(nodeID)) {
				jsonMsg.nodeID = nodeID->valueint;
			}

			// Extract "data" from JSON and copy it into jsonMsg
			cJSON *data = cJSON_GetObjectItemCaseSensitive(json, "data");
			if (cJSON_IsString(data) && (data->valuestring != NULL)) {
				strncpy(jsonMsg.data, data->valuestring, sizeof(jsonMsg.data) - 1);
				jsonMsg.data[sizeof(jsonMsg.data) - 1] = '\0';  // Ensure null termination
			}

			// Enqueue the parsed message for further processing
			xQueueSend(xJsonQueue, &jsonMsg, 0);

			// Clean up by freeing the allocated JSON object
			cJSON_Delete(json);
		}
	}
}





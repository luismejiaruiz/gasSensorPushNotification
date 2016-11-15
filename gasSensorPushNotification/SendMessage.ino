#include <Bridge.h>
#include <Temboo.h>
#include "utility/TembooGPIO.h"
#include "TembooAccount.h" // Contains Temboo account information  
 
// The number of times to trigger the action if the condition is met
// We limit this so you won't use all of your Temboo calls while testing
int maxCalls = 10;

// The number of times this Choreo has been run so far in this sketch
int calls = 0;

// Declaring sensor configs
TembooGPIOConfig sensor6Config;

// Declaring TembooSensors
TembooSensor sensor6;

void setup() {
  Serial.begin(9600);

  // For debugging, wait until the serial console is connected
  delay(4000);
  while(!Serial);
  Bridge.begin();
  
  // Initialize sensors and configs
  tembooAnalogGPIOInit(&sensor6Config, &sensor6, 6, 0, INPUT);

  Serial.println("Setup complete.\n");
}

void loop() {
  int sensorValue = sensor6.read(sensor6.sensorConfig);
  Serial.println("Sensor: " + String(sensorValue));
  if (sensorValue >= 0) {
    if (calls < maxCalls) {
      Serial.println("\nTriggered! Calling SendMessage Choreo...");
      runSendMessage(sensorValue);
      calls++;
    } else {
      Serial.println("\nTriggered! Skipping to save Temboo calls. Adjust maxCalls as required.");
    }
  }
  delay(250);
}

void runSendMessage(int sensorValue) {
  TembooChoreo SendMessageChoreo;

  // Invoke the Temboo client
  SendMessageChoreo.begin();

  // Set Temboo account credentials
  SendMessageChoreo.setAccountName(TEMBOO_ACCOUNT);
  SendMessageChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
  SendMessageChoreo.setAppKey(TEMBOO_APP_KEY);

  // Identify the Choreo to run
  SendMessageChoreo.setChoreo("/Library/Google/Gmailv2/Messages/SendMessage");

  // Run the Choreo
  unsigned int returnCode = SendMessageChoreo.run();

  // Read and print the error message
  while (SendMessageChoreo.available()) {
    char c = SendMessageChoreo.read();
    Serial.print(c);
  }
  Serial.println();
  SendMessageChoreo.close();
}
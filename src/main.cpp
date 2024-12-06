#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>
#include <WiFiClientSecure.h>

// Certificates
const char *root_ca = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

const char *client_cert = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDWjCCAkKgAwIBAgIVAPJNinp3GVRTMjD6wFP4iXodAH9FMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yNDExMjExMjA4
MjBaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCjQ+88W7M4BHLSuc7e
VQgNgwbPym52m5H5QEQNjiNs/dJDaZsP7N03FIfog8gbbv+Z85km1qeRUjmK9rf+
Yj/4V2VJWIoDlz81ORpnprRaycZRRSFYjilvF9CgY+wVSvQYvpRdsIUkVfJh72HF
VOpk/KPpcoDSx2d1hcwi8ka69s8x+NwOLMtgAQJq8tawiF+AY4COMyyjYF5feFvg
9oBtG/t1xwdLbNBmhKzVvwnJ8KBWGtsCzk+dHcIQGsK5OWjV9V4QG5ouAmQdTWaf
JwDfqXzSbOWK6Ka7Uzrwk7GRtO89O3Fd8nOGEl1hC3pHVQjYyrUxZ8Y4QBmYU0PD
GbmXAgMBAAGjYDBeMB8GA1UdIwQYMBaAFDYP9Kn+GynEbQYfyJjFEQpE6rZmMB0G
A1UdDgQWBBRYFm6JbDqpjSRrV+x++0QYiPqwwjAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAIeaWM4BmcpjISuMlIAqVkvh0
zuma3uPL+/zK3LJIVKREXq8ncdIx4CyGXMwwJiRNO+DfHjfLrKtg55Qe3DtOjUya
GZ1cb1bLrXj2qZowgwzJcA7E0zRm82X2pliRulK31tq5qnXW3l37ATV7mWQAK5Tj
O9eTYcB6Q2zEOHZjFsfK2Gi8Ixwn16rziE+D0H7AFukviMTAzizRhmO7pFuyXRRT
u+LNok4M696QVD8+l5GUvYmu1yyzAPusya8YJC+7z9yy2t+XJZVih1EfLB9KuwQr
4j0KCXjR8RF4YyESV2U0AKaLdtgNU0wYeZgKbH+HXYEWsWbUKTfVzuC2P4iB4A==
-----END CERTIFICATE-----
)EOF";

const char *client_key = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEAo0PvPFuzOARy0rnO3lUIDYMGz8pudpuR+UBEDY4jbP3SQ2mb
D+zdNxSH6IPIG27/mfOZJtankVI5iva3/mI/+FdlSViKA5c/NTkaZ6a0WsnGUUUh
WI4pbxfQoGPsFUr0GL6UXbCFJFXyYe9hxVTqZPyj6XKA0sdndYXMIvJGuvbPMfjc
DizLYAECavLWsIhfgGOAjjMso2BeX3hb4PaAbRv7dccHS2zQZoSs1b8JyfCgVhrb
As5PnR3CEBrCuTlo1fVeEBuaLgJkHU1mnycA36l80mzliuimu1M68JOxkbTvPTtx
XfJzhhJdYQt6R1UI2Mq1MWfGOEAZmFNDwxm5lwIDAQABAoIBAHRuUSEZQEEVoxE2
YxnZSAnixCvRcF9zaHF5bE7Ca+8hQB7FJEkCIWxTXuzpdf5idTPELhctg4i2O8Un
V2W5iWdAZEIfOyV+ahw5oeQDVaFdPendQ/+UOmY4dJa9S7KPpm4zEzmRVhHonRdM
kMwaV6I3LWmhN8+ZVuXPyhmt79UKZFoo9F9udDx+QEKRashEUddoFLdPn0teWiLw
rNCt2WKlZBg0jIdyZNjKsLgjgXrbdvd6ZmoLFlC9WJWFTosdMAgj5H+LAsjYhwIg
SMPGI74H3O2P2WFGBzTK49DQ3XADPAw6c38FO6PoCZIJH7zXdvwCmKitDv4lNthx
Q1qlrZkCgYEA0igMRT+JhGBFhheFS08WIxeBhoaHlIxajY147oVWjl8FwclF9X/g
agPa2ONLin7eos+jmOUzsNj07Grw7a5Vqf7Ckz7zPUmIaG7nTWNyqyz99isXexLz
qxAMFGjr71DRonAwnmk9bGnslUPNFsjxYrGMB2GkxJ4H3UTfITOvcFMCgYEAxuFO
oJDelABZQRaR00Hx3wJy/eJKTj4tcqUOZf6IH6so3zoN++xzs1IeV3TRpVlAcK2d
4WMC+CMtyM6GOFuGV9GbONRTijVSc/7bCZVz/8ndHn9YDQW/7QBb2LPJ+Dk6Cmwt
GReYNQ3aaiuA0g0JoMFiAUDxq6jz8Vdpy2+tuS0CgYBaW6G6/HCSHBqIZwCGusBX
4jlsr9cHhblBd3xxgZ8Po8eD5JhRz+ObcYDdFA1cxOPQuNosWQ/6j63vIKPqbbSj
XMgoKz7GMSGXVWfaHFInSzb+atSztBhUipfHBr26rdMUtowQ8EranEh+fSJnE4rF
1nTQj6kaaKAVARG24tQhxQKBgG4/dADajihpUREd29TKl7RSfUB0c1CQc008ZpDv
2rIDhfLJEft3hmP/v8YnghgxM+HCvOS2qGy5w6WvpwQS6JRCI9QLEAxrpruaqf3U
FavUUXmYik0dxNky+7dTY0DxKFWixhVE2SF//nYgi5bll1LAIP4k7FOGo0REeudF
j345AoGBAMyCvDRU/6UvyZL6l9qEGREWgX4u6oljX+DoFIa9FhYE5axDIdplz0un
Fy+aXC6G09mOVLtnOc416EjurCeOorrWMDShrLrLsHwWvjM1UfW/LH/n4OAeD0cU
2Tm+6HZntBpku6m0RfgouRAznEULNdaI1duvfooyuZHlkAoU1GH1
-----END RSA PRIVATE KEY-----
)EOF";

// Wifi Credentials (Connect Laptop to Wifi, and Phone if viewing Node Red on Phone)
// const char *ssid = "PhoPhone";
// const char *password = "i82fxb88ksrmo";
const char *ssid = "지현";
const char *password = "hotdog23";

// MQTT broker ID
const char *mqttBroker = "a2qrzuxbj02fml-ats.iot.us-east-2.amazonaws.com";
const int mqttPort = 8883;

// MQTT topics
const char *pillsTopic = "sensor/pillsRemaining";
const char *dosageTimeTopic = "sensor/dosageTime";
const char *notificationTopic = "sensor/notification";
const char *refillTopic = "sensor/refillPills";

// MQTT Client
WiFiClientSecure espClient;
// WiFiClient espClient;
PubSubClient client(espClient);

// Servo and Button
Servo servo;
const int buttonPin = 15; 
const int servoPin = 27; 
const int resetPin = 17;
int servoPosition = 0;

// Variables
int pillsRemaining = 10;
int dosageTimeHours = 6; // Default dosage time in hours
unsigned long lastNotificationMillis = 0;
unsigned long notificationIntervalMillis = 0; // Will be calculated based on dosageTimeHours
unsigned long debounceDelay = 200;
unsigned long lastButtonPress = 0;

#define MSG_BUFFER_SIZE (5)

// Cycle time
unsigned long previousMillis = 0;
const int interval = 2000;

// Callback function whenever an MQTT message is received
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String message;
  for (int i = 0; i < length; i++)
  {
    Serial.print(message += (char)payload[i]);
  }
  Serial.println();

  if (strcmp(topic, pillsTopic) == 0)
  {
    pillsRemaining = atoi((char *)payload);
    Serial.printf("Updated pills remaining: %d\n", pillsRemaining);
  }
  else if (strcmp(topic, dosageTimeTopic) == 0)
  {
    dosageTimeHours = atoi((char *)payload);
    notificationIntervalMillis = dosageTimeHours * 3600000; // Convert hours to milliseconds
    Serial.printf("Updated dosage time: %d hours\n", dosageTimeHours);
  }
  else if (strcmp(topic, refillTopic) == 0)
  {
    servo.write(90);
    delay(500);
    servo.write(0);
    delay(500);
    // // Increment servo position by 90 degrees
    // servoPosition += 90;

    // // Reset position if it exceeds 360 degrees
    // if (servoPosition > 360)
    // {
    //   servoPosition = 0;
    // }

    // servo.write(servoPosition);
    // delay(1000);

    // // Increment servo position by 90 degrees
    // servoPosition += 90;

    // // Reset position if it exceeds 360 degrees
    // if (servoPosition > 360)
    // {
    //   servoPosition = 0;
    // }

    Serial.printf("Refilling pills\n");
  }
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");

    // Client ID
    String clientId = "iotconsole-9e75787a-e5c9-40f7-9d3c-c21cd4914d17";
    clientId += String(random(0xffff), HEX);

    // Attempt to connect
    if (client.connect(clientId.c_str()))
    {
      Serial.println("MQTT Broker connected!");
      client.subscribe(pillsTopic);
      Serial.printf("Subscribed to topic: %s\n", pillsTopic);
      client.subscribe(dosageTimeTopic);
      Serial.printf("Subscribed to topic: %s\n", dosageTimeTopic);
      client.subscribe(refillTopic);
      Serial.printf("Subscribed to topic: %s\n", refillTopic);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void publishMessage(const char *topic, float value)
{
  char msgBuffer[MSG_BUFFER_SIZE];
  snprintf(msgBuffer, MSG_BUFFER_SIZE, "%g", value);
  Serial.printf("Publishing to topic :: %s, value :: %s", topic, msgBuffer);
  Serial.println("");
  client.publish(topic, msgBuffer);
}

// Connect to Wifi
void setup_wifi()
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup()
{
  Serial.begin(9600);
  // Setup the wifi
  setup_wifi();

  // setup the mqtt server and callback
  espClient.setCACert(root_ca);
  espClient.setCertificate(client_cert);
  espClient.setPrivateKey(client_key);
  client.setServer(mqttBroker, mqttPort);
  client.setCallback(callback);
  client.setKeepAlive(30);

  pinMode(buttonPin, INPUT_PULLUP);
  servo.attach(servoPin);
  servo.write(0); // Start position

  notificationIntervalMillis = dosageTimeHours * 3600000;
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }

  client.loop();

  unsigned long currentMillis = millis();

  // Handle button press
  if (digitalRead(buttonPin) == LOW && (currentMillis - lastButtonPress > debounceDelay))
  {
    Serial.println("Button pressed.");
    lastButtonPress = currentMillis;

    if (pillsRemaining > 0)
    {
      pillsRemaining--;
      publishMessage(pillsTopic, pillsRemaining);

      servo.write(90);
      delay(1000);
      servo.write(0);
      delay(1000);
      // servo.write(servoPosition);
      // delay(500);

      // // Increment servo position by 90 degrees
      // servoPosition += 90;

      // // Reset position if it exceeds 360 degrees
      // if (servoPosition > 360)
      // {
      //   servoPosition = 0;
      // }
    }
    else
    {
      Serial.println("No pills remaining!");
    }

  }

  // // Handle dosage time notification
  // if (currentMillis - lastNotificationMillis >= notificationIntervalMillis)
  // {
  //   lastNotificationMillis = currentMillis;
  //   client.publish(notificationTopic, "Time to take your dosage!");
  //   Serial.println("Notification sent!");
  // }
}
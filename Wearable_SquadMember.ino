#include <avr/pgmspace.h>

#define WIFI_ENABLE_PIN 13
#define DEBUG true
#define VIBRATOR 21

#define SSID "Project11x"      //your wifi ssid here
#define PASS "abcd1234"   //your wifi wep key here

void setup()
{
  Serial1.begin(9600);
  Serial1.setTimeout(10000);
  Serial.begin(9600);  //can't be faster than 19200 for softserial

  if (DEBUG) {
    while (!Serial);
  }

  pinMode(WIFI_ENABLE_PIN, OUTPUT);
  pinMode(VIBRATOR, OUTPUT);
  
  digitalWrite(WIFI_ENABLE_PIN, HIGH);
  sendData("AT\r\n", 5000);//Test AT startup
  //sendData("AT+GMR\r\n", 5000);//View version info
  sendData("AT+CWMODE=3\r\n", 5000); //WiFi mode
  //sendData("AT+CWLAP\r\n", 5000);//List available APs
  sendData("AT+CWJAP=\"Project11x\",\"abcd1234\"\r\n", 5000); //join AP
  sendData("AT+CIPMUX=0\r\n", 5000);
  sendData("AT+CIFSR\r\n", 5000); //get ip address                                                                                                                                                                                                                                                                                                                                                                   
  sendData("AT+CIPSTART=\"TCP\",\"192.168.0.12\",10026\r\n", 10000);
  sendData("AT+CIPMODE=1\r\n", 5000);
  sendData("AT+CIPSEND=0,5\r\n", 6000);
  /*Serial1.print("AT+RST\r\n");
    Serial1.print("AT+CWMODE=3\r\n");
    Serial1.print("AT+CWJAP=\"Project11x\",\"abcd1234\"\r\n");
    Serial1.print("AT+CIPMUX=0\r\n");
    Serial1.print("AT+CIFSR\r\n");
    Serial1.print("AT+CIPSTART=\"TCP\",\"192.168.0.7\",9001\r\n");
    Serial1.print("AT+CIPMODE=1\r\n");
    Serial1.print("AT+CIPSEND=0,5\r\n");
    Serial.println("Start Socket Client");*/
}

void loop() {

  /*delay(5000);
    clearRx();
    delay(1000);*/
    
  /*if (Serial1.find("+IPD.")){
    delay(1000);
    int connectionID = Serial1.read()-48;
    Serial.println((char)connectionID);
    }*/
  ReceiveMotion();
}

void ReceiveMotion() {
  char recvData;
  while (Serial1.available() > 0) {
    recvData = (char)Serial1.read();
    Serial.println(recvData);
    RefineData(recvData);
  }
}

void RefineData(char recvData){
  boolean RecvDataOk = false;
  int refineData = recvData - 48;
//수신한 바이트 데이터 암호화 풀기
  MotionRecognization(refineData);
  /*if(RecvData == true){
    MotionRecognization();
  }else{
    
  }*/
}

void MotionRecognization(int refineData){
//움직임 파악
  int vibrationNum = 0;
  
  switch(refineData){
    case 1:
    vibrationNum = 1;
    Serial.println("진동중");
    break;
    
    case 2:
    break;
    
    case 3:
    break;

    case 4:
    break;

    case 5:
    break;
  }
  delay(1);
  DoVibration(vibrationNum);
}

void DoVibration(int vibrationNum){
  //진동센서
  for(int i = 0; i < vibrationNum; i++){
    digitalWrite(VIBRATOR, HIGH);
    delay(1000);
    digitalWrite(VIBRATOR, LOW);
    delay(1000);
  }
}


String sendData(String command, const int timeout)
{
  String response = "";

  Serial1.print(command); // send the read character to the esp8266
  long int time = millis();
  while ( (time + timeout) > millis()) {
    while (Serial1.available()) {
      // The esp has data so display its output to the serial window
      char c = (uint8_t)Serial1.read(); // read the next character.
      response += c;
    }
  }
  if (DEBUG)  {
    Serial.print(response);
    Serial.print("\n");
  }
  return response;
}

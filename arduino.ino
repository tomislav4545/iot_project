#include <Keypad.h>
#include <Servo.h>
const byte ROW_NUM    = 4; // four rows
const byte COLUMN_NUM = 4; // four columns
Servo servo;
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte pin_rows[ROW_NUM] = {2, 3, 4, 5}; // connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {6, 7, 8, 9}; // connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

char password[] = "1234"; // Set your desired password here

String enteredPassword = "";

const int BUZZER=11;
const int RLED=13;
const int YLED=10;

void setup() {
  Serial.begin(9600);
  pinMode(A0, OUTPUT);
  pinMode(13, OUTPUT);

  servo.attach(12);
  servo.write(0);
}

void loop() {
  char key = keypad.getKey();
if (Serial.available() > 0) {
      
    String receivedData = Serial.readStringUntil('\n');
    receivedData.trim();

    if (receivedData.equals("M1")) {
      digitalWrite(YLED, HIGH);
    } else if (receivedData.equals("M0")) {
      digitalWrite(YLED, LOW);
    }
    else if (receivedData.equals("G1")){
      digitalWrite(BUZZER, HIGH);
      digitalWrite(RLED,HIGH);
      delay(2000);
      digitalWrite(BUZZER, LOW);
      digitalWrite(RLED,LOW);
    }
  }

  if (key) {
    if (key == '#') {
      // Check the entered password when '#' is pressed
      if (enteredPassword == password) {
        //Serial.println("Access Granted!");
        Serial.println("C1");
        digitalWrite(A0, HIGH);
        servo.write(90);
        delay(1000);
        digitalWrite(A0, LOW);
        servo.write(0);
        
      } else {
        //Serial.println("Access Denied! Incorrect Password");
        Serial.println("C0");
        digitalWrite(13, HIGH);
        delay(1000);
        digitalWrite(13, LOW);
      }

      // Clear the entered password
      enteredPassword = "";
    } else {
      // Append the pressed key to the entered password
      enteredPassword += key;
    }

    // Display the pressed key in the Serial Monitor
    //Serial.println("Key Pressed: " + String(key));
  }
}

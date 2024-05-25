#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

// LCD
LiquidCrystal_I2C lcd(0x27,16,2);  

// LED PINS
const int redPin = 3;
const int greenPin = 5;

// ULTRASONIC PINS
const int trigPin = 9;
const int echoPin1 = 10;
const int echoPin2 = 11;

float distance1 = 100, duration1 = 10000;
float distance2 = 100, duration2 = 10000;
int turn = 0;

// BUZZER PIN
const int buzzPin = 6;

// BUTTON PIN
const int button = 2;
int active = 1;

void setColor(float red, float green);

void setup()
{
  // INIT LCD
  lcd.init();                      
  lcd.backlight();
  lcd.clear();

  // LED PINS
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, INPUT);

  // ULTRASONIC PINS
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(echoPin2, INPUT);
  Serial.begin(9600); // BAUD RATE 9600

  // BUZZER PIN
  pinMode(buzzPin, OUTPUT);
  analogWrite(buzzPin, LOW);

  // BUTTON
  pinMode(button, INPUT_PULLUP);

  setColor(0, 0);
}


void loop()
{
  // Send Ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Only register one of them
  if(!turn) {
    duration1 = pulseIn(echoPin1, HIGH);
    distance1 = (duration1*.0343)/2;
  } else {
    duration2 = pulseIn(echoPin2, HIGH);
    distance2 = (duration2*.0343)/2;
  }
  turn = (turn + 1) % 2;

  if(digitalRead(button) == true) {
    active = ++active%2;
  }
  while(digitalRead(button) == true)delay(50);

  if((distance1 > 0 && distance1 <= 40) || (distance2 > 0 && distance2 <= 40)) {

    float minDistance = MIN(distance1, distance2);

    if(active) {

    int colorValue = map(minDistance, 5, 40, 0, 255);
    colorValue = (colorValue > 0)? colorValue: colorValue * (-1);
    setColor(255-colorValue, colorValue);

    analogWrite(buzzPin, colorValue);
    } else {
      analogWrite(buzzPin, LOW);
      setColor(0, 255);
    }

    lcd.setCursor(0, 0);
    if(distance1 <= 40) {
      lcd.print(distance1);
      lcd.print(" cm");
    } else {
      lcd.print("0");
      lcd.print(" cm");
    }

    lcd.setCursor(0, 1);
    if(distance2 <= 40) {
      lcd.print(distance2);
      lcd.print(" cm");
    } else {
      lcd.print("0");
      lcd.print(" cm");
    }
    delay(100);
  } else {
    lcd.clear();
    analogWrite(buzzPin, LOW);
    setColor(0,0);
  }
}

void setColor(float red, float green) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
}

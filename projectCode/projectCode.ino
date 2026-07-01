#include <LiquidCrystal.h>
#include <Servo.h>

// LCD pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(10, 11, 12, 13, A3, A4);
Servo barrier;

// Sensor pins
const int soilPin = A0;
const int rainPin = A1;
const int tempPin = A2;
const int tiltPin = 2;

// Output pins
const int greenLED = 3;
const int yellowLED = 4;
const int redLED = 5;
const int buzzer = 6;
const int servoPin = 7;

// Ultrasonic pins
const int trigPin = 8;
const int echoPin = 9;

long duration;
int distance;

void setup() {
  lcd.begin(16, 2);
  barrier.attach(servoPin);

  pinMode(tiltPin, INPUT_PULLUP);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(buzzer, OUTPUT);

  lcd.print("Smart Disaster");
  lcd.setCursor(0, 1);
  lcd.print("Monitoring");
  barrier.write(0);
  delay(2000);
  lcd.clear();
}

void loop() {

  int soil = analogRead(soilPin);
  int rain = analogRead(rainPin);
  int temp = analogRead(tempPin);
  int tilt = digitalRead(tiltPin);

  // Ultrasonic
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // LCD Display
  lcd.setCursor(0, 0);
  lcd.print("S:");
  lcd.print(soil);
  lcd.print(" T:");
  lcd.print(temp);

  lcd.setCursor(0, 1);
  lcd.print("R:");
  lcd.print(rain);
  lcd.print(" W:");
  lcd.print(distance);
  lcd.print(" ");

  // SAFE
  if (soil < 400 && rain < 400 && distance > 20 && tilt == HIGH) {
    digitalWrite(greenLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, LOW);
    digitalWrite(buzzer, LOW);
    barrier.write(0);
    lcd.setCursor(11, 1);
    lcd.print("SAFE ");
  }

  // ALERT
  else if ((soil >= 400 && soil <= 700) ||
           (rain >= 400 && rain <= 700) ||
           (distance <= 20 && distance > 10)) {
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, HIGH);
    digitalWrite(redLED, LOW);
    digitalWrite(buzzer, LOW);
    barrier.write(45);
    lcd.setCursor(11, 1);
    lcd.print("ALERT");
  }

  // DANGER
  if (soil > 700 || rain > 700 || distance <= 10 || tilt == LOW) {
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, HIGH);
    digitalWrite(buzzer, HIGH);
    barrier.write(90);
    lcd.setCursor(11, 1);
    lcd.print("DANGER");
  }

  delay(1000);
}

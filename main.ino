#include <LiquidCrystal.h> 
#include <Servo.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Servo water_servo;
Servo coffe_servo;

const int buttonStart = 8; 
const int ledGeneral = A1; 
const int buttonB1 = 7;
const int buttonB2 = 9;
const int interruptor = 1;
 
int start_state = 0;
int start_previous_state = 0;
int save_start_state_button;
int b1_state = 0;
int b1_previous_state = 0;
int save_b1_state_button;
int b2_state = 0;
int b2_previous_state = 0;
int save_b2_state_button;
int alert = 10;
bool opened = false;
bool with_caps = false;
bool main_options = true;
bool to_close = false;

void addWater(float value) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Volume de agua:");
    lcd.setCursor(0,1);
    lcd.print("0 ml - 0%");
    
    int total = 0;
    
    water_servo.write(150);
        
    delay(2000);
    
    while(total < value) {
        total += random(10, 80);
        
        if (total > value) {
            total = value;
        }
        
        delay(300);
        
        int percentage = (total / value)*100;
        
        lcd.setCursor(0,1);
        lcd.print(total);
        lcd.print("ml - ");
        lcd.print(percentage);
        lcd.print("%");
        
        delay(300);
    }
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Agua preenchida!");
    
    water_servo.write(0);
    
    delay(1000);
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Aquecendo...");
};

void measureTemperature() {
    int initial_temperature = analogRead(A5);
    int temperature = (((initial_temperature * 5.0) / 1024.0) - 0.5) * 100;
  
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Temperatura:");
    lcd.setCursor(0,1);
    lcd.print(temperature);
    lcd.print(" graus");
  
    while(temperature < 90.0) {
        temperature += 5.0; 
    
        if (temperature > 90) {
            temperature = 90;
        }
        
        lcd.setCursor(0,0);
        lcd.print("Temperatura: ");
        lcd.setCursor(0,1);
        lcd.print(temperature); lcd.print(" graus");
        
        delay(500);
    }
  
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Preparando!");
    
    coffe_servo.write(150);
    
    delay(2000);
    
    coffe_servo.write(0);
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Cafe pronto!");
    lcd.setCursor(0,1);
    lcd.print("Retire o copo!");
    
    tone(alert,423,300);
    
    delay(700);
    
    tone(alert,423,300);
    
    delay(3000);
}

void setup() {
    water_servo.attach(13);
    water_servo.write(0);
    coffe_servo.attach(6);
    coffe_servo.write(0);

    pinMode(ledGeneral, OUTPUT);
    pinMode(buttonStart, INPUT);
    pinMode(buttonB1, INPUT);
    pinMode(buttonB2, INPUT);
    pinMode(alert, OUTPUT);
    pinMode(interruptor, INPUT);
    
    lcd.begin(16, 2);
    
    Serial.begin(9600);
}
 
void loop(){
    start_state = digitalRead(buttonStart);
    b1_state = digitalRead(buttonB1);
    b2_state = digitalRead(buttonB2);

    if ((!start_state) && (start_previous_state)) { 
        save_start_state_button = !save_start_state_button;
    }
  
    if (save_start_state_button == HIGH) {

        digitalWrite(ledGeneral, HIGH);
        
        if (digitalRead(interruptor) == HIGH && with_caps == false) {
            main_options = false;
          	lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Atencao!");
            lcd.setCursor(0, 1);
            lcd.print("Comp. aberto!");

            delay(3000);

            opened = true;
        }
        
        if (opened == true && with_caps == false) {
            with_caps = true;
          
          	lcd.clear();

            lcd.setCursor(0, 0);
            lcd.print("Atencao");
            lcd.setCursor(0, 1);
            lcd.print("Caps. no lugar");

            delay(2000);
        }
      
        if (opened == true && with_caps == true) {
        	lcd.clear();
          	lcd.setCursor(0, 0);
            lcd.print("Atencao");
            lcd.setCursor(0, 1);
            lcd.print("Fechar comp.");
          	delay(2000);
        }
      
      	if (digitalRead(interruptor) == LOW && with_caps == true && (save_b1_state_button == LOW || save_b2_state_button == LOW)) {
        	main_options = true;
          	opened = false;
        }
        
        if (main_options == true) {
          	lcd.setCursor(0, 0);
            lcd.print("Maquina de Cafe!");
          	lcd.setCursor(0, 1);
          	lcd.print("1-Curto 2-Longo");
        }
        
    
        if ((!b1_state) && (b1_previous_state)) { 
            save_b1_state_button = !save_b1_state_button;
        }
    
        if ((!b2_state) && (b2_previous_state)) { 
            save_b2_state_button = !save_b2_state_button;
        }
    
        if (with_caps == true && save_b1_state_button == HIGH) {
            lcd.clear();
            addWater(200);
            
            save_b1_state_button = !save_b1_state_button;
            
            measureTemperature();
          
          	with_caps = false;

        } else if ((with_caps == false && save_b1_state_button == HIGH)) {
            lcd.clear();

          	main_options = false;
          	
            lcd.setCursor(0, 0);
            lcd.print("Colocar");
          	lcd.setCursor(0, 1);
          	lcd.print("Capsula");
            delay(2000);
          	b1_previous_state = b1_state;
            save_b1_state_button = !save_b1_state_button;
          	
          	main_options = true;
        }
        
        if (with_caps == true && save_b2_state_button == HIGH) {
            lcd.clear();
            addWater(400);
            
            save_b2_state_button = !save_b2_state_button;
            
            measureTemperature();
          
          	with_caps = false;

        } else if ((with_caps == false && save_b2_state_button == HIGH)) {
            main_options = false;
          	lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Colocar");
          	lcd.setCursor(0, 1);
          	lcd.print("Capsula");
            delay(2000);
          	b2_previous_state = b2_state;
            save_b2_state_button = !save_b2_state_button;
          	main_options = true;
        }
    
    } else {
        digitalWrite(ledGeneral, LOW);
    }

    start_previous_state = start_state;
    b1_previous_state = b1_state;
    b2_previous_state = b2_state;
}
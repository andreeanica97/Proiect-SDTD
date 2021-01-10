// Sistem de masurare prin ultrasunete cu indicatori acustici si transmitere a datelor prin Bluethooth
#include <Arduino_FreeRTOS.h>
#define buzzer 11 // pin 11 pentru buzzer
// senzor 1 = senzorul din fata;
// senzor 2 = senzorul din spate;

int trigPin2 = 2; // pin Trigger pentru senzorul 2;
int echoPin2 = 19; // pin Echo pentru senzorul 2;
int trigPin = 9;  // pin Trigger pentru senzorul 1;
int echoPin = 12;  //Pin Echo pentru senzorul 1;
int duration, distance, duration2, distance2;    // durata si distanta pentru senzor HC-SR04;
int sound = 0;

void setup() 
{
  Serial.begin(9600);
  delay(500);
  pinMode(16, OUTPUT); // definire pin pentru led ROSU, senzor 1;
  pinMode(14, OUTPUT);  //definire pin pentru led ALBASTRU, senzor 1;
  pinMode(13, OUTPUT);  //definire pin pentru led VERDE, senzor 1;
  pinMode(buzzer, OUTPUT);
 
  pinMode(8, OUTPUT); //definire pin pentru led VERDE, senzor 2;
  pinMode(17, OUTPUT); //definire pin pentru led ALBASTRU, senzor 2;
  pinMode(7, OUTPUT); // definire pin pentru led ROSU, senzor 2;
  pinMode(buzzer, OUTPUT);
 
  pinMode(trigPin2, OUTPUT);  // definire pini senzor distanta2;
  pinMode(echoPin2, INPUT);
  pinMode(trigPin, OUTPUT); // definire pini senzor distanta;
  pinMode(echoPin, INPUT);
  
  xTaskCreate(MyTask1, "Task1", 100, NULL, 1, NULL);  // primul proces;
  xTaskCreate(MyTask2, "Task2", 100, NULL, 1, NULL);  // al doilea proces;
  xTaskCreate(MyTask3, "Task2", 100, NULL, 2, NULL);  // al treilea proces;
  xTaskCreate(MyTask4, "Task2", 100, NULL, 2, NULL);  // al patrulea proces;
  xTaskCreate(MyTask5, "Task2", 100, NULL, 2, NULL);  // al cincilea proces;

}

static void MyTask1(void* pvParameters) // definire primul proces;
{ 
  (void) pvParameters;
  Serial.begin(9600);
  while (1)
  {
    digitalWrite(trigPin, HIGH); //triggering de un impuls de 10 microsec;
    delay(10);
    digitalWrite(trigPin, LOW);
       
    duration = pulseIn(echoPin, HIGH);  // funtie pentru urmarirea impulsului;
                                        // HIGH-impuls al cărui durata este timpul de la trimitere la receptia ecoului sau pentru un obiect;
    distance = (duration / 2) / 29.1;   // conversie din timp in distanta(cm);
    
    vTaskDelay(150 / portTICK_PERIOD_MS); // functia specifica o perioada in care procesul este deblocat fata perioada in care este apelata fucntia vTaskDelay();
                                          // perioada de blocare de 150ticks determina deblocarea procesului de 150ticks dupa ce functia este apelata;
  }
}

static void MyTask2(void* pvParameters2) // definire al doilea proces;
 {
  (void) pvParameters2;
  Serial.begin(9600);
  while (1)
  {
    digitalWrite(trigPin2, HIGH); //triggering de un impuls de 10 microsec;
    delay(10);
    digitalWrite(trigPin2, LOW);
       
    duration2 = pulseIn(echoPin2, HIGH); //funtie speciala pentru urmarirea impulsului;
    distance2 = (duration2 / 2) / 29.1; // conversie din timp in distanta(cm);
    
    vTaskDelay(150 / portTICK_PERIOD_MS);
  }
 }

static void MyTask3(void* pvParameters) // definire al treilea proces;
{
  (void) pvParameters;
  
  while (1)
  {
    if (distance > 25  )  // daca distanta > 25 cm;
    { 
      digitalWrite(16, LOW); // led rosu off;
      digitalWrite(14, LOW);  // led albastru off;
      digitalWrite(13, LOW);  // led verde off;
      noTone(buzzer);
    }
    else if (distance < 25 && distance >= 20 ) // daca distanta intre 25cm si 20cm -> led VERDE ON si buzzer activ;
    {
      digitalWrite(16, LOW); // led rosu off;
      digitalWrite(14, LOW);  // led albastru off;
      digitalWrite(13, HIGH); // led verde on;
      sound = 200; // Hz note c;
      tone(buzzer, sound, 300); // primul argument din paranteza - pin-ul buzzerului, al doilea - frecventa sunetului din buzzer, al treilea - durata unui bipait al buzzer-ului)
      delay(700);
    }
    
    else if (distance <= 20 && distance >= 10 ) //daca distanta intre 20 si 10 cm -> led GALBEN ON si buzzer activ;
    {
      digitalWrite(16, LOW);
      digitalWrite(14, HIGH);
      digitalWrite(13, LOW);
      sound = 200; //Hz note f;
      tone(buzzer, sound, 250);
      delay(350);
    } 
    
    else if (distance < 10 && distance > 0 ) // daca distanta < 10 cm -> led ROSU ON si buzzer activ;
    {
      digitalWrite(16, HIGH);
      digitalWrite(14, LOW);
      digitalWrite(13, LOW);
      sound = 200; // Hz note C;
      tone(buzzer, sound, 150);
      delay(200);
    }
    
    vTaskDelay(150 / portTICK_PERIOD_MS);
  }
}

static void MyTask4(void* pvParameters) // definire al patrulea proces;
{
  (void) pvParameters;
  while (1)
  {
    if (distance2 > 25) // daca distanta > 25 cm;
    { 
      digitalWrite(8, LOW);
      digitalWrite(17, LOW);
      digitalWrite(7, LOW);
      noTone(buzzer);
    }
    else if (distance2 < 25 && distance2 >= 20 )  //daca distanta intre 20 si 10 cm -> led VERDE on si buzzer activ;
    {
      digitalWrite(8, HIGH);
      digitalWrite(17, LOW);
      digitalWrite(7, LOW);
      sound = 500;//Hz note c
      tone(buzzer, sound, 300); // buzzer = nr pin buzzer, sound = frecventa sunetului din buzzer, 300 = durata unui sunet al buzzer-ului);
      delay(700);
    }
    else if (distance2 <= 20 && distance2 >= 10 ) // daca distanta intre 20 si 10 cm -> led GALBEN ON si buzzer activ;
    {
      digitalWrite(8, LOW);
      digitalWrite(17, HIGH);
      digitalWrite(7, LOW);
      sound = 500; //Hz note f
      tone(buzzer, sound, 250);
      delay(350);
    } 
    
    else if (distance2 < 10 && distance2 > 0 ) // daca distanta <10 cm -> led ROSU ON si buzzer activ;
    {
      digitalWrite(8, LOW);
      digitalWrite(17, LOW);
      digitalWrite(7, HIGH);
      sound = 500; // Hz note C;
      tone(buzzer, sound, 150);
      delay(200);
    }

    vTaskDelay(150 / portTICK_PERIOD_MS);
  }
}

static void MyTask5(void* pvParameters) // definire al cincilea proces;
{
  (void) pvParameters;
  Serial.begin(9600);
  while (1)
  {
    Serial.print("Distanta masurata in fata(cm): ");
    Serial.print(distance); // afisare distanta(cm);
    Serial.println(" ");
    Serial.print("Distanta masurata in spate(cm): ");    
    Serial.print(distance2); // afisare distanta(cm);
    Serial.println(" ");
    delay(1000);
    vTaskDelay(150 / portTICK_PERIOD_MS);
  }
}

void loop()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
}

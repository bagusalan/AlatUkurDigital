/*Project Praktikum Mikrokontroler
==>>Alat Ukur Digital<==
"Bismillah"*/

#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte numRows = 4;
const byte numCols = 4;

char msg[18];
char key;
int number = 0;
int count = 0;
int sv1 = 0;
int cLed = 0;
int ukur1, ukur2;

char keys[numRows][numCols] = 
{
{'1', '2', '3', 'A'},
{'4', '5', '6', 'B'},
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};

byte rowPins[numRows] = {13, 12, 11, 10};
byte colPins[numCols] = {9, 8, 7, 6,};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, numRows, numCols);

int trigA = 2;           // membuat varibel trig yang di set ke-pin 3
int echoA = 3;           // membuat variabel echo yang di set ke-pin 2
int trigB = 4;
int echoB = 5;
long durasi1, jarak1, durasi2, jarak2;   // membuat variabel durasi dan jarak

int buzz = A1;

int led = A0;

void (* mau_reset)(void)=0;

void tulis(int a, int b, String c)
{
  lcd.setCursor(a,b);
  lcd.print(c);
}

void tampil()
{
  tulis(0,0,"METERAN DIGITAL");
  tulis(2,1,"KELOMPOK 2");
  delay(2000);
  lcd.clear();
  tulis(5,0,"ELKA");
  tulis(4,1,"2020 B");
  delay(2000);
  lcd.clear();
  tulis(2,0,"PILIH MENU");
  delay(1000);
  lcd.clear();
  tulis(0,0,"A: Manual");
  tulis(0,1,"B: Monitoring");
}

void ultrsnk()
{
  // program dibawah ini ultrasonic A
  digitalWrite(trigA, LOW);
  delayMicroseconds(8);
  digitalWrite(trigA, HIGH);
  delayMicroseconds(8);
  digitalWrite(trigA, LOW);
  delayMicroseconds(8);

  durasi1 = pulseIn(echoA, HIGH); // menerima suara ultrasonic
  jarak1 = (durasi1 / 2) / 29.1;  // mengubah durasi menjadi jarak (cm)

  // program dibawah ini  ultrasonic B
  digitalWrite(trigB, LOW);
  delayMicroseconds(8);
  digitalWrite(trigB, HIGH);
  delayMicroseconds(8);
  digitalWrite(trigB, LOW);
  delayMicroseconds(8);

  durasi2 = pulseIn(echoB, HIGH); // menerima suara ultrasonic
  jarak2 = (durasi2 / 2) / 29.1;  // mengubah durasi menjadi jarak (cm)
}

void kipet()
{
  key = keypad.getKey(); 
  switch(key){
    case'0'...'9':
      number = number*10+(key-'0');
      tone(buzz, 1000, 50);
      if(count==1)
      {
        sprintf(msg, "P:%3i cm", number);
        tulis(0,0,msg);
      }
      if(count==2)
      {
        sprintf(msg, "P:%3i cm", ukur1);
        tulis(0,0,msg);
        sprintf(msg, "L:%3i cm", number);
        tulis(0,1,msg);
      }
      if(count==3)
      {
        sprintf(msg, "L:%3i cm", ukur2);
        tulis(0,1,msg);
      }
      Serial.println(key);
    break;
    case'A':
      tone(buzz, 800, 200);
      count++; //1
      lcd.clear();
      sv1=0;
      if(count%2!=0 || count%2==0)
      {
        tulis(0,0,"Masukkan Panjang");
        tulis(0,1,"Masukkan Lebar");
        delay(2000);
        lcd.clear();
      }
      tulis(8,0," ");
      sprintf(msg, "P:    cm");
      tulis(0,0,msg);
      tulis(8,1," ");
      sprintf(msg, "L:    cm");
      tulis(0,1,msg);        
      Serial.println(key);    
    break;
    case'B':
      tone(buzz, 800, 200);
      lcd.clear();
      count=0;      
      sv1=1;
    break;
    case'C':
      tone(buzz, 800, 200);
      cLed++;
      if(cLed%2!=0)
      {
        digitalWrite(led, HIGH);
      }
      if(cLed%2==0)
      {
      digitalWrite(led, LOW); 
      }
    break;   
    case'D':
      mau_reset();
    break;
    case'*':
      tone(buzz, 800, 200);
      if(count==1)
      {
        lcd.clear();
        number = 0;
        tulis(8,0," ");
        sprintf(msg, "P:    cm");
        tulis(0,0,msg);
        tulis(8,1," ");
        sprintf(msg, "L:    cm");
        tulis(0,1,msg);
      }
      if(count==2) 
      {
        number = 0;
        tulis(8,1," ");
        sprintf(msg, "L:    cm");
        tulis(0,1,msg);
      }
    break;
    case'#':
      tone(buzz, 800, 200);
      count++; 
      if(count==2)
      {
        ukur1 = number;
        tone(buzz, 800, 200);
        number = 0;
        Serial.println(ukur1); 
      }
      if(count==3)
      {
        ukur2=number;
        tone(buzz, 800, 200);
      }
    break;
 }
}

void proses()
{
  if(sv1==1) //coba count==5
  {
    tulis(8,0," ");
    sprintf(msg, "P:%3d cm", jarak1);
    tulis(0,0,msg);
    tulis(8,1," ");
    sprintf(msg, "L:%3d cm", jarak2);
    tulis(0,1,msg);  
  }
  Serial.println(jarak1);
  
  if(count==3)
  {
    tulis(10,0," ");
    tulis(14,0," ");
    sprintf(msg, "%3d", jarak1);
    tulis(11,0,msg);
    tulis(10,1," ");
    tulis(14,1," ");
    sprintf(msg, "%3d", jarak2);
    tulis(11,1,msg);
  }
  if(jarak1==ukur1 && jarak2==ukur2)
  {
    tone(buzz, 1000, 100);
  }  
}

void menu()
{
  ultrsnk();
  kipet();
  proses();
}

void setup() {
  lcd.begin();
  lcd.backlight();
  pinMode(trigA, OUTPUT);    // set pin trig menjadi OUTPUT
  pinMode(echoA, INPUT);     // set pin echo menjadi INPUT
  pinMode(trigB, OUTPUT);    // set pin trig menjadi OUTPUT
  pinMode(echoB, INPUT);     // set pin echo menjadi INPUT

  pinMode(buzz, OUTPUT);
  pinMode(led, OUTPUT);  
  Serial.begin(9600);     // digunakan untuk komunikasi Serial dengan komputer
  tampil();
}

void loop() {
  menu();
}


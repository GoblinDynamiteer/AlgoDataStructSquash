#include <Wire.h>

const int MPU_addr = 0x68;
int16_t AcZ;
uint16_t ticker = 0;
uint16_t diff = 0;
int time;

/*  Struct för hållande av slag */
typedef struct{
  int16_t styrka;
  int tid;
  int langd;
  bool typ;
}slagS;

enum{FOREHAND, BACKHAND};
#define MAXSLAG 10

/*  Struct array */
slagS slag[MAXSLAG];
byte antal = 0;
int ix = 0;

void readMPUData(){
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr,14,true);
    AcZ = Wire.read()<<8|Wire.read();
}

void writeData(){
    Serial.println("----------------------");
    for(int i = 0; i < MAXSLAG; i++){
        Serial.print("Slag "); Serial.println(i);
        Serial.print("Typ ");
        if(slag[i].typ == BACKHAND){
            Serial.println("Backhand");
        }
        else{
            Serial.println("Forehand");
        }
        Serial.print("Styrka "); Serial.println(slag[i].styrka);
        Serial.print("Tid "); Serial.println(slag[i].tid);
        Serial.print("Längd "); Serial.println(slag[i].langd);
        Serial.println("----------------------");
    }
    //Serial.println("----------------------");
    Serial.println("Writing Done!");
    exit(0);  //The 0 is required to prevent compile error.
}

void setup(){
    Serial.begin(9600);
    while (!Serial) {
      ;
    }
  Serial.println("initialization done?");
    Wire.begin();
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);

    int countdown = 10;


  /*  Hitta diff-värde för kalibration   */
  Serial.print("Calibrating completed in ");
  for(int i = 0; i < 10; i++){
    readMPUData();
    diff = diff + AcZ;
    delay(1000);
    Serial.println(countdown--);
    Serial.println(AcZ);
  }
  diff = diff / 10;
}

void loop(){
  readMPUData();
  AcZ = ((AcZ - diff));
  if(AcZ < -3300){
    time = millis();
    Serial.println("Forehand");
    Serial.println(AcZ);
    slag[ix].styrka = fabs(AcZ);
    slag[ix].tid = (int)fabs(time);
    slag[ix].typ = FOREHAND;
    while(fabs(AcZ) > 400){
        readMPUData();
        AcZ = ((AcZ - diff));
    }
    time = millis() - time;
    Serial.println("tid för slag:");
    Serial.print(time);
    slag[ix].langd = (int)time;
    delay(10);
    ix++;
  }

  if(AcZ > 3300){
    time = millis();
    Serial.println("Backhand");
    Serial.println(AcZ);
    slag[ix].styrka = fabs(AcZ);
    slag[ix].tid = (int)time;
    slag[ix].typ = BACKHAND;
    while(fabs(AcZ) > 400){
        readMPUData();
        AcZ = ((AcZ - diff));
    }
    time = millis() - time;
    Serial.println("tid för slag:");
    Serial.print(time);
    slag[ix].langd = (int)time;
    delay(10);
    ix++;
  }
  /*    Output index value of number of added structs */
    Serial.print("----IX: "); Serial.println(ix);
    if(ix == MAXSLAG-1){
        Serial.println("DONE!");
        writeData();
    }
    delay(10);
}

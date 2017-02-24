/*  Kod för "Projekt Squashslag"
    Grupp 33

    Projekt i kurs Algoritmer och Datastrukturer

    Medlemmar:
      Dylan Saleh
      Johan Kämpe
    */

#include <Wire.h>

const int MPU_addr = 0x68;
int16_t AcZ; //värde för Acceleration i z-rikning
uint16_t diff = 0; //För kalibration av accelerometer
int time;

/*  Struct för hållande av slag */
typedef struct{
  int16_t styrka;
  int slagnummer; //Slag i ordningen
  int tid;  //Tidpunkt för slag, i millisekunder efter programstart
  int langd;  //Tislängd för slag
  bool typ; //Slagtyp, forehand eller backhand
}slagS;

enum{FOREHAND, BACKHAND};
#define MAXSLAG 100 //Satt maxstorlek för struct array

/*  Struct array */
slagS slag[MAXSLAG];
int ix = 0; //Arrayindex

/*   Implementerad insertion sort, för test av sortering,
    baserad på kod from TomKis sort.c */
void insert_sort(slagS* slag, int size) {
  int i, j;
  for (i = 1; i < size; i++) {
    slagS temp = slag[i];
    j = i - 1;
    while ((temp.styrka < slag[j].styrka) && (j >= 0)) {
      slag[j+1] = slag[j];
      j = j - 1;
    }
    slag[j+1] = temp;
  }
}

/*   Implementerad bubble sort, för test av sortering
  baserad på kod from TomKis sort.c */
void bubble_sort(slagS* slag, int size) {
   int ix, jx;
   for(ix = 0; ix < size-1; ix++) {
      for(jx = ix+1; jx < size; jx++) {
         if(slag[ix].styrka > slag[jx].styrka) {
            _swap(&slag[ix], &slag[jx]);
         }
      }
   }
}

/*   Implementerad selection sort, för test av sortering
  baserad på kod from TomKis sort.c */
void select_sort(slagS* slag, int size) {
   int ix, jx;
   for(ix = 0; ix < size-1; ix++) {
      int minx = ix;
      for(jx = ix+1; jx < size; jx++) {
         if(slag[jx].styrka < slag[minx].styrka) {
            minx = jx;
         }
      }
      _swap(&slag[ix], &slag[minx]);
   }
}

/*   För swap av två structar,
  baserad på kod from TomKis sort.c */
void _swap(slagS* s1, slagS* s2) {
   slagS temp = *s1;
   *s1 = *s2;
   *s2 = temp;
}

/*   Läser och sätter data från MPU,
    Acceleration i Z-riktning */
void readMPUData(){
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr,14,true);
    AcZ = Wire.read()<<8|Wire.read();
}

/*  Skriver ut sorterad array till Serial Montior
    Denna skulle kunna modiferas så att data skrivs
    till annan plats. T.ex en textfil på SD-kort. */
void writeData(){
    Serial.println("----------------------");
    for(int i = 0; i < MAXSLAG; i++){
        Serial.print("Slag "); Serial.println(slag[i].slagnummer);
        Serial.print("Typ ");
        if(slag[i].typ == BACKHAND){
            Serial.println("Backhand");
        }
        else{
            Serial.println("Forehand");
        }
        Serial.print("Styrka "); Serial.println(slag[i].styrka);
        Serial.print("Tid fran start: "); Serial.println(slag[i].tid);
        Serial.print("Langd "); Serial.println(slag[i].langd);
        Serial.println("----------------------");
    }
    Serial.println("Writing Done!");
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

  /*   Triggar vid "forehand"  */
  if(AcZ < -3300 && ix < MAXSLAG){
    time = millis();
    Serial.println("Forehand");
    Serial.println(AcZ);
    slag[ix].styrka = fabs(AcZ);
    slag[ix].tid = (int)fabs(time);
    slag[ix].typ = FOREHAND;
    slag[ix].slagnummer = ix + 1;
    while(fabs(AcZ) > 400){
        readMPUData();
        AcZ = ((AcZ - diff));
    }
    time = millis() - time;
    Serial.print("tid for slag: ");
    Serial.println(time);
    slag[ix].langd = (int)time;
    delay(10);
    ix++;
  }

  /*   Triggar vid "backhand"  */
  if(AcZ > 3300 && ix < MAXSLAG){
    time = millis();
    Serial.println("Backhand");
    Serial.println(AcZ);
    slag[ix].styrka = fabs(AcZ);
    slag[ix].tid = (int)time;
    slag[ix].typ = BACKHAND;
    slag[ix].slagnummer = ix + 1;
    while(fabs(AcZ) > 400){
        readMPUData();
        AcZ = ((AcZ - diff));
    }
    time = millis() - time;
    Serial.print("tid for slag: ");
    Serial.println(time);
    slag[ix].langd = (int)time;
    delay(10);
    ix++;
  }
    /*  Triggar när array är full, 100 st element.
      Arrayen sorteras med vald algoritm. */
    if(ix == MAXSLAG-1){
      Serial.println("DONE!");
      time = millis();
      //insert_sort(slag, MAXSLAG);
      //bubble_sort(slag, MAXSLAG);
      select_sort(slag, MAXSLAG);
      time = millis() - time;
      writeData();
      Serial.print("Select sort: ");
      Serial.println(time);
      /*  Tom while-loop som gör att programmet
          inte forstätter efter arrayen har sorterats
          och skrivits ut. */
      while(1){
        ;
        }
    }
    delay(10);
}

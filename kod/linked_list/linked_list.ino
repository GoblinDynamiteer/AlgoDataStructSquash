typedef struct{
  int styrka;
  int tid;
  bool typ;
}slagS;

enum{FOREHAND, BACKHAND};
#define MAXSLAG 100

slagS slag[MAXSLAG];
byte antal = 0;
int ix = 0;

void setup(){
  Serial.begin(9600);
}

void loop(){
  slag[ix].styrka = 300+ix;
  slag[ix].tid = 21+ix;
  slag[ix].typ = BACKHAND;
  Serial.print(ix);
  Serial.println(":");
  Serial.println(slag[ix].styrka);
  Serial.println(slag[ix].tid);
  Serial.println(slag[ix].typ);
  Serial.println("-------------------");
  ix++;
  delay(500);
}

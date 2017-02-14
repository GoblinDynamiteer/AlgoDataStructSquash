
enum{ FOREHAND, BACKHAND};

typedef struct _S_SLAG{
  int styrka;
  bool typ;
  int tid;
  _S_SLAG * next;
}slag;

void setup(){
  Serial.begin (9600);
  slag slag1;
  slag1.styrka = 500;
  slag1.typ = FOREHAND;
  slag1.tid = 897;
}

void loop(){
  
}

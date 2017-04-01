#define light_pin 19 //сюда подключен свет
#define buttUP 10 //кнопка вверх
#define buttDWN 5 //кнопка вниз
#define buttDWNgnd 7 //кнопка вниз
#define buttUPgnd 8 //кнопка вниз

boolean upS, dwnS;    // храним состояния кнопок (S - State)
boolean upF, dwnF;    // флажки кнопок (F - Flag)
boolean upP, dwnP;    // флажки кнопок на нажатие (P - Press)
boolean upH, dwnH;    // флажки кнопок на удержание (H - Hold)
unsigned long last_press;  // таймер последнего нажатия кнопки
#define hold 400           // время (мс), после которого кнопка считается зажатой
#define debounce 200       // (мс), антидребезг

unsigned long last_fps, last_shutter;
long shutter = 16666 + 35;
int tune;

void setup() {
  Serial.begin(9600);
  pinMode(light_pin, OUTPUT);
  pinMode(buttDWNgnd, OUTPUT);
  pinMode(buttUPgnd, OUTPUT);
  pinMode(buttUP, INPUT_PULLUP);
  pinMode(buttDWN, INPUT_PULLUP);
  digitalWrite(buttDWNgnd, 0);
  digitalWrite(buttUPgnd, 0);
}

void loop() {
  upS = !digitalRead(buttUP);
  dwnS = !digitalRead(buttDWN);
  buttons();

  if (upP) {
    upP = 0;
    shutter++;
  }
  if (dwnP) {
    dwnP = 0;
    shutter--;
  }
  if (upH) tune = 100;
  if (dwnH) tune = -100;
  if (!upH && !dwnH) tune = 0;

  if (micros() - last_fps > (long)shutter + tune) {
    last_fps = micros();
    digitalWrite(light_pin, 1);
    delay(5);
    digitalWrite(light_pin, 0);
  }
}

void buttons() {
  //-------------------------UP--------------------------
  // нажатие
  if (upS && !upF && millis() - last_press > debounce) {
    upF = 1;
    last_press = millis();
  }
  // Если отпущена до hold, то считать нажатием
  if (!upS && upF && millis() - last_press < hold) {
    upF = 0;
    upP = 1;
  }
  // Если удерживается более hold, то считать удержанием
  if (upS && upF && millis() - last_press > hold) {
    upH = 1;
  }
  // Если отпущена после hold, то считать удержанием
  if (!upS && upF && millis() - last_press > hold) {
    upF = 0;
    upH = 0;
  }
  //-------------------------UP--------------------------
  //-------------------------DOWN--------------------------
  // нажатие
  if (dwnS && !dwnF && millis() - last_press > debounce) {
    dwnF = 1;
    last_press = millis();
  }
  // Если отпущена до hold, то считать нажатием
  if (!dwnS && dwnF && millis() - last_press < hold) {
    dwnF = 0;
    dwnP = 1;
  }
  // Если удерживается более hold, то считать удержанием
  if (dwnS && dwnF && millis() - last_press > hold) {
    dwnH = 1;
  }
  // Если отпущена после hold, то считать удержанием
  if (!dwnS && dwnF && millis() - last_press > hold) {
    dwnF = 0;
    dwnH = 0;
  }
  //-------------------------DOWN--------------------------
}


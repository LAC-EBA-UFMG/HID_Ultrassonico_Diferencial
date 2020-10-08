#include <Arduino.h>
#include <Ultrasonic.h>
#include <Mouse.h>

#define DEBUG
#define MAX_RANGE 50 //max range dos sonares
#define MAX_ACCEL 20 //max diferença entre as mãos e max aceleracao do stick analogico virtual
#define MAX_STICK 50 //max range do stick analogico virtual pra cada lado
#define DEAD_ZONE 2 //zona morta do stick analógico virtual

Ultrasonic L(4,5);
Ultrasonic R(2,3);

void setup() {
  //Analogico virtual
  Mouse.begin();
  // put your setup code here, to run once:
  Serial.begin(115200);
  L.setTimeout(40000UL);
  R.setTimeout(40000UL);
}

bool validPack(uint8_t distl, uint8_t distr){
  if(max(distl,distr) > MAX_RANGE){
    return false;
  }
  else return true;
}

int8_t geraX(int8_t dir, uint8_t acc){
  int8_t x = dir * (float(float(acc) / float(MAX_ACCEL)) * MAX_STICK);
  return x;
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t Dist_L, Dist_R, Accel = 0;
  int8_t JoyDir = 0;
  Dist_L = L.read();
  delay(25);
  Dist_R = R.read();
  delay(25);

  //Testa pacote válido, distâncias dentro do range máximo
  if(validPack(Dist_L, Dist_R)){
    Accel = constrain(abs(Dist_L - Dist_R),0,MAX_ACCEL);
    if( Accel > DEAD_ZONE){
      if(Dist_L < Dist_R){
        JoyDir = -1;
      }
      else JoyDir = 1;
    }
    else JoyDir = 0;
  }
  else
  {
    JoyDir = 0;
    Accel = 0;
  }

  int8_t eixo_x = geraX(JoyDir, Accel); //calcula valor de X do stick analogico virtual

#ifdef DEBUG
  Serial.print("L: ");
  Serial.print(Dist_L);
  Serial.print(" R: ");
  Serial.println(Dist_R);

  Serial.print("DIR:");
  Serial.print(JoyDir);

  Serial.print(" ACC:");
  Serial.print(Accel);

  Serial.print(" EIXO X:");
  Serial.println(eixo_x);
#endif
 
 Mouse.move(eixo_x, 0, 0);
}
#include <EEPROM.h>

int motorPin1 = 8;
int motorPin2 = 10;
int motorPin3 = 9;
int motorPin4 = 11;
int delayTime1 = 50;
int delayTime2 = 250;
int motorPin5 = 12;
int motorPin6 = 3;
int motorPin7 = 13;
int motorPin8 = 2;
int elevationAddr = 0;
int azimuthAddr = 1;

#define DEG_TO_RAD 0.01745329
#define PI 3.141592654
#define TWOPI 6.28318531

int hour=16,minute=0,second=0,month=4,day=22,zone=5;
float Lon=-74.304*DEG_TO_RAD, Lat=31.481*DEG_TO_RAD;
float T,JD_frac,L0,M,e,C,L_true,f,R,GrHrAngle,Obl,RA,Decl,HrAngle,elev,azimuth;
long JD_whole,JDx;
int year=2016;

void setup() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(motorPin5, OUTPUT);
  pinMode(motorPin6, OUTPUT);
  pinMode(motorPin7, OUTPUT);
  pinMode(motorPin8, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  Serial.begin(9600);
}

void setAngleElevation(int angle){
  if(angle > 35 && angle < 90){
    int reqAngle = angle - EEPROM.read(elevationAddr);
    int steps = angleToStepsElevation(abs(reqAngle));
    if(reqAngle > 0){
      MotorUpperForword(steps);
    }else{
      MotorUpperBackword(steps);
    }
    EEPROM.write(elevationAddr,angle);
  }
}

void setAngleAzimuth(int angle){
  
  int reqAngle = angle - EEPROM.read(azimuthAddr);
  int steps = angleToStepsAzimuth(abs(reqAngle));
  if(reqAngle > 0){
    MotorLowerBackword(steps);
  }else{
    MotorLowerForword(steps);
  }
  EEPROM.write(azimuthAddr,angle);
}


void MotorUpperForword(int steps){
  int count = 0;
  digitalWrite(6,LOW);
  digitalWrite(7,LOW);
  while(count < steps){
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    digitalWrite(motorPin3, LOW);
    digitalWrite(motorPin4, LOW);
    count++;
    delay(delayTime1);
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
    digitalWrite(motorPin3, LOW);
    digitalWrite(motorPin4, LOW);
    count++;
    delay(delayTime1);
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
    digitalWrite(motorPin3, HIGH);
    digitalWrite(motorPin4, LOW);
    count++;
    delay(delayTime1);
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
    digitalWrite(motorPin3, LOW);
    digitalWrite(motorPin4, HIGH);
    count++;
    delay(delayTime1); 
  }
}

void MotorUpperBackword(int steps){
  int count = 0;
  digitalWrite(6,LOW);
  digitalWrite(7,LOW);
  while(count < steps){
    digitalWrite(motorPin4, HIGH);
    digitalWrite(motorPin3, LOW);
    digitalWrite(motorPin2, LOW);
    digitalWrite(motorPin1, LOW);
    count++;
    //decrementAngleElevationEEPROM();
    delay(delayTime1);
    digitalWrite(motorPin4, LOW);
    digitalWrite(motorPin3, HIGH);
    digitalWrite(motorPin2, LOW);
    digitalWrite(motorPin1, LOW);
    count++;
    //decrementAngleElevationEEPROM();
    delay(delayTime1);
    digitalWrite(motorPin4, LOW);
    digitalWrite(motorPin3, LOW);
    digitalWrite(motorPin2, HIGH);
    digitalWrite(motorPin1, LOW);
    count++;
    //decrementAngleElevationEEPROM();
    delay(delayTime1);
    digitalWrite(motorPin4, LOW);
    digitalWrite(motorPin3, LOW);
    digitalWrite(motorPin2, LOW);
    digitalWrite(motorPin1, HIGH);
    count++;
    //decrementAngleElevationEEPROM();
    delay(delayTime1);  
  }
}

void MotorLowerForword(int steps){
  int count = 0;
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
  while(count < steps){
    digitalWrite(motorPin5, HIGH);
    digitalWrite(motorPin6, LOW);
    digitalWrite(motorPin7, LOW);
    digitalWrite(motorPin8, LOW);
    count++;
    //incrementAngleAzimuthEEPROM();
    delay(delayTime2);
    digitalWrite(motorPin5, LOW);
    digitalWrite(motorPin6, HIGH);
    digitalWrite(motorPin7, LOW);
    digitalWrite(motorPin8, LOW);
    count++;
    //incrementAngleAzimuthEEPROM();
    delay(delayTime2);
    digitalWrite(motorPin5, LOW);
    digitalWrite(motorPin6, LOW);
    digitalWrite(motorPin7, HIGH);
    digitalWrite(motorPin8, LOW);
    count++;
    //incrementAngleAzimuthEEPROM();
    delay(delayTime2);
    digitalWrite(motorPin5, LOW);
    digitalWrite(motorPin6, LOW);
    digitalWrite(motorPin7, LOW);
    digitalWrite(motorPin8, HIGH);
    count++;
    //incrementAngleAzimuthEEPROM();
    delay(delayTime2); 
  }
}

void MotorLowerBackword(int steps){
  int count = 0;
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
  while(count < steps){
    digitalWrite(motorPin8, HIGH);
    digitalWrite(motorPin7, LOW);
    digitalWrite(motorPin6, LOW);
    digitalWrite(motorPin5, LOW);
    count++;
    //decrementAngleAzimuthEEPROM();
    delay(delayTime2);
    digitalWrite(motorPin8, LOW);
    digitalWrite(motorPin7, HIGH);
    digitalWrite(motorPin6, LOW);
    digitalWrite(motorPin5, LOW);
    count++;
    //decrementAngleAzimuthEEPROM();
    delay(delayTime2);
    digitalWrite(motorPin8, LOW);
    digitalWrite(motorPin7, LOW);
    digitalWrite(motorPin6, HIGH);
    digitalWrite(motorPin5, LOW);
    count++;
    //decrementAngleAzimuthEEPROM();
    delay(delayTime2);
    digitalWrite(motorPin8, LOW);
    digitalWrite(motorPin7, LOW);
    digitalWrite(motorPin6, LOW);
    digitalWrite(motorPin5, HIGH);
    count++;
    //decrementAngleAzimuthEEPROM();
    delay(delayTime2);
  }
}

void getAngles(){
    JD_whole=JulianDate(year,month,day);
    JD_frac=(hour+minute/60.+second/3600.)/24.-.5;
    T=JD_whole-2451545; T=(T+JD_frac)/36525.;
    L0=DEG_TO_RAD*fmod(280.46645+36000.76983*T,360);
    M=DEG_TO_RAD*fmod(357.5291+35999.0503*T,360);
    e=0.016708617-0.000042037*T;
    C=DEG_TO_RAD*((1.9146-0.004847*T)*sin(M)+(0.019993-0.000101*T)*sin(2*M)+0.00029*sin(3*M));
    f=M+C;
    Obl=DEG_TO_RAD*(23+26/60.+21.448/3600.-46.815/3600*T);
    JDx=JD_whole-2451545;
    GrHrAngle=280.46061837+(360*JDx)%360+.98564736629*JDx+360.98564736629*JD_frac;
    GrHrAngle=fmod(GrHrAngle,360.);
    L_true=fmod(C+L0,TWOPI);
    R=1.000001018*(1-e*e)/(1+e*cos(f));
    RA=atan2(sin(L_true)*cos(Obl),cos(L_true));
    Decl=asin(sin(Obl)*sin(L_true));
    HrAngle=DEG_TO_RAD*GrHrAngle+Lon-RA;
    
    elev=(asin(sin(Lat)*sin(Decl)+cos(Lat)*(cos(Decl)*cos(HrAngle))))/DEG_TO_RAD;
    // Azimuth measured eastward from north.
    azimuth=(PI+atan2(sin(HrAngle),cos(HrAngle)*sin(Lat)-tan(Decl)*cos(Lat)))/DEG_TO_RAD;
  
}

long JulianDate(int year, int month, int day) {
 long JD_whole;
 int A,B;
 if (month<=2) {
   year--; month+=12;
 }
 A=year/100; B=2-A+A/4;
 JD_whole=(long)(365.25*(year+4716))+(int)(30.6001*(month+1))+day+B-1524;
 return JD_whole;
}

int angleToStepsElevation(float angle){
  return angle * 40;
}

int angleToStepsAzimuth(float angle){
  return angle * 2.2;
}


void loop() {
//    EEPROM.write(azimuthAddr,0);
//    EEPROM.write(elevationAddr,57);
  
  getAngles();
  
  Serial.println("Current Elevation = " + String(EEPROM.read(elevationAddr)));
  Serial.println("Required Elevation = " + String(elev));
  //Serial.println("hour = " + String(hour));
  
  delay(3000);
  setAngleElevation(elev);
  delay(5000);
  
  Serial.println("CurrentAzimuth = " + String(EEPROM.read(azimuthAddr)));
  Serial.println("RequiredAzimuth = " + String(azimuth));
  Serial.println("hour = " + String(hour));
  
  if(EEPROM.read(azimuthAddr) > 254){
    setAngleAzimuth(0);
  }
  setAngleAzimuth(azimuth);
  
  delay(5000);
  Serial.println("currentAzimuth = " + String(EEPROM.read(azimuthAddr)));
  Serial.println("azimuth = " + String(azimuth));
  Serial.println("hour = " + String(hour));
  hour ++;
  Serial.println("hour - zone = " + String(hour-zone));
   
  if(hour > 18){
    setAngleAzimuth(0);
  }
  
  if(azimuth > 254){
    setAngleAzimuth(0);
  }

 
}

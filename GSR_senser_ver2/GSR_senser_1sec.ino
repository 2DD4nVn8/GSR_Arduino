#include <FlexiTimer2.h>
#include <SoftwareSerial.h>

//#define LED 13

SoftwareSerial soft_serial_d(11, 12);
const int GSR=A0;
int sensorValue=0;
int gsr_average=0;
double human_registar = 0;

double humanRegistar(int senserValue){
  double humanRegistar = ((1024.0+2.0*senserValue)*10000.0)/(512.0-senserValue);
  if (humanRegistar < 100000)
    return humanRegistar;
  else return -1;
}

int get_gsrData(){
//  digitalWrite(LED, HIGH);
  long sum_senserValue=0;
  long sum_humanRegistar=0;
  for(int i=0;i<10;i++){           //Average the 10 measurements to remove the glitch
    sensorValue=analogRead(GSR);
    sum_senserValue += sensorValue;
    delay(5);
  }
  gsr_average = sum_senserValue/10;
//  digitalWrite(LED, LOW);
  return gsr_average;
}

void setup(){
  soft_serial_d.begin(9600); 
//  pinMode(LED, INPUT);
  FlexiTimer2::set(1000, run);
  FlexiTimer2::start();
  
}

void loop() {

}

void run(){
  int gsr_data;
  gsr_data = get_gsrData();
  human_registar = humanRegistar(gsr_data);
  String postData = "{'gsr_data': ";
  postData.concat(gsr_average);
  postData.concat(", 'human_registar': ");
  postData.concat(human_registar);
  postData.concat("}");
  soft_serial_d.println(postData);  
}

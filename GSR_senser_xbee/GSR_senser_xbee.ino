#include <FlexiTimer2.h>
#include <SoftwareSerial.h>
#include <XBee.h>

#define LED 2
#define GW_XBEE_MAC_HIGH 0x0013A200 // ゲートウェイ側のXBeeの上位MACアドレス
#define GW_XBEE_MAC_LOW  0x40E6EBC2 // ゲートウェイ側のXBeeの下位MACアドレス

XBee xbee;
XBeeAddress64  gw_xbee_mac_long = XBeeAddress64(GW_XBEE_MAC_HIGH, GW_XBEE_MAC_LOW);
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
  digitalWrite(LED, HIGH);
  long sum_senserValue=0;
  long sum_humanRegistar=0;
  for(int i=0;i<10;i++){           //Average the 10 measurements to remove the glitch
    sensorValue=analogRead(GSR);
    sum_senserValue += sensorValue;
    delay(5);
  }
  gsr_average = sum_senserValue/10;
  digitalWrite(LED, LOW);
  return gsr_average;
}

void setup(){
  soft_serial_d.begin(9600);
  xbee.begin(soft_serial_d); 
  pinMode(LED, INPUT);
  digitalWrite(LED, HIGH);
  FlexiTimer2::set(1000, run);
  FlexiTimer2::start();
  
}

void loop() {

}

void run(){
  int gsr_data;
  char data_json_char[128];
  char human_registar_char[16];
  
  /* 変数の初期化 */
  memset(data_json_char, 0, 128);
  memset(human_registar_char, 0, 16);
  
  gsr_data = get_gsrData();
  human_registar = humanRegistar(gsr_data);
  
  ((String)human_registar).toCharArray(human_registar_char,16);

  sprintf(data_json_char, "{'gsr_senser':{'human_registar':%s, 'gsr_data':%d}}", human_registar_char, gsr_data);
  ZBTxRequest zbTx = ZBTxRequest(gw_xbee_mac_long, data_json_char, strlen(data_json_char));
  xbee.send(zbTx);

}

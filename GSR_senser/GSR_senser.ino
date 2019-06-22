#define LED_PIN 13

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
  long sum_senserValue=0;
  long sum_humanRegistar=0;
  for(int i=0;i<10;i++){           //Average the 10 measurements to remove the glitch
    sensorValue=analogRead(GSR);
    sum_senserValue += sensorValue;
    delay(5);
  }
  gsr_average = sum_senserValue/10;
  return gsr_average;
}

void setup(){
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  Serial.println(gsr_average);  
  delay(5);
}

void serialEvent(){
  digitalWrite(LED_PIN, LOW);
  String read_str = "hogehoge";
  int gsr_data;
  if(Serial.available()>0) {
    read_str=Serial.readString();
    if(read_str = "get"){
      gsr_data = get_gsrData();
      human_registar = humanRegistar(gsr_data);
      String postData = "{'gsr_data': ";
      postData.concat(gsr_average);
      postData.concat(", 'human_registar': ");
      postData.concat(human_registar);
      postData.concat("}");
      Serial.println(postData);  
    }
    else {
      delay(5);
      Serial.println(read_str);
    }
  }
}

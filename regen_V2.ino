int DAC_Ref = 4095; //2.75 V = 4095
int DAC_Pin = DAC1; //DAC0 is Kaput!
int ADC_Pin = A1;
float ADC_Value = 0.000; 
unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period =10; 
float dQ=0.000; 
float Q=0.000; 
float voltage=0.000; 
float V_hall=0.000; 
float Current=0.000; 
bool cap_charging=false; 
bool regen_complete=false; 


float Curr(ADC_Pin){ 

  voltage=ADC_Value*(3.3000/4095.000); 
  
  V_hall=((voltage-2.7500)*(16875.0000/6875.0000))+2.75000; 

  Current=V_hall*25.0000; //*2 

return Current;
}

float Charge(unsigned long startMillis, unsigned long currentMillis,unsigned long period,int ADC_pin, float Q,float dQ, bool cap_charging, float voltage, float V_hall, float Current){ 

ADC_Value = analogRead(ADC_Pin);
  Serial.print("ADC Value = "); 
  Serial.println(ADC_Value);
  voltage=ADC_Value*(3.3000/4095.000); 
  
  V_hall=((voltage-2.7500)*(16875.0000/6875.0000))+2.75000; 

  Current=V_hall*25.0000; //*2 

  currentMillis= millis();
if ((currentMillis-startMillis>=period) ){

    if((Current>0.5||Current<-0.5)){ //This section acts as a deadband as hall sensor gives ghost reading

    
    dQ= Current*(currentMillis-startMillis)*1e-3;
    
    } else{ 

      dQ=0;

      }
   
   startMillis=currentMillis;
} 
  
  Q=Q+dQ;
  return Q;
  //Serial.println(voltage,4); 

  Serial.print("Current ");
  Serial.println(Current,4);
  //erial.println("Time");
  //Serial.println(startMillis);
  //Serial.println(currentMillis);
  Serial.println("dQ");
  Serial.println(dQ,4);
 // Serial.println("Q"); 
  //Serial.println(Q,4); 
  //digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  //delay(250);                       // wait
  //digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  //delay(250);                       // wait

}





void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  analogWriteResolution(12);
  analogReadResolution(12);
  Serial.begin(115200);
  analogWrite(DAC_Pin, DAC_Ref);
   while (!Serial) {;}
  startMillis = millis(); 


}






void loop() {
  // put your main code here, to run repeatedly:


 float bob=Charge(startMillis, currentMillis, ADC_Pin,Q, dQ,cap_charging, voltage,V_hall,Current); 
   Serial.println("Q"); 
  Serial.println(bob,4);
  delay(1000); 
  
          if(bob>10){ // This ensures capacitor is genuinly being charged up

      cap_charging=true;

    } 

  if(cap_charging==true && bob<1){  // This section is used to determine if capacitor has fully discharged during regen challenge

    cap_charging=false; 

    *regen_complete=true; 

    Serial.print("capacitor is successfully discharged");


  }

}

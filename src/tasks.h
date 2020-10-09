#include <Arduino.h>

double k = 0.01;
double step = 0.01;
int delay_time = 80;

void motor(int H1_,int H2_,int H3_, int L1_,int L2_,int L3_){
    H1_=H1_*k; 
    H2_=H2_*k;
    H3_=H3_*k;
    L1_=L1_*k;
    L2_=L2_*k;
    L3_=L3_*k; 
    ledcWrite(L1_val, L1_); //L1
    ledcWrite(L2_val, L2_); //L2
    ledcWrite(L3_val, L3_); //L3
    ledcWrite(H1_val, H1_); //H1
    ledcWrite(H2_val, H2_); //H2
    ledcWrite(H3_val, H3_); //H3
}

void SPWM( void * parameter)
{
    Serial.println("SPWM");

    int ShuntValue = 0;
    int max = 0;

    while(1){
    if(!emergency){
    for(int Cycle = 0; Cycle <= 480; Cycle++){   
        if((Cycle>=0)&&(Cycle<=80)){    //___1
        motor(SINE_LOOKUP_TABLE[Cycle],0,SINE_LOOKUP_TABLE[Cycle+160], 0, SINE_LOOKUP_TABLE[Cycle+80], 0); //H1 H2 H3 L1 L2 L3

        if(Cycle==40){
            ShuntValue = 0;//analogRead(shunt);
            if(ShuntValue>max) max = ShuntValue;
            if(max>2000){
            emergency = true;
            break;
        }}

        }
        
        if((Cycle>80)&&(Cycle<=160)){    //___2 
        motor(SINE_LOOKUP_TABLE[Cycle],0,0,0,SINE_LOOKUP_TABLE[Cycle+80],SINE_LOOKUP_TABLE[Cycle+160]); //H1 H2 H3 L1 L2 L3
        
        if(Cycle==120){
            ShuntValue = 0;//analogRead(shunt);
            if(ShuntValue>max) max = ShuntValue;
            if(max>2000){
            emergency = true;
            break;
        }}

        }
        
        if((Cycle>160)&&(Cycle<=240)){    //___3
        motor(SINE_LOOKUP_TABLE[Cycle],SINE_LOOKUP_TABLE[Cycle+80],0,0,0,SINE_LOOKUP_TABLE[Cycle+160]); //H1 H2 H3 L1 L2 L3

        if(Cycle==200){
            ShuntValue = 0;//analogRead(shunt);
            if(ShuntValue>max) max = ShuntValue;
            if(max>2000){
            emergency = true;
            break;
        }}

        }

        if((Cycle>240)&&(Cycle<=320)){    //___4
        motor(0,SINE_LOOKUP_TABLE[Cycle+80],0,SINE_LOOKUP_TABLE[Cycle],0,SINE_LOOKUP_TABLE[Cycle+160]); //H1 H2 H3 L1 L2 L3
        
        if(Cycle==280){
            ShuntValue = 0;//analogRead(shunt);
            if(ShuntValue>max) max = ShuntValue;
            if(max>2000){
            emergency = true;
            break;
        }}

        }

        if((Cycle>320)&&(Cycle<=400)){    //___5
        motor(0,SINE_LOOKUP_TABLE[Cycle+80],SINE_LOOKUP_TABLE[Cycle-80],SINE_LOOKUP_TABLE[Cycle],0,0);  //H1 H2 H3 L1 L2 L3

        if(Cycle==360){
            ShuntValue = 0;//analogRead(shunt);
            if(ShuntValue>max) max = ShuntValue;
            if(max>2000){
            emergency = true;
            break;
        }}

        }

        if((Cycle>=400)&&(Cycle<=480)){    //___6
        motor(0,0,SINE_LOOKUP_TABLE[Cycle-80],SINE_LOOKUP_TABLE[Cycle],SINE_LOOKUP_TABLE[Cycle-160],0); //H1 H2 H3 L1 L2 L3
        
        if(Cycle==440){
            ShuntValue = 0;//analogRead(shunt);
            if(ShuntValue>max) max = ShuntValue;
            if(max>2000){
            emergency = true;
            break;
        }}

        }
        delayMicroseconds(delay_time);
        
        if(Cycle==480){
        if(k>1) {k=1; step = 0;}
        else{delay_time = map(k*100, 0.0, 100.0, 90, 3); k += step;}
        }

    }}
    else{
        ledcWrite(L1_val, 0);
        ledcWrite(L2_val, 0);
        ledcWrite(L3_val, 0);
        ledcWrite(H1_val, 0);
        ledcWrite(H2_val, 0);
        ledcWrite(H3_val, 0);

        max = 0;
        k=0.01;
        step = 0.01;
        vTaskDelay(1000);
    }
    }

    Serial.println("Ending SPWM");
    vTaskDelete(Task1);
}

void Servises( void * parameter)
{
    Serial.println("Servises");
    while(1){
        Serial.print("Servises running on core ");
        Serial.println(xPortGetCoreID());
        esp_task_wdt_reset();
        vTaskDelay(1000);
    }

    Serial.println("Ending Servises");
    vTaskDelete(Task2);
}
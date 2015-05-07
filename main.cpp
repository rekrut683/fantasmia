

/*************ПОДКЛЮЧЕНИЕ БИБЛИОТЕК*******************/
#include <Ethernet.h>
#include <OneWire.h>
#include <TimerOne.h>
#include <SPI.h>

/**************ОПРЕДЕЛЕНИЕ ПИНОВ***********************/  
#define KLETKA                A0
#define KOMOD                 24

#define KARTA                 22
#define SHKAF                 23

#define COMPAS                25
#define KARTINA               26

#define BUTYLKI               31
#define SUNDUK                32
#define SVET_KOMNATA          47
#define AVAR_SVET             44
#define DYM_220               37

#define EKIPAZH_1              7
#define EKIPAZH_2              6
#define EKIPAZH_3              5
#define EKIPAZH_4              27
#define EKIPAZH_5              3
#define TRUM                   28

#define PTICA                  A1
#define RELE                   30
#define SV_L1                  41

#define KNOPKA                 33
#define SKRYTAYA_DVER          42

#define KABEL                  35
#define SVET_2                 46

#define K_PRIEM                36
#define MAGN_PRIEM             43
#define DYM_12                 34

#define SHESTIGRANNIK          38

#define SEVEN_KNOPOK           39
#define SV_KNOPKI              A2

#define VYH_IND                40
#define VYHOD                  48
/***************************************************/


/******************НАСТРОЙКИ ETHERNET***************/
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress server(192,168,0,100);
IPAddress ip(192,168,0,101);
EthernetClient client;
#define W5200_CS  53
#define SDCARD_CS 4
boolean connect_=false;
/*****************************************************/

/*****************СОСТОЯНИЯ ДАТЧИКОВ******************/
boolean state[12];

/*******************ЗАДАЧИ****************************/
boolean task1,task2,task3,task4,task5,task6,task7,task8,task9,task10,task11,task12,half_task9;

/********************ФЛАГ ИГРЫ*************************/
boolean game;


/***********************ПЕРЕМЕННЫЕ ДЛЯ РАБОТЫ***************/
//===============таймер
int cur_timer1;								//счетчик таймера
int cur_timer2;								//счетчик таймера2
int state_timer;						 	//текущее состояние таймера
//===============автопилот
int state_7_knopki;							//состояние кнопок автопилота
//===============светодиоды в машине времени
int state_led;								//состояние светодиодов в машине времени
//===============клетка
int calibration_kletka;						//начальное калибровочное значение клетки
boolean calib_kl;							//флаг акта калибровки клетки
unsigned long kl;							//отметка времени для задержки клетки
int num_kletka;								//номер текущего измерения клетки
int KLETKA_ONE;							  	//первое измерение клетки
int kletka_0;								//нулевое значение кнопка
int kletka_min;								//минимальное значение клетки
int kletka_max;								//максимальное значение клетки
boolean flag_kletka;						//флаг работы по измерению клетки
/***************************************************************/

/***********************КОНСТАНТНЫ ДЛЯ РАБОТЫ****************/
const int timer_On_dym = 29;							//таймер ожидания дыма
const int timer_Work_dym = 3;						//таймер работы дым-машины
const int timer_On_karta = 6;						//таймер отпускания карты
const int timer_proverka = 25;						   //таймер на проверку всех систем по игре
/************************************************************/

/***********************ПЕРЕМЕННЫЕ ONEWIRE ДЛЯ ЭКИПАЖА********/
OneWire ek1(EKIPAZH_1);
OneWire ek2(EKIPAZH_2);
OneWire ek3(EKIPAZH_3);
OneWire ek4(EKIPAZH_4);
OneWire ek5(EKIPAZH_5);
  
byte i;
byte addr[8];
  
  
String right_1 = "1342191120117042";
String right_2 = "113522311201170130";
String right_3 = "111519411201170188";
String right_4 = "123058124018088";
String right_5 = "17422111201170212";    

String card1 = "";
String card2 = ""; 
String card3 = "";
String card4 = "";
String card5 = "";
  
boolean is_card1,is_card2,is_card3,is_card4,is_card5 = false;
/*****************************************************/

/*******************ПЕРЕМЕННАЯ ДЛЯ ОТПРАВКИ НА КОМП ОПЕРАТОРА*******************/
String send_;







/*добавил 07.05*/
  unsigned long ml_knopka; 
  int knopka_time;
  boolean flag_knopka;
/***************/



/************************ФУНКЦИЯ ИНИЦИАЛИЗАЦИИ********************************/
void init_() {
  
  game = false;
  cur_timer1 = 0;								//обнуляем счетчик таймера
  cur_timer2 = 0;								//обнуляем счетчик таймера2
  state_timer=0;  								//обнуляем состояние таймера
  state_7_knopki = false;						//обнуляем состояние кнопок автопилота
  state_led = 0;								//обнуляем состояние светодиодов
  
  /********************ОБНУЛЯЕМ СОСТОЯНИЕ КАРТОЧЕК ЭКИПАЖА***************/
  is_card1 = false;
  is_card2 = false;
  is_card3 = false;
  is_card4 = false;
  is_card5 = false;
  
  /*******************ОБНУЛЯЕМ КАЛИБРОВКУ КЛЕТКИ**************************/
  calibration_kletka = 0;  
  calib_kl = false;
  num_kletka = 0;
  flag_kletka = false;
   
	//==============================   вставил игорь
      kletka_0 = analogRead(KLETKA); 
		// Serial.println(kletka_0);
      delay(200);
      kletka_0 = kletka_0+analogRead(KLETKA); 
		//  Serial.println(kletka_0);
      delay(200);
      kletka_0 = kletka_0+analogRead(KLETKA); 
		//   Serial.println(kletka_0);

       kletka_0 = kletka_0/3;
       
       kletka_min = kletka_0+197-12;
         kletka_max = kletka_0+197+12;
    // Serial.println("среднее:");
   //  Serial.println(kletka_0);
   //  delay(2000);

      
  //============================ вставил игорь
  
/***********************ОБНУЛЯЕМ СОСТОЯНИЕ КНОПКИ В СУНДУКЕ******************************/
  /***добавил строчку, ранее не было 07.05*/ 
		ml_knopka = 0; 
		knopka_time = 200;
		flag_knopka = false;
  /***************************************/


  send_="";											//обнуляем строчку отправки на оператора
  
  //ИНИЦАЛИЗИРУЕМ CS ETHERNET
  digitalWrite(SDCARD_CS,HIGH);
  digitalWrite(W5200_CS,LOW);
  
  //ОБНУЛЯЕМ ВСЕ СОСТОЯНИЯ ДАТЧИКОВ
  for (int k=0;k<12;k++){
     state[k]=false; 
  }
  
  /*************УСТАНАВЛИВАЕМ СОСТОЯНИЯ ЗАДАЧ***********************/
  task1 = true;
  task2 = true;
  task3 = true;
  task4 = true;
  task5 = true;
  task6 = true;
  
  /*изменил 07.05, ранее было task7 = true*/
  task7 = false;
  /**************************************/
  
  task8 = true;
  task9 = false;
  task10 = false;
  task11 = false;
  task12 = false;
  half_task9 = true;
  /*********************************************************************/
  
  
  /***********************************УСТАНАВЛИВАЕМ ВСЕ ИСПОЛНИТЕЛИ****************/
  digitalWrite(KOMOD,LOW);
  digitalWrite(KARTA,HIGH);
  digitalWrite(SHKAF,LOW);
  digitalWrite(COMPAS,HIGH);
  digitalWrite(KARTINA,LOW);
  digitalWrite(SUNDUK,LOW);
  digitalWrite(SVET_KOMNATA,LOW);
  digitalWrite(AVAR_SVET,LOW);
  digitalWrite(DYM_220,LOW);
  digitalWrite(TRUM,LOW);
  digitalWrite(RELE,LOW);
  digitalWrite(SV_L1,LOW);
  digitalWrite(KNOPKA,HIGH);
  digitalWrite(SKRYTAYA_DVER,LOW);
  digitalWrite(KABEL,HIGH);  
  digitalWrite(SVET_2,HIGH);
  digitalWrite(MAGN_PRIEM,LOW);
  digitalWrite(DYM_12,LOW);
  digitalWrite(SEVEN_KNOPOK,HIGH);
  digitalWrite(SV_KNOPKI,LOW);
  digitalWrite(VYHOD,LOW);
  /*******************************************************/
  
  //////////ФЛАГ ИГРЫ СТАВИМ
  game = true;
  
  //////////ОТПРАВКА СТАРТА ИГРЫ НА МУЗЫКУ
  Serial1.print("start");
  delay(3000);
  
  /////////////МОРГАЕМ ВСТРОЕННЫМ СВЕТОДИОДОМ, ПРИЗНАК СТАРТА
  digitalWrite(13,HIGH);
  delay(150);
  digitalWrite(13,LOW);
  delay(150);
  digitalWrite(13,HIGH);
  delay(150);
  digitalWrite(13,LOW);
  delay(150);
  digitalWrite(13,HIGH);
  delay(150);
  digitalWrite(13,LOW);
  delay(150);
  digitalWrite(13,HIGH);
  
  
  
}

/*******************ФУНКЦИЯ НАЧАЛЬНЫХ УСТАНОВОК*************************/
void setup() {
  
  sei();										//разрешаем прерывания
  
  /**********************ИНИЦИАЛИЗИРУЕМ CS ETHERNET**********************/
  pinMode(SDCARD_CS,OUTPUT);
  digitalWrite(SDCARD_CS,HIGH);
  pinMode(W5200_CS,OUTPUT);
  digitalWrite(W5200_CS,LOW);
 
  /*********************УСТАНАВЛИВАЕМ ВСТРОЕННЫЙ СВЕТОДИОД*****************/
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  
  /*********************ИНИЦИАЛИЗИРУЕМ UART для отладки и для музыки****************/
  Serial.begin(9600);
  Serial1.begin(9600);
  delay(200);
  
  /********************НАСТРОЙКИ СЕТИ*******************************/
  Ethernet.begin(mac,ip);
  delay(1500);
  Serial.print("ip is: ");
  Serial.println(Ethernet.localIP());
 
  /*******************ИНИЦИАЛИЗИРУЕМ ТАЙМЕР**************************/
  Timer1.initialize();
 
  /********************ВНЕШНЕЕ ОПОРНОЕ НАПРЯЖЕНИЕ********************/
  analogReference(EXTERNAL);
  
  /*********************ИНИЦИАЛИЗИРУЕМ ПИНЫ**************************/
  pinMode(KLETKA,INPUT);
  pinMode(KOMOD,OUTPUT);
  
  pinMode(KARTA,INPUT);
  
  pinMode(SHKAF,OUTPUT);
  
  pinMode(COMPAS,INPUT);
  
  pinMode(KARTINA,OUTPUT);
  
  pinMode(BUTYLKI,INPUT);
  pinMode(SUNDUK,OUTPUT);
  
  pinMode(SVET_KOMNATA,OUTPUT);
  
  pinMode(AVAR_SVET,OUTPUT);
  
  pinMode(DYM_220,OUTPUT);
 
  pinMode(TRUM,OUTPUT);
 
  pinMode(RELE,OUTPUT);
  pinMode(SV_L1,OUTPUT);
  
  pinMode(KNOPKA,INPUT);
  
  pinMode(SKRYTAYA_DVER,OUTPUT);
  
  pinMode(KABEL,INPUT);
  
  pinMode(SVET_2,OUTPUT);
  
  pinMode(K_PRIEM,INPUT);
  pinMode(MAGN_PRIEM,OUTPUT);
  
  pinMode(DYM_12,OUTPUT);
  
  pinMode(SEVEN_KNOPOK,INPUT);
  
  pinMode(SV_KNOPKI,OUTPUT);
  
  pinMode(VYH_IND,INPUT);
  pinMode(VYHOD,OUTPUT);
  
  //ЗАПУСК ИНИЦИАЛИЗАЦИИ
  init_();
  
}


/***********************ОБРАБОТЧИК ТАЙМЕРА*************************/
void timerIsr()
{
  
  //инкремент счетчика таймера
  cur_timer1++;
  
  ///////УСЛОВИЕ НА МОРГАНИЕ СВЕТОДИОДАМИ
  if (cur_timer1 == timer_proverka && state_timer==0) {
    
          digitalWrite(SVET_2,HIGH);
		  state_led = 1;
             
    }
  
  ///////УСЛОВИЕ НА ОЖИДАНИЕ ДЫМА
  if (cur_timer1 == timer_On_dym && state_timer==0) {
    
      digitalWrite(DYM_12,HIGH);
      state_timer=1;
      cur_timer1 = 0;
    
    }
  
  ///////УСЛОВИЕ НА РАБОТУ ДЫМА
  if (cur_timer1 == timer_Work_dym && state_timer==1) {
     digitalWrite(DYM_12,LOW);
     digitalWrite(DYM_220,LOW);
     state_timer=2;
     cur_timer1 = 0;
     Timer1.stop();
    
  }
  
  ///////УСЛОВИЕ НА ОТПУСКАНИЕ КАРТЫ
  if (cur_timer1 == timer_On_karta && state_timer==2) {
    
      digitalWrite(MAGN_PRIEM,LOW); 
      Timer1.stop();
      cur_timer1=0;
     
  }
  
  
  
  
    
}


/************************МОРГАНИЕ СВЕТОДИОДАМИ**********************/
void blink_led() {
  
			 delay(100);
             digitalWrite(AVAR_SVET,LOW);
             delay(800);
             digitalWrite(AVAR_SVET,HIGH);
             delay(500);
             digitalWrite(AVAR_SVET,LOW);
             delay(400);
             digitalWrite(AVAR_SVET,HIGH); 
             delay(300);
             digitalWrite(AVAR_SVET,LOW);
             delay(200);
             digitalWrite(AVAR_SVET,HIGH);
             delay(100);
             digitalWrite(AVAR_SVET,LOW);
             delay(50);
             digitalWrite(AVAR_SVET,HIGH);
  
}

/********************ОТКРЫВАЕМ ШКАФ ПО КЛЕТКЕ******************/
void make_task1() {
  
                Serial.println("task1 OK");
                Serial1.print("kletka");
                state[0]=true;
                digitalWrite(SHKAF,HIGH);
				delay(1000);
				task1 = false;
                          
}

/********************ОТКРЫВАЕМ КОМОД ПО КОРАБЛЯМ******************/
void make_task2() {
  
        delay(1);
        Serial1.print("karta");
        state[1]=true;
        Serial.println("task2 OK");
        digitalWrite(KOMOD,HIGH);
        task2 = false;
        delay(1000);
  
  
  
}

/*********************ОТКРЫВАЕМ КАРТИНУ******************************/
void make_task3() {
  
              Serial1.print("compas");
              state[2]=true;
              delay(1);
              digitalWrite(KARTINA,HIGH);
              delay(1000);
              task3 = false;
  
}


/***********************ОТКРЫВАЕМ ТРЮМ**********************************/
void make_task4() {
  
 
                             digitalWrite(TRUM,HIGH); 
                             Serial.print("Card5: ");
                             Serial.print(card5);
                             state[3]=true;
                             delay(1);
                             Serial1.print("ekipazh");  
                             task4 = false;
                             delay(1000); 
  
  
}

/**************************РОНЯЕМ КЛЮЧ*************************************/
void make_task5() {
  
							state[6]=true;
                            digitalWrite(RELE,HIGH);
                            digitalWrite(SV_L1,HIGH);
                            Serial.println("ON RELE");
                            delay(200);
                            Serial.println("OFF RELE");
                            digitalWrite(RELE,LOW);
                            Serial.println("task5 OK");
                            digitalWrite(DYM_220,HIGH);
                            delay(1000);
                            task5 = false;      
                            
							
}							

/*************************ОТКРЫВАЕМ СУНДУК************************************/
void make_task6() {
  
      Serial.println("task6 OK");
      digitalWrite(SUNDUK,HIGH);
      digitalWrite(SVET_KOMNATA,HIGH);
      state[4]=true;
      delay(1);
      Serial1.print("sunduk");
      delay(1000);
      task6 = false;
      
      /****добавил 07.05, ранее этой строчки не было вообще****/
      task7 = true;
      /**********************/
  
}

/*******************************ОТКРЫВАЕМ СКРЫТУЮ ДВЕРЬ**************************/
void make_task7() {
  
  
                state[5]=true;
                digitalWrite(SKRYTAYA_DVER,HIGH);
                delay(1);
                Serial1.print("knopka");
                delay(2000);
                task7 = false;
  
}

/******************************ВКЛЮЧАЕМ СВЕТ*****************************/
void make_task8() {
  
             state[7]=true;
             Serial1.print("kabel");
             delay(100);
             digitalWrite(SV_L1,LOW);
             digitalWrite(AVAR_SVET,HIGH);
             delay(500);
             digitalWrite(AVAR_SVET,LOW);
             delay(400);
             digitalWrite(AVAR_SVET,HIGH);
             delay(300);
             digitalWrite(AVAR_SVET,LOW);
             delay(200);
             digitalWrite(AVAR_SVET,HIGH);
             delay(100);
             digitalWrite(AVAR_SVET,LOW);
             delay(50);
             digitalWrite(AVAR_SVET,HIGH);
             digitalWrite(SVET_2,LOW);
             delay(15000);
             task8 = false;
             task9 = true;
             
}

/***********************АКТИВИРУЕМ КАРТОПРИЕМНИК*******************************/
void make_task9() {
  
             state[8]=true;            
             delay(1);
             Serial1.print("check");
             delay(10);
             Timer1.attachInterrupt( timerIsr );
			 digitalWrite(MAGN_PRIEM,HIGH);
			 half_task9 = false;
  
  
  
}

/**************************ВКЛЮЧАЕМ СВЕТ ПОСЛЕ ШЕСТИГРАННИКА**********************/
void make_task10() {
  
             delay(1);
             Serial1.print("hex_key");
             state[9]=true;
             digitalWrite(SVET_2,LOW);
             delay(100);
             delay(2000);
             task10 = false;
             task11 = true; 
  
  
}

/*************************АКТИВАЦИЯ АВТОПИЛОТА*************************************/
void make_task11() {
  
             digitalWrite(SV_KNOPKI,HIGH);
             Timer1.attachInterrupt( timerIsr );
             state[10]=true;
             delay(1);
             Serial1.print("twister");
             state_7_knopki = true;
             task12 = true; 
  
  
}

/****************************ОТКРЫВАЕМ ВЫХОД*****************************/
void make_task12() {
  
             digitalWrite(VYHOD,HIGH);
             state[11]=true;
             delay(1); 
             Serial1.print("vyhod");
             task12 = false;
             
  
}


/*****************************ГЛАВНЫЙ ЦИКЛ******************************/
void loop() {
  
  /***********************ОБРАБОТКА ПРОГИ ОПЕРАТОРА**********************/
  if (client.connected()) {

      Serial.println("true");
        if (client.available()>0) {
          
             String str = client.readStringUntil('\r\n'); 
          
          
            if (str == "11") {make_task1();}
            if (str == "21") {make_task2();}
            if (str == "31") {make_task3();}
            if (str == "41") {make_task4();}
            if (str == "51") {make_task6();}
            if (str == "61") {make_task7();}
            if (str == "71") {make_task5();}
            if (str == "81") {make_task8();}
            if (str == "91") {make_task9();}
            if (str == "A1") {make_task10();}
            if (str == "B1") {make_task11();}
            if (str == "C1") {make_task12();}
            if (str == "AA") {init_();}
            
            if (str != "") {Serial.print("str: "); Serial.println(str);}
            
          
        }
      
      
        connect_=true;
      
           

    }     
   
   
   else {
    
       
     
         if (connect_){
        client.stop();Serial.println("client stop");
       }
         
         connect_=false;
         delay(100);
		 Serial.println("try");
     
       
        client.connect(server,10001);
        
   } 
/***********************************************/


//////КЛЕТКА
  if (task1) {
    
      delay(300);
      int b1;
      
      
      if (!calib_kl) {
        
        
           calibration_kletka = analogRead(KLETKA); 
           calib_kl = true;
        
      }
      
      
      else {
        
        
          if (num_kletka == 0 && !flag_kletka) {
            
               kl = millis();
               b1 = analogRead(KLETKA);
               KLETKA_ONE = b1; 
               num_kletka++;
              
            
          }
          
          else if (num_kletka == 1 && !flag_kletka) {
            
            
            unsigned long ml;
            ml = millis();
            b1 = analogRead(KLETKA);
            num_kletka = 0;
           
            
            
            //////////////////ВОТ ЗДЕСЬ ЦИФРЫ НА КЛЕТКУ////////////////////////////////////////
            ////////////////////KLETKA_ONE - первое измерение
            /////////////////////b1 - второе
            //////////////////////отличие между ними стоит в 10-очку и сами они лежат в диапазоне 590-600
            if (abs(KLETKA_ONE-b1)<25 && b1 > kletka_min && b1 < kletka_max && !flag_kletka) {
              //////////////////////////////////////////////////////////////////////////
              
                  kl = millis();
                  flag_kletka = true;
                 // Serial.println("pervaya proverka ok");
                  
            }
                  
              
              
                  
            
        
      }
    
    if (flag_kletka) {
               
                  unsigned long ml;
                   ml = millis();
                   
                  
                  
                  /////////////////////////////Задержка в 3 секунды///////////////////////////
                  if (ml-kl >= 3000) {
                    b1 = analogRead(KLETKA);
                    
                    
                    ////////////////////////////Еще раз теже цифры//////////////////////////////////////
                    if (abs(KLETKA_ONE-b1)<25 && b1 > kletka_min && b1 < kletka_max) {
                    
                      
                            make_task1();
                             
                   
                  }
                 else {
                      kl = 0;
                      ml = 0;
                      flag_kletka = false;
                      num_kletka = 0;
                  
                  
                 } 
                
              }
    
    }
  }
  }
  
///////КОРАБЛИ  
  if (task2) {
    
    if (digitalRead(KARTA) == 0) {
      
      
      make_task2();
      
     
    }
    
    
  }
 
////////КОМПАС 
  if (task3) {
    
    
      int b1,b2;
    
      
      b1 = digitalRead(COMPAS);
      
      if (b1 == 0) {
      
            delay(3000);
            
            b2 = digitalRead(COMPAS);
            
            if (b2 == 0) {
              
              make_task3();
              
               
              
            }
          
        }
      
      
      
        
      
    
    
  }
  
  
  
///////ЭКИПАЖ  
  if (task4) {
    

    card1="";
    card2="";
    card3="";
    card4="";
    card5="";
    
    is_card3=false;
    is_card4=false;
    is_card5=false;
    is_card1=false;
    is_card2=false;
    
    if (ek1.search(addr)) {
      
         
          for(i=0;i<8;i++) {
            
           card1.concat(String(addr[i]));
            
          }
          
          
            if (card1 == right_1) {is_card1 = true;
          
        }
            else {card1 = "";
          
        }
            
          
                if(ek2.search(addr) && is_card1) {
               
                     
                      for(i=0;i<8;i++) {
           
                           
                            card2.concat(String(addr[i]));
            
                      } 
                      
                      
                   if (card2 == right_2) {is_card2 = true;
                
               }  
                  else {card2 = "";
               
             }
                  
         
                        if (ek3.search(addr) && is_card2) {
                              
                             
                              for(i=0;i<8;i++) {
           
                               
                                   card3.concat(String(addr[i]));
            
                              } 
                              
                  if (card3 == right_3) {is_card3 = true;}
                  else {card3 = "";}           
            
                                  if (ek4.search(addr) && is_card3) {
                                  
                                        for(i=0;i<8;i++) {
           
                                            
                                              card4.concat(String(addr[i]));
                                        } 
                  if (card4 == right_4) {is_card4 = true;}
                  else {card4 = "";}
                  
              
                                              if (ek5.search(addr) && is_card4) {
                                                   
                                                    for(i=0;i<8;i++) {
           
                                                    
                                                          card5.concat(String(addr[i]));
                                                       } 
                                                       
                                                    if (card5 == right_5) {is_card5 = true;}
                                                    else {card5 = "";}
                                                    
                
                
                if (is_card1 && is_card2 && is_card3 && is_card4 && is_card5) {
                  
                          make_task4();
                  
                  
                          
                        }
                
                
              }
             
              
            }
            
            
          }
            
           
          }
      
      
    }
    
    
    
    
    ek1.reset_search();
    ek2.reset_search();
    ek3.reset_search();
    ek4.reset_search();
    ek5.reset_search();
    
  }
  
 ////////////ПТИЦА 
  if (task5) {
      
      unsigned long mil;
      int b1,b2,b3;
      
      delay(30);
      mil = millis();
      b1 = analogRead(PTICA);
     
      if (b1 >= 70) {
          
                make_task5();
           }
          
    
  }
  
 //////БУТЫЛКИ 
  if (task6) {
    
  
    if (digitalRead(BUTYLKI) == 1) {
     
      
      make_task6();
      
      
      
      
    }
    
  }
  ////////////////КНОПКА ШЕСТИГРАННИК
  if (task7) {
    
      
            
            
            ///////проверяем на отпускание кнопки////////////////////
            if (digitalRead(KNOPKA) == 1) {
              
              ////////////если кнопка отжата, то проверяем установлен ли флаг разрешения////////////////
              
              if (!flag_knopka) {ml_knopka = millis(); flag_knopka = true;}   //////если флаг еще не был установлен, то начинаем отсчет времени и ставим флаг разрешения
              
              else if (flag_knopka) {unsigned long kl; kl = millis(); if (kl - ml_knopka >= knopka_time) {  ///если стоит разрешающий флаг, то проверяем, прошла ли секунда
        
                
                if (digitalRead(KNOPKA) == 1) {  ///если прошла секунда и кнопка вновь отжата, то делаем дело
                    make_task7(); 
                }  
                else {flag_knopka = false;} //если через секунду, кнопка не отпущена, то устанавливаем запрет флага, далее будем сначала проверять
              
              }}
              
           
           
            } 
     
    
  }
  
  
 /////////КАБЕЛЬ 
  if (task8) {
    
          if (digitalRead(KABEL) == 0) {
            
            
            delay(100);
            
            if (digitalRead(KABEL) == 0) {
              
            
            
                  make_task8();
            
            
            }
          }
    
    
  }
  
 ///////////КАРТОПРИЕМНИК 
  if (task9) {
    
        if (digitalRead(K_PRIEM) == 1 && state_led == 0 && half_task9) {
            
          delay(100);
          
          if (digitalRead(K_PRIEM) == 1 && state_led == 0 && half_task9) {
          
          
            make_task9();
            
            
              
              
             
            
            
          }
          }
        
        if (state_led == 1) {
          
         blink_led(); 
         
         delay(8000);
         
          task9 = false;
             task10 = true; 
          
        }
        
    
  }
 

/////////////ШЕСТИГРАННИК 
  if (task10) {
    
    if (digitalRead(SHESTIGRANNIK) == 1) {
            
             make_task10();
             
          }
          
    
    
  }
  
  
 /////АВТОПИЛОТ 
  if (task11) {
    
    if (digitalRead(SEVEN_KNOPOK) == 0 && !state_7_knopki) {
            
      
            
            make_task11();
      
            
            
          }
          
      if (digitalRead(SEVEN_KNOPOK) == 1 && state_7_knopki) {
          delay(1500);
          digitalWrite(SV_KNOPKI,LOW);
          
          delay(1500);
          
          task11 = false;
        
      }  
    
    
    
  }
  
  
  
/////ВЫХОД  
  if (task12) {
    
    
    
    if (digitalRead(VYH_IND) == 1) {
            
      
            make_task12();
            
          }
    
    
    
    
  }
  
  


/////////ФОРМИРОВАНИЕ И ОТПРАВКА ПОСЫЛКИ НА КОМП ОПЕРАТОРА

if (state[0]) {send_="#11";}
    else {send_="#10";}
    
    if (state[1]) {send_=send_+"21";}
    else {send_=send_+"20";}
    
    if (state[2]) {send_=send_+"31";}
    else {send_=send_+"30";}
    
    if (state[3]) {send_=send_+"41";}
    else {send_=send_+"40";}
    
    if (state[4]) {send_=send_+"51";}
    else {send_=send_+"50";}
    
    if (state[5]) {send_=send_+"61";}
    else {send_=send_+"60";}
    
    if (connect_ && game) {
      Serial.print("send1: ");
      Serial.println(send_);
        client.print(send_);
        send_="";
    }
    
    delay(200);
    if (state[6]) {send_=send_+"71";}
    else {send_=send_+"70";}
    
    if (state[7]) {send_=send_+"81";}
    else {send_=send_+"80";}
    
    if (state[8]) {send_=send_+"91";}
    else {send_=send_+"90";}
    
    if (state[9]) {send_=send_+"A1";}
    else {send_=send_+"A0";}
    
    if (state[10]) {send_=send_+"B1";}
    else {send_=send_+"B0";}
    
    if (state[11]) {send_=send_+"C1~";game = false;}
    else {send_=send_+"C0~";}
    
    
    if (connect_ && game) {
      Serial.print("send2: ");
      Serial.println(send_);
      client.print(send_);
      send_="";
    }





}

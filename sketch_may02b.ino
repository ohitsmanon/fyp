#include <SoftwareSerial.h>

#include <TinyGPS.h>


#define Relay 8
#define Relay2 9
#define Relay3 10
#define Relay4 11

TinyGPS gps;
SoftwareSerial ss(2, 3);
SoftwareSerial GSMx(5, 7);

static void smartdelay(unsigned long ms);
static void print_float(float val, float invalid, int len, int prec);
static void print_int(unsigned long val, unsigned long invalid, int len);
static void print_date(TinyGPS &gps);
static void print_str(const char *str, int len);
//-------------------VARIABLE-------------------------------
float LastLon;
String dataString = "";
String DateString = "";
String TimeString = "";
int Sens1Pin = 0;
float Sens1;
int update=0;
float LastLat;
int dummy=0;
int RQScount;
int countERROR;
int countPHONE;
int countOK;
int commaPosition;
int index = 0;
int SMSx=0;
int ALARM=0;
int LOCK=0;
int GPSReady=0;
float latx;
float lonx;
int OUTRANGE=0;
int CheckCall=0;
String result[10];
//String USER1 = "\"0178730054\"";
String USER1 = "\"0178463547\"";
//String USER1 = "\"01123413946\"";
String Data1 = "";
String Data2 = "";
String Data3 = "";
String Data4 = "";
String head = "";
String phone = "";
String pword = "";
String MESSAGE1="";
String MESSAGE2="";
String inputString = "";         // a string to hold incoming data
boolean stringComplete = true;  // whether the string is complete
boolean command = true;
boolean Enabled = false;
boolean Enabled1 = false;
boolean Enabled2 = false;
boolean Enabled3 = false;
boolean GPS_NoData = false;
float SHOCK;
int count;
float flat, flon;
  unsigned long age, date, time, chars = 0;
  float ExistingSpeed;
float SpeedGPS;

int CheckVib=0;
int VibStat=0;
int CheckV=0;
int ChangeStat=0;
float locklat;
float locklon;


//----------------------------------------------------------

void setup()
{
 pinMode(Relay,OUTPUT);
 digitalWrite(Relay,HIGH);
  pinMode(Relay2,OUTPUT);
 digitalWrite(Relay2,HIGH);
  pinMode(Relay3,OUTPUT);
 digitalWrite(Relay3,HIGH);
  pinMode(Relay4,OUTPUT);
 digitalWrite(Relay4,HIGH);
 
  Serial.begin(9600);
  GSMx.begin(9600);
  inputString.reserve(150);
 delay(8000);
  
  Serial.println("AT");
  GSMx.println("AT");
  delay(2000);
  Serial.println("AT+CMGF=1");
  GSMx.println("AT+CMGF=1");
  delay(2000);
  Serial.println("AT+CMGD=1");
  GSMx.println("AT+CMGD=1");
  delay(2000);
  Serial.println("AT&W");
  GSMx.println("AT&W");
  delay(2000);
  
  Serial.print("AT+CMGS=");
  delay(300);
  Serial.println(USER1);
  GSMx.print("AT+CMGS=");
  delay(300);
  GSMx.println(USER1);
  delay(1000);
  Serial.print("GPS-GSM TRACKER---ACTIVATED ");
  GSMx.print("Car Tracker`1 q21QA Controller is now ready...");
  delay(200);
  Serial.println(char(26));
  GSMx.println(char(26));
  delay(4000);
 
  //Serial.println("Sats HDOP Latitude  Longitude  Fix  Date       Time     Date Alt    Course Speed Card  Distance Course Card  Chars Sentences Checksum");
  //Serial.println("          (deg)     (deg)      Age                      Age  (m)    --- from GPS ----  ---- to London  ----  RX    RX        Fail");
  //Serial.println("-------------------------------------------------------------------------------------------------------------------------------------");
  
  ss.begin(9600);
  //--------------------------------------

  
  stringComplete = true;
}

void loop()
{

   unsigned short sentences = 0, failed = 0;
  static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;
 





  
  if (ALARM==1){
    delay(100);
  
    if (SMSx==0){
     //  Serial.print("AT+CMGS=");
                digitalWrite(Relay,HIGH);   
                GSMx.print("AT+CMGS=");
                Serial.println(USER1);
                GSMx.println(USER1);
                delay(1000);
                Serial.print("ALARM ALERT!!");
                GSMx.println("SECURITY ALERT!!");
                Serial.print("at lat:");
                GSMx.print("at lat:");
                Serial.print(flat,5);
                GSMx.print(flat,5);
                Serial.print(", lon:");
                GSMx.print(", lon:");
                Serial.println(flon,5);
                GSMx.println(flon,5);
                Serial.print("www.google.co.in/maps/place/");
                Serial.print(flat,5);
                Serial.print(",");
                Serial.println(flon,5);
                GSMx.println(" SPEED:");
                GSMx.println(SpeedGPS);
                GSMx.print("www.google.co.in/maps/place/");
                GSMx.print(flat,5);
                GSMx.print(",");
                GSMx.println(flon,5);
               
              
                delay(200);
               Serial.println(char(26));
                GSMx.println(char(26));
                delay(5000);
                CheckCall=0;
                
      SMSx=1;
      ALARM=0;
    }
  }
  
  
  
  print_int(gps.satellites(), TinyGPS::GPS_INVALID_SATELLITES, 5);
  print_int(gps.hdop(), TinyGPS::GPS_INVALID_HDOP, 5);
  gps.f_get_position(&flat, &flon, &age);
  print_float(flat, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6);
  print_float(flon, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6);
  print_int(age, TinyGPS::GPS_INVALID_AGE, 5);
  print_date(gps);
  print_float(gps.f_altitude(), TinyGPS::GPS_INVALID_F_ALTITUDE, 7, 2);
  print_float(gps.f_course(), TinyGPS::GPS_INVALID_F_ANGLE, 7, 2);
  print_float(gps.f_speed_kmph(), TinyGPS::GPS_INVALID_F_SPEED, 6, 2);
  print_str(gps.f_course() == TinyGPS::GPS_INVALID_F_ANGLE ? "*** " : TinyGPS::cardinal(gps.f_course()), 6);
  print_int(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0xFFFFFFFF : (unsigned long)TinyGPS::distance_between(flat, flon, LONDON_LAT, LONDON_LON) / 1000, 0xFFFFFFFF, 9);
  print_float(flat == TinyGPS::GPS_INVALID_F_ANGLE ? TinyGPS::GPS_INVALID_F_ANGLE : TinyGPS::course_to(flat, flon, LONDON_LAT, LONDON_LON), TinyGPS::GPS_INVALID_F_ANGLE, 7, 2);
  print_str(flat == TinyGPS::GPS_INVALID_F_ANGLE ? "*** " : TinyGPS::cardinal(TinyGPS::course_to(flat, flon, LONDON_LAT, LONDON_LON)), 6);

  gps.stats(&chars, &sentences, &failed);
  print_int(chars, 0xFFFFFFFF, 6);
  print_int(sentences, 0xFFFFFFFF, 10);
  print_int(failed, 0xFFFFFFFF, 9);
  //Serial.println();
  
   if (flat < 0.1){
    GPS_NoData = true;
  }
  if (flat > 0.1){
    GPS_NoData = false;
  }

  if (flat<1000){
    GPSReady=1;
  }

  
  if (flat != LastLat){
               if (GPS_NoData == false){
                 LastLat = flat;
               }
             }
             if (flon != LastLon){
               if (GPS_NoData == false){
                 LastLon = flon;
               }
             }
  
  if (LOCK==1){
    if (flat >locklat + 0.00001 || flat < locklat - 0.00001){
      ALARM=1;
    }
    if (flon >locklon + 0.00001 || flon < locklon - 0.00001){
      ALARM=1;
    }
  }
  //---------------------------------GSM FUNCTION-----------------------------------------
if (GPSReady==1){
                
if (stringComplete == true){
  
  Serial.println("AT+CMGR=1");
   GSMx.println("AT+CMGR=1");
    
  count = 0;
  countERROR = 0;
  countPHONE = 0;
  countOK = 0;
  stringComplete = false;
  Enabled = false;
  Enabled1 = false;
  Enabled2 = false;
  Enabled3 = false;
}

  while (stringComplete == false){
 // count = count + 1;
       while (Serial.available() > 0){
        
        char inChar = Serial.read(); 
        inputString += inChar;


        /*
        if (inChar == 'R'){
          CheckCall=1;
         }
          if (inChar == 'I'){
            if (CheckCall==1){
              CheckCall=2;
            }
         }
         if (inChar == 'N'){
            if (CheckCall==2){
              CheckCall=3;
            }
         }
         if (inChar == 'G'){
            if (CheckCall==3){
              CheckCall=4;  ALARM=1;
            }
         }
         */
         
        if (Enabled2){
          Data2 += inChar;
          countPHONE = countPHONE + 1;
            if (countPHONE == 10){
            Enabled2 = false;
            }
        }
       
        
        if (inChar == 'E'){
          Enabled1 = true;
         }
         if (Enabled1){
          Data1 += inChar;
          countERROR = countERROR + 1;
                if (countERROR == 5){
                 Enabled1 = false;
                      if (Data1 == "ERROR"){
                        stringComplete = true;
                      }
                      if (Data1 != "ERROR"){
                        countERROR = 0;
                        Data1 += "";
                        Enabled1 = false;
                      }
                }
          
        }
        
        if (inChar == 'O'){
          Enabled3 = true;
         }
         if (Enabled3){
          Data4 += inChar;
          countOK = countOK + 1;
               if (countOK == 2){
                if (Data4 == "OK"){
                  stringComplete = true;
                }
                if (Data4 != "OK"){
                  countOK = 0;
                  Data4 = "";
                  Enabled3 = false;
                }
               } 
         }
        if (inChar == '+'){
          if (count > 5){
            Enabled2 = true;
          }
        }
        if (count > 80){
            stringComplete = true;
               
         }
         
         
    
       }
}


       if (stringComplete){
         /*
         
                */
             Serial.println(inputString);
            index = 0; 
    do
  {
      commaPosition = inputString.indexOf('*');
      if(commaPosition != -1)
      {
          result[index++] = (inputString.substring(0,commaPosition));
          inputString = inputString.substring(commaPosition+1, inputString.length());
      }
    //  else
    //  {  // here after the last comma is found
    //     if(inputString.length() > 0)
    //       Serial.println(inputString);  // SMS message after final splitting
                             // print it
     // }
   }
   while(commaPosition >=0);
     MESSAGE1 = result[2];
     MESSAGE2 = result[3];
 
       
                
       
       
         if (result[1] == "TRACK") {
                Serial.print("AT+CMGS=");
                GSMx.print("AT+CMGS=");
                Serial.println(USER1);
                GSMx.println(USER1);
                delay(1000);
                Serial.print("TRACKING result, lat:");
                GSMx.print("TRACKING result, lat:");
                Serial.print(flat,5);
                GSMx.print(flat,5);
                Serial.print(", lon:");
                GSMx.print(", lon:");
                Serial.println(flon,5);
                GSMx.println(flon,5);
                Serial.print("www.google.co.in/maps/place/");
                Serial.print(flat,5);
                Serial.print(",");
                Serial.println(flon,5);
                GSMx.println(" SPEED:");
                GSMx.println(SpeedGPS);
                GSMx.print("www.google.co.in/maps/place/");
                GSMx.print(flat,5);
                GSMx.print(",");
                GSMx.println(flon,5);
               
              
                delay(200);
                Serial.println(char(26));
                GSMx.println(char(26));
                delay(4000);
                                Serial.println("AT+CMGD=1");
                GSMx.println("AT+CMGD=1");
                
                delay(2000);
                                Serial.println("AT+CMGD=1");
                GSMx.println("AT+CMGD=1");
                
                delay(2000);
             
            }
                  
                             
           
                     if (result[1] == "FINDME") {
                      
                      digitalWrite(Relay3,LOW);   
                Serial.print("AT+CMGS=");
                GSMx.print("AT+CMGS=");
                Serial.println(USER1);
                GSMx.println(USER1);
                delay(1000);
                Serial.print("I'm here atlat:");
                GSMx.print("I'm here at lat:");
                Serial.print(flat,5);
                GSMx.print(flat,5);
                Serial.print(", lon:");
                GSMx.print(", lon:");
                Serial.println(flon,5);
                GSMx.println(flon,5);
                Serial.print("www.google.co.in/maps/place/");
                Serial.print(flat,5);
                Serial.print(",");
                Serial.println(flon,5);
                GSMx.println(" SPEED:");
                GSMx.println(SpeedGPS);
                GSMx.print("www.google.co.in/maps/place/");
                GSMx.print(flat,5);
                GSMx.print(",");
                GSMx.println(flon,5);
               
              
                delay(200);
                Serial.println(char(26));
                GSMx.println(char(26));
                delay(4000);
                                Serial.println("AT+CMGD=1");
                GSMx.println("AT+CMGD=1");
                
                delay(2000);
                                Serial.println("AT+CMGD=1");
                GSMx.println("AT+CMGD=1");
                
                delay(2000);
             
            }

             if (result[1] == "RST") {
              digitalWrite(Relay3,HIGH);   
                      ALARM=0;  SMSx=0;  LOCK=0;
                 
                Serial.print("AT+CMGS=");
                GSMx.print("AT+CMGS=");
                Serial.println(USER1);
                GSMx.println(USER1);
                delay(1000);
                Serial.print("ALarm reset..");
                GSMx.print("Alarm reset..");
               
              
                delay(200);
                Serial.println(char(26));
                GSMx.println(char(26));
                delay(4000);
                                Serial.println("AT+CMGD=1");
                GSMx.println("AT+CMGD=1");
                
                delay(2000);
                                Serial.println("AT+CMGD=1");
                GSMx.println("AT+CMGD=1");
                
                delay(2000);
             
            }

             if (result[1] == "LOCK") {
              LOCK=1;
              locklat=flat;
              locklon=flon;
                      ALARM=0;  SMSx=0;
                       
                Serial.print("AT+CMGS=");
                GSMx.print("AT+CMGS=");
                Serial.println(USER1);
                GSMx.println(USER1);
                delay(1000);
                Serial.print("Car locked!");
                GSMx.print("Car locked!");
               
              
                delay(200);
                Serial.println(char(26));
                GSMx.println(char(26));
                delay(4000);
                                Serial.println("AT+CMGD=1");
                GSMx.println("AT+CMGD=1");
                
                delay(2000);
                                Serial.println("AT+CMGD=1");
                GSMx.println("AT+CMGD=1");

                digitalWrite(Relay2,LOW);  
                digitalWrite(Relay4,HIGH);  
                delay(2000);
                digitalWrite(Relay2,HIGH);  
                digitalWrite(Relay4,HIGH);  
             
            }

             if (result[1] == "ULOCK") {
              LOCK=1;
              locklat=flat;
              locklon=flon;
                      ALARM=0;  SMSx=0;
                      digitalWrite(Relay2,HIGH);   
                Serial.print("AT+CMGS=");
                GSMx.print("AT+CMGS=");
                Serial.println(USER1);
                GSMx.println(USER1);
                delay(1000);
                Serial.print("Car Unlocked!");
                GSMx.print("Car Unlocked!");
               
              
                delay(200);
                Serial.println(char(26));
                GSMx.println(char(26));
                delay(4000);
                               Serial.println("AT+CMGD=1");
                GSMx.println("AT+CMGD=1");
                
                delay(2000);
                                Serial.println("AT+CMGD=1");
                GSMx.println("AT+CMGD=1");
                
                digitalWrite(Relay4,LOW);  
                digitalWrite(Relay2,HIGH);  
                delay(2000);
                digitalWrite(Relay2,HIGH);  
                digitalWrite(Relay4,HIGH);  
             
            }

            
             if (result[1] == "EGON") {
              LOCK=1;
              locklat=flat;
              locklon=flon;
                      ALARM=0;  SMSx=0;
                      digitalWrite(Relay,LOW);   
                Serial.print("AT+CMGS=");
                GSMx.print("AT+CMGS=");
                Serial.println(USER1);
                GSMx.println(USER1);
                delay(1000);
                Serial.print("Engine On");
                GSMx.print("Engine On");
               
              
                delay(200);
                Serial.println(char(26));
                GSMx.println(char(26));
                delay(4000);
                Serial.println("AT+CMGD=1");
                GSMx.println("AT+CMGD=1");
                
                delay(2000);
                                Serial.println("AT+CMGD=1");
                GSMx.println("AT+CMGD=1");
                
                delay(2000);
             
            }
             if (result[1] == "EGOFF") {
              LOCK=1;
              locklat=flat;
              locklon=flon;
                      ALARM=0;  SMSx=0;
                      digitalWrite(Relay,HIGH);   
                Serial.print("AT+CMGS=");
                GSMx.print("AT+CMGS=");
                Serial.println(USER1);
                GSMx.println(USER1);
                delay(1000);
                Serial.print("Engine Off");
                GSMx.print("Engine Off");
               
              
                delay(200);
                Serial.println(char(26));
                GSMx.println(char(26));
                delay(4000);
                                Serial.println("AT+CMGD=1");
                GSMx.println("AT+CMGD=1");
                
                delay(2000);
                                Serial.println("AT+CMGD=1");
                GSMx.println("AT+CMGD=1");
                
                delay(2000);
             
            }
           
            
       inputString = "";
       result[1] = "";
       result[2] = "";
       result[3] = "";
       result[4] = "";
       result[5] = "";
       result[6] = "";
       Data1 = "";
       Data2 = "";
       Data3 = "";
       Data4 = "";
       CheckCall=0;
      // Serial.flush();
       
      // stringComplete = true;
delay(2000);

       }
}
  
  
 // delay(100);
  smartdelay(300);
}
//--------------------------------------START OF SUBROUTINES-------------------------------------
static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

static void print_float(float val, float invalid, int len, int prec)
{
  if (val == invalid)
  {
    while (len-- > 1)
    //  Serial.print('*');
    //Serial.print(' ');
    dummy=0;
  }
  else
  {
   // Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
     // Serial.print(' ');
     dummy=0;
  }
  smartdelay(0);
}

static void print_int(unsigned long val, unsigned long invalid, int len)
{
  char sz[32];
  if (val == invalid)
    strcpy(sz, "*******");
  else
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
 // Serial.print(sz);
  smartdelay(0);
}

static void print_date(TinyGPS &gps)
{
  int year;
  byte month, day, hour, minute, second, hundredths;
  float Speed;
  unsigned long age;
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  if (age == TinyGPS::GPS_INVALID_AGE)
   // Serial.print("********** ******** ");
   dummy = 1;  
    
  else
  {
    
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d %02d:%02d:%02d ",
        month, day, year, hour, minute, second);
   // Serial.print(sz);
    
    
    
  //  hour = hour+8;
  //  sprintf(lcd_buffer,"%02d/%02d/%02d %02d:%02d",day,month,year,hour,minute );
    
    
    hour = hour + 8;
    if (hour > 23){
      hour = hour - 24;
    }
    dataString="";
    TimeString="";
    DateString="";
    ExistingSpeed = gps.f_speed_kmph();
    Speed = gps.f_speed_kmph();
    SpeedGPS = Speed;
   

    
   TimeString = String(hour) + ":" + String(minute) + ":" + String(second);
   DateString = String(day) + "/" + String(month) + "/" + String(year);
   dataString = String(flat,5) + "  " + String(flon,5) + "  " + String(Speed,2);
   
   
  
    
    
   
  
  }
  print_int(age, TinyGPS::GPS_INVALID_AGE, 5);
  smartdelay(0);
  

   
    
    
  
    }


static void print_str(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
  //  Serial.print(i<slen ? str[i] : ' ');
  smartdelay(0);
}

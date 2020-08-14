//-Includes
#include <LiquidCrystal.h>

//-Constances
const int rs = 6,en = 8, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
const int t = 545;    //sensor value if not wet
const int n = 272;    //sensor value if wet
const int maxw = t-n; 
const int maxsamples = 50;  //set how many samples you want to get
bool flag;
const char ok[16] = "ALL OKAY";
const char nok[30] = "NEED WATER";

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//-Structures
struct PLANT
{
  int minv; //minimum tolerance
  int maxv; //maximum tolerance
};


//-----------------------------------------------------------------------------------------
void setup() 
{
  //-Initialisations
  Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
  
  //set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.clear();
}


//-----------------------------------------------------------------------------------------
//main function
//gets analog values of moisture sensor and shows them on the terminal and LCD
//calculate avaerage values over time and also check if its in tolerance of selected plant
void loop() 
{
  //-Definitions
  
  int soilMoistureValue = 0, avrg= 0, avrgv = 0;
  PLANT bambus = {75,85};
  
  //get moisture sensor data
  soilMoistureValue = analogRead(A0);  

  //-Calculation
  for(int i = 0; i<maxsamples; i++)
  {
     //calculate the value of the data in percent
     int value = 100-(((soilMoistureValue-n)*100)/maxw);
     avrg+=value;
     delay(5);
  }
  //calculate the avarage humidity
  avrgv = avrg / maxsamples; 
  if(avrgv < 0)
    avrgv=0;

  //check the value
  flag = check_value(bambus, avrgv);
  
  put_lcd(flag, avrgv);
}


//-Functions
//-----------------------------------------------------------------------------------------
//check if selected plant does need water 
//*PLANT plant* : selected plant 
//                plant.minv = minimum humidity tolerance
//                plant.maxv = maximum humidity tolerance
//*avrgv*:  average sensor data calculated in the main loop()
bool check_value(PLANT plant, int avrgv)
{
   //-Check value
  if ((avrgv <= plant.minv)|| (avrgv >= plant.maxv))
  {
    flag = false;
  }
  else
  {
    flag = true;
  }
  return flag;
  
}


//-----------------------------------------------------------------------------------------
//puts out the sensor data
//param:
//*flag* :  true if selected plant needs water
//          false if selceted plant is okay
//*avrgv*:  average sensor data calculated in the main loop()
void put_lcd(bool flag, int avrgv)
{
  char buf[100];
  
  lcd.clear();
  //-Serial Monitor
  //writing humidity percentage in buffer
  sprintf(buf, "Humidity: %d%%", avrgv);
  //print out the sensor data
  Serial.println(buf);

  //-LCD
  //Print out the sensor data
  lcd.setCursor(0, 0);
  lcd.print(buf);
  lcd.setCursor(0, 1);
  if(flag == true)
    lcd.print(ok);
  else
    lcd.print(nok);
}

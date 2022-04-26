// Sıcaklık ve Nem Sensör Kullanımı (DHT11(Sıcaklık-Nem Sensörü)+ LCD5110+DS3231(Saat Sensörü))

#include <LCD5110_Basic.h>
LCD5110 myGLCD(8,9,10,11,12);// Sırasıyla clk, din, dc, rst, ce pinleri arduinoya belirtiyoruz. 
extern uint8_t SmallFont[]; // Yazı Fontlarımızı arduino'ya tanıtıyoruz.
extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];

int BL = 4;//LCD5110 Ekranının arkaplan aydınlatması için 4. pini BackLamp(BL) isminde bir değişkene atadık.

#include <DHT.h> // DHT Kütüphanemizi ekliyoruz.  
#define DHTPIN 2 // Tanımlama kısmında, DHT-11 sensörümüzü kaçıncı pine bağladığımızı DHTPIN X şeklinde arduinoya aktarıyoruz.
#define DHTTYPE DHT11 // DHT kütüphandesinden bizim kullandığımız DHT sensörünü çağrıyoruz. 
DHT dht(DHTPIN,DHTTYPE); // Burada define ettiğimiz verileri tek çatı altında topluyoruz. Yani, dht adlı bir nesne oluşturuyoruz. 
// dht adlı nesnemize hangi pine bağlı olduğu ve hangi tipte bir yapıya sahip olduğunu aktarıyoruz.

#include <DS3231.h> //Saat modül kütüphanesini ekliyoruz. 
DS3231  myClock(SDA, SCL); // Aynı şekilde burada nesne oluşturuyoruz, (Signal-Data-Signal-Clock)--> Aynı şekilde nesnemizin sahip olacağı 
//karakteri tanımlıyoruz.

void setup(){
  pinMode(A0, INPUT);
  pinMode(BL,OUTPUT); // Arka plan ışığını çıkış pini olarak atıyoruz. 
  Serial.begin(9600);
  
  myGLCD.InitLCD(); //LCD ekranı aktif hale getiriyoruz. 
  myGLCD.setContrast(65); // LCD ekranın contrastını ayarlıyoruz.
  dht.begin(); //dht nesnemizi başlatıyotuz. (Nem-Sıcaklık Sensörü)
  myClock.begin(); //myClock nesnemizi başlatıyotuz. (Saat Sensörü)

//Saat Ayar Bölümü
  myClock.setDOW(SUNDAY);  
  myClock.setTime(13, 05, 00); //Sırasıyla(Saat, Dakika, Saniye)
  myClock.setDate(16, 2, 2020); //Sırasıyla (Gün, Ay, Yıl)
}

void loop(){
  int Isik= analogRead(A0);
  Isik = map(Isik,0,1023,0,100);
  Serial.println(Isik);
  delay(100);
  
  if(Isik >= 25)
  {
    analogWrite(BL,0);  
  }
  else
  {
    analogWrite(BL,205);
  }
  float temp = dht.readTemperature(); //temp adlı float(Ondalıklı ifadeler için kullanılan değişken oluşturma yapısıdır. Yani bize tam sayı değilde kesirli ifade verir.2,3 - 5,4 gibi) yapısnda 
  //bir değişken oluşturuyoruz. Ve bu değişken DHT sensörünün Sıcaklık datasını(verilerini) kaydetmesini sisteme yazdırıyoruz. 
  float hum = dht.readHumidity(); // Aynı şekilde burada Nem için aynı işlem yapılıyor. 

 

// Verilerin Ekrana Yazdırma Ayarları
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myGLCD.print("Sicaklik; ",CENTER,1);
  myGLCD.printNumF(temp,1,CENTER,14);
  
  myGLCD.print("NEM; ",CENTER,29);
  myGLCD.printNumI(hum,CENTER,35);
  myGLCD.setFont(SmallFont);
  myGLCD.print("%",26,35);
  delay(5000);

  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myGLCD.print(myClock.getDOWStr(),CENTER,17);
  myGLCD.print(" ",LEFT+60,1);
  if(Isik >= 25)
  {
    myGLCD.print("GUNDUZ",CENTER,28); 
  }
  else
  {
    myGLCD.print("GECE",CENTER,28);
  }
  delay(2000);
  
  myGLCD.clrScr();
  myGLCD.print(myClock.getDateStr(),LEFT,10);
  myGLCD.print(myClock.getTimeStr(),LEFT,20);
  delay(1000);
  
  delay(5000);
  
  myGLCD.enableSleep();
  delay(2000);
  myGLCD.disableSleep();
}

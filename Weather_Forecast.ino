// Sıcaklık ve Nem Sensör Kullanımı (DHT11+ LCD5110)

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
  pinMode(BL,OUTPUT); // Arka plan ışığını çıkış pini olarak atıyoruz. 
  myGLCD.InitLCD(); //LCD ekranı aktif hale getiriyoruz. 
  myGLCD.setContrast(65); // LCD ekranın contrastını ayarlıyoruz.
  dht.begin(); //dht nesnemizi başlatıyotuz. (Nem-Sıcaklık Sensörü)
  myClock.begin(); //myClock nesnemizi başlatıyotuz. (Saat Sensörü)

//Saat Ayar Bölümü
  //myClock.setDOW(SATURDAY);  
  //myClock.setTime(00, 45, 30); Sırasıyla(Saat, Dakika, Saniye)
  //myClock.setDate(8, 2, 2020); Sırasıyla (Gün, Ay, Yıl)
}

void loop(){
  analogWrite(BL,175); //Arka plan aydınlatmasına 255/175 Güç veriyoruz. Yaklaşık 3,4 veriyoruz. Bu isteğe bağlı 0'dan 255'e kadar yazılabilir.
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
  myGLCD.print(myClock.getDOWStr(),CENTER,22);
  myGLCD.print(" ",LEFT+60,1);
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

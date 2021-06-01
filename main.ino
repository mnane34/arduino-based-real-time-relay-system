
#include <LiquidCrystal.h>  // LCD Kütüphanesi Dahil Edildi

#include <DS3231.h>  // RTC Kütüphanesi Dahil Edildi

#define SDA A2  // RTC Pin
#define SCL A3

DS3231  rtc(SDA, SCL);  // RTC Nesnesi

Time t; // Gerçek Zaman Değerine Ulaşmak İçin Gerekli bir nesne tanımlaması

#define RS 13 // LCD Pin
#define E 12
#define D4 11
#define D5 10
#define D6 9
#define D7 8

LiquidCrystal lcd (13,12,11,10,9,8); // LCD Nesnesi Oluşturuldu

#define Joystick_X A0  // Joystick Pin 
#define Joystick_Y A1
#define Joystick_SW 7

#define BUZZER 6  // Buzzer Pin

#define Role 5 // Role Pin

boolean Joystick_SW_Durum = 1 ;  // Joystick Anahtar Durumu

boolean BaslangicSet = 0; // MOD-A Saat Değişkenleri
boolean BitisSet = 0;

byte OzelKarakter0[]={0x10,0x18,0x1C,0x1E,0x1E,0x1C,0x18,0x10};   // İmlec Karakteri

byte OzelKarakter1[]={0x4,0xE,0x15,0x4,0x4,0x4,0x4,0x4}; // Setİmlec Yukarı

byte OzelKarakter2[]={0x4,0x4,0x4,0x4,0x4,0x15,0xE,0x4}; // Setİmlec Aşşağı

byte OzelKarakter3[]={0x0,0x1,0x3,0x16,0x1C,0x8,0x0,0x0}; // Tik İşareti

int Joystick_X_Durum=0;  // Joystick Eksen Durum Bilgilerinin Saklandığı Değişkenler
int Joystick_Y_Durum=0;

byte imlecDurum=1;   // OzelKarakter1 Özel Karakterinin İmlec Değişkeni

byte EkranDurum=1; // Modlar Arası Geçişte Kullanılan Değişken

byte SonDurum=0;  // MOD-A imlec değişkeni 

byte imlecKonum=0; // EkranDurum1 İmlec Değişkeni


byte CursorA=1; // Cursor Ayarlamak İçin Kullanılan Değişken

int BaslangicSaatOnlar=0;   // MOD A Saat Değişkenleri
int BaslangicSaatBirler=0;

int BaslangicDakikaOnlar=0;
int BaslangicDakikaBirler=0;

int BitisSaatOnlar=0;
int BitisSaatBirler=0;

int BitisDakikaOnlar=0;
int BitisDakikaBirler=0;


int GercekSaatOnlar=0;     // Gercek Zaman Değerleri
int GercekSaatBirler=0;

int GercekDakikaOnlar=0;
int GercekDakikaBirler=0;

boolean CikisDurum=false; // Çıkış Durum Değikeni

int CikisAktifSuresiOnlar=0;   // Mod B Değişkenleri

int CikisAktifSuresiBirler=0;

String SureType = "Saniye";

int SureTypeDeger=1;

byte CursorB=2;

boolean CikisAktifSet=false;

unsigned long int Bekleme=0;

int ButonDeger=0;


void setup() 
{
  Serial.begin(9600); // Seri Haberleşme Hızı 9600 Bps
  
  lcd.begin(16,2); // LCD Başlatıldı

  rtc.begin(); // RTC Başlatıldı

  /* Yapılmak İstenen RTC Ayarlarının Kurulduğu Fonksiyonlar */

  //rtc.setDOW(SALI);            //  BU günü pazartesi olarak ayarladık
  //rtc.setTime(14, 1, 0);       //  saati 12:00:00 şeklinde ayarladık
  //rtc.setDate(21, 04, 2020);   // Tarihi 1/1/2014 olarak ayarladık

  
  pinMode(BUZZER,OUTPUT);  // BUZZER Çıkış ve Pasif
  digitalWrite(BUZZER,LOW);

  pinMode(Role,OUTPUT);  // Röle Çıkış ve Aktif
  digitalWrite(Role,HIGH);

  lcd.createChar(1,OzelKarakter0);  // Özel Karakter Tanımlamaları
  lcd.createChar(2,OzelKarakter1);
  lcd.createChar(3,OzelKarakter2);
  lcd.createChar(4,OzelKarakter3);

  lcd.clear();      // LCD Başlangıç Ekranında kalmasını istediğim Fontlar
  lcd.setCursor(0,0);
  lcd.write(1); 
  
} // Setup Fonksiyonun Bitiş Bloğu

void loop() 
{

while(EkranDurum == 1)     // MOD Seçme Ekranı
{
  
Joystick_X_Durum =  analogRead(Joystick_X);  // İmlec Yukarı - Aşşağı Değer Ataması
Joystick_Y_Durum =  analogRead(Joystick_Y);

Joystick_SW_Durum = digitalRead(Joystick_SW);  // Joystick Butonu Değer Ataması

lcd.clear();
lcd.setCursor(5,0);     // Seçilecek Mod Seçenekleri
lcd.print("Mod A");
lcd.setCursor(5,1);
lcd.print("Mod B");
lcd.setCursor(0,imlecKonum);
lcd.write(1);
delay(200); 


  if (Joystick_X_Durum>=0 && Joystick_Y_Durum == 0)    // İmlec Yukarda
{

imlecDurum=1;
imlecKonum=0;
}

else if(Joystick_X_Durum>=0 && Joystick_Y_Durum>900)    // İmlec Aşşağıda
{
imlecDurum=2;
imlecKonum=1;
}

 if(Joystick_SW_Durum == false)     // Mod Seçildiği Koşul
  {
    while(Joystick_SW_Durum == false)
    Joystick_SW_Durum = digitalRead(Joystick_SW);
    Buzzer();  
    if(imlecDurum==1)       // MOD A Seçeneği Seçilir
    {
      EkranDurum=2;
    }
    else if (imlecDurum==2)   // MOD B Seçeneği  Seçilir
    {
     EkranDurum=3; 
    }
  }

} // Ekran Durum 1 Bitiş Bloğu (Mod Seçme Ekranı)

while(EkranDurum == 2)     // MOD 2 Ekranı
{
 
Joystick_X_Durum =  analogRead(Joystick_X);  // İmlec Yukarı - Aşşağı Değer Ataması
Joystick_Y_Durum =  analogRead(Joystick_Y);

Joystick_SW_Durum = digitalRead(Joystick_SW);  // Joystick Butonu Değer Ataması

Serial.print("X:");
Serial.print(Joystick_X_Durum);
Serial.print("Y");
Serial.println(Joystick_Y_Durum);

                                // Sürekli Ekranda Kalmasını İstediklerim

 lcd.clear();                                     
 lcd.setCursor(0,SonDurum); //SonDurum Değişkenine göre İmlec Aşşağı Yada Yukarda Gözükür
 lcd.write(1);
 lcd.setCursor(1,0);
 lcd.print(BaslangicSaatOnlar);
 lcd.setCursor(2,0);
 lcd.print(BaslangicSaatBirler);
 lcd.setCursor(3,0);
 lcd.print(":");
 lcd.setCursor(4,0);
 lcd.print(BaslangicDakikaOnlar);
 lcd.setCursor(5,0);
 lcd.print(BaslangicDakikaBirler);
 lcd.setCursor(8,0);
 lcd.print(rtc.getTimeStr());

 lcd.setCursor(1,1);
 lcd.print(BitisSaatOnlar);
 lcd.setCursor(2,1);
 lcd.print(BitisSaatBirler);
 lcd.setCursor(3,1);
 lcd.print(":");
 lcd.setCursor(4,1);
 lcd.print(BitisDakikaOnlar);
 lcd.setCursor(5,1);
 lcd.print(BitisDakikaBirler);
 delay(200);



 if (Joystick_X_Durum>=0 && Joystick_Y_Durum == 0)      // Joystick Yukarı İmlec Değişkeni Atanır
{
SonDurum=0;
}

else if(Joystick_X_Durum>=0 && Joystick_Y_Durum>900)    // Joystick Aşşağı İmlec Değişkeni Atanır
{
 SonDurum=1;
}

           // Saat Ayarı Yapılması İçin olan koşul
 
if(Joystick_SW_Durum == false)      
  {
    while(Joystick_SW_Durum == false)
    {
      ButonDeger++;
      delay(100);
      Joystick_SW_Durum = digitalRead(Joystick_SW);
    }

    if(ButonDeger>10)
    {
      lcd.clear();
      EkranDurum=1;
      ButonDeger=0;
      break;
    }
    
   else 
   {
    Buzzer();
   
    if(SonDurum==0)
    {
        BaslangicSet=1;   // Başlangıç Değer Seçilir
    }

    if(SonDurum==1)
    {
      BitisSet=1;        // Bitiş Değer Seçilir
    }
   }
  }
 
   while (BaslangicSet == 1 )    // Başlangıç Değer İçin Yapılan Ayarlamalar
   {
    
    Joystick_X_Durum =  analogRead(Joystick_X);  // İmlec Yukarı - Aşşağı Değer Ataması
    Joystick_Y_Durum =  analogRead(Joystick_Y);

    Joystick_SW_Durum = digitalRead(Joystick_SW);  // Joystick Butonu Değer Ataması

Serial.print("X:");
Serial.print(Joystick_X_Durum);
Serial.print("Y");
Serial.println(Joystick_Y_Durum);
    
 lcd.clear();
 lcd.setCursor(1,0);
 lcd.print(BaslangicSaatOnlar);
 lcd.setCursor(2,0);
 lcd.print(BaslangicSaatBirler);
 lcd.setCursor(3,0);
 lcd.print(":");
 lcd.setCursor(4,0);
 lcd.print(BaslangicDakikaOnlar);
 lcd.setCursor(5,0);
 lcd.print(BaslangicDakikaBirler);
 lcd.setCursor(8,0);
 lcd.print(rtc.getTimeStr());

 lcd.setCursor(CursorA,1);  // Cursor Değerine Göre OzelKarakter2 Fontu Gözükür
 lcd.write(2);
 delay(200);

    if (Joystick_X_Durum==0 && 500<Joystick_Y_Durum && Joystick_Y_Durum<1000)   // Joystick Sağ
    {
    CursorA++;

    if(CursorA==3)
    {
      CursorA=4;
    }
    if(CursorA==6)
    {
      CursorA=1;
    }
    
    
    }

        if (Joystick_X_Durum>1000 && Joystick_Y_Durum>500 && Joystick_Y_Durum<1000)    // Joystick Sol
    {
   CursorA--;
   if(CursorA==0)
   {
    CursorA=5;
   }

   else if(CursorA==3)
   {
    CursorA=2;
   }
   
    }


if (Joystick_X_Durum>=0 && Joystick_Y_Durum == 0)  // Joystick Yukarı
{

if(CursorA == 1)
{
  BaslangicSaatOnlar++;

    if(BaslangicSaatBirler>3 && BaslangicSaatOnlar>1)
  {
    BaslangicSaatOnlar=0;
    BaslangicSaatBirler=0;
  }
  
  if(BaslangicSaatOnlar==3)
  {
    BaslangicSaatOnlar=0;
  }

}

else if(CursorA == 2)
{
  BaslangicSaatBirler++;
  if(BaslangicSaatBirler==10)
  {
    BaslangicSaatBirler=0;
  }

  if(BaslangicSaatOnlar==2 && BaslangicSaatBirler>=4)
  {
    BaslangicSaatBirler=0;
    BaslangicSaatOnlar=0;
  }
 
}

else if(CursorA == 4)
{
  BaslangicDakikaOnlar++;
  if(BaslangicDakikaOnlar==6)
  {
    BaslangicDakikaOnlar=0;
  }

  if(BaslangicSaatOnlar==2 && BaslangicSaatBirler > 3)
  {
    BaslangicDakikaOnlar=0;
    
  }
 
}

else if(CursorA == 5)
{
  BaslangicDakikaBirler++;
  if(BaslangicDakikaBirler==10)
  {
    BaslangicDakikaBirler=0;
  }

    if(BaslangicSaatOnlar==2 && BaslangicSaatBirler > 3)
  {
    BaslangicDakikaBirler=0;
  }
}

}

else if(Joystick_X_Durum>=0 && Joystick_Y_Durum>1000)  // Joystick Aşşağı
{

if(CursorA==1)
{
 BaslangicSaatOnlar--;

if(BaslangicSaatBirler>3 && BaslangicSaatOnlar<0)
{
  BaslangicSaatOnlar=0;
  BaslangicSaatBirler=0;
}

if(BaslangicSaatOnlar<0)
{
  BaslangicSaatOnlar=2;
}

}

else if(CursorA==2)
{
 BaslangicSaatBirler--;

if( BaslangicSaatOnlar == 2 &&  BaslangicSaatBirler < 0)
{
 BaslangicSaatOnlar=0;
 BaslangicSaatBirler=0;
}


if( BaslangicSaatBirler<0)
{
  BaslangicSaatBirler=9;
}

}

else if(CursorA==4)
{
  
 BaslangicDakikaOnlar--;

if(BaslangicDakikaOnlar<0)
{
  BaslangicDakikaOnlar=5;
} 
}
else if(CursorA==5)
{
  BaslangicDakikaBirler--;

if(BaslangicDakikaBirler<0)
{
  BaslangicDakikaBirler=9;
} 
}

}


if(Joystick_SW_Durum == false)       // Başlangıç Değeri Ayarlamadan Çıkarken Yapılanlar
  {
    while(Joystick_SW_Durum == false)
    Joystick_SW_Durum = digitalRead(Joystick_SW);
    Buzzer();
    BaslangicSet=false; 
  }

  
} // Başlangıç Set Bitiş Bloğu


while(BitisSet==1)   // Bitiş Değeri Ayarlaması Fonksiyonu
{
    Joystick_X_Durum =  analogRead(Joystick_X);  // İmlec Yukarı - Aşşağı Değer Ataması
    Joystick_Y_Durum =  analogRead(Joystick_Y);

    Joystick_SW_Durum = digitalRead(Joystick_SW);  // Joystick Butonu Değer Ataması

    Serial.print("X:");
    Serial.print(Joystick_X_Durum);
    Serial.print("Y");
    Serial.println(Joystick_Y_Durum);

 lcd.clear();
 lcd.setCursor(1,1);
 lcd.print(BitisSaatOnlar);
 lcd.setCursor(2,1);
 lcd.print(BitisSaatBirler);
 lcd.setCursor(3,1);
 lcd.print(":");
 lcd.setCursor(4,1);
 lcd.print(BitisDakikaOnlar);
 lcd.setCursor(5,1);
 lcd.print(BitisDakikaBirler);
 lcd.setCursor(8,0);
 lcd.print(rtc.getTimeStr());

 lcd.setCursor(CursorA,0); // Cursor Değerine Göre OzelKarakter3 Fontu Gözükür
 lcd.write(3);
 delay(200);

 if (Joystick_X_Durum==0 && 500<Joystick_Y_Durum && Joystick_Y_Durum<1000)      // Joystick Sağ
    {
    CursorA++;

    if(CursorA==3)
    {
      CursorA=4;
    }
   else  if(CursorA==6)
    {
      CursorA=1;
    }
    
    }

        if (Joystick_X_Durum>1000 && Joystick_Y_Durum>500 && Joystick_Y_Durum<1000)    // Joystick Sol
    {
   CursorA--;
   if(CursorA==0)
   {
    CursorA=5;
   }

   else if(CursorA==3)
   {
    CursorA=2;
   }
   
    }


if (Joystick_X_Durum>=0 && Joystick_Y_Durum == 0)  // Joystick Yukarı
{

if(CursorA == 1)
{
  BitisSaatOnlar++;

  if(BitisSaatBirler>3 && BitisSaatOnlar>1)
  {
    BitisSaatOnlar=0;
    BitisSaatBirler=0;
  }
  
  if(BitisSaatOnlar==3)
  {
    BitisSaatOnlar=0;
  }

}

else if(CursorA == 2)
{
  BitisSaatBirler++;
  if(BitisSaatBirler==10)
  {
    BitisSaatBirler=0;
  }

  if(BitisSaatOnlar==2 && BitisSaatBirler>=4)
  {
   BitisSaatBirler=0;
   BitisSaatOnlar=0;
  }
 
}

 else if(CursorA == 4)
{
  BitisDakikaOnlar++;
  if(BitisDakikaOnlar==6)
  {
    BitisDakikaOnlar=0;
  }

  if(BitisSaatOnlar==2 && BitisSaatBirler > 3)
  {
    BitisDakikaOnlar=0;
    
  }
  
}

else if(CursorA== 5)
{
  BitisDakikaBirler++;
  if(BitisDakikaBirler==10)
  {
    BitisDakikaBirler=0;
  }

    if(BitisSaatOnlar==2 && BitisSaatBirler > 3)
  {
    BitisDakikaBirler=0;
  }
}

}


else if(Joystick_X_Durum>=0 && Joystick_Y_Durum>1000)  // Joystick Aşşağı
{

if(CursorA==1)
{
BitisSaatOnlar--;

if(BitisSaatBirler>3 && BitisSaatOnlar<0)
{
  BitisSaatOnlar=0;
  BitisSaatBirler=0;
}

if(BitisSaatOnlar<0)
{
 BitisSaatOnlar=2;
} 
}

else if(CursorA==2)
{
BitisSaatBirler--;

if(BitisSaatOnlar==2 && BitisSaatBirler<0)
{
BitisSaatOnlar=0;
BitisSaatBirler=0;
}

if(BitisSaatBirler<0)
{
  BitisSaatBirler=9;
}

}


else if(CursorA==4)
{
  
 BitisDakikaOnlar--;

if(BitisDakikaOnlar<0)
{
  BitisDakikaOnlar=5;
} 
}
else if(CursorA==5)
{
  BitisDakikaBirler--;

if(BitisDakikaBirler<0)
{
  BitisDakikaBirler=9;
} 
}

}


if(Joystick_SW_Durum == false)       // Bitiş Değeri Ayarlamadan Çıkarken Yapılanlar
  {
    while(Joystick_SW_Durum == false)
   
    Joystick_SW_Durum = digitalRead(Joystick_SW);
    
    Buzzer();
    BitisSet=false;
  }
  
} // Bitiş Set Çıkış Bloğu

/* Sorgulama İşlemleri */


  t = rtc.getTime();    //  getTime Fonksiyonlarını t değişkenine ata            
  
  GercekSaatOnlar = (int)t.hour/10;  // Gerçek Zamanın Değerleri

  GercekSaatBirler = (int)t.hour%10;

  GercekDakikaOnlar = (int)t.min/10;

  GercekDakikaBirler = (int)t.min%10;


  if(GercekSaatOnlar==BaslangicSaatOnlar && GercekSaatBirler==BaslangicSaatBirler && GercekDakikaOnlar==BaslangicDakikaOnlar && GercekDakikaBirler==BaslangicDakikaBirler)
{
  CikisDurum=true;   // Eğer Başlangıç Değeri = Gerçek Değer İse Çıkış Aktif
}

  while(CikisDurum==true)
  {
    
  t = rtc.getTime();                    // Döngüden Çıkabilmek için gerekli input
  
  GercekSaatOnlar = (int)t.hour/10;

  GercekSaatBirler = (int)t.hour%10;

  GercekDakikaOnlar = (int)t.min/10;

  GercekDakikaBirler = (int)t.min%10;

  lcd.setCursor(8,0);            // Saat Saymaya devam etsin
  lcd.print(rtc.getTimeStr());
  delay(200);

  digitalWrite(Role,LOW);    // RÖLE AKTİF

 if(GercekSaatOnlar==BitisSaatOnlar && GercekSaatBirler==BitisSaatBirler && GercekDakikaOnlar==BitisDakikaOnlar && GercekDakikaBirler==BitisDakikaBirler)
  {
        digitalWrite(Role,HIGH);  // Eğer Bitiş Değeri = Gerçek Değer İse Çıkış & Röle Pasif
        CikisDurum=false;
        break;
   }
      
} // Çıkış Durum Bitiş Bloğu

} // Ekran Durum2 Bitiş Bloğu



            while(EkranDurum==3)         // MOD B Ekranı Aktif
{

 Joystick_X_Durum =  analogRead(Joystick_X);  // İmlec Yukarı - Aşşağı Değer Ataması
 Joystick_Y_Durum =  analogRead(Joystick_Y);

 Joystick_SW_Durum = digitalRead(Joystick_SW);  // Joystick Butonu Değer Ataması


if(SureTypeDeger==1)
{
  SureType="Saniye";
}

else if (SureTypeDeger==2)
{
  SureType="Dakika";
}

else if (SureTypeDeger==3)
{
  SureType="Saat";
}


  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print(CikisAktifSuresiOnlar);
  lcd.setCursor(3,0);
  lcd.print(CikisAktifSuresiBirler);
  lcd.setCursor(5,0);
  lcd.print(SureType);


  lcd.setCursor(CursorB,1);
  lcd.write(2);
  delay(200);

  
if (Joystick_X_Durum==0 && 500<Joystick_Y_Durum && Joystick_Y_Durum<1000)      // Joystick Sağ
{
  CursorB++;

  if(CursorB==4)
  {
   CursorB=5; 
  }

  else if(CursorB==6)
  {
   CursorB=2; 
  }
   
}  // Joystick Sağ Bitiş Bloğu


if (Joystick_X_Durum>1000 && Joystick_Y_Durum>500 && Joystick_Y_Durum<1000)    // Joystick Sol

{
  CursorB--;

if(CursorB==1)
{
 CursorB=5; 
}

else if(CursorB==4)
{
  CursorB=3;
}
  
} // Joystick Sol Bitiş Bloğu



if (Joystick_X_Durum>=0 && Joystick_Y_Durum == 0)  // Joystick Yukarı
{

  
if(CursorB==2)
{
  CikisAktifSuresiOnlar++;

if(CikisAktifSuresiOnlar>=2 && CikisAktifSuresiBirler>=4 && SureTypeDeger==3)
  {
    CikisAktifSuresiOnlar=2;
    CikisAktifSuresiBirler=4;
  }


   if(CikisAktifSuresiOnlar>=6 && CikisAktifSuresiBirler>=0 && SureTypeDeger==2)
  {
    CikisAktifSuresiOnlar=6;
    CikisAktifSuresiBirler=0;
  }

   if(CikisAktifSuresiOnlar>=6 && CikisAktifSuresiBirler>=0 && SureTypeDeger==1)
  {
    CikisAktifSuresiOnlar=6;
    CikisAktifSuresiBirler=0;
  }

 if(CikisAktifSuresiOnlar>9)
{
  CikisAktifSuresiOnlar=0;
}

} // Cursor B2 Çıkış Bloğu 

else if(CursorB==3)
{
   CikisAktifSuresiBirler++; 

 if(CikisAktifSuresiBirler>9)
{
  CikisAktifSuresiBirler=0;
}

 if(CikisAktifSuresiOnlar>=6 && CikisAktifSuresiBirler>=0 && SureTypeDeger==2)
  {
    CikisAktifSuresiOnlar=6;
    CikisAktifSuresiBirler=0;
  }

   if(CikisAktifSuresiOnlar>=6 && CikisAktifSuresiBirler>=0 && SureTypeDeger==1)
  {
    CikisAktifSuresiOnlar=6;
    CikisAktifSuresiBirler=0;
  }


 if(CikisAktifSuresiOnlar==2 && CikisAktifSuresiBirler>4 && SureTypeDeger==3)
{
  CikisAktifSuresiOnlar=2;
  CikisAktifSuresiBirler=4;
}

} // CursorB3 Çıkış Bloğu

else if(CursorB==5)
{
  SureTypeDeger++;

  if(SureTypeDeger>3)
  {
    SureTypeDeger=1;
  }



  if(CikisAktifSuresiOnlar>2 && SureTypeDeger==3)
  {
    CikisAktifSuresiOnlar=2;
    CikisAktifSuresiBirler=4; 
  }
 

  if(CikisAktifSuresiOnlar==6 && CikisAktifSuresiBirler>0 && SureTypeDeger==1)
  {
    CikisAktifSuresiOnlar=6;
    CikisAktifSuresiBirler=0;
  }


  if(CikisAktifSuresiOnlar>=6 && CikisAktifSuresiBirler>0 && SureTypeDeger==2)
  {
    CikisAktifSuresiOnlar=6;
    CikisAktifSuresiBirler=0;
  }

  
} // Cursor B5 Çıkış Bloğu

}// Joystick Yukarı Çıkış Bloğu


if(Joystick_X_Durum>=0 && Joystick_Y_Durum>1000)  // Joystick Aşşağı

{



  if(CursorB==2)
  {
    CikisAktifSuresiOnlar--;

    
    if(CikisAktifSuresiOnlar<0)
    {
      CikisAktifSuresiOnlar=9;
    }

    if(CikisAktifSuresiOnlar>=2 && CikisAktifSuresiBirler>=4 && SureTypeDeger==3)
  {
    CikisAktifSuresiOnlar=2;
    CikisAktifSuresiBirler=4;
  }

  if(CikisAktifSuresiOnlar>2 && SureTypeDeger==3 )
  {
    CikisAktifSuresiOnlar=2;
    CikisAktifSuresiBirler=4;
  }

   if(CikisAktifSuresiOnlar>=6 && CikisAktifSuresiBirler>=0 && SureTypeDeger==2)
  {
    CikisAktifSuresiOnlar=6;
    CikisAktifSuresiBirler=0;
  }

   if(CikisAktifSuresiOnlar>=6 && CikisAktifSuresiBirler>=0 && SureTypeDeger==1)
  {
    CikisAktifSuresiOnlar=6;
    CikisAktifSuresiBirler=0;
  }

   
  } // Cursor B2 Çıkış Bloğu

  else if(CursorB==3)
  {
    
    CikisAktifSuresiBirler--;
    
     if(CikisAktifSuresiBirler<0)
    {
      CikisAktifSuresiBirler=9;      
    }

        if(CikisAktifSuresiOnlar>=2 && CikisAktifSuresiBirler>=4 && SureTypeDeger==3)
  {
    CikisAktifSuresiOnlar=2;
    CikisAktifSuresiBirler=4;
  }

  if(CikisAktifSuresiOnlar>2 && SureTypeDeger==3 )
  {
    CikisAktifSuresiOnlar=2;
    CikisAktifSuresiBirler=4;
  }
  

   if(CikisAktifSuresiOnlar>=6 && CikisAktifSuresiBirler>=0 && SureTypeDeger==2)
  {
    CikisAktifSuresiOnlar=6;
    CikisAktifSuresiBirler=0;
  }

   if(CikisAktifSuresiOnlar>=6 && CikisAktifSuresiBirler>=0 && SureTypeDeger==1)
  {
    CikisAktifSuresiOnlar=6;
    CikisAktifSuresiBirler=0;
  }

  
  } // Cursor B3 Çıkış Bloğu

  else if(CursorB==5)
  {

  SureTypeDeger--;

  if(SureTypeDeger<1)
  {
    SureTypeDeger=3;
  }

      if(CikisAktifSuresiOnlar>=2 && CikisAktifSuresiBirler>=4 && SureTypeDeger==3)
  {
    CikisAktifSuresiOnlar=2;
    CikisAktifSuresiBirler=4;
  }

  if(CikisAktifSuresiOnlar>2 && SureTypeDeger==3 )
  {
    CikisAktifSuresiOnlar=2;
    CikisAktifSuresiBirler=4;
  }


  if(CikisAktifSuresiOnlar==6 && CikisAktifSuresiBirler>0 && SureTypeDeger==1)
  {
    CikisAktifSuresiOnlar=6;
    CikisAktifSuresiBirler=0;
  }


  if(CikisAktifSuresiOnlar>=6 && CikisAktifSuresiBirler>0 && SureTypeDeger==2)
  {
    CikisAktifSuresiOnlar=6;
    CikisAktifSuresiBirler=0;
  }
   
  } // CursorB5 Çıkış Bloğu

} // Joystick Aşşağı Çıkış Bloğu



   
  


  if(Joystick_SW_Durum == false)       // Bitiş Değeri Ayarlamadan Çıkarken Yapılanlar
  {
    while(Joystick_SW_Durum == false)
    {
      ButonDeger++;
      delay(100);
      Joystick_SW_Durum = digitalRead(Joystick_SW);
    }

    if(ButonDeger>10)
    {
      EkranDurum=1;
      ButonDeger=0;
      break;
    }
    
   else 
   {

    Buzzer();

    CikisAktifSet=true;

    Bekleme = (CikisAktifSuresiOnlar*10)+(CikisAktifSuresiBirler);

    if(SureTypeDeger==1)
    {
      Bekleme=Bekleme*1000;
    }

        if(SureTypeDeger==2)
    {
      Bekleme=Bekleme*60000;
    }

    if(SureTypeDeger==3)
    {
      Bekleme=Bekleme*3600000;
    } 

   }
  }

  while(CikisAktifSet==true)
  {
    lcd.setCursor(12,0);
    lcd.write(4);
    digitalWrite(Role,LOW);
    delay(Bekleme);
    digitalWrite(Role,HIGH);
    Bekleme=0;
    CikisAktifSet=false;
  }

} // EkranDurum 3 Çıkış Bloğu
  

 
} // Void Loop Çıkış Bloğu


void Buzzer()
{
  
    digitalWrite(BUZZER,HIGH);
    delay(300);
    digitalWrite(BUZZER,LOW);  
}

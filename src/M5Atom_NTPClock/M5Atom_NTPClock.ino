#include <M5Atom.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <time.h>
#include <WiFi.h>

// OLED
#define DEF_OLED_SCLPIN (25)
#define DEF_OLED_SDAPIN (21)
#define DEF_OLED_WIDTH  (128)
#define DEF_OLED_HEIGHT (32)
#define DEF_OLED_RESET  (4)
Adafruit_SSD1306 display(DEF_OLED_WIDTH, DEF_OLED_HEIGHT, &Wire, DEF_OLED_RESET);

// WiFi
static const char* ssid        = "****";
static const char* password    = "****";
WiFiClient client;

// NTP
#define DEF_TIMEZONE_JST        (3600 * 9)      // +9h
#define DEF_DAYLIGHTOFFSET_JST  (0)             // サマータイムなし
#define DEF_NTP_SERVER          ("ntp.nict.jp") // NTP Server

void setup()
{
    Serial.begin(115200);
    M5.begin(true,false,true);
    Wire.begin(DEF_OLED_SDAPIN, DEF_OLED_SCLPIN);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }

    // Title
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println("M5 Atom NTP Clock!!");
    display.display();
    delay(1000);

    // WiFi
    WiFi.begin(ssid, password);
    display.setCursor(0, 12);
    display.print("WiFi start");
    while (WiFi.status() != WL_CONNECTED) {
        display.print(".");
        display.display();
        delay(500);
    }
    display.setCursor(0, 24);
    display.println(WiFi.localIP());
    display.display();

    // NTP
    configTime(DEF_TIMEZONE_JST, DEF_DAYLIGHTOFFSET_JST, DEF_NTP_SERVER);

    delay(1500);
}


void loop()
{
    char szTime[32];
    struct tm timeinfo;
    static int tm_sec_old;

    getLocalTime(&timeinfo);
    if (timeinfo.tm_sec != tm_sec_old)
    {
        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor(0, 0);
        sprintf(szTime, "%02d:%02d:%02d",
            timeinfo.tm_hour,
            timeinfo.tm_min,
            timeinfo.tm_sec);
        display.println(String(szTime));
        display.display();
        tm_sec_old = timeinfo.tm_sec;
    }

    delay(10);
}

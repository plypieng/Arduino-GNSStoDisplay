#include <GNSS.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define STRING_BUFFER_SIZE  128 

/* LCD SCREEN SIZE */
#define SCREEN_WIDTH  (320)
#define SCREEN_HEIGHT (240)

/* LCD SETTINGS */
#define TFT_DC  (9)
#define TFT_CS (-1)
#define TFT_RST (8)

static SpGnss Gnss;
static Adafruit_ILI9341 tft = Adafruit_ILI9341(&SPI, TFT_DC, TFT_CS, TFT_RST); 

void setup() {
    /* Setup serial output for printing. */
    Serial.begin(115200);

    /* Initialize GNSS. */
    Gnss.begin();
    Gnss.select(QZ_L1S);
    Gnss.start();

    // Initialize TFT display
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);

}

void loop()
{
  /* Wait for an update. */
  if (Gnss.waitUpdate(-1))
  {
    /* Get navigation data. */
    SpNavData NavData;
    Gnss.getNavData(&NavData);
    char TimeBuffer[STRING_BUFFER_SIZE];

    /* Print position and satellite count to serial monitor */
    Serial.print("Lat=");
    Serial.print(NavData.latitude, 6);
    Serial.print(", Lon=");
    Serial.print(NavData.longitude, 6);
    Serial.print(", Satellites=");
    Serial.print(NavData.numSatellites);
    Serial.print("time=");
    Serial.print(timeToString(NavData.time));
    Serial.println("");

    //screenbackground black
    tft.fillScreen(ILI9341_BLACK);
    // Display latitude and longitude
    tft.setCursor(10, 10);
    tft.print("Latitude: ");
    tft.println(NavData.latitude, 6);
    tft.setCursor(10, 40);
    tft.print("Longitude: ");
    tft.println(NavData.longitude, 6);
      
    // Display other GPS information
    tft.setCursor(10, 70);
    tft.print("Satellites: ");
    tft.println(NavData.numSatellites);
    
    // Display current time
    tft.setCursor(10, 160);
    tft.print("Time: ");
    //tft.println(timeToString(NavData.time));

    snprintf(TimeBuffer, STRING_BUFFER_SIZE, "%04d/%02d/%02d ", NavData.time.year, NavData.time.month, NavData.time.day);
    tft.println(TimeBuffer);

    snprintf(TimeBuffer, STRING_BUFFER_SIZE, "%02d:%02d:%02d.%06ld, ", NavData.time.hour, NavData.time.minute, NavData.time.sec, NavData.time.usec);
    tft.println(TimeBuffer);
    
  }
  /*if (Gnss.isUpdate()) {
      // Clear screen
      tft.fillScreen(ILI9341_BLACK);
      
      SpNavData NavData;
      Gnss.getNavData(&NavData);
      
      // Display latitude and longitude
      tft.setCursor(10, 10);
      tft.println(NavData.latitude, 6);
      tft.setCursor(10, 40);
      tft.println(NavData.longitude, 6);
      
      // Display other GPS information
      tft.setCursor(10, 70);
      tft.print("Satellites: ");
      tft.println(NavData.numSatellites);
      // Display current time
      tft.setCursor(10, 160);
      tft.print("Time: ");
      tft.println(timeToString(NavData.time));
    }*/
}

String timeToString(SpGnssTime time) {
  String timeStr = "";
  timeStr += String(time.hour, DEC);
  timeStr += ":";
  timeStr += String(time.minute, DEC);
  timeStr += ":";
  timeStr += String(time.sec, DEC);
  return timeStr;
}
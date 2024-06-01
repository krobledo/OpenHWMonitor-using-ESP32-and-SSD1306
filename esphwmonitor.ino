#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C  // Common I2C address for SSD1306

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Array to store GPU Core Load values
#define NUM_POINTS 128
float gpuCoreValues[NUM_POINTS];
int currentIndex = 0;

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Initializing..."));
  display.display();
  
  // Initialize the array with 0s
  for (int i = 0; i < NUM_POINTS; i++) {
    gpuCoreValues[i] = 0;
  }
}

void loop() {
  if (Serial.available()) {
    String data = "";
    while (Serial.available()) {
      data += (char)Serial.read();
      delay(10);  // Small delay to ensure complete data is read
    }

    Serial.println("Received data:");
    Serial.println(data);  // Debugging print

    // Extract GPU Core Load and Memory Load from the data
    int gpuCoreStart = data.indexOf("GPU Core: ");
    int memoryLoadStart = data.indexOf("Memory Load: ");
    if (gpuCoreStart != -1 && memoryLoadStart != -1) {
      gpuCoreStart += 10;
      int gpuCoreEnd = data.indexOf('%', gpuCoreStart);
      memoryLoadStart += 12;
      int memoryLoadEnd = data.indexOf('%', memoryLoadStart);

      if (gpuCoreEnd != -1 && memoryLoadEnd != -1) {
        String gpuCoreStr = data.substring(gpuCoreStart, gpuCoreEnd);
        float gpuCore = gpuCoreStr.toFloat();

        String memoryLoadStr = data.substring(memoryLoadStart, memoryLoadEnd);
        float memoryLoad = memoryLoadStr.toFloat();

        // Update the GPU Core Load values array
        gpuCoreValues[currentIndex] = gpuCore;
        currentIndex = (currentIndex + 1) % NUM_POINTS;

        // Display the data and graph
        display.clearDisplay();
        display.setCursor(0, 0);

        // Split data into lines and display each line
        int lineHeight = 10;
        int cursorY = 0;
        int startIndex = 0;
        int endIndex = data.indexOf('\n');
        while (endIndex != -1) {
          String line = data.substring(startIndex, endIndex);
          display.setCursor(0, cursorY);
          display.println(line);
          cursorY += lineHeight;
          startIndex = endIndex + 1;
          endIndex = data.indexOf('\n', startIndex);
        }

        // Display the last line (if there's no trailing newline)
        if (startIndex < data.length()) {
          display.setCursor(0, cursorY);
          display.println(data.substring(startIndex));
        }

        // Draw the graph
        int graphHeight = 20;
        int graphY = SCREEN_HEIGHT - graphHeight - 1;
        display.drawRect(0, graphY, SCREEN_WIDTH, graphHeight, SSD1306_WHITE);
        for (int i = 0; i < NUM_POINTS - 1; i++) {
          int x1 = i;
          int y1 = graphY + graphHeight - (gpuCoreValues[(currentIndex + i) % NUM_POINTS] * graphHeight / 100);
          int x2 = i + 1;
          int y2 = graphY + graphHeight - (gpuCoreValues[(currentIndex + i + 1) % NUM_POINTS] * graphHeight / 100);
          display.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
        }

        display.display();
        delay(2000);  // Delay for 2 seconds to make the display readable
      }
    }
  }
}

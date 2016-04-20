/* The circuit:
   SDA is A4 -- red (above A2 and A3)
   SCL is A5 -- black (above A3 and VCC)
    
    On PSoC it's SDA 0.5 and SCL 0.4
*/
#define TWI_FREQ 100000L

#include <Pan4.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

//Whatever Pin we want to control the lights from
#define PIN 6

//Establish light strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(50, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Wire.begin();
  Serial.begin(9600);
  strip.begin();
  strip.show();
  setGrid();
}

// uint16_t grid[7][7];
// int registers[8] = {0, 0, 0, 0, 0, 0, 0, 0}; 
// int reading;

// //Starting locations of the 8 sensor animations
// int animationx[8] = {6, 2, 4, 6, 2, 4, 0, 0};
// int animationy[8] = {3, 2, 4, 6, 6, 0, 4, 0};



// //0 means this node is not in the midst of an animation
// //1 means this node is in state one (animation node lit)
// //2 means this node is in state two (square around animation node lit)
// //3 means this node is in state three (super-square around animation node)
// int triggered[8] = {0, 0, 0, 0, 0, 0, 0, 0};
// int vals[8] = {0, 0, 0, 0, 0, 0, 0, 0};
// uint32_t color = strip.Color(127, 127, 127);

// //----> Look-up tables for IR sensor orders
// //### 3 panel board: 0xC, 0xE, 0xD
// //first
// int panel12[8] = {4, 0, 6, 2, 5, 1, 7, 3};
// //second
// int panel14[8] = {3, 6, 5, 7, 0, 4, 1, 2};
// //third
// int panel13[8] = {3, 7, 1, 6, 0, 2, 5, 4};

// //### 4 panel board: 0x09, 0x08, 0x0A, 0x0F
// int panel9[8] = {7, 1, 2, 3, 0, 6, 4, 5};
// int panel8[8] = {7, 6, 1, 4, 5, 2, 0, 3};
// int panel10[8] = {6, 2, 4, 1, 7, 3, 5, 0};
// int panel15[8] = {7, 4, 2, 0, 6, 3, 5, 1};

//Led no for x, y position
//extern const int lednobyxy[28][7];

//LED x coord by # in strip
//extern const int ledxbyno[200];

//LED y coord by # in strip
//extern const int ledybyno[200];

//IR Addresses
//extern const int IRaddr[32];

//IR registers
//extern const int IRreg[32];


void loop() {
  for (int i=0; i<32; i++ ){
    readNodes(iRaddr[32]);
    for (int t=0; t<8; t++){
      if (checkAvg(t) > 4 || triggered[t] > 0) {
        setAnimation(t);  
      }
    }
     strip.show();
     delay(1000);
  }
}

int checkAvg(int t) {
  int diff = registers[t] - vals[t];
  return diff;
}

void setAnimation(int c) {
   uint16_t pixel;
     switch (triggered[c]) {
       case 0:
         pixel = grid[animationx[c]][animationy[c]];
         strip.setPixelColor(pixel, color);
         triggered[c] = 1;
         break;
        case 1:
          map_r(animationx[c],animationy[c],1);
          clear_r(animationx[c],animationy[c],0);
          triggered[c] = 2;
          break;
        case 2:
          map_r(animationx[c],animationy[c],2);
          clear_r(animationx[c],animationy[c],1);
          triggered[c] = 3;
          break;
        case 3:
          clear_r(animationx[c],animationy[c],2);
          triggered[c] = 0;
          break;
     }
}


void map_r(int x, int y, int r) {
  int max_x, max_y, min_x, min_y;
  
  max_x = min(6, x+r);
  max_y = min(6, y+r);
  min_x = max(0, x-r);
  min_y = max(0, x-r);
  
  for(int i=min_x; i <= max_x; i++) {
    for(int j=min_y; j <= max_y; j++) {
      if (j == min_y || j == max_y || i == min_x || i == max_x) {
        strip.setPixelColor(grid[i][j], color);
      }
    }
  }
}

void clear_r(int x, int y, int r) {
   int max_x, max_y, min_x, min_y;
  
  max_x = min(6, x+r);
  max_y = min(6, y+r);
  min_x = max(0, x-r);
  min_y = max(0, x-r);
  
  for(int i=min_x; i <= max_x; i++) {
    for(int j=min_y; j <= max_y; j++) {
      if (j == min_y || j == max_y || i == min_x || i == max_x) {
         strip.setPixelColor(grid[i][j], strip.Color(0,0,0));
      }
    }
  }
}
  
void lightGrid() {
  for(int i=0; i<7; i++){
        for(int j=0; j<7; j++){
            if(i%2 == 0){
                grid[i][j] = (i * 7) + j;
                strip.setPixelColor(grid[i][j], strip.Color(127, 127, 200));
                strip.show();
//                delay(200);
            } else {
                grid[i][j] = (i * 7) + (6 - j);
                strip.setPixelColor(grid[i][j], strip.Color(127, 127, 200));
                strip.show();
//                delay(200);
            }
        }
    }
}
  
void setGrid() {
  int count=0;
    for(int i=0; i<7; i++){
        for(int j=0; j<7; j++){
            if(i%2 == 0){
                grid[i][j] = (i * 7) + j;

            } else {
                grid[i][j] = (i * 7) + (7 - j);
  
            }
        }
    }
}


//This may need to be combining bytes??
void readNodes(int x) {
   int reg = 0;
    for (int byteno=36; byteno < 52; byteno=byteno+2) {
      
        int read1;
        int read2;
        int final_reading;
        
        Wire.beginTransmission(x);
        Wire.write(byteno);
        Wire.endTransmission();

        Wire.requestFrom(x, 1);
        if (1 <= Wire.available()) {
            read1 = Wire.read();
            
        }
        Wire.beginTransmission(x);
        Wire.write(byteno + 1);
        Wire.endTransmission();

        Wire.requestFrom(x, 1);
        if (1 <= Wire.available()) {
            read2 = Wire.read();        
        }
        read1 = read1 * 256;
        final_reading = read1 + read2;
        
        vals[reg] = registers[reg];
        registers[reg] = final_reading;
        int diff = vals[reg] - registers[reg];
        Serial.print("Register");
        Serial.print(reg);
        Serial.print(": diff = ");
        Serial.print(final_reading);
        Serial.println();
        
        reg++;
        
    }
    Serial.println("------------------");
}

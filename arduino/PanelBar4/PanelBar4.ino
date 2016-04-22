/* The circuit:
   SDA is A4 -- red (above A2 and A3)
   SCL is A5 -- black (above A3 and VCC)
    
    On PSoC it's SDA 0.5 and SCL 0.4
*/
#define TWI_FREQ 100000L

#include "Pan4.h"
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

//Whatever Pin we want to control the lights from
#define PIN 6


//Who is in motion
int triggered[28][7];
Adafruit_NeoPixel strip = Adafruit_NeoPixel(200, PIN, NEO_GRB + NEO_KHZ800);
//Used to store most recent and prior read values
int regs[32] = {0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0};
int oldRegs[32] = {0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0};

//Need to fill this in the right locations of the IR nodes
int placementsx[32] = {0, 0, 2, 2, 4, 4, 6, 6,
                      13, 13, 11, 11, 9, 9, 8, 8,
                      14, 14, 16, 16, 18, 18, 20, 20,
                      22, 22, 23, 23, 25, 25, 27, 27};
                      
int placementsy[32] = {0, 4, 2, 4, 0, 4, 3, 5,
                      2, 5, 1, 4, 2, 5, 1, 4,
                      1, 4, 2, 5, 1, 4, 2, 5,
                      1, 4, 2, 5, 1, 4, 2, 5};
float breathState = 0;
int breathUp = true;

//whatever our standard breathe tends toward
int rgb[3] = {0, 200, 0};

void setup() {
  Wire.begin();
  Serial.begin(57600);
  strip.begin();
  initializeTriggers();
  strip.show();
}

void initializeTriggers() {
  for (int x=0; x<28; x++) {
    for (int y=0; y<7; y++) {
      triggered[x][y] = 0;
    }
  }
}

// For new code:
// Will need something like:
// 0. Do standard breathing up and down
// 1. Read all IR Addr
// 2. Read all Register
// 3. Figure out who has changed
// 4. Set animations
// 5. Update animation states

int checkAvg(int node) {
  int diff = regs[node] - oldRegs[node];
  return diff;
}

void loop() {
  //Set initial normal state of all LEDs for breathe pattern
  breathe();
  //Fill in new register values
   for (int p=0; p<32; p++) {
     readNodes(IRaddr[p], IRreg[p], p);
     //During testing, we could look for abs values
     if (checkAvg(p) > 4) {
        int x = placementsx[p];
        int y = placementsy[p];
        triggered[x][y] = 1;
     }
  }
  for (int col = 0; col < 28; col++){
    for (int row = 0; row < 7; row++) {
      if (triggered[col][row] > 0) {
          setAnimation(col,row);  
      }
    }
  }
   strip.show();
   //delay(90);
   //Do we want some kind of delay?
}

void breathe() {

  if (breathUp){
    breathState += .05;
  } else {
    breathState -= .05;
  }
  
  int r = breathState*rgb[0];
  int g = breathState*rgb[1];
  int b = breathState*rgb[2];
  
  uint32_t colorBr = strip.Color(g, r, b);
  for (int x=0; x<28; x++) {
    for (int y=0; y<7; y++) {
      uint16_t pixel = lednobyxy[x][y];
      strip.setPixelColor(pixel, colorBr);
    }
  } 
    if (breathState > 1.0){
    breathUp = false;
  } else if (breathState < 0.05) {
    breathUp = true;
  }

}

void readNodes(int x, int y, int reg) {
   int read1;
   int read2;
   int final_reading;
   
   //Wire.beginTransmission(x);
   //Wire.write(y);
   //Wire.endTransmission();
//   
//
    //Wire.requestFrom(x, 1);
    Wire.requestFrom((uint8_t)x,(uint8_t)1,(uint32_t)y,(uint8_t)1,(uint8_t)1);
    //Wire.requestFrom((uint8_t)x,(uint8_t)1,(uint32_t)y,(uint8_t)1);
    //if (1 <= Wire.available()) {
        read1 = Wire.read();
    //}
    
    //Wire.beginTransmission(x);
    //Wire.write(y + 1);
    //Wire.endTransmission();

    //Wire.requestFrom(x, 1);
    Wire.requestFrom((uint8_t)x,(uint8_t)1,(uint32_t)y+1,(uint8_t)1,(uint8_t)1);
    //Wire.requestFrom((uint8_t)x,(uint8_t)1,(uint32_t)y+1,(uint8_t)1);
    //if (1 <= Wire.available()) {
        read2 = Wire.read();  
    //}
  
    read1 = read1 * 256;
    final_reading = read1 + read2;
    oldRegs[reg] = regs[reg];
    
    //Store value
    regs[reg] = final_reading;
    if (final_reading > 3000) {
      Serial.print("Sensor: ");
      Serial.print(reg);
      Serial.print(" = ");
      Serial.print(final_reading);
      Serial.println();
    }
}
//
void setAnimation(int col, int row) {
   uint16_t pixel;
   long dir = random(4);
   if (triggered[col][row] == 7) {
    Serial.print("TiggerState: ");
    Serial.print(triggered[col][row]);
    Serial.println();
    triggered[col][row] = 0;
    return; 
   } else {
      //Play with different animation colors or we could randomize
      uint32_t colorsp = strip.Color(0, 255, 0); 
      int temp = triggered[col][row];
      temp++;
      triggered[col][row] = 0;
      switch (dir){
        case 0:
          if(row-1 < 0) {  
            triggered[col][row] = 0;
            break;
          }
          triggered[col][row-1] = temp;
          pixel = lednobyxy[col][row-1];
          strip.setPixelColor(pixel, colorsp);
          break;

        case 1:
          if(row+1 > 6) { 
            triggered[col][row] = 0;
            break;
          }
          triggered[col][row+1] = temp;
          pixel = lednobyxy[col][row+1];
          strip.setPixelColor(pixel, colorsp);
          break;

        case 2:
          if(col-1 < 0) {  
            triggered[col][row] = 0;
            break;;
          }
          triggered[col-1][row] = temp;
          pixel = lednobyxy[col-1][row];
          strip.setPixelColor(pixel, colorsp);
          break;

        case 3:
          if(col+1 > 27) {  
            triggered[col][row] = 0;
            break;
          }
          triggered[col+1][row] = temp;
          pixel = lednobyxy[col+1][row];
          strip.setPixelColor(pixel, colorsp);
          break;
      }
   }
 }

     // switch (triggered[pan][c]) {
     //   case 0:
     //     pixel = grid[animationx[c]][animationy[c]];
     //     strip.setPixelColor(pixel, color);
     //     triggered[c] = 1;
     //     break;
     //    case 1:
     //      map_r(animationx[c],animationy[c],1);
     //      clear_r(animationx[c],animationy[c],0);
     //      triggered[c] = 2;
     //      break;
     //    case 2:
     //      map_r(animationx[c],animationy[c],2);
     //      clear_r(animationx[c],animationy[c],1);
     //      triggered[c] = 3;
     //      break;
     //    case 3:
     //      clear_r(animationx[c],animationy[c],2);
     //      triggered[c] = 0;
     //      break;
     // }


// void map_r(int x, int y, int r) {
//   int max_x, max_y, min_x, min_y;
  
//   max_x = min(6, x+r);
//   max_y = min(6, y+r);
//   min_x = max(0, x-r);
//   min_y = max(0, x-r);
  
//   for(int i=min_x; i <= max_x; i++) {
//     for(int j=min_y; j <= max_y; j++) {
//       if (j == min_y || j == max_y || i == min_x || i == max_x) {
//         strip.setPixelColor(grid[i][j], color);
//       }
//     }
//   }
// }

// void clear_r(int x, int y, int r) {
//   int max_x, max_y, min_x, min_y;
  
//   max_x = min(6, x+r);
//   max_y = min(6, y+r);
//   min_x = max(0, x-r);
//   min_y = max(0, x-r);
  
//   for(int i=min_x; i <= max_x; i++) {
//     for(int j=min_y; j <= max_y; j++) {
//       if (j == min_y || j == max_y || i == min_x || i == max_x) {
//          strip.setPixelColor(grid[i][j], strip.Color(0,0,0));
//       }
//     }
//   }
// }
//  
//void lightGrid() {
//  
//  for(int i=0; i<28; i++){
//        for(int j=0; j<7; j++){
//            if(i%2 == 0){
//                strip.setPixelColor(lednobyxy[i][j], strip.Color(127, 127, 200));
//                strip.show();
//                delay(200);
//            } else {
//                strip.setPixelColor(lednobyxy[i][j], strip.Color(127, 127, 200));
//                strip.show();
//                delay(200);
//            }
//        }
//    }
//}
  
// void setGrid(int g) {
//   //Go through each panel
//     for (int pan=0; pan<g; pan++) {
//       int start = pan*7;
//       int stop = start+7;
//       bool flip = flipped[pan];
//   //fill cols/rows
//       for(int i=start; i<stop; i++){
//           for(int j=0; j<7; j++){

//             //Different pending if wire btw panels is flipped
//             int k = i;
//             if (flip) {
//               k = i + 1;
//             }
//             if(k%2 == 0){
//                 grid[i][j] = (i * 7) + j + pan;

//             } else {
//                 grid[i][j] = (i * 7) + (6 - j) + pan;
  
//             }

//             triggered[i][j] = 0;
//             strip.setPixelColor(grid[i][j], strip.Color(0,0,0));
            
//           }
//       }
//     }   
// }




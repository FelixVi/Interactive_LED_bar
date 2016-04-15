/* The circuit:
   SDA is A4 -- red (above A2 and A3)
   SCL is A5 -- black (above A3 and VCC)
    
    On PSoC it's SDA 0.5 and SCL 0.4
*/
#define TWI_FREQ 100000L

#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

//Whatever Pin we want to control the lights from
#define PIN 6

//Establish light strip

// 4 panels
// uint16_t grid[28][7];

//Which ones aren't chained correctly
bool flipped[4] = { false, false, true, false};

//----> Look-up tables for IR sensor orders
//### 3 panel board: 0xC, 0xE, 0xD
//first
//int panel12[8] = {4, 0, 6, 2, 5, 1, 7, 3};
//second
//int panel14[8] = {3, 6, 5, 7, 0, 4, 1, 2};
//third
//int panel13[8] = {3, 7, 1, 6, 0, 2, 5, 4};

//### 4 panel board: 0x09, 0x08, 0x0A, 0x0F
// int panel9[8] = {7, 1, 2, 3, 0, 6, 4, 5};
// int panel9old[8] = {0, 0, 0, 0, 0, 0, 0};

// int panel8[8] = {0, 3, 5, 2, 1, 4, 7, 6};
// int panel8old[8] = {0, 0, 0, 0, 0, 0, 0};

// int panel10[8] = {6, 2, 4, 1, 7, 3, 5, 0};
// int panel10old[8] = {0, 0, 0, 0, 0, 0, 0};

// int panel15[8] = {7, 4, 2, 0, 6, 3, 5, 1};
// int panel15old[8] = {0, 0, 0, 0, 0, 0, 0};

// int *all[4] = {panel9, panel8, panel10, panel15};
// int *allOld[4] = {panel9old, panel8old, panel10old, panel15old};

int myPanels[4] = {9, 8, 10, 15};



//Who is in motion
int triggered[28][7];

int animationx[8] = {0, 0, 2, 2, 4, 4, 6, 6};
int animationy[8] = {0, 4, 2, 6, 0, 4, 3, 6};
int loc[2];
Adafruit_NeoPixel strip = Adafruit_NeoPixel(200, PIN, NEO_GRB + NEO_KHZ800);
uint32_t color = strip.Color(13, 197, 127);
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
int placementsx[32];
int placementsy[32];
int breathState = 0;
int breathUp = true;

uint32_t colorsp = strip.Color(0, 197, 0); 
//whatever our standard breathe tends toward
int rgb[3] = {55,72,62}

void breathe() {

  if (breathUp){
    breathState += .05;
  } else {
    breathState -= .05;
  }
  
  r = breathState*rgb[0]
  g = breathState*rgb[1]
  b = breathState*rgb[2]
  
  uint32_t colorBr = strip.Color(r, g, b);
  for (int x=0; x<28; x++) {
    for (int y=0; y<7; y++) {
      pixel = lednobyxy[x][y];
      strip.setPixelColor(pixel, colorBr);
    }
  } 
  if (breathState == 1){
    breathUp = false;
  } else if (breathState == 0) {
    breathUp = true;
  }

}


void setup() {
  Wire.begin();
  Serial.begin(9600);
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
     if (checkAvs(p) > 4) {
        x = placementsx[p];
        y = placementsy[p];
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
   //Do we want some kind of delay?
   delay(2000);

}


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
      uint32_t colorsp = strip.Color(0, 197, 0); 
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
          pixel = grid[col][row-1];
          strip.setPixelColor(pixel, colorsp);
          break;

        case 1:
          if(row+1 > 6) { 
            triggered[col][row] = 0;
            break;
          }
          triggered[col][row+1] = temp;
          pixel = grid[col][row+1];

          strip.setPixelColor(pixel, colorsp);
          break;

        case 2:
          if(col-1 < 0) {  
            triggered[col][row] = 0;
            break;;
          }
          triggered[col-1][row] = temp;
          pixel = grid[col-1][row];
          strip.setPixelColor(pixel, colorsp);
          break;

        case 3:
          if(col+1 > 27) {  
            triggered[col][row] = 0;
            break;
          }
          triggered[col+1][row] = temp;
          pixel = grid[col+1][row];
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
  
void lightGrid() {
  
  for(int i=0; i<28; i++){
        for(int j=0; j<7; j++){
            if(i%2 == 0){
                strip.setPixelColor(grid[i][j], strip.Color(127, 127, 200));
                strip.show();
                delay(200);
            } else {
                strip.setPixelColor(grid[i][j], strip.Color(127, 127, 200));
                strip.show();
                delay(200);
            }
        }
    }
}
  
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


void readNodes(int x, int y, int reg) {
   int read1;
   int read2;
   int final_reading;

   Wire.beginTransmission(x);
   Wire.write(y);
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
    oldRegs[reg] = regs[reg]
    
    //Store value
    regs[reg] = final_reading;
        
    // }
    Serial.println("------------------");
}

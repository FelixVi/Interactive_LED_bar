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

int num_panels = 4;
//Establish light strip

// 4 panels
uint16_t grid[28][7];

int reading;

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
int panel9[8] = {7, 1, 2, 3, 0, 6, 4, 5};
int panel9old[8] = {0, 0, 0, 0, 0, 0, 0};

int panel8[8] = {0, 3, 5, 2, 1, 4, 7, 6};
int panel8old[8] = {0, 0, 0, 0, 0, 0, 0};

int panel10[8] = {6, 2, 4, 1, 7, 3, 5, 0};
int panel10old[8] = {0, 0, 0, 0, 0, 0, 0};

int panel15[8] = {7, 4, 2, 0, 6, 3, 5, 1};
int panel15old[8] = {0, 0, 0, 0, 0, 0, 0};

int *all[4] = {panel9, panel8, panel10, panel15};
int *allOld[4] = {panel9old, panel8old, panel10old, panel15old};

int myPanels[4] = {9, 8, 10, 15};

int registers[8] = {0, 0, 0, 0, 0, 0, 0, 0};

//Who is in motion
int triggered[28][7];

int animationx[8] = {0, 0, 2, 2, 4, 4, 6, 6};
int animationy[8] = {0, 4, 2, 6, 0, 4, 3, 6};
int loc[2];
Adafruit_NeoPixel strip = Adafruit_NeoPixel(200, PIN, NEO_GRB + NEO_KHZ800);
uint32_t color = strip.Color(13, 197, 127);


void setup() {
  Wire.begin();
  Serial.begin(9600);
  strip.begin();
  setGrid(num_panels);
  strip.show();
  // &panel9, &panel8, &panel10, &panel15
}

void loop() {
   for (int p=0; p<num_panels; p++) {
     readNodes(myPanels[p], p);
     for (int t=0; t<8; t++){
       //If the threshold was met or if its in process of animating
       if (checkAvg(p, t) > 4){
         Serial.print("Triggered Panel");
         Serial.print(p);
         Serial.print(", Sensor");
         Serial.print(t);
         Serial.println();
         getLocation(p, t);
         triggered[loc[0]][loc[1]] = 1;
       }
     }
   }
   lightGrid();

//   for (int col = 0; col < 28; col++){
//     for (int row = 0; row < 7; row++) {
//       if (triggered[col][row] > 0) {
//           setAnimation(col,row);  
//       }
//     }
//   }
    strip.show();
    delay(2000);
}

void getLocation(int p, int t) {

  int *dx = all[p] + t; 
  int x = animationx[*dx];
  int y = animationy[*dx];
  loc[0] = x + (7*p);
  loc[1] = y;
}

int checkAvg(int pan, int t) {
  int *myVal = allOld[pan] + t;
  int diff = registers[t] - *myVal;
  *(allOld[pan] + t) = registers[t];
  return diff;
}

void setAnimation(int col, int row) {
   uint16_t pixel;
   int dir = random(4);
   if (triggered[col][row] >= 7) {
    Serial.print("TiggerState: ");
    Serial.print(triggered[col][row]);
    Serial.println();
    triggered[col][row] = 0;
    return; 
   } else {
      uint32_t colorsp = strip.Color(0, 197, 0); 
      int temp = triggered[col][row];
      triggered[col][row] = 0;
      strip.setPixelColor(grid[col][row], strip.Color(0,0,0));
      switch (dir){
        case 0:
          if(row-1 < 0) {  
            break;
          }
          triggered[col][row-1] = temp+=1;
          pixel = grid[col][row-1];
          strip.setPixelColor(pixel, colorsp);
          break;

        case 1:
          if(row+1 > 6) { 

            break;
          }
          triggered[col][row+1] = temp+=1;
          pixel = grid[col][row+1];

          strip.setPixelColor(pixel, colorsp);
          break;

        case 2:
          if(col-1 < 0) {  
            break;;
          }
          triggered[col-1][row] = temp+=1;
          pixel = grid[col-1][row];
          strip.setPixelColor(pixel, colorsp);
          break;

        case 3:
          if(col+1 > 27) {  
            break;
          }
          triggered[col+1][row] = temp+=1;
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
                Serial.println("testing123");
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
  
void setGrid(int g) {
  //Go through each panel
    for (int pan=0; pan<g; pan++) {
      int start = pan*7;
      int stop = start+7;
      bool flip = flipped[pan];
  //fill cols/rows
      for(int i=start; i<stop; i++){
          for(int j=0; j<7; j++){

            //Different pending if wire btw panels is flipped
            int k = i;
            if (flip) {
              k = i + 1;
            }
            if(k%2 == 0){
                grid[i][j] = (i * 7) + j + pan;

            } else {
                grid[i][j] = (i * 7) + (6 - j) + pan;
  
            }

            triggered[i][j] = 0;
            strip.setPixelColor(grid[i][j], strip.Color(0,0,0));
            
          }
      }
    }   
}


void readNodes(int x, int pa) {
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
        
        //Store value
        registers[reg] = final_reading;
        

        int diff = (*allOld[pa] + reg) - registers[reg];
        // Serial.print("Register");
        // Serial.print(reg);
        // Serial.print(": diff = ");
        // Serial.print(final_reading);
        // Serial.println();
        
        reg++;
        
    }
    Serial.println("------------------");
}

/* 
 * PolylineTest Example
 * ---------------------
 * This will run the encoding algorithm against 
 * unit test cases from the Google example page. 
 */

#include <Polyline.h>

Polyline mypolyline;

void setup() {
  Serial.begin(9600);
}

void loop() {
  mypolyline.testAlgorithm();
  delay(30*100UL);
}
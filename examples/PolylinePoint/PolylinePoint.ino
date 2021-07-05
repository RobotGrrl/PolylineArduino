/* 
 * PolylinePoint Example
 * ---------------------
 * Outputs an encoded polyline string from
 * two points. Alternative method with one
 * double value. 
 */

#include <Polyline.h>

Polyline mypolyline;

double mylat = 100.0;
double mylon = 100.0;

void setup() {
  Serial.begin(9600);
}

void loop() {
	// point with two coordinates
  String s1 = mypolyline.encodePolylinePoint(mylat, mylon);
  delay(30*100UL);

  // single value
  String s2 = mypolyline.encodePolylineVal(mylat);
  Serial.println(s2);
  delay(30*100UL);
}

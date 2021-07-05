#include "Arduino.h"
#include "Polyline.h"

Polyline::Polyline() {

  // init the vars
  test_vals[0] = -179.9832104;
  test_vals[1] = 38.5;
  test_vals[2] = -120.2;
  test_vals[3] = 2.2;
  test_vals[4] = -0.75;
  test_vals[5] = 2.552;
  test_vals[6] = -5.503;

  test_polylines[0] = "`~oia@";
  test_polylines[1] = "_p~iF";
  test_polylines[2] = "~ps|U";
  test_polylines[3] = "_ulL";
  test_polylines[4] = "nnqC";
  test_polylines[5] = "_mqN";
  test_polylines[6] = "vxq`@";

}

String Polyline::encodePolylinePoint(double lat, double lon) {

  // encode for each val in the point
  String s1 = encodePolylineVal(lat);
  String s2 = encodePolylineVal(lon);
  String encoded_polyline = s1+s2;

  // output
  Serial.print("Encoded polyline for point (");
  Serial.print(lat);
  Serial.print(",");
  Serial.print(lon);
  Serial.print(") = ");
  Serial.println(encoded_polyline);

  return encoded_polyline;
}


String Polyline::encodePolylineVal(double p) {

  char charnum[10]; // char array holding the long
  uint8_t bytenum[10]; // byte array holding the long
  unsigned long pointnum = 0; // absolute value of the point
  unsigned long chunk[6]; // 5 bit chunks
  char polylineval[6]; // resulting polyline char array for val
  String polylineval_str = ""; // polyline for val
  
  // ////////////////////////
  // step 1: prep the number
  // ////////////////////////

  #ifdef POLYLINE_VERBOSE
  Serial.println("\n--- prep the number ---");
  #endif
  
  // multiply by 10^5
  long plong = p * pow(10, 5);

  // put into a char array
  sprintf(charnum,"%ld",plong);

  // output step 1
  #ifdef POLYLINE_VERBOSE
  Serial.println(p, DEC);
  Serial.println(p, 10);
  Serial.println(pointnum, DEC);
  Serial.println(charnum);
  #endif

  // find if negative, count index, convert to bytes
  uint8_t count = 0;
  bool negative = false;
  
  for(uint8_t i=0; i<10; i++) { // max number of digits = 10 (arbitrary)
    
    // check if negative, and skip that index
    if(charnum[i] == '-') {
      negative = true;
      #ifdef POLYLINE_VERBOSE
      Serial.print("skip ind ");
      Serial.println(i);
      #endif
      continue;
    }
    
    // check for end of string 
    if(charnum[i] == '\0') {
      #ifdef POLYLINE_VERBOSE
      Serial.print("skip ind ");
      Serial.println(i);
      #endif
      continue;
    }
    
    // convert to byte
    bytenum[count] = charnum[i]-'0'; // subtract ascii to convert to byte
    
    // output debug
    #ifdef POLYLINE_VERBOSE
    Serial.print(count);
    Serial.print(": ");
    Serial.println(bytenum[count]);
    #endif
    
    count++;
  }

  // output the number in binary
  #ifdef POLYLINE_VERBOSE
  for(uint8_t i=0; i<count; i++) {
    Serial.println(bytenum[i],BIN);
  }
  #endif

  // /////////////////////////
  // step 2: two's complement
  // ////////////////////////

  #ifdef POLYLINE_VERBOSE
  Serial.println("--- two's complement ---");
  #endif

  // save abs value to unsigned long
  pointnum = abs(plong);
  
  if(negative) {
    #ifdef POLYLINE_VERBOSE
    Serial.println("it's a negative number");
    #endif
    // two's complement
    pointnum = ~pointnum;
    pointnum += 0b1;
  }

  // output to binary
  #ifdef POLYLINE_VERBOSE
  Serial.println(pointnum, BIN);
  // 11111110 11101101 01011110 00001111
  #endif

  #ifdef POLYLINE_VERBOSE
  Serial.println("--- left shift ---");
  #endif

  // left shift 1 bit
  pointnum = pointnum << 1;
  #ifdef POLYLINE_VERBOSE
  Serial.println(pointnum, BIN);
  // 11111101 11011010 10111100 00011110
  #endif

  // if negative, invert this encoding
  if(negative) {
    pointnum = ~pointnum;
    #ifdef POLYLINE_VERBOSE
    Serial.println(pointnum, BIN);
    // 00000010 00100101 01000011 11100001
    #endif
  }

  // ////////////////////////////////
  // step 3: convert to 5 bit chunks
  // ///////////////////////////////
  
  #ifdef POLYLINE_VERBOSE
  Serial.println("--- 5 bit chunks ---");
  #endif

  // 5 bit chunks start with a mask
  unsigned long mask = 0b0000000000000000000000000000011111;
  for(uint8_t i=0; i<6; i++) {
    
    // chunk is pointnum (unsigned long) AND mask
    chunk[i] = pointnum & mask;
    // trim the chunk to 5 bits by moving the other bits off, only 5 bits get saved, so the trailing 0's don't fit
    chunk[i] = chunk[i] >> (5*i);
    // move the mask to the next 5 bits by moving to the left
    mask = mask << 5;
    #ifdef POLYLINE_VERBOSE
    // output
    Serial.print(chunk[i], BIN);
    Serial.print(" ");
    #endif
    
  }
  #ifdef POLYLINE_VERBOSE
  Serial.println("");
  #endif

  // ////////////////
  // step 4: OR 0x20
  // ////////////////

  #ifdef POLYLINE_VERBOSE
  Serial.println("--- OR 0x20 ---");

  // OR each value w 0x20 except for last chunk
  
  // output
  Serial.println("before");
  for(uint8_t i=0; i<6; i++) {
    Serial.print(chunk[i], DEC);
    Serial.print(" ");
  }
  Serial.println("");
  #endif

  // find the index of last non-empty chunk
  uint8_t last_ind = 6-1;
  for(uint8_t i=last_ind; i>0; i--) { // start from end
    if(chunk[i] == 0b0) { // empty
      last_ind = i-1;
    } else {
      break;
    }
  }
  #ifdef POLYLINE_VERBOSE
  Serial.print("last index = ");
  Serial.println(last_ind);
  #endif
  
  // OR the chunk up to last_ind
  #ifdef POLYLINE_VERBOSE
  Serial.println("after");
  #endif
  for(uint8_t i=0; i<last_ind; i++) {
    chunk[i] = chunk[i] | 0x20; // OR with 0x20
    #ifdef POLYLINE_VERBOSE
    Serial.print(chunk[i], DEC);
    Serial.print(" ");
    #endif
  }
  #ifdef POLYLINE_VERBOSE
  Serial.println("");
  #endif

  // /////////////////////////
  // step 5: convert to ascii
  // ////////////////////////

  #ifdef POLYLINE_VERBOSE
  Serial.println("--- ascii ---");
  #endif

  // add decimal 63
  for(uint8_t i=0; i<6; i++) {
    chunk[i] = chunk[i] + '?'; // '?' is decimal 63
    #ifdef POLYLINE_VERBOSE
    Serial.print(chunk[i], DEC);
    Serial.print(" ");
    #endif
  }
  #ifdef POLYLINE_VERBOSE
  Serial.println("");
  #endif

  // create ascii
  uint8_t len = 0;
  for(uint8_t i=0; i<6; i++) {
    if(chunk[i] != '?') { // don't include it if it's 0 // TODO: is this still needed?
      polylineval[i] = (char)chunk[i]; // cast to char and save it
      len++;
    }
  }
  
  // add to string
  for(uint8_t i=0; i<len; i++) {
    polylineval_str+=polylineval[i];
    #ifdef POLYLINE_VERBOSE
    Serial.print(polylineval[i]);
    #endif
  }
  #ifdef POLYLINE_VERBOSE
  Serial.println("\n");
  #endif

  return polylineval_str;
}


void Polyline::testAlgorithm() {

  uint8_t num_pass = 0;
  uint8_t num_fail = 0;
  uint8_t num_elements = (sizeof(test_vals)/sizeof(test_vals[0])); // size of the array divided by an element in the array, to give the length of the array
  
  for (uint8_t i=0; i<num_elements; i++) { 
    String s = encodePolylineVal(test_vals[i]);
    test_results[i] = s;
    if(s == test_polylines[i]) {
      #ifdef POLYLINE_VERBOSE
      Serial.print(i);
      Serial.print(": PASS ");
      Serial.print(s);
      Serial.print(" = ");
      Serial.println(test_polylines[i]);
      #endif
      num_pass++;
    } else {
      #ifdef POLYLINE_VERBOSE
      Serial.print(i);
      Serial.print(": FAIL ");
      Serial.print(s);
      Serial.print(" != ");
      Serial.println(test_polylines[i]);
      #endif
      num_fail++;
    }
  }

  Serial.println("\n\n---------");
  Serial.println("Test results: ");
  Serial.println("(result) - (expected)");
  for(uint8_t i=0; i<num_elements; i++) {
    Serial.print(test_results[i]);
    Serial.print(" - ");
    Serial.print(test_polylines[i]);
    Serial.print("\n");
  }
  Serial.print("PASS: ");
  Serial.print(num_pass);
  Serial.print(" FAIL: ");
  Serial.println(num_fail);
  if(num_fail == 0) {
    Serial.println("TEST SUCCESS\n");
  }
  
}


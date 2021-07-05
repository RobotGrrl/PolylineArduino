# Polyline Library

Encode latitude/longitude coordinates to a Polyline string.

Based on the algorithm by Google: Polyline encoding is a lossy compression algorithm that allows you to store a series of coordinates as a single string.

[Read more about Polyline encoding by Google here](https://developers.google.com/maps/documentation/utilities/polylineutility)

**Version 1.0.0**


## Usage

*Note: Presently only encoding is implemented, as this would be the most common use case on embedded devices.*

Include the library in your sketch:
#include <Polyline.h>

Instantiate your polyline:
Polyline mypolyline;

Encode your points:
String s = mypolyline.encodePolylinePoint(100.0, 100.0);

Alternatively, encode a single value:
String s = mypolyline.encodePolylineVal(100.0);


## Examples

### PolylinePoint

**Expected output:**

		Encoded polyline for point (100.00,100.00) = _gjaR_gjaR


### PolylineTest

**Expected output:**

		Test results: 
		(result) - (expected)
		`~oia@ - `~oia@
		_p~iF - _p~iF
		~ps|U - ~ps|U
		_ulL - _ulL
		nnqC - nnqC
		_mqN - _mqN
		vxq`@ - vxq`@
		PASS: 7 FAIL: 0
		TEST SUCCESS


## Debug

* Uncomment #define POLYLINE_VERBOSE in Polyline.h
* Run the unit test cases in PolylineTest.ino


## Questions

Feel free to open a Github issue if there's anything broken.


## Credit

The Polyline encoding algorithm is by Google.

This library was developed thanks to a learning project at the Bayview Yards Prototyping Lab.



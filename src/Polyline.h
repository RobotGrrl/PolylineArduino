/*
 * Polyline
 * ---------
 * Encode latitude/longitude coordinates to a Polyline string.
 * Based on the algorithm by Google: Polyline encoding is a 
 * lossy compression algorithm that allows you to store a series
 * of coordinates as a single string.
 * Read more: 
 * https://developers.google.com/maps/documentation/utilities/polylineutility
 *
 * Erin Kennedy <erin.kennedy@thebayviewyards.com>
 * July 5, 2021
 */

#ifndef Polyline_h
#define Polyline_h

#include "Arduino.h"

//#define POLYLINE_VERBOSE // comment this out if you don't want verbose

class Polyline {
	public:
		Polyline();

		String encodePolylinePoint(double lat, double lon);
		String encodePolylineVal(double p);
		void testAlgorithm();

	private:
		double test_vals[7];
		String test_polylines[7];
		String test_results[7];

};

#endif
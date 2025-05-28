#ifndef POINT_H
#define POINT_H

#include <string>
#include <iostream>
#include "Arduino.h"

class Point {

	double	x ;
	double	y ;

  public :
  	Point() : x(0), y(0) {}
  	Point(double x, double y ) { set(x, y ) ; }
  	Point(const Point& p ) { set(p.x, p.y ) ; }

  	double getX() const { return x ; }
  	double getY() const { return y ; }
  	void setX(double x ) { this->x = x ; }
  	void setY(double y ) { this->y = y ; }
  	void set(double x, double y ) { this->x = x ; this->y = y ; }

  	bool isNull() const { return ( x == 0 )&&( y == 0 ) ; }
  	std::string asString() const ;
  	
  	Point&	operator +=(const Point& p ) ;
  	Point& operator *=(double v ) ;
  	bool operator ==(const Point& p ) ;
  	bool operator !=(const Point& p ) ;
  	
  	friend std::ostream& operator <<(std::ostream& s, const Point& p ) ;
} ;

#endif


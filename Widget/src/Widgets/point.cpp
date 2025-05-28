#include "point.h"
#include <sstream>
#include "Arduino.h"

std::string Point::asString() const
{
	std::ostringstream	oss ;
	oss << '(' << x << ',' << y << ')' ;
	return oss.str() ;
}

Point&	Point::operator +=(const Point& p )
{
	set( x + p.x, y + p.y ) ;
	return *this ;
}

Point& Point::operator *=(double v )
{
	set( x * v, y * v ) ;
	return *this ;
}

bool Point::operator ==(const Point& p )
{
	return ( x == p.x )&&( y == p.y ) ;
}

bool Point::operator !=(const Point& p )
{
	return !( *this == p ) ;
}

std::ostream& operator <<(std::ostream& s, const Point& p )
{
	s << p.asString() ;
	return s ;
}


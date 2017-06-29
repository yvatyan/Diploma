#include "Core/Misc.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////// Annotated Rectangle /////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AnnotatedRectangle::AnnotatedRectangle() {
}
AnnotatedRectangle::AnnotatedRectangle(const Rectangle& rect, const RgbaColor rectColor, const std::string& rectCaption)
	: rectangle(rect)
	, color(rectColor)
	, text(rectCaption)
{
}
Rectangle AnnotatedRectangle::GetRectangle() const {
	return rectangle;
}
RgbaColor AnnotatedRectangle::GetRgbaColor() const {
	return color;
}
const std::string& AnnotatedRectangle::GetStdString() const {
	return text;
}
bool AnnotatedRectangle::operator==(const AnnotatedRectangle& rhp) const {
	return (rectangle == rhp.rectangle);
}
bool AnnotatedRectangle::operator!=(const AnnotatedRectangle& rhp) const {
	return !(*this == rhp);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// Annotated Point ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AnnotatedPoint::AnnotatedPoint() {
}
AnnotatedPoint::AnnotatedPoint(const Point& p, const RgbaColor rectColor, const std::string& rectCaption)
	: point(p)
	, color(rectColor)
	, text(rectCaption)
{
}
Point AnnotatedPoint::GetPoint() const {
	return point;
}
RgbaColor AnnotatedPoint::GetRgbaColor() const {
	return color;
}
const std::string& AnnotatedPoint::GetStdString() const {
	return text;
}
bool AnnotatedPoint::operator==(const AnnotatedPoint& rhp) const {
	return (point == rhp.point);
}
bool AnnotatedPoint::operator!=(const AnnotatedPoint& rhp) const {
	return !(*this == rhp);
}

#include "Core/Misc.h"

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
	return (rectangle == rhp.rectangle) || (color == rhp.color && text == rhp.text);
}
bool AnnotatedRectangle::operator!=(const AnnotatedRectangle& rhp) const {
	return !(*this == rhp);
}

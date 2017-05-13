#ifndef MISC_H
#define MISC_H

#include <string>
#include <QColor>
#include <QRect>
#include <QSize>

struct Size {
    size_t height;
    size_t width;
	Size(size_t h = 0, size_t w = 0) {
		height = h;
		width = w;
	}
	QSize QtSize() {
		return QSize(width, height);
	}
	bool operator==(const Size& rhp) const {
		return height == rhp.height && width == rhp.width;
	}
	bool operator!=(const Size& rhp) const {
		return !(*this == rhp);
	}
};
struct Rectangle {
	// |---------------->
	// |   *---------
	// |   |   RR   |
	// |   ----------
	// v
    size_t height;
    size_t width;
    int lt_point_x;
    int lt_point_y;
	Rectangle(int x, int y, size_t h, size_t w) {
		lt_point_x = x;
		lt_point_y = y;
		height = h;
		width = w;
	}
	QRect QtRect() {
		return QRect(lt_point_x, lt_point_y, width, height);
	}
	bool operator==(const Rectangle& rhp) const {
		return height == rhp.height && width == rhp.width && lt_point_x == rhp.lt_point_x && lt_point_y == rhp.lt_point_y;
	}
	bool operator!=(const Rectangle& rhp) const {
		return (*this == rhp);
	}
};
struct RgbaColor {
	uchar R;
	uchar G;
	uchar B;
	uchar A;
	RgbaColor(uchar r = 0, uchar g = 0, uchar b = 0, uchar a = 255) {
		R = r; G = g; B = b; A = a;
	}
	QColor QtColor() {
		return QColor(R, G, B, A);
	}
	bool operator==(const RgbaColor& rhp) const {
		return R == rhp.R && G == rhp.G && B == rhp.B && A == rhp.A;
	}
	bool operator!=(const RgbaColor& rhp) const {
		return !(*this == rhp);
	}
};
class AnnotatedRectangle {
	public:
		AnnotatedRectangle(const Rectangle& rect, const RgbaColor regionColor, const std::string& rectCaption);

		Rectangle GetRectangle() const; 
		RgbaColor GetRgbaColor() const;
		const std::string& GetStdString() const;

		bool operator==(const AnnotatedRectangle& rhp) const;
		bool operator!=(const AnnotatedRectangle& rhp) const;
	private:
		Rectangle   rectangle;
		RgbaColor   color;
		std::string text;
};
struct Point {
	int x;
	int y;
	Point(int x_ = -1, int y_ = -1) {
		x = x_;
		y = y_;
	}
	bool operator==(const Point& point) const {
		return x == point.x && y == point.y;
	}
};
class AnnotatedPoint {
	public:
		AnnotatedPoint();
		AnnotatedPoint(const Point& p, const RgbaColor pointColor, const std::string& pointCaption);

		Point GetPoint() const; 
		RgbaColor GetRgbaColor() const;
		const std::string& GetStdString() const;

		bool operator==(const AnnotatedPoint& rhp) const;
		bool operator!=(const AnnotatedPoint& rhp) const;
	private:
		Point point;
		RgbaColor   color;
		std::string text;
};
#endif // MISC_H

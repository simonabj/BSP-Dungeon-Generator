#pragma once
namespace SMath
{
	enum DIRECTION {
		UP, DOWN, LEFT, RIGHT, IN, OUT
	};

	enum AXIS {
		HORIZ, VERT, DEPTH, COMPLX
	};

	struct Region;
	struct Vector2D;

	struct Vector2D {
		int x, y;

		Vector2D() : x(0), y(0) {}
		Vector2D(int x) : x(x), y(x) {}
		Vector2D(int x, int y) : x(x), y(y) {}

		Vector2D operator+(const Vector2D& a) const {
			return Vector2D(x + a.x, y + a.y);
		}

		Vector2D operator-(const Vector2D& a) const {
			return Vector2D(x + a.x*-1, y + a.y*-1);
		}

		Vector2D operator+(const int& a) const {
			return Vector2D(x + a, y + a);
		}

		Vector2D operator-(const Vector2D& a) const {
			return Vector2D((x*(-1)) - a, (y*(-1)) - a);
		}

		double getLength() {
			return sqrt(pow(x, 2) + pow(y, 2));
		}

		bool inside(Region& reg);
	};

	struct Region {
		Vector2D min, max;

		Region() : 
			min(Vector2D(0,0)),
			max(Vector2D(0,0)) {}

		Region(Vector2D size) :
			min(Vector2D(0, 0)), 
			max(size) {}

		Region(Vector2D min, Vector2D max) :
			min(min), 
			max(max) {}

		Region(int minX, int minY, int maxX, int maxY) :
			min(Vector2D(minX,maxX)), 
			max(Vector2D(minY,maxY)) {}

		Vector2D getSize();

		int Area() {
			return this->getSize().x * this->getSize().y;
		}

		bool inside(Vector2D& pos);
	};

	bool Vector2D::inside(Region& reg) {
		return (x >= reg.min.x && x <= reg.max.x &&
				y >= reg.min.y && y <= reg.max.y
		);
	}

	bool Region::inside(Vector2D& pos) {
		return (pos.x >= min.x && pos.x <= max.x &&
				pos.y >= min.y && pos.y <= max.y
		);
	}

	Vector2D Region::getSize() {
		return Vector2D(max.x - min.x, max.y - min.y);
	}
};


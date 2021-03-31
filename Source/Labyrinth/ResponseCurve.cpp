#include "ResponseCurve.h"

float ResponseCurve::Calculate(CurveType type, float x, float m, float b, float c, float k)
{
	x = FMath::Clamp(x, 0.f, 1.f);
	float res;
	switch (type) {
		case CurveType::Quadratic :
			res = m * pow((x - c), k) + b;
			return FMath::Clamp(res, 0.f, 1.f);
		case CurveType::Logistic :
			res = k / (1 + 1000 * exp(m * (-x + c))) + b;
			return FMath::Clamp(res, 0.f, 1.f);
		default :
			return 0.0f;
	}
		
}

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#include "cx-Diagram.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	void DrawPie(
		DrawingContext const& dctx,
		Coord centerX, 
		Coord centerY,
		Coord radius,
		float startRadianAngle,
		float sweepRadianAngle,
		std::size_t segments,
		Color const& fillColor,
		Color const& lineColor,
		Coord lineSize
	)
	{
		using namespace winrt::Windows::Foundation::Numerics;
		using namespace winrt::Microsoft::Graphics::Canvas::Geometry;

		// 파이의 시작점과 끝점 계산
		Coord x0 = centerX + radius * std::cos(startRadianAngle);
		Coord y0 = centerY + radius * std::sin(startRadianAngle);
		Coord x1 = centerX + radius * std::cos(startRadianAngle + sweepRadianAngle);
		Coord y1 = centerY + radius * std::sin(startRadianAngle + sweepRadianAngle);

		// 파이 조각 경로 생성
		std::vector<float2> points;
		points.push_back(float2{ centerX, centerY });
		points.push_back(float2{ x0, y0 });

		for (std::size_t i = 1; i < segments; ++i)
		{
			float t = static_cast<float>(i) / segments;
			float radianAngle = startRadianAngle + sweepRadianAngle * t;
			Coord x = centerX + radius * std::cos(radianAngle);
			Coord y = centerY + radius * std::sin(radianAngle);
			points.push_back(float2{ x, y });
		}
		points.push_back(float2{ x1, y1 });

		// 경로 닫기
		points.push_back(float2{ centerX, centerY });

		// 파이 조각 geometry 생성	
		auto geometry = CanvasGeometry::CreatePolygon(dctx.Device(), points);

		// 채우기 및 테두리 그리기
		dctx.FillGeometry(geometry, fillColor);
		dctx.DrawGeometry(geometry, lineColor, lineSize);
	}

	void DrawRotatedText(
		DrawingContext const& dctx,
		StringView const& text,
		Coord x, 
		Coord y,
		float radianAngle,
		Color const& color,
		TextFormat const& textFormat
	)
	{
		using namespace winrt::Windows::Foundation::Numerics;

		// 현재 변환 상태 저장
		auto oldTransform = dctx.Transform();

		// 회전 변환 행렬 생성 (기준점 x, y에서 radianAngle만큼 회전)
		auto rotation = make_float3x2_rotation(radianAngle, { x, y });
		dctx.Transform(rotation * oldTransform);

		// 텍스트 출력 (회전된 좌표계에서 기준점에 출력)
		dctx.DrawText(text, x, y, color, textFormat);

		// 변환 상태 복원
		dctx.Transform(oldTransform);
	}
}

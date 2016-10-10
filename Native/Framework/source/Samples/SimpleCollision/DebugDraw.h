#pragma once
#include "Box2D\Common\b2Draw.h"

namespace Library
{
	class Camera;
	class Game;
}
namespace SimpleCollision {
	class DebugDraw :
		public b2Draw, public Library::DrawableGameComponent
	{
	public:
		DebugDraw( Library::Game& game, std::shared_ptr<Library::Camera> camera);
		~DebugDraw();

		void Initialize() override;
		void Draw(const Library::GameTime& gameTime) override;
		void Update(const Library::GameTime& gameTime) override;

		/// Draw a closed polygon provided in CCW order.
		void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

		/// Draw a solid closed polygon provided in CCW order.
		void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

		/// Draw a circle.
		void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override;

		/// Draw a solid circle.
		void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override;

		/// Draw a line segment.
		void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;

		/// Draw a transform. Choose your own length scale.
		/// @param xf a transform.
		void DrawTransform(const b2Transform& xf) override;

		/// Draw a point.
		void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) override;

	private:

		struct CBufferPerObject
		{
			DirectX::XMFLOAT4X4 WorldViewProjection;

			CBufferPerObject() : WorldViewProjection() { }
			CBufferPerObject(const DirectX::XMFLOAT4X4& wvp) : WorldViewProjection(wvp) { }
		};


		Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mConstantBuffer;

		DirectX::XMFLOAT4X4 mWorldMatrix;
		CBufferPerObject mCBufferPerObject;


		std::shared_ptr<Library::Camera> mCamera;
	};

}
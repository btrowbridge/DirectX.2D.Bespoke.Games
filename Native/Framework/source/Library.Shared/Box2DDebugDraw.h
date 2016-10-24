#pragma once

#include "DrawableGameComponent.h"
#include <cstdint>
#include <DirectXMath.h>
#include <wrl.h>
#include <Box2D\Box2D.h>

namespace Library
{
	class Box2DComponent;

	class Box2DDebugDraw final : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(Box2DDebugDraw, DrawableGameComponent)

	public:
		enum DrawOptions
		{
			DrawOptionsShapes = 0x0001,
			DrawOptionsJoints = 0x0002,
			DrawOptionsAABBs = 0x0004,
			DrawOptionsPairs = 0x0008,
			DrawOptionsCenterOfMass = 0x0010,
			DrawOptionsAll = DrawOptionsShapes | DrawOptionsJoints | DrawOptionsAABBs | DrawOptionsPairs | DrawOptionsCenterOfMass
		};

		Box2DDebugDraw(Game& game, const std::shared_ptr<Camera>& camera);

		DrawOptions DrawFlags() const;
		void SetDrawFlags(DrawOptions flags);
		void ToggleDrawingFlag(DrawOptions flag);

		virtual void Initialize() override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		struct VSCBufferPerObject
		{
			DirectX::XMFLOAT4X4 WorldViewProjection;

			VSCBufferPerObject() = default;
			VSCBufferPerObject(const DirectX::XMFLOAT4X4& wvp) :
				WorldViewProjection(wvp) { }
		};

		struct PSCBufferPerObject
		{
			DirectX::XMFLOAT4 Color;

			PSCBufferPerObject() = default;
			PSCBufferPerObject(const DirectX::XMFLOAT4& color) :
				Color(color) { }
		};

		static const std::uint32_t CircleResolution;
		static const std::uint32_t CircleVertexCount;		

		void InitializeCircleVertices();
		void InitializePointVertices();

		void DrawWorld();
		void DrawShape(const b2Fixture& fixture, const DirectX::XMFLOAT4& color, const b2Transform& transform);
		void DrawCircle(const b2Vec2& center, float radius, const DirectX::XMFLOAT4& color, const b2Transform& transform);
		void DrawPolygon(const VertexPosition* vertices, uint32_t vertexCount, const DirectX::XMFLOAT4& color, const b2Transform& transform);
		void DrawLineSegment(const VertexPosition& v1, const VertexPosition& v2, const DirectX::XMFLOAT4& color, const b2Transform& transform);
		void DrawPoint(const VertexPosition& vertex, float size, const DirectX::XMFLOAT4& color, const b2Transform& transform);
		void DrawAABB(const b2AABB& aabb, const DirectX::XMFLOAT4& color);
		void DrawTransform(const b2Transform& transform);
		void DrawJoint(b2Joint& joint);

		Box2DComponent* mPhysicsEngine;
		DrawOptions mDrawFlags;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mPolygonVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mCircleVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mPointVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mPointIndexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVSCBufferPerObject;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mPSCBufferPerObject;
		VSCBufferPerObject mVSCBufferPerObjectData;		
		PSCBufferPerObject mPSCBufferPerObjectData;
		b2Transform mIdentityTransform;
	};
}
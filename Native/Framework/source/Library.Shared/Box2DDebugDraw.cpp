#include "pch.h"
#include "Box2DDebugDraw.h"

using namespace std;
using namespace DirectX;

namespace Library
{
	RTTI_DEFINITIONS(Box2DDebugDraw)

	const uint32_t Box2DDebugDraw::CircleResolution = 16;
	const uint32_t Box2DDebugDraw::CircleVertexCount = Box2DDebugDraw::CircleResolution + 2;	

	Box2DDebugDraw::Box2DDebugDraw(Game& game, const shared_ptr<Camera>& camera) :
		DrawableGameComponent(game, camera),
		mPhysicsEngine(nullptr), mDrawFlags(DrawOptionsShapes)
	{
	}

	Box2DDebugDraw::DrawOptions Box2DDebugDraw::DrawFlags() const
	{
		return mDrawFlags;
	}

	void Box2DDebugDraw::SetDrawFlags(DrawOptions flags)
	{
		mDrawFlags = flags;
	}

	void Box2DDebugDraw::ToggleDrawingFlag(DrawOptions flag)
	{
		int32_t drawFlags = mDrawFlags;

		if (drawFlags & flag)
		{
			drawFlags &= ~flag;
		}
		else
		{
			drawFlags |= flag;
		}

		mDrawFlags = static_cast<Box2DDebugDraw::DrawOptions>(drawFlags);
	}

	void Box2DDebugDraw::Initialize()
	{
		// Retrieve physics engine
		mPhysicsEngine = reinterpret_cast<Box2DComponent*>(mGame->Services().GetService(Box2DComponent::TypeIdClass()));
		assert(mPhysicsEngine != nullptr);

		// Load a compiled vertex shader
		vector<char> compiledVertexShader;
		Utility::LoadBinaryFile(L"Content\\Shaders\\Box2DDebugDrawVS.cso", compiledVertexShader);
		ThrowIfFailed(mGame->Direct3DDevice()->CreateVertexShader(&compiledVertexShader[0], compiledVertexShader.size(), nullptr, mVertexShader.ReleaseAndGetAddressOf()), "ID3D11Device::CreatedVertexShader() failed.");

		// Load a compiled pixel shader
		vector<char> compiledPixelShader;
		Utility::LoadBinaryFile(L"Content\\Shaders\\Box2DDebugDrawPS.cso", compiledPixelShader);
		ThrowIfFailed(mGame->Direct3DDevice()->CreatePixelShader(&compiledPixelShader[0], compiledPixelShader.size(), nullptr, mPixelShader.ReleaseAndGetAddressOf()), "ID3D11Device::CreatedPixelShader() failed.");

		// Create an input layout
		D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		ThrowIfFailed(mGame->Direct3DDevice()->CreateInputLayout(inputElementDescriptions, ARRAYSIZE(inputElementDescriptions), &compiledVertexShader[0], compiledVertexShader.size(), mInputLayout.ReleaseAndGetAddressOf()), "ID3D11Device::CreateInputLayout() failed.");

		// Create a vertex buffer for polygon rendering
		D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
		vertexBufferDesc.ByteWidth = sizeof(VertexPosition) * (b2_maxPolygonVertices + 1);
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&vertexBufferDesc, nullptr, mPolygonVertexBuffer.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");	

		InitializeCircleVertices();
		InitializePointVertices();

		// Create a per-object constant buffer for the vertex shader
		D3D11_BUFFER_DESC constantBufferDesc = { 0 };
		constantBufferDesc.ByteWidth = sizeof(VSCBufferPerObject);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&constantBufferDesc, nullptr, mVSCBufferPerObject.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");

		// Create a per-object constant buffer for the pixel shader
		constantBufferDesc.ByteWidth = sizeof(PSCBufferPerObject);
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&constantBufferDesc, nullptr, mPSCBufferPerObject.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");

		mIdentityTransform.SetIdentity();
	}

	void Box2DDebugDraw::Draw(const GameTime& gameTime)
	{
		UNREFERENCED_PARAMETER(gameTime);

		ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();		
		direct3DDeviceContext->IASetInputLayout(mInputLayout.Get());
		direct3DDeviceContext->IASetIndexBuffer(mPointIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		direct3DDeviceContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
		direct3DDeviceContext->PSSetShader(mPixelShader.Get(), nullptr, 0);
		direct3DDeviceContext->VSSetConstantBuffers(0, 1, mVSCBufferPerObject.GetAddressOf());
		direct3DDeviceContext->PSSetConstantBuffers(0, 1, mPSCBufferPerObject.GetAddressOf());

		DrawWorld();
	}

	void Box2DDebugDraw::InitializeCircleVertices()
	{
		static const float increment = XM_2PI / CircleResolution;

		VertexPosition vertices[CircleVertexCount];

		for (int i = 0; i < CircleResolution; i++)
		{
			VertexPosition& vertex = vertices[i];

			vertex.Position.x = cosf(i * increment);
			vertex.Position.y = sinf(i * increment);
			vertex.Position.z = 0.0f;
			vertex.Position.w = 1.0f;
		}

		// Closing line to complete the circle
		vertices[CircleResolution] = VertexPosition(vertices[0]);

		// Axis line for visualizing rotation
		vertices[CircleResolution + 1] = VertexPosition(XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

		// Create a vertex buffer for circle rendering
		D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
		vertexBufferDesc.ByteWidth = sizeof(VertexPosition) * CircleVertexCount;
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData = { 0 };
		vertexSubResourceData.pSysMem = vertices;
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, mCircleVertexBuffer.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");
	}

	void Box2DDebugDraw::InitializePointVertices()
	{
		VertexPosition vertices[] =
		{
			VertexPosition(XMFLOAT4(-1.0f, -1.0f, 0.0f, 1.0f)),
			VertexPosition(XMFLOAT4(-1.0f, +1.0f, 0.0f, 1.0f)),
			VertexPosition(XMFLOAT4(+1.0f, +1.0f, 0.0f, 1.0f)),
			VertexPosition(XMFLOAT4(+1.0f, -1.0f, 0.0f, 1.0f))
		};

		// Create a vertex buffer for point rendering
		D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
		vertexBufferDesc.ByteWidth = sizeof(VertexPosition) * ARRAYSIZE(vertices);
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData = { 0 };
		vertexSubResourceData.pSysMem = vertices;
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, mPointVertexBuffer.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");

		uint32_t indices[] =
		{
			0, 1, 2,
			0, 2, 3
		};

		D3D11_BUFFER_DESC indexBufferDesc = { 0 };
		indexBufferDesc.ByteWidth = sizeof(uint32_t) * ARRAYSIZE(indices);
		indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA indexSubResourceData = { 0 };
		indexSubResourceData.pSysMem = indices;
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&indexBufferDesc, &indexSubResourceData, mPointIndexBuffer.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");
	}

	void Box2DDebugDraw::DrawWorld()
	{
		b2World& world = mPhysicsEngine->World();

		if (mDrawFlags & DrawOptionsShapes)
		{
			for (const b2Body* body = world.GetBodyList(); body != nullptr; body = body->GetNext())
			{
				const b2Transform& transform = body->GetTransform();
				for (const b2Fixture* fixture = body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext())
				{
					if (body->IsActive() == false)
					{
						static const XMFLOAT4 color(0.5f, 0.5f, 0.3f, 1.0f);
						DrawShape(*fixture, color, transform);
					}
					else if (body->GetType() == b2_staticBody)
					{
						static const XMFLOAT4 color(0.5f, 0.9f, 0.5f, 1.0f);
						DrawShape(*fixture, color, transform);
					}
					else if (body->GetType() == b2_kinematicBody)
					{
						static const XMFLOAT4 color(0.5f, 0.5f, 0.9f, 1.0f);
						DrawShape(*fixture, color, transform);
					}
					else if (body->IsAwake() == false)
					{
						static const XMFLOAT4 color(0.6f, 0.6f, 0.6f, 1.0f);
						DrawShape(*fixture, color, transform);
					}
					else
					{
						static const XMFLOAT4 color(0.9f, 0.7f, 0.7f, 1.0f);
						DrawShape(*fixture, color, transform);
					}
				}
			}
		}

		if (mDrawFlags & DrawOptionsJoints)
		{
			for (b2Joint* joint = world.GetJointList(); joint != nullptr; joint = joint->GetNext())
			{
				DrawJoint(*joint);
			}
		}

		if (mDrawFlags & DrawOptionsAABBs)
		{
			static const XMFLOAT4 color(0.9f, 0.3f, 0.9f, 1.0f);

			for (const b2Body* body = world.GetBodyList(); body != nullptr; body = body->GetNext())
			{
				if (body->IsActive() == false)
				{
					continue;
				}

				for (const b2Fixture* fixture = body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext())
				{					
					for (int32_t childIndex = 0; childIndex < fixture->GetShape()->GetChildCount(); childIndex++)
					{
						const b2AABB& aabb = fixture->GetAABB(childIndex);
						DrawAABB(aabb, color);
					}
				}
			}
		}

		if (mDrawFlags & DrawOptionsPairs)
		{
			//TODO:
		}

		if (mDrawFlags & DrawOptionsCenterOfMass)
		{
			for (const b2Body* body = world.GetBodyList(); body != nullptr; body = body->GetNext())
			{
				b2Transform transform = body->GetTransform();
				transform.p = body->GetWorldCenter();
				DrawTransform(transform);
			}
		}
	}

	void Box2DDebugDraw::DrawShape(const b2Fixture& fixture, const XMFLOAT4& color, const b2Transform& transform)
	{
		switch (fixture.GetType())
		{
			case b2Shape::e_circle:
			{
				const b2CircleShape* circle = static_cast<const b2CircleShape*>(fixture.GetShape());
				DrawCircle(circle->m_p, circle->m_radius, color, transform);
			}
			break;

			case b2Shape::e_edge:
			{
				const b2EdgeShape* edge = static_cast<const b2EdgeShape*>(fixture.GetShape());
				const VertexPosition vertex1(XMFLOAT4(edge->m_vertex1.x, edge->m_vertex1.y, 0.0f, 1.0f));
				const VertexPosition vertex2(XMFLOAT4(edge->m_vertex2.x, edge->m_vertex2.y, 0.0f, 1.0f));
				DrawLineSegment(vertex1, vertex2, color, transform);
			}
			break;

			case b2Shape::e_chain:
			{
				const b2ChainShape* chain = static_cast<const b2ChainShape*>(fixture.GetShape());				
				const b2Vec2* vertices = chain->m_vertices;

				const XMFLOAT4 ghostColor(0.75f * color.x, 0.75f * color.y, 0.75f * color.z, color.w);

				const float pointSize = 0.1f;
				const b2Vec2& v1 = vertices[0];
				VertexPosition vertex1(XMFLOAT4(v1.x, v1.y, 0.0f, 1.0f));
				DrawPoint(vertex1, pointSize, color, transform);

				if (chain->m_hasPrevVertex)
				{
					VertexPosition previousVertex(XMFLOAT4(chain->m_prevVertex.x, chain->m_prevVertex.y, 0.0f, 1.0f));
					DrawLineSegment(previousVertex, vertex1, ghostColor, transform);

					const float vertexRadius = 0.1f;
					DrawCircle(chain->m_prevVertex, vertexRadius, ghostColor, transform);
				}

				for (int32_t i = 1; i < chain->m_count; ++i)
				{
					const b2Vec2& v2 = vertices[i];
					VertexPosition vertex2(XMFLOAT4(v2.x, v2.y, 0.0f, 1.0f));
					DrawLineSegment(vertex1, vertex2, color, transform);
					DrawPoint(vertex2, pointSize, color, transform);
					vertex1 = vertex2;
				}

				if (chain->m_hasNextVertex)
				{
					VertexPosition nextVertex(XMFLOAT4(chain->m_nextVertex.x, chain->m_nextVertex.y, 0.0f, 1.0f));
					DrawLineSegment(vertex1, nextVertex, ghostColor, transform);

					const float vertexRadius = 0.1f;
					DrawCircle(chain->m_nextVertex, vertexRadius, ghostColor, transform);
				}
			}
			break;

			case b2Shape::e_polygon:
			{
				const b2PolygonShape* poly = static_cast<const b2PolygonShape*>(fixture.GetShape());
				int32_t shapeVertexCount = poly->m_count;
				b2Assert(shapeVertexCount <= b2_maxPolygonVertices);

				VertexPosition vertices[b2_maxPolygonVertices + 1];
				for (int32_t i = 0; i < shapeVertexCount; ++i)
				{
					const b2Vec2& position = poly->m_vertices[i];
					vertices[i] = VertexPosition(XMFLOAT4(position.x, position.y, 0.0f, 1.0f));
				}

				vertices[shapeVertexCount] = VertexPosition(vertices[0]);
				uint32_t totalVertexCount = shapeVertexCount + 1;

				DrawPolygon(vertices, totalVertexCount, color, transform);
			}
			break;
		}
	}

	void Box2DDebugDraw::DrawCircle(const b2Vec2& center, float radius, const XMFLOAT4& color, const b2Transform& transform)
	{
		ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();	
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		
		const XMMATRIX world = XMMatrixScaling(radius, radius, radius) * XMMatrixRotationZ(transform.q.GetAngle()) * XMMatrixTranslation(center.x + transform.p.x, center.y + transform.p.y, 0.0f);
		const XMMATRIX wvp = XMMatrixTranspose(world * mCamera->ViewProjectionMatrix());
		XMStoreFloat4x4(&mVSCBufferPerObjectData.WorldViewProjection, wvp);
		direct3DDeviceContext->UpdateSubresource(mVSCBufferPerObject.Get(), 0, nullptr, &mVSCBufferPerObjectData, 0, 0);

		mPSCBufferPerObjectData.Color = color;
		direct3DDeviceContext->UpdateSubresource(mPSCBufferPerObject.Get(), 0, nullptr, &mPSCBufferPerObjectData, 0, 0);

		static const UINT stride = sizeof(VertexPosition);
		static const UINT offset = 0;
		direct3DDeviceContext->IASetVertexBuffers(0, 1, mCircleVertexBuffer.GetAddressOf(), &stride, &offset);

		direct3DDeviceContext->Draw(CircleVertexCount, 0);
	}

	void Box2DDebugDraw::DrawPolygon(const VertexPosition* vertices, uint32_t vertexCount, const XMFLOAT4& color, const b2Transform& transform)
	{
		ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		D3D11_MAPPED_SUBRESOURCE mappedSubResource;
		direct3DDeviceContext->Map(mPolygonVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &mappedSubResource);
		memcpy(mappedSubResource.pData, vertices, sizeof(VertexPosition) * vertexCount);
		direct3DDeviceContext->Unmap(mPolygonVertexBuffer.Get(), 0);

		const XMMATRIX world = XMMatrixRotationZ(transform.q.GetAngle()) * XMMatrixTranslation(transform.p.x, transform.p.y, 0.0f);
		const XMMATRIX wvp = XMMatrixTranspose(world * mCamera->ViewProjectionMatrix());
		XMStoreFloat4x4(&mVSCBufferPerObjectData.WorldViewProjection, wvp);
		direct3DDeviceContext->UpdateSubresource(mVSCBufferPerObject.Get(), 0, nullptr, &mVSCBufferPerObjectData, 0, 0);

		mPSCBufferPerObjectData.Color = color;
		direct3DDeviceContext->UpdateSubresource(mPSCBufferPerObject.Get(), 0, nullptr, &mPSCBufferPerObjectData, 0, 0);

		static const UINT stride = sizeof(VertexPosition);
		static const UINT offset = 0;
		direct3DDeviceContext->IASetVertexBuffers(0, 1, mPolygonVertexBuffer.GetAddressOf(), &stride, &offset);

		direct3DDeviceContext->Draw(vertexCount, 0);
	}

	void Box2DDebugDraw::DrawLineSegment(const VertexPosition& v1, const VertexPosition& v2, const XMFLOAT4& color, const b2Transform& transform)
	{
		ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		const VertexPosition vertices[] = { v1, v2 };		
		uint32_t vertexCount = ARRAYSIZE(vertices);
		D3D11_MAPPED_SUBRESOURCE mappedSubResource;
		direct3DDeviceContext->Map(mPolygonVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &mappedSubResource);
		memcpy(mappedSubResource.pData, vertices, sizeof(VertexPosition) * vertexCount);
		direct3DDeviceContext->Unmap(mPolygonVertexBuffer.Get(), 0);

		const XMMATRIX world = XMMatrixRotationZ(transform.q.GetAngle()) * XMMatrixTranslation(transform.p.x, transform.p.y, 0.0f);
		const XMMATRIX wvp = XMMatrixTranspose(world * mCamera->ViewProjectionMatrix());
		XMStoreFloat4x4(&mVSCBufferPerObjectData.WorldViewProjection, wvp);
		direct3DDeviceContext->UpdateSubresource(mVSCBufferPerObject.Get(), 0, nullptr, &mVSCBufferPerObjectData, 0, 0);

		mPSCBufferPerObjectData.Color = color;
		direct3DDeviceContext->UpdateSubresource(mPSCBufferPerObject.Get(), 0, nullptr, &mPSCBufferPerObjectData, 0, 0);

		static const UINT stride = sizeof(VertexPosition);
		static const UINT offset = 0;
		direct3DDeviceContext->IASetVertexBuffers(0, 1, mPolygonVertexBuffer.GetAddressOf(), &stride, &offset);

		direct3DDeviceContext->Draw(vertexCount, 0);
	}

	void Box2DDebugDraw::DrawPoint(const VertexPosition& vertex, float size, const XMFLOAT4& color, const b2Transform& transform)
	{
		ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		const XMMATRIX world = XMMatrixScaling(size, size, size) * XMMatrixRotationZ(transform.q.GetAngle()) * XMMatrixTranslation(transform.p.x + vertex.Position.x, transform.p.y + vertex.Position.y, 0.0f);
		const XMMATRIX wvp = XMMatrixTranspose(world * mCamera->ViewProjectionMatrix());
		XMStoreFloat4x4(&mVSCBufferPerObjectData.WorldViewProjection, wvp);
		direct3DDeviceContext->UpdateSubresource(mVSCBufferPerObject.Get(), 0, nullptr, &mVSCBufferPerObjectData, 0, 0);

		mPSCBufferPerObjectData.Color = color;
		direct3DDeviceContext->UpdateSubresource(mPSCBufferPerObject.Get(), 0, nullptr, &mPSCBufferPerObjectData, 0, 0);

		static const UINT stride = sizeof(VertexPosition);
		static const UINT offset = 0;
		direct3DDeviceContext->IASetVertexBuffers(0, 1, mPointVertexBuffer.GetAddressOf(), &stride, &offset);

		static const uint32_t indexCount = 6;
		direct3DDeviceContext->DrawIndexed(indexCount, 0, 0);
	}

	void Box2DDebugDraw::DrawAABB(const b2AABB& aabb, const XMFLOAT4& color)
	{
		static const uint32_t vertexCount = 5;
		const VertexPosition vertices[vertexCount] =
		{
			VertexPosition(XMFLOAT4(aabb.lowerBound.x, aabb.lowerBound.y, 0.0f, 1.0f)),
			VertexPosition(XMFLOAT4(aabb.upperBound.x, aabb.lowerBound.y, 0.0f, 1.0f)),
			VertexPosition(XMFLOAT4(aabb.upperBound.x, aabb.upperBound.y, 0.0f, 1.0f)),
			VertexPosition(XMFLOAT4(aabb.lowerBound.x, aabb.upperBound.y, 0.0f, 1.0f)),
			VertexPosition(XMFLOAT4(aabb.lowerBound.x, aabb.lowerBound.y, 0.0f, 1.0f)),
		};

		DrawPolygon(vertices, vertexCount, color, mIdentityTransform);
	}

	void Box2DDebugDraw::DrawTransform(const b2Transform& transform)
	{
		static const float axisScale = 0.4f;
		static const XMFLOAT4 red = XMFLOAT4(&Colors::Red[0]);
		static const XMFLOAT4 green = XMFLOAT4(&Colors::LightGreen[0]);

		const b2Vec2 p1 = transform.p;
		b2Vec2 p2 = p1 + axisScale * transform.q.GetXAxis();
		const XMFLOAT4 vertex1(p1.x, p1.y, 0.0f, 1.0f);
		XMFLOAT4 vertex2(p2.x, p2.y, 0.0f, 1.0f);
		DrawLineSegment(vertex1, vertex2, red, mIdentityTransform);

		p2 = p1 + axisScale * transform.q.GetYAxis();
		vertex2 = XMFLOAT4(p2.x, p2.y, 0.0f, 1.0f);
		DrawLineSegment(vertex1, vertex2, green, mIdentityTransform);
	}

	void Box2DDebugDraw::DrawJoint(b2Joint& joint)
	{
		static const XMFLOAT4 color(0.5f, 0.8f, 0.8f, 1.0f);

		const b2Transform& x1 = joint.GetBodyA()->GetTransform();
		const b2Transform& x2 = joint.GetBodyB()->GetTransform();
		const XMFLOAT4 transform1(x1.p.x, x1.p.y, 0.0f, 1.0f);
		const XMFLOAT4 transform2(x2.p.x, x2.p.y, 0.0f, 1.0f);

		const b2Vec2 v1 = joint.GetAnchorA();
		const b2Vec2 v2 = joint.GetAnchorB();
		const XMFLOAT4 vertex1(v1.x, v1.y, 0.0f, 1.0f);
		const XMFLOAT4 vertex2(v2.x, v2.y, 0.0f, 1.0f);

		switch (joint.GetType())
		{
			case e_distanceJoint:
			{
				DrawLineSegment(vertex1, vertex2, color, mIdentityTransform);
				break;
			}

			case e_pulleyJoint:
			{
				b2PulleyJoint& pulley = static_cast<b2PulleyJoint&>(joint);
				const b2Vec2 s1 = pulley.GetGroundAnchorA();
				const b2Vec2 s2 = pulley.GetGroundAnchorB();
				const XMFLOAT4 anchor1(s1.x, s1.y, 0.0f, 1.0f);
				const XMFLOAT4 anchor2(s2.x, s2.y, 0.0f, 1.0f);

				DrawLineSegment(anchor1, vertex1, color, mIdentityTransform);
				DrawLineSegment(anchor2, vertex2, color, mIdentityTransform);
				DrawLineSegment(anchor1, anchor2, color, mIdentityTransform);
				break;
			}			

			case e_mouseJoint:
			{
				// Draw a line for the mouse joint
				b2MouseJoint& mouseJoint = static_cast<b2MouseJoint&>(joint);
				const b2Vec2 p1 = mouseJoint.GetAnchorB();
				const b2Vec2 p2 = mouseJoint.GetTarget();
				const XMFLOAT4 point1(p1.x, p1.y, 0.0f, 1.0f);
				const XMFLOAT4 point2(p2.x, p2.y, 0.0f, 1.0f);

				static const XMFLOAT4 pointColor(&Colors::Red[0]);
				const float pointSize = 0.1f;
				DrawPoint(point1, pointSize, color, mIdentityTransform);
				DrawPoint(point2, pointSize, color, mIdentityTransform);

				static const XMFLOAT4 lineColor(0.8f, 0.8f, 0.8f, 1.0f);
				DrawLineSegment(point1, point2, color, mIdentityTransform);
				break;
			}			

			default:
			{
				DrawLineSegment(transform1, vertex1, color, mIdentityTransform);
				DrawLineSegment(vertex1, vertex2, color, mIdentityTransform);
				DrawLineSegment(transform2, vertex2, color, mIdentityTransform);
				break;
			}
		}
	}
}
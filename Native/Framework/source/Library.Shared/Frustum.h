#pragma once

#include "Ray.h"
#include <DirectXMath.h>

namespace Library
{
	enum class FrustumPlane
	{
		Near = 0,
		Far,
		Left,
		Right,
		Top,
		Bottom
	};

    class Frustum final : public RTTI
    {
		RTTI_DECLARATIONS(Frustum, RTTI)

    public:
        Frustum(DirectX::CXMMATRIX matrix);
		Frustum(const Frustum&) = default;
		Frustum& operator=(const Frustum&) = default;
		Frustum(Frustum&&) = default;
		Frustum& operator=(Frustum&&) = default;
		~Frustum() = default;
		
		const DirectX::XMFLOAT4& Near() const;
		const DirectX::XMFLOAT4& Far() const;
		const DirectX::XMFLOAT4& Left() const;		
		const DirectX::XMFLOAT4& Right() const;
		const DirectX::XMFLOAT4& Top() const;
		const DirectX::XMFLOAT4& Bottom() const;		

		DirectX::XMVECTOR NearVector() const;
		DirectX::XMVECTOR FarVector() const;
		DirectX::XMVECTOR LeftVector() const;
		DirectX::XMVECTOR RightVector() const;
		DirectX::XMVECTOR TopVector() const;
		DirectX::XMVECTOR BottomVector() const;
        
		const DirectX::XMFLOAT3* Corners() const;

		DirectX::XMMATRIX Matrix() const;
		void SetMatrix(DirectX::CXMMATRIX matrix);
        void SetMatrix(const DirectX::XMFLOAT4X4& matrix);
    
	private:
		static const std::int32_t CornerCount = 8;
		static const std::int32_t PlaneCount = 6;

		static Ray ComputeIntersectionLine(DirectX::FXMVECTOR p1, DirectX::FXMVECTOR p2);
		static DirectX::XMVECTOR ComputeIntersection(DirectX::FXMVECTOR& plane, Ray& ray);

        DirectX::XMFLOAT4X4 mMatrix;
		DirectX::XMFLOAT3 mCorners[CornerCount];
		DirectX::XMFLOAT4 mPlanes[PlaneCount];
    };
}


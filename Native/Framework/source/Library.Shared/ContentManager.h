#pragma once

namespace Library
{
	class ContentManager
	{
	public:
		static void Initialize(ID3D11Device* direct3DDevice);
		static void Shutdown();

		ContentManager() = delete;
		ContentManager(const ContentManager&) = delete;
		ContentManager& operator=(const ContentManager&) = delete;
		ContentManager(ContentManager&&) = delete;
		ContentManager& operator=(ContentManager&&) = delete;
		~ContentManager() = default;
	};
}
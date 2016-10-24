#pragma once

#include "RTTI.h"
#include "GameException.h"
#include "StringHelper.h"
#include <memory>
#include <map>
#include <algorithm>

namespace Library
{
	class Game;

	class ContentManager final
	{
	public:
		ContentManager(Library::Game& game, const std::wstring& rootDirectory = DefaultRootDirectory);
		ContentManager(const ContentManager&) = delete;
		ContentManager& operator=(const ContentManager&) = delete;
		ContentManager(ContentManager&&) = delete;
		ContentManager& operator=(ContentManager&&) = delete;
		~ContentManager() = default;

		const std::map<std::wstring, std::shared_ptr<RTTI>>& LoadedAssets() const;
		const std::wstring& RootDirectory() const;
		void SetRootDirectory(const std::wstring& rootDirectory);

		template <typename T>
		std::shared_ptr<T> Load(const std::wstring& assetName);

	private:
		static const std::wstring DefaultRootDirectory;
		std::shared_ptr<RTTI> ReadAsset(const std::int64_t targetTypeId, const std::wstring& assetName);

		Library::Game& mGame;
		std::map<std::wstring, std::shared_ptr<RTTI>> mLoadedAssets;
		std::wstring mRootDirectory;
	};
}

#include "ContentManager.inl"
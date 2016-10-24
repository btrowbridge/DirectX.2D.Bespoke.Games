#pragma once

namespace Library
{
	inline const std::map<std::wstring, std::shared_ptr<RTTI>>& ContentManager::LoadedAssets() const
	{
		return mLoadedAssets;
	}

	inline const std::wstring& ContentManager::RootDirectory() const
	{
		return mRootDirectory;
	}

	inline void ContentManager::SetRootDirectory(const std::wstring& rootDirectory)
	{
		mRootDirectory = rootDirectory + (StringHelper::EndsWith(rootDirectory, L"\\") ? std::wstring() : L"\\");
	}

	template<typename T>
	inline std::shared_ptr<T> ContentManager::Load(const std::wstring& assetName)
	{
		std::map<std::wstring, std::shared_ptr<RTTI>>::iterator it = mLoadedAssets.find(assetName);
		if (it != mLoadedAssets.end())
		{
			return std::static_pointer_cast<T>(it->second);
		}	
		
		uint64_t targetTypeId = T::TypeIdClass();
		std::shared_ptr<RTTI> asset = ReadAsset(targetTypeId, mRootDirectory + assetName);
		mLoadedAssets[assetName] = asset;

		return std::static_pointer_cast<T>(asset);
	}
}
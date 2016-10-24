#include "pch.h"
#include "ContentManager.h"

using namespace std;

namespace Library
{
	const wstring ContentManager::DefaultRootDirectory = L"Content\\";

	ContentManager::ContentManager(Game& game, const wstring& rootDirectory) :
		mGame(game), mRootDirectory(rootDirectory)
	{
	}

	shared_ptr<RTTI> ContentManager::ReadAsset(const int64_t targetTypeId, const wstring& assetName)
	{
		const auto& contentTypeReaders = ContentTypeReaderManager::ContentTypeReaders();
		auto it = contentTypeReaders.find(targetTypeId);
		if (it == contentTypeReaders.end())
		{
			throw GameException("Content type reader not registered.");
		}

		auto& reader = it->second;
		return reader->Read(assetName);
	}
}
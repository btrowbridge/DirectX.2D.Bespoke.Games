#include "pch.h"
#include "CppUnitTest.h"
#include "Quaternion.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace LibraryDesktopTests
{		
	TEST_CLASS(QuaternionTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}

		TEST_METHOD(TestAdd)
		{
			// TODO
		}

		TEST_METHOD(TestToString)
		{
			// TODO
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState QuaternionTest::sStartMemState;
}
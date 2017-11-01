#include "stdafx.h"
#include "CppUnitTest.h"
#include "symbols/SymbolRepository.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace PerfFlow;

namespace PerfFlowTests
{


	TEST_CLASS(ThreadSampleTests)
	{

		TEST_METHOD(emptyOnConstruction)
		{
			SymbolRepository repo;

			Assert::AreEqual(size_t(0), repo.count());
		}

		TEST_METHOD(canAddSymbols)
		{
			SymbolRepository repo;

			for (size_t i = 0; i < 10; i++)
			{
				repo.add(SymbolId(i), std::to_string(i), 0);

				Assert::AreEqual(i + 1, repo.count());
			}
		}

		TEST_METHOD(canRetrieveSymbol)
		{
			SymbolRepository repo;

			for (size_t i = 0; i < 10; i++)
				repo.add(SymbolId(i), std::to_string(i), 0);

			for (size_t i = 0; i < 10; i++)
			{
				auto symbol = repo.tryGet(SymbolId(i));
				Assert::IsNotNull(symbol);
			}
		}

	};


}
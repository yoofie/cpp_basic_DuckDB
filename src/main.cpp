// Dear ImGui: standalone example application for SDL2 + OpenGL
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// (GL3W is a helper library to access OpenGL functions since there is no standard header to access modern OpenGL functions easily. Alternatives are GLEW, Glad, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs
#include <iostream>
#include <duckdb.hpp>
#include <filesystem>
//#include <windows.h>
//using namespace std;

namespace fs = std::filesystem;

template<typename T>
T getTheType(duckdb::Value & inputValue);

/* ********************************************************
	So it begins()
******************************************************** */
int main(int argc, char**) {
	std::cout << "Helo World\n";
	duckdb::DuckDB db(nullptr);
	duckdb::Connection con(db);
	// Basic create table and insert
	con.Query("CREATE TABLE people(id INTEGER, name VARCHAR, last VARCHAR)");
	con.Query("CREATE TABLE test");
	con.Query("INSERT INTO people VALUES (0,'Mark', 'ooga'), (1, 'Hannes', 'booga')");
	auto result = con.Query("SELECT * FROM people");
	if (result->HasError()) {
		std::cout << "error: " + result->GetError() + "\n";
	} else {
		result->Print();
	}

	// List results columns by name
	for (auto& i : result->names) {
		std::cout << "\t" << i;
	}
	std::cout << "\n";

	std::cout << "\n\nTYPES OF THE COLUMNS\n";
	// Get the Types of the column
	for (auto& i : result->types) {
		std::cout << i.ToString() << " ";
	}

	// Return # of Rows in result set
	auto collectionCount = result->collection.Count();

	// Return # types (columns)
	auto collectiontypes = result->collection.Types();

	std::cout << "\n\nTYPES OF THE COLUMNS II \n";
	for (size_t i = 0; i < collectiontypes.size(); i++) {
		std::cout << collectiontypes.at(i).ToString() << "\n";
	}
	std::cout << "\n";

	std::cout << R"(
		/* ********************************************************
			s2
		******************************************************** */
		)" << "\n";
	auto s2 = con.Query("SELECT * FROM people");

	if (s2->collection.ChunkCount() > 0) {
		for (auto& i : s2->names) {
			std::cout << i << "\t";
		}

		std::cout << "\n";
		// Go Row by Row
		for (size_t resultRow = 0; resultRow < s2->collection.Count(); resultRow++) {
			// Go by column
			for (size_t resultColumn = 0; resultColumn < s2->collection.ColumnCount(); resultColumn++) {
				std::cout << s2->GetValue(resultColumn, resultRow) << "\t";
				auto misc = s2->GetValue(resultColumn, resultRow);

				// Get the type of the value
				auto k = misc.type();
				switch (k.id()) {
					case duckdb::LogicalType::INTEGER: {
							auto x = misc.GetValueUnsafe<int32_t>();
							auto y = misc.GetValue<int32_t>();
							//duckdb::IntegerValue y = misc;
							std::cout << "INT: " << std::to_string(x);
						} break;

					case duckdb::LogicalType::VARCHAR: {
							auto x = misc.GetValueUnsafe<std::string>();
							auto y = misc.GetValue<std::string>();
							//duckdb::IntegerValue y = misc;
							std::cout << "STR:" << x;
						} break;
					default:
						break;
				}

				//int32_t c = getTheType(misc);
			}
			std::cout << "\n";
		}
	} else {
		std::cout << "\nnopes\n";
	}

	std::cout << "\n===\n\n";
	std::cout << R"(
		/* ********************************************************
			DESCRIBE
		******************************************************** */
		)" << "\n";
	auto result2 = con.Query("DESCRIBE;");
	if (result2->HasError()) {
		std::cout << "error: " + result2->GetError() + "\n";
	} else {
		result2->Print();
	}
	return 0;
}


template<typename T>
T getTheType(duckdb::Value & inputValue) {
	T retval;
	switch (inputValue.type().id()) {
		case duckdb::LogicalType::INTEGER: {
				//auto x = inputValue.GetValueUnsafe<int32_t>();
				retval = inputValue.GetValue<int32_t>();
			} break;

		case duckdb::LogicalType::VARCHAR: {
				//auto x = inputValue.GetValueUnsafe<std::string>();
				retval = inputValue.GetValue<std::string>();

			} break;
		default:
			break;
	}

	return retval;
}

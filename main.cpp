#include <fmt/core.h>
#include <memory>
#include <vector>


std::unique_ptr<std::vector<int>> create_vector() {
	auto temp_vec = std::make_unique<std::vector<int>>();

	temp_vec->push_back(1);
	temp_vec->push_back(10);
	temp_vec->push_back(100);

	return temp_vec;
}

int main() {
	fmt::println("Hello, World!");

	auto v = create_vector();
	for (auto i : *v) {
		fmt::println("--> {}", i);
	}
	return 0;
}

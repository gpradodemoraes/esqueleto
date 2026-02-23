#include <iostream>
#include <filesystem>
#include <string>
#include <thread>
#include "BS_thread_pool.hpp"

namespace fs = std::filesystem;

int BSThreadpoolmain() {
	// --- Create thread pool with 10 threads ---
	BS::thread_pool pool(10);
	std::vector<std::future<std::string>> futures;

	int timemilis = 0;
	try {
		for (const auto &entry : fs::directory_iterator("C:\\Users\\gabriel.moraes")) {
			if (entry.is_regular_file()) {
				std::string filename = entry.path().string();
				std::cout << "Pushing file " << filename << "\n";

				timemilis += 50;
				// Submit one task per file
				futures.push_back(pool.submit_task([filename, timemilis]() -> std::string {
					std::this_thread::sleep_for(std::chrono::milliseconds(timemilis));
					// Do work and return a result
					return "Processed: " + filename;
				}));
			}
		}
		std::cout << "Finished reading dir\n";
	} catch (const fs::filesystem_error &e) {
		std::cerr << "Directory error: " << e.what() << "\n";
		return 1;
	}

	// --- Wait for all tasks to complete ---
	// Collect results
	for (auto &f : futures) {
		std::cout << f.get() << "\n";
	}

	std::cout << "\nAll files processed.\n";
	return 0;
}

#include <iostream>
#include <filesystem>
#include <string>
#include <thread>
#include "BS_thread_pool.hpp"

namespace fs = std::filesystem;

int BSThreadpoolmain() {
	// --- Create thread pool with 10 threads ---
	BS::thread_pool pool(10);
	std::mutex mtx;
	std::condition_variable cv;
	std::queue<std::string> finished_threads;
	bool finished = false;

	std::vector<std::future<std::string>> futures;

	int timemilis = 58 * 100;
	try {
		for (const auto &entry : fs::directory_iterator("C:\\Users\\gabriel.moraes")) {
			if (entry.is_regular_file()) {
				std::string filename = entry.path().string();
				std::cout << "Pushing file " << filename << "\n";

				timemilis -= 100;
				// Submit one task per file
				futures.push_back(
					pool.submit_task([timemilis, filename, &mtx, &finished_threads, &cv]() -> std::string {
						std::this_thread::sleep_for(std::chrono::milliseconds(timemilis));
						// Do work and return a result
						std::cout << "Processing " << filename << "\n";
						std::string retval = "(" + std::to_string(timemilis) + ") Processed : " + filename;
						{
							std::lock_guard<std::mutex> lock(mtx);
							finished_threads.push(retval);
						}
						cv.notify_one();
						return retval;
					}));
			}
		}
		std::cout << "Finished reading dir\n";
	} catch (const fs::filesystem_error &e) {
		std::cerr << "Directory error: " << e.what() << "\n";
		return 1;
	}

	std::cout << "Wait for all tasks to complete --- Collect results\n";

	std::thread notify_when_finished([&]() {
		pool.wait();
		{
			std::lock_guard<std::mutex> lock(mtx);
			finished = true;
		}
	});

	while (true) {
		std::unique_lock<std::mutex> lock(mtx);
		cv.wait(lock, [&] { return finished || !finished_threads.empty(); });
		std::cout << "\nhello 3\n";
		// if (finished) break;
		if (finished_threads.empty()) break;
		std::cout << "WHILE " << finished_threads.front() << "\n";
		finished_threads.pop();
		// lock.unlock();
	}
	std::cout << "\nhello 1\n";
	notify_when_finished.join();
	std::cout << "\nhello 2\n";
	for (auto &f : futures) {
		std::cout << "FOR " << f.get() << "\n";
	}

	std::cout << "\nAll files processed.\n";
	return 0;
}

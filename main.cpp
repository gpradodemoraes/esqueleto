#include <iostream>
#include <sstream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <atomic>

std::ostringstream oss;
std::mutex mtx;
std::atomic<bool> done(false);

void writer() {
	std::vector<std::string> words = { "Hello", "from", "the", "writer", "thread!", "Streaming", "words",
									   "one",	"by",	"one", "into",	 "the",		"string",	 "stream." };

	for (const auto &word : words) {
		{
			std::lock_guard<std::mutex> lock(mtx);
			oss << word << " ";
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(400));
	}

	done = true;
}

int main() {
	std::thread writerThread(writer);

	std::string lastSeen;

	std::cout << "Reading stream in real time:\n\n";

	while (!done || oss.str().size() > lastSeen.size()) {
		std::string current;
		{
			std::lock_guard<std::mutex> lock(mtx);
			current = oss.str();
		}

		// Only print newly added content
		if (current.size() > lastSeen.size()) {
			std::cout << current.substr(lastSeen.size()) << std::flush;
			lastSeen = current;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	std::cout << "\n\nDone.\n";

	writerThread.join();
	return 0;
}

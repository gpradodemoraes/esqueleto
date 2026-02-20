#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

std::mutex mtx;
std::condition_variable cv;
std::queue<int> dataQueue;
bool finished = false;

static void producer() {
	for (int i = 0; i < 5; ++i) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		{
			std::lock_guard<std::mutex> lock(mtx);
			dataQueue.push(i);
			std::cout << "Produced: " << i << "\n";
		}
		cv.notify_one(); // Wake up consumer
	}

	{
		std::lock_guard<std::mutex> lock(mtx);
		finished = true;
	}
	cv.notify_one();
}

static void consumer() {
	while (true) {
		std::unique_lock<std::mutex> lock(mtx);
		cv.wait(lock, [] { return !dataQueue.empty() || finished; });

		while (!dataQueue.empty()) {
			int value = dataQueue.front();
			dataQueue.pop();
			std::cout << "Consumed: " << value << "\n";
		}

		if (finished && dataQueue.empty()) {
			break;
		}
	}
}

int thread_main() {
	std::thread prod(producer);
	std::thread cons(consumer);

	prod.join();
	cons.join();

	return 0;
}

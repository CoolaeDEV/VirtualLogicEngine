#pragma once
#include <atomic>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <iostream>

class TickRunner {
public:
	TickRunner(std::function<void()> onTickFn, double frequencyHz);

	void start();
	void stop();
	void stepOnce();
	void setFrequency(double Hz);
	bool isRunning() const;
private:
	std::function<void()> onTick;
	std::atomic<bool> running;
	std::atomic<bool> singleStep;
	std::thread tickThread;
	double frequency;

	std::condition_variable cv;
	std::mutex cvMutex;
	
	void run();
};
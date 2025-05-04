#include "headers/TickRunner.h"

TickRunner::TickRunner(std::function<void()> onTickFn, double frequencyHz)
	: onTick(onTickFn), running(false), singleStep(false), frequency(frequencyHz) {}

void TickRunner::start() {
	if (running) return;
	running = true;

	tickThread = std::thread([this]() {run(); });
    std::cout << "[ TICKRUNNER ] Sim Started\n";
}

void TickRunner::stop() {
	{
		std::lock_guard<std::mutex> lock(cvMutex);
		running = false;
	}
	cv.notify_all();
	if (tickThread.joinable())
		tickThread.join();

    std::cout << "[ TICKRUNNER ] Sim Stopped\n";
}

void TickRunner::stepOnce() {
	singleStep = true;
	if (!running) {
		onTick();
	}
}

void TickRunner::setFrequency(double Hz) {
	frequency = Hz;
}

bool TickRunner::isRunning() const {
	return running;
}

void TickRunner::run() {
    using clock = std::chrono::high_resolution_clock;
    auto tickDuration = std::chrono::duration<double>(1.0 / frequency);

    while (running) {
        auto start = clock::now();

        if (singleStep) {
            singleStep = false;
            onTick();
            running = false;
            break;
        }
        else {
            onTick();
        }

        auto end = clock::now();
        auto elapsed = end - start;
        auto sleepTime = tickDuration - elapsed;

        if (sleepTime.count() > 0) {
            std::unique_lock<std::mutex> lock(cvMutex);
            cv.wait_for(lock, sleepTime, [this]() { return !running; });
        }
    }
}
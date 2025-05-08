#include "headers/TickRunner.h"

TickRunner::TickRunner(std::function<void()> onTickFn, double frequencyHz)
    : onTick(onTickFn), running(false), singleStep(false), frequency(frequencyHz) {}

void TickRunner::start() {
    using clock = std::chrono::high_resolution_clock;

	if (running) return;
	running = true;

    tickThread = std::thread([this]() { run(); });
    std::cout << "[ TICKRUNNER ] Sim Started\n";
    currentTicks = 0;
}

void TickRunner::stop() {
    running = false;
    if (tickThread.joinable())
        tickThread.join();

    std::cout << "[ TICKRUNNER ] Sim Stopped\n";
    //std::cout << "[ TICKRUNNER ] finished sim ticks: " << currentTicks << std::endl;
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

    if (running) {
        auto start = clock::now();

        if (singleStep) {
            singleStep = false;
            onTick();
            running = false;
        }
        else {
            onTick();
        }
        auto end = clock::now();
        auto elapsed = end - start;
        auto sleepTime = tickDuration - elapsed;

        if (sleepTime.count() > 0) {
            std::this_thread::sleep_for(sleepTime);
        }
        
    }
}
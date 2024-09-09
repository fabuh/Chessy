#include "pch.h"
#include "FPSCounter.h"

#include <chrono>
#include <thread>

namespace chessy
{
    FPSCounter::FPSCounter() : frameCount(0), fps(0)
    {
        lastTime = std::chrono::high_resolution_clock::now();
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    void FPSCounter::Update()
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsedTime = currentTime - lastTime;

        ++frameCount;
        if (elapsedTime.count() >= 1.0) { // Every second
            fps = frameCount / elapsedTime.count();
            printf_s("FPS: %lf\n", fps);
            frameCount = 0;
            lastTime = currentTime;
        }

        //if (frameCount >= 60) {
        //    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000.0 - elapsedTime.count() * 1000)));
        //}

		//frames per delta time instead of per second
		//check out how to do it to sleep for the remaining time of the delta instead of the remaining time of the second
    }
}
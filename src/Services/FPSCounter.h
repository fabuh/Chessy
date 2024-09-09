#pragma once

namespace chessy
{
    class FPSCounter
    {
    public:
        FPSCounter();
        ~FPSCounter() = default;
        void Update();

    private:
        std::chrono::high_resolution_clock::time_point lastTime;
        int frameCount;
        double fps;
    };
}
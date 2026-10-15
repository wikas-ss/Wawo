#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

#include <algorithm>
#include <chrono>
#include <cstdint>

using namespace geode::prelude;

namespace {
    using Clock = std::chrono::steady_clock;

    // All counters are only touched on the game thread.
    std::uint64_t samples = 0;
    std::uint64_t overBudget = 0;
    double totalMs = 0.0;
    double maxMs = 0.0;
    double reportElapsed = 0.0;

    constexpr double k60FpsBudgetMs = 1000.0 / 60.0;
    constexpr double kReportIntervalSeconds = 5.0;
}

class $modify(AstralPlayLayer, PlayLayer) {
    void update(float dt) {
        const auto start = Clock::now();

        // Keep the original game update intact.
        PlayLayer::update(dt);

        const auto finish = Clock::now();
        const double elapsedMs =
            std::chrono::duration<double, std::milli>(finish - start).count();

        ++samples;
        totalMs += elapsedMs;
        maxMs = std::max(maxMs, elapsedMs);

        if (elapsedMs > k60FpsBudgetMs) {
            ++overBudget;
        }

        // Guard against invalid/negative dt values.
        if (dt > 0.f) {
            reportElapsed += static_cast<double>(dt);
        }

        if (reportElapsed >= kReportIntervalSeconds) {
            const double averageMs =
                samples > 0 ? totalMs / static_cast<double>(samples) : 0.0;

            log::info(
                "[Astral FPS Optimizer] PlayLayer::update window={:.1f}s "
                "samples={} avg={:.3f}ms max={:.3f}ms over16.67ms={}",
                reportElapsed, samples, averageMs, maxMs, overBudget
            );

            samples = 0;
            overBudget = 0;
            totalMs = 0.0;
            maxMs = 0.0;
            reportElapsed = 0.0;
        }
    }
};

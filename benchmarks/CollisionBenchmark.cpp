// Benchmarks CollisionsHandler's three contact-building strategies:
//   - buildContacts:            naive all-pairs, swept AABB
//   - buildContactsFaster:      spatial hash + swept AABB
//   - buildContactsBlankFaster: spatial hash + direct rect intersection (the one production update() calls)
//
// Run (Release build only -- see README's "Benchmarks" section):
//   ./benchmarks --benchmark_min_time=0.2s

#include <benchmark/benchmark.h>

#include <deque>
#include <random>

#include "events/EventBus.h"
#include "events/Listener.h" // completes ListenerComparator, which EventBus's implicit ctor/dtor needs
#include "physics/2d/CollidableObject.h"
#include "physics/2d/CollisionsHandler.h"
#include "physics/2d/ContactsHandler.h"
#include "utility/EmptyTextures.h"

// Friend of CollisionsHandler (see CollisionsHandler.h) -- exposes its private
// buildContacts/buildContactsFaster/buildContactsBlankFaster methods to this translation unit only.
class CollisionsHandlerBenchmarkAccess {
public:
    static ContactsPtrHashMap buildContacts(const CollisionsHandler& handler, float deltaTime) {
        return handler.buildContacts(deltaTime);
    }

    static ContactsPtrHashMap buildContactsFaster(CollisionsHandler& handler, float deltaTime) {
        return handler.buildContactsFaster(deltaTime);
    }

    static ContactsPtrHashMap buildContactsBlankFaster(CollisionsHandler& handler, float deltaTime) {
        return handler.buildContactsBlankFaster(deltaTime);
    }
};

namespace {

// A "game-sized" play area: roughly 16x8 screens of the shipped 46x30 (8px-tile) rooms in assets/levels,
// i.e. a modestly large explorable multi-room level rather than one screen. Objects are scattered across
// the whole area (not stacked in one spot) so the spatial hash actually partitions the world instead of
// degenerating to a single occupied cell.
constexpr float kWorldWidth = 4000.f;
constexpr float kWorldHeight = 2000.f;

// Hitbox side length range, in pixels: 1-8 tiles (8px tiles), matching the range of terrain/entity sizes
// used throughout src/entity/objects (e.g. OneWayPlatform 24x2, SwapBlock 24x24, TouchSwitch 16x16).
constexpr float kMinHitboxSize = 8.f;
constexpr float kMaxHitboxSize = 64.f;

// Typical platformer speeds (PhysicsObject::MAX_FALL is 240 px/s).
constexpr float kMaxSpeed = 150.f;

// A single physics frame at 60 FPS.
constexpr float kDeltaTime = 1.f / 60.f;

// Fraction of objects that are Movable (players, enemies, items) vs Immovable (terrain). Most of a level
// is static geometry, so we skew towards Immovable.
constexpr float kMovableFraction = 0.3f;

constexpr unsigned kSeed = 42;

struct Scenario {
    EventBus bus;
    CollisionsHandler handler{bus};
    std::deque<CollidableObject> objects; // deque: emplace_back never invalidates existing element addresses,
                                           // which CollidableObject's Hitbox relies on (it stores a reference
                                           // to its owning object's position).
};

std::unique_ptr<Scenario> makeScenario(int count) {
    // ContactsHandler is a process-wide singleton (CollisionsHandler.h/.cpp): buildContactsFaster and
    // buildContactsBlankFaster leave it holding std::reference_wrapper<const CollidableObject> into
    // *this* scenario's objects. The previous benchmark's Scenario (and its CollidableObjects) is already
    // destroyed by the time this runs, so those references are dangling. newFrame() moves
    // contacts -> previous_frame_contacts and clears contacts without dereferencing anything, so calling
    // it twice flushes both maps before the new scenario's objects are used (e.g. by emitPlayerEvents).
    ContactsHandler::getInstance().newFrame();
    ContactsHandler::getInstance().newFrame();

    auto scenario = std::make_unique<Scenario>();

    std::mt19937 rng(kSeed);
    std::uniform_real_distribution<float> posX(0.f, kWorldWidth);
    std::uniform_real_distribution<float> posY(0.f, kWorldHeight);
    std::uniform_real_distribution<float> extent(kMinHitboxSize, kMaxHitboxSize);
    std::uniform_real_distribution<float> speed(-kMaxSpeed, kMaxSpeed);
    std::uniform_real_distribution<float> typeRoll(0.f, 1.f);

    for (int i = 0; i < count; i++) {
        const float w = extent(rng);
        const float h = extent(rng);
        const sf::Vector2f position{posX(rng), posY(rng)};
        const bool movable = typeRoll(rng) < kMovableFraction;
        const auto type = movable ? CollidableObjectType::Movable : CollidableObjectType::Immovable;

        scenario->objects.emplace_back(
            std::vector<sf::FloatRect>{{{0.f, 0.f}, {w, h}}},
            sf::Sprite{EmptyTextures::getInstance().getEmpty(
                {static_cast<unsigned>(w), static_cast<unsigned>(h)})},
            position,
            type,
            1.f);

        auto& object = scenario->objects.back();
        if (movable) {
            object.base_velocity = {speed(rng), speed(rng)};
        }
        scenario->handler.addObject(object);
    }

    return scenario;
}

} // namespace

static void BM_BuildContactsNaive(benchmark::State& state) {
    const int count = static_cast<int>(state.range(0));
    auto scenario = makeScenario(count);

    for (auto _ : state) {
        auto contacts = CollisionsHandlerBenchmarkAccess::buildContacts(scenario->handler, kDeltaTime);
        benchmark::DoNotOptimize(contacts);
        state.PauseTiming();
        scenario->bus.clear(); // drain events queued by this frame so the queue doesn't grow unbounded
        state.ResumeTiming();
    }
    state.SetLabel(std::to_string(count) + " objects");
}
BENCHMARK(BM_BuildContactsNaive)->Arg(100)->Arg(500)->Arg(1000)->Arg(2000)->Unit(benchmark::kMillisecond);

static void BM_BuildContactsFaster(benchmark::State& state) {
    const int count = static_cast<int>(state.range(0));
    auto scenario = makeScenario(count);

    for (auto _ : state) {
        auto contacts = CollisionsHandlerBenchmarkAccess::buildContactsFaster(scenario->handler, kDeltaTime);
        benchmark::DoNotOptimize(contacts);
        state.PauseTiming();
        scenario->bus.clear();
        state.ResumeTiming();
    }
    state.SetLabel(std::to_string(count) + " objects");
}
BENCHMARK(BM_BuildContactsFaster)->Arg(100)->Arg(500)->Arg(1000)->Arg(2000)->Unit(benchmark::kMillisecond);

static void BM_BuildContactsBlankFaster(benchmark::State& state) {
    const int count = static_cast<int>(state.range(0));
    auto scenario = makeScenario(count);

    for (auto _ : state) {
        auto contacts = CollisionsHandlerBenchmarkAccess::buildContactsBlankFaster(scenario->handler, kDeltaTime);
        benchmark::DoNotOptimize(contacts);
        state.PauseTiming();
        scenario->bus.clear();
        state.ResumeTiming();
    }
    state.SetLabel(std::to_string(count) + " objects");
}
BENCHMARK(BM_BuildContactsBlankFaster)->Arg(100)->Arg(500)->Arg(1000)->Arg(2000)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();

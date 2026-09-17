#pragma once

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/resource_loader.hpp"

#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/display_server.hpp>
#include <godot_cpp/classes/input_event_key.hpp>

#include "godot_cpp/variant/utility_functions.hpp"

#include "tetra/Tetra.h"
#include "tetra/TetraBlocks.h"
#include "tetra/TetraWorldImpl.h"

#include <format>

class TetraInputEvent {

    const float mRepeatDwell;

    bool mIsPressed = false;
    double mConsumedAccumulator = 0.0;

public:
    explicit TetraInputEvent(float repeatDwell = .2) :
        mRepeatDwell(repeatDwell) {}

    bool canConsume() {
        if (mIsPressed && mConsumedAccumulator <= 0.0) {
            mConsumedAccumulator = mRepeatDwell;
            return true;
        }
        return false;
    }
    void release() {
        mIsPressed = false;
        mConsumedAccumulator = 0.0;
    }
    void press() {
        mIsPressed = true;
        mConsumedAccumulator = 0.0;
    }
    void subtractDelta(double delta) {
        if (mIsPressed) {
            mConsumedAccumulator -= delta;
        }
    }
};

#pragma mark TetraDisplayBlock ---------------------------------------------------------------------------------------------------------
class TetraDisplayBlock : public godot::Sprite2D {
    GDCLASS(TetraDisplayBlock, godot::Sprite2D);

public:
    void _ready() override {
        if (const auto &currEngine = godot::Engine::get_singleton();
            currEngine->is_editor_hint()) {
            return;
        }
        godot::Ref<godot::Resource> textureRef = godot::ResourceLoader::get_singleton()->load("res://icon.svg");
        set_texture(textureRef);
    }
    void _process(double delta_time) override {
        if (const auto &currEngine = godot::Engine::get_singleton();
            currEngine->is_editor_hint()) {
            return;
        }
    }
    void _enter_tree() override {
    }
    void _exit_tree() override {
    }

protected:
    static void _bind_methods() {
    }
};

#pragma mark TetraPlayRegion -----------------------------------------------------------------------------------------------------------
class TetraPlayRegion : public godot::Node2D {
    GDCLASS(TetraPlayRegion, godot::Node2D);

    static constexpr size_t sMaxX{10};
    static constexpr size_t sMaxY{18};

    std::shared_ptr<TetraBoard> mTetraBoard{};

    TetraDisplayBlock *mBlocks[sMaxX][sMaxY]{};

    float mScaleToSet = 1.0;

    bool mInitialized = false;
    bool mRotateLeft = false;
    bool mRotateRight = false;
    bool mHardPush = false;

    TetraInputEvent mMoveLeft{};
    TetraInputEvent mMoveRight{};

    double mCumulativeTime = 0.0;

public:
    TetraPlayRegion() {
        godot::UtilityFunctions::print("TetraPlayRegion created");
        // rl::resource::preload::packed_scene<rl::Level> level{path::scene::Level1};
    }
    ~TetraPlayRegion() override {
        godot::UtilityFunctions::print("TetraPlayRegion destroyed");
    }

    void _ready() override {
        if (const auto &currEngine = godot::Engine::get_singleton();
            currEngine->is_editor_hint()) {
            return;
        }

        // godot::DisplayServer::get_singleton().screen_get_size()
        // godot::OS::get_singleton().
        mTetraBoard = std::make_shared<TetraBoard>(sMaxX, sMaxY);
        mTetraBoard->initialize();

        for (size_t x = 0; x < sMaxX; x++) {
            for (size_t y = 0; y < sMaxY; y++) {
                auto block = memnew(TetraDisplayBlock);
                add_child(block);
                mBlocks[x][y] = block;
            }
        }
    }

    bool initializeBlocks() {
        for (size_t x = 0; x < sMaxX; x++) {
            for (size_t y = 0; y < sMaxY; y++) {
                if (!mBlocks[x][y]->is_node_ready()) {
                    return false;
                }
            }
        }

        const auto viewportSize = get_viewport()->get_visible_rect().size;

        godot::UtilityFunctions::print(
            std::format("TetraPlayRegion screen viewPortSize:({},{})",
                        static_cast<int>(viewportSize.height),
                        static_cast<int>(viewportSize.width))
                .c_str());

        const auto eachBlockHeight = viewportSize.height / static_cast<float>(sMaxY);
        const auto graphicsScale = eachBlockHeight / mBlocks[0][0]->get_texture()->get_size().height;
        const auto halfHeight = eachBlockHeight / 2;

        godot::UtilityFunctions::print(
            std::format("TetraPlayRegion regionWidth:{:.2f} blockHeight:{:.2f} scale:{:.2f}",
                        eachBlockHeight * sMaxX,
                        eachBlockHeight,
                        graphicsScale)
                .c_str());

        for (size_t x = 0; x < sMaxX; x++) {
            for (size_t y = 0; y < sMaxY; y++) {
                const auto &b = mBlocks[x][y];
                b->set_scale(godot::Vector2{graphicsScale, graphicsScale});
                auto vec = godot::Vector2{(halfHeight + eachBlockHeight * x),
                                          (halfHeight + eachBlockHeight * y)};
                b->set_position(vec);

                // godot::UtilityFunctions::print(std::format("TetraPlayRegion screen {},{} {},{}", x, y, vec.x, vec.y).c_str());
            }
        }

        return true;
    }

    void _input(const godot::Ref<godot::InputEvent> &event) override {
        const auto &inputEvent = godot::Object::cast_to<const godot::InputEvent>(*event);
        mRotateLeft = inputEvent->is_action_pressed("spin-left");
        mRotateRight = inputEvent->is_action_pressed("spin-right");

        if (inputEvent->is_action_pressed("ui_right")) {
            mMoveRight.press();
        } else if (inputEvent->is_action_released("ui_right")) {
            mMoveRight.release();
        }

        if (inputEvent->is_action_pressed("ui_left")) {
            mMoveLeft.press();
        } else if (inputEvent->is_action_released("ui_left")) {
            mMoveLeft.release();
        }

        if (inputEvent->is_action_pressed("ui_up")) {
            mHardPush = true;
        }
    }

    void _process(double delta_time) override {
        if (const auto &currEngine = godot::Engine::get_singleton();
            currEngine->is_editor_hint()) {
            return;
        }

        if (!mInitialized) {
            mInitialized = initializeBlocks();
        }

        mCumulativeTime += delta_time;
        mMoveLeft.subtractDelta(delta_time);
        mMoveRight.subtractDelta(delta_time);

        bool screenNeedsUpdated = false;
        std::vector<TetraInput> inputs{};
        auto godotInput = godot::Input::get_singleton();
        if (mMoveLeft.canConsume()) {
            inputs.emplace_back(moveLeft);
        }
        if (mMoveRight.canConsume()) {
            inputs.emplace_back(moveRight);
        }
        if (godotInput->is_key_pressed(godot::Key::KEY_DOWN)) {
            inputs.emplace_back(moveDown);
        }
        if (mRotateLeft) {
            inputs.emplace_back(spinLeft);
            mRotateLeft = false;
        }
        if (mRotateRight) {
            inputs.emplace_back(spinRight);
            mRotateRight = false;
        }

        if (!inputs.empty()) {
            screenNeedsUpdated = true;
            mTetraBoard->inputReceived(inputs);
        }

        if (mCumulativeTime >= .5 || mHardPush) {
            mCumulativeTime = 0.0;
            mTetraBoard->worldStep(mHardPush);
            mHardPush = false;
            screenNeedsUpdated = true;
        }

        if (screenNeedsUpdated) {
            for (size_t x = 0; x < sMaxX; x++) {
                for (size_t y = 0; y < sMaxY; y++) {
                    const auto &b = mBlocks[x][y];
                    b->set_visible(false);
                }
            }
            for (const auto &[x, y] : mTetraBoard->getCurrentWorld()) {
                mBlocks[x][y]->set_visible(true);
            }
        }
    }

protected:
    static void _bind_methods() {
    }
};

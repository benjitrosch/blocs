#include <blocs/platform/input.h>

using namespace blocs;
using namespace blocs::input;

#ifndef __EMSCRIPTEN__
GamepadPtr      input::gamepads[MAX_NUM_CONTROLLERS];
ControllerState input::controllers[MAX_NUM_CONTROLLERS];
#endif

KeyboardState input::keyboard;
MouseState    input::mouse;

void input::update()
{
    memcpy(
        keyboard.prev_keyboard_state, keyboard.curr_keyboard_state,
        sizeof(u8) * NUM_SCANCODES
    );
    memcpy(
        keyboard.curr_keyboard_state, SDL_GetKeyboardState(NULL),
        sizeof(u8) * NUM_SCANCODES
    );

    mouse.prev_mouse_x     = mouse.curr_mouse_x;
    mouse.prev_mouse_y     = mouse.curr_mouse_y;
    mouse.prev_mouse_state = mouse.curr_mouse_state;
    mouse.curr_mouse_state =
        SDL_GetMouseState(&mouse.curr_mouse_x, &mouse.curr_mouse_y);

#ifndef __EMSCRIPTEN__
    for (i8 i = 0; i < MAX_NUM_CONTROLLERS; i++)
    {
        input::ControllerState& controller = controllers[i];
        if (controller.connected)
        {
            memcpy(
                controller.prev_button_state, controller.curr_button_state,
                sizeof(u8) * MAX_NUM_BUTTONS
            );
            memcpy(
                controller.prev_axis_state, controller.curr_axis_state,
                sizeof(f32) * MAX_NUM_AXIS
            );

            for (i32 j = 0; j < controller.num_buttons; j++)
                controller.curr_button_state[j] = SDL_GameControllerGetButton(
                    gamepads[i], (SDL_GameControllerButton)j
                );

            for (i32 j = 0; j < controller.num_axis; j++)
            {
                f32 value = SDL_GameControllerGetAxis(
                    gamepads[i], (SDL_GameControllerAxis)j
                );

                if (value >= 0)
                    value /= AXIS_RANGE;
                else
                    value /= AXIS_RANGE + 1.f;

                controller.curr_axis_state[j] = value;
            }
        }
    }
#endif
}

void input::events()
{
    mouse.curr_mouse_wheel_x = 0;
    mouse.curr_mouse_wheel_y = 0;
    mouse.prev_mouse_wheel_x = mouse.curr_mouse_wheel_x;
    mouse.prev_mouse_wheel_y = mouse.curr_mouse_wheel_y;
}

bool input::key_pressed(u8 key)
{
    return keyboard.curr_keyboard_state[key] &&
           !keyboard.prev_keyboard_state[key];
}

bool input::key_pressed(Key key)
{
    u8 key_code = (u8)key;
    return input::key_pressed(key_code);
}

bool input::key_down(u8 key) { return keyboard.curr_keyboard_state[key]; }

bool input::key_down(Key key)
{
    u8 key_code = (u8)key;
    return input::key_down(key_code);
}

bool input::key_up(u8 key)
{
    return !keyboard.curr_keyboard_state[key] &&
           keyboard.prev_keyboard_state[key];
}

bool input::key_up(Key key)
{
    u8 key_code = (u8)key;
    return input::key_up(key_code);
}

bool input::shift()
{
    return keyboard.curr_keyboard_state[KEY_LeftShift] ||
           keyboard.curr_keyboard_state[KEY_RightShift];
}

bool input::ctrl()
{
    return keyboard.curr_keyboard_state[KEY_LeftControl] ||
           keyboard.curr_keyboard_state[KEY_RightControl];
}

bool input::alt()
{
    return keyboard.curr_keyboard_state[KEY_LeftAlt] ||
           keyboard.curr_keyboard_state[KEY_RightAlt];
}

bool input::mouse_click(const u32 button)
{
    return ((MOUSE(button) & mouse.curr_mouse_state) != 0) &&
           ((MOUSE(button) & mouse.prev_mouse_state) == 0);
}

bool input::mouse_click(const MouseButton button)
{
    u8 mouse_button = (1 << (((u8)button) - 1));
    return input::mouse_click(mouse_button);
}

bool input::mouse_down(const u32 button)
{
    return (MOUSE(button) & mouse.curr_mouse_state) != 0;
}

bool input::mouse_down(const MouseButton button)
{
    u8 mouse_button = (1 << (((u8)button) - 1));
    return input::mouse_down(mouse_button);
}

bool input::mouse_up(const u32 button)
{
    return ((MOUSE(button) & mouse.curr_mouse_state) == 0) &&
           ((MOUSE(button) & mouse.prev_mouse_state) != 0);
}

bool input::mouse_up(const MouseButton button)
{
    u8 mouse_button = (1 << (((u8)button) - 1));
    return input::mouse_up(mouse_button);
}

point input::mouse_pos()
{
    return point(mouse.curr_mouse_x, mouse.curr_mouse_y);
}

point input::get_mouse_delta()
{
    return point(
        mouse.curr_mouse_x - mouse.prev_mouse_x,
        mouse.curr_mouse_y - mouse.prev_mouse_y
    );
}

i32 input::get_mouse_wheel_delta_x()
{
    return mouse.curr_mouse_wheel_x - mouse.prev_mouse_wheel_x;
}

i32 input::get_mouse_wheel_delta_y()
{
    return mouse.curr_mouse_wheel_y - mouse.prev_mouse_wheel_y;
}

#ifndef __EMSCRIPTEN__
void input::connect_controller(
    i32 index, str name, u16 vendor, u16 product, u16 version, i32 num_buttons,
    i32 num_axis
)
{
    controllers[index].name        = name;
    controllers[index].vendor      = vendor;
    controllers[index].product     = product;
    controllers[index].version     = version;
    controllers[index].num_buttons = num_buttons;
    controllers[index].num_axis    = num_axis;
    controllers[index].connected   = true;

    LOG_DEBUG(
        "CONNECTED: " << name << " (vendor:" << i32(vendor) << ", product:"
                      << i32(product) << ", version:" << i32(version) << ")"
    );
    LOG_DEBUG("num buttons: " << num_buttons << ", num axis: " << num_axis);
}

void input::disconnect_controller(i32 index)
{
    auto ctrl = controllers[index];
    LOG_DEBUG(
        "DISCONNECTED: " << ctrl.name << " (" << i32(ctrl.vendor) << ", "
                         << i32(ctrl.product) << ", " << i32(ctrl.version)
                         << ")"
    );

    controllers[index] = {};
}

i32 input::get_controller_index_by_instance_id(i32 instance_id)
{
    for (i32 i = 0; i < MAX_NUM_CONTROLLERS; i++)
    {
        SDL_GameController* instance = gamepads[i];
        if (instance != nullptr)
        {
            SDL_Joystick* joystick = SDL_GameControllerGetJoystick(instance);
            if (SDL_JoystickInstanceID(joystick) == instance_id)
            {
                return i;
            }
        }
    }

    return -1;
}

bool input::btn_pressed(i32 controller_index, u8 btn)
{
    if (controller_index >= 0 && controllers[controller_index].connected)
    {
        auto ctrl = controllers[controller_index];
        return ctrl.curr_button_state[btn] && !ctrl.prev_button_state[btn];
    }
    else
        return false;
}

bool input::btn_pressed(i32 controller_index, Button btn)
{
    u8 btn_code = (u8)btn;
    return input::btn_pressed(controller_index, btn_code);
}

bool input::btn_down(i32 controller_index, u8 btn)
{
    if (controller_index >= 0 && controllers[controller_index].connected)
    {
        auto ctrl = controllers[controller_index];
        return ctrl.curr_button_state[btn];
    }
    else
        return false;
}

bool input::btn_down(i32 controller_index, Button btn)
{
    u8 btn_code = (u8)btn;
    return input::btn_down(controller_index, btn_code);
}

bool input::btn_up(i32 controller_index, u8 btn)
{
    if (controller_index >= 0 && controllers[controller_index].connected)
    {
        auto ctrl = controllers[controller_index];
        return !ctrl.curr_button_state[btn] && ctrl.prev_button_state[btn];
    }
    else
        return false;
}

bool input::btn_up(i32 controller_index, Button btn)
{
    u8 btn_code = (u8)btn;
    return input::btn_up(controller_index, btn_code);
}

f32 input::get_axis(i32 controller_index, u8 axis)
{
    if (controller_index >= 0 && controllers[controller_index].connected)
    {
        f32 value = SDL_GameControllerGetAxis(
            gamepads[controller_index], (SDL_GameControllerAxis)axis
        );

        if (value >= 0)
            value /= AXIS_RANGE;
        else
            value /= AXIS_RANGE + 1.f;

        return value;
    }
    else
        return 0;
}

f32 input::get_axis(i32 controller_index, Axis axis)
{
    u8 axis_code = (u8)axis;
    return input::get_axis(controller_index, axis_code);
}

bool input::axis_pressed(i32 controller_index, u8 axis, f32 threshold)
{
    if (controller_index >= 0 && controllers[controller_index].connected)
    {
        auto ctrl = controllers[controller_index];
        return std::abs(ctrl.curr_axis_state[axis]) >= threshold &&
               std::abs(ctrl.prev_axis_state[axis]) < threshold;
    }
    else
        return false;
}

bool input::axis_pressed(i32 controller_index, Axis axis, f32 threshold)
{
    u8 axis_code = (u8)axis;
    return input::axis_pressed(controller_index, axis_code, threshold);
}

bool input::axis_down(i32 controller_index, u8 axis, f32 threshold)
{
    if (controller_index >= 0 && controllers[controller_index].connected)
    {
        auto ctrl = controllers[controller_index];
        return std::abs(ctrl.curr_axis_state[axis]) >= threshold;
    }
    else
        return false;
}

bool input::axis_down(i32 controller_index, Axis axis, f32 threshold)
{
    u8 axis_code = (u8)axis;
    return input::axis_down(controller_index, axis_code, threshold);
}

bool input::axis_up(i32 controller_index, u8 axis, f32 threshold)
{
    if (controller_index >= 0 && controllers[controller_index].connected)
    {
        auto ctrl = controllers[controller_index];
        return std::abs(ctrl.curr_axis_state[axis]) < threshold &&
               std::abs(ctrl.prev_axis_state[axis]) >= threshold;
    }
    else
        return false;
}

bool input::axis_up(i32 controller_index, Axis axis, f32 threshold)
{
    u8 axis_code = (u8)axis;
    return input::axis_up(controller_index, axis_code, threshold);
}

f32 input::get_horizontal_axis(i32 controller_index, bool alt)
{
    return input::get_axis(controller_index, alt ? AXIS_RIGHT_X : AXIS_LEFT_X);
}

f32 input::get_vertical_axis(i32 controller_index, bool alt)
{
    return input::get_axis(controller_index, alt ? AXIS_RIGHT_Y : AXIS_LEFT_Y);
}

f32 input::get_left_trigger(i32 controller_index)
{
    return input::get_axis(controller_index, LEFT_TRIGGER);
}

f32 input::get_right_trigger(i32 controller_index)
{
    return input::get_axis(controller_index, RIGHT_TRIGGER);
}

bool input::pressed(const input::bind::ButtonBind button)
{
    for (auto key : button.keys)
        if (key_pressed(key)) return true;

    for (auto mouse : button.mouse)
        if (mouse_click(mouse)) return true;

    for (auto btn : button.buttons)
        if (btn_pressed(btn.controller_index, btn.button)) return true;

    for (auto trigger : button.triggers)
        if (axis_pressed(
                trigger.controller_index, trigger.axis, trigger.threshold
            ))
            return true;

    return false;
}

bool input::down(const input::bind::ButtonBind button)
{
    for (auto key : button.keys)
        if (key_down(key)) return true;

    for (auto mouse : button.mouse)
        if (mouse_down(mouse)) return true;

    for (auto btn : button.buttons)
        if (btn_down(btn.controller_index, btn.button)) return true;

    for (auto trigger : button.triggers)
    {
        f32 raw = input::get_axis(trigger.controller_index, trigger.axis);
        if ((raw > 0 && trigger.pos) || (raw < 0 && !trigger.pos))
        {
            if (axis_down(
                    trigger.controller_index, trigger.axis, trigger.threshold
                ))
                return true;
        }
    }

    return false;
}

bool input::up(const input::bind::ButtonBind button)
{
    for (auto key : button.keys)
        if (key_up(key)) return true;

    for (auto mouse : button.mouse)
        if (mouse_up(mouse)) return true;

    for (auto btn : button.buttons)
        if (btn_up(btn.controller_index, btn.button)) return true;

    for (auto trigger : button.triggers)
        if (axis_up(trigger.controller_index, trigger.axis, trigger.threshold))
            return true;

    return false;
}

f32 input::val(const input::bind::ButtonBind button)
{
    for (auto key : button.keys)
        if (key_down(key)) return 1.0f;

    for (auto mouse : button.mouse)
        if (mouse_down(mouse)) return 1.0f;

    for (auto btn : button.buttons)
        if (btn_down(btn.controller_index, btn.button)) return 1.0f;

    f32 max = 0.0f;
    for (auto trigger : button.triggers)
    {
        f32 raw = input::get_axis(trigger.controller_index, trigger.axis);
        if ((raw > 0 && trigger.pos) || (raw < 0 && !trigger.pos))
        {
            if (axis_down(
                    trigger.controller_index, trigger.axis, trigger.threshold
                ))
            {
                f32 mapped = calc::clamp_map(
                    std::abs(raw), trigger.threshold, 1.0f, 0.0f, 1.0f
                );
                if (mapped > max) max = mapped;
            }
        }
    }
    return max;
}

bool input::pressed(const input::bind::AxisBind bind)
{
    return input::pressed(bind.pos) || input::pressed(bind.neg);
}

bool input::down(const input::bind::AxisBind bind)
{
    return input::down(bind.pos) || input::down(bind.neg);
}

bool input::up(const input::bind::AxisBind bind)
{
    return input::up(bind.pos) || input::up(bind.neg);
}

f32 input::val(const input::bind::AxisBind bind)
{
    f32 pos = input::val(bind.pos);
    f32 neg = input::val(bind.neg);

    if (neg <= 0 && pos <= 0) return 0;

    if (neg > 0 && pos <= 0) return -neg;

    if (pos > 0 && neg <= 0) return pos;

    return pos - neg;
}

i32 input::sgn(const input::bind::AxisBind bind)
{
    return calc::sgn(input::val(bind));
}

vec2f input::val(const input::bind::StickBind bind)
{
    return {input::val(bind.x), input::val(bind.y)};
}

point input::sgn(const input::bind::StickBind bind)
{
    return {input::sgn(bind.x), input::sgn(bind.y)};
}
#endif

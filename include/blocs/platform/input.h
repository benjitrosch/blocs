#pragma once

#include <sstream>
#include <vector>

#include <SDL.h>

#include <blocs/math/vec.h>
#include <blocs/math/calc.h>

#define NUM_SCANCODES 				512
#define REPEAT_DELAY				0.35f
#define REPEAT_INTERVAL				0.025f

#define MAX_NUM_CONTROLLERS			8
#define MAX_NUM_BUTTONS				64
#define MAX_NUM_AXIS				16
#define AXIS_RANGE					32767.0f

#define KEY_A						4
#define KEY_B						5
#define KEY_C						6
#define KEY_D						7
#define KEY_E						8
#define KEY_F						9
#define KEY_G						10
#define KEY_H						11
#define KEY_I						12
#define KEY_J						13
#define KEY_K						14
#define KEY_L						15
#define KEY_M						16
#define KEY_N						17
#define KEY_O						18
#define KEY_P						19
#define KEY_Q						20
#define KEY_R						21
#define KEY_S						22
#define KEY_T						23
#define KEY_U						24
#define KEY_V						25
#define KEY_W						26
#define KEY_X						27
#define KEY_Y						28
#define KEY_Z						29
	\
#define KEY_Num1					30
#define KEY_Num2					31
#define KEY_Num3					32
#define KEY_Num4					33
#define KEY_Num5					34
#define KEY_Num6					35
#define KEY_Num7					36
#define KEY_Num8					37
#define KEY_Num9					38
#define KEY_Num0					39
	\
#define KEY_Enter					40
#define KEY_Escape					41
#define KEY_Backspace				42
#define KEY_Tab						43
#define KEY_Space					44
	\
#define KEY_Minus					45
#define KEY_Equals					46
#define KEY_LeftBracket				47
#define KEY_RightBracket			48
#define KEY_Backslash				49
#define KEY_Semicolon				51
#define KEY_Apostrophe				52
#define KEY_Tilde					53
#define KEY_Comma					54
#define KEY_Period					55
#define KEY_Slash					56
#define KEY_Capslock				57
	\
#define KEY_F1						58
#define KEY_F2						59
#define KEY_F3						60
#define KEY_F4						61
#define KEY_F5						62
#define KEY_F6						63
#define KEY_F7						64
#define KEY_F8						65
#define KEY_F9						66
#define KEY_F10						67
#define KEY_F11						68
#define KEY_F12						69
	\
#define KEY_Insert					73
#define KEY_Home					74
#define KEY_Delete					76
#define KEY_End						77
#define KEY_Right					79
#define KEY_Left					80
#define KEY_Down					81
#define KEY_Up						82
	\
#define KEY_LeftControl				224
#define KEY_LeftShift				225
#define KEY_LeftAlt					226
#define KEY_LeftOS					227
#define KEY_RightControl			228
#define KEY_RightShift				229
#define KEY_RightAlt				230
#define KEY_RightOS					231

#define MOUSE(X)       				(1 << ((X) - 1))
#define MOUSE_Left    				1
#define MOUSE_Middle  				2
#define MOUSE_Right    				3

#define BTN_A						0
#define BTN_B						1
#define BTN_X						2
#define BTN_Y						3
#define BTN_Back 					4
#define BTN_Select 					5
#define BTN_Start 					6
#define BTN_LeftStick 				7
#define BTN_RightStick	 			8
#define BTN_LeftShoulder 			9
#define BTN_RightShoulder 			10
#define BTN_Up    					11
#define BTN_Down  					12
#define BTN_Left  					13
#define BTN_Right 					14

#define AXIS_LEFT_X					0
#define AXIS_LEFT_Y					1
#define AXIS_RIGHT_X				2
#define AXIS_RIGHT_Y				3
	\
#define LEFT_TRIGGER				4
#define RIGHT_TRIGGER				5

#define KEY_DEFINITIONS \
	KEYCODE_TO_ENUM(A, 				KEY_A)\
	KEYCODE_TO_ENUM(B, 				KEY_B)\
	KEYCODE_TO_ENUM(C, 				KEY_C)\
	KEYCODE_TO_ENUM(D, 				KEY_D)\
	KEYCODE_TO_ENUM(E, 				KEY_E)\
	KEYCODE_TO_ENUM(F, 				KEY_F)\
	KEYCODE_TO_ENUM(G, 				KEY_G)\
	KEYCODE_TO_ENUM(H, 				KEY_H)\
	KEYCODE_TO_ENUM(I, 				KEY_I)\
	KEYCODE_TO_ENUM(J, 				KEY_J)\
	KEYCODE_TO_ENUM(K, 				KEY_K)\
	KEYCODE_TO_ENUM(L, 				KEY_L)\
	KEYCODE_TO_ENUM(M, 				KEY_M)\
	KEYCODE_TO_ENUM(N, 				KEY_N)\
	KEYCODE_TO_ENUM(O, 				KEY_O)\
	KEYCODE_TO_ENUM(P, 				KEY_P)\
	KEYCODE_TO_ENUM(Q, 				KEY_Q)\
	KEYCODE_TO_ENUM(R, 				KEY_R)\
	KEYCODE_TO_ENUM(S, 				KEY_S)\
	KEYCODE_TO_ENUM(T, 				KEY_T)\
	KEYCODE_TO_ENUM(U, 				KEY_U)\
	KEYCODE_TO_ENUM(V, 				KEY_V)\
	KEYCODE_TO_ENUM(W, 				KEY_W)\
	KEYCODE_TO_ENUM(X, 				KEY_X)\
	KEYCODE_TO_ENUM(Y, 				KEY_Y)\
	KEYCODE_TO_ENUM(Z, 				KEY_Z)\
		\
	KEYCODE_TO_ENUM(One,   			KEY_Num1)\
	KEYCODE_TO_ENUM(Two,   			KEY_Num2)\
	KEYCODE_TO_ENUM(Three, 			KEY_Num3)\
	KEYCODE_TO_ENUM(Four,  			KEY_Num4)\
	KEYCODE_TO_ENUM(Five,  			KEY_Num5)\
	KEYCODE_TO_ENUM(Six,   			KEY_Num6)\
	KEYCODE_TO_ENUM(Seven, 			KEY_Num7)\
	KEYCODE_TO_ENUM(Eight, 			KEY_Num8)\
	KEYCODE_TO_ENUM(Nine,  			KEY_Num9)\
	KEYCODE_TO_ENUM(Zero,  			KEY_Num0)\
		\
	KEYCODE_TO_ENUM(Enter, 			KEY_Enter)\
	KEYCODE_TO_ENUM(Escape, 		KEY_Escape)\
	KEYCODE_TO_ENUM(Backspace, 		KEY_Backspace)\
	KEYCODE_TO_ENUM(Tab, 			KEY_Tab)\
	KEYCODE_TO_ENUM(Space, 			KEY_Space)\
		\
	KEYCODE_TO_ENUM(Minus, 			KEY_Minus)\
	KEYCODE_TO_ENUM(Equals, 		KEY_Equals)\
	KEYCODE_TO_ENUM(LeftBracket, 	KEY_LeftBracket)\
	KEYCODE_TO_ENUM(RightBracket, 	KEY_RightBracket)\
	KEYCODE_TO_ENUM(Backslash, 		KEY_Backslash)\
	KEYCODE_TO_ENUM(Semicolon, 		KEY_Semicolon)\
	KEYCODE_TO_ENUM(Apostrophe, 	KEY_Apostrophe)\
	KEYCODE_TO_ENUM(Tilde, 			KEY_Tilde)\
	KEYCODE_TO_ENUM(Comma, 			KEY_Comma)\
	KEYCODE_TO_ENUM(Period, 		KEY_Period)\
	KEYCODE_TO_ENUM(Slash, 			KEY_Slash)\
	KEYCODE_TO_ENUM(Capslock,		KEY_Capslock)\
		\
	KEYCODE_TO_ENUM(F1,  			KEY_F1)\
	KEYCODE_TO_ENUM(F2,  			KEY_F2)\
	KEYCODE_TO_ENUM(F3,  			KEY_F3)\
	KEYCODE_TO_ENUM(F4,  			KEY_F4)\
	KEYCODE_TO_ENUM(F5,  			KEY_F5)\
	KEYCODE_TO_ENUM(F6,  			KEY_F6)\
	KEYCODE_TO_ENUM(F7,  			KEY_F7)\
	KEYCODE_TO_ENUM(F8,  			KEY_F8)\
	KEYCODE_TO_ENUM(F9,  			KEY_F9)\
	KEYCODE_TO_ENUM(F10,  			KEY_F10)\
	KEYCODE_TO_ENUM(F11, 			KEY_F11)\
	KEYCODE_TO_ENUM(F12, 			KEY_F12)\
		\
	KEYCODE_TO_ENUM(Insert, 		KEY_Insert)\
	KEYCODE_TO_ENUM(Home, 			KEY_Home)\
	KEYCODE_TO_ENUM(Delete, 		KEY_Delete)\
	KEYCODE_TO_ENUM(End, 			KEY_End)\
	KEYCODE_TO_ENUM(Right, 			KEY_Right)\
	KEYCODE_TO_ENUM(Left, 			KEY_Left)\
	KEYCODE_TO_ENUM(Down, 			KEY_Down)\
	KEYCODE_TO_ENUM(Up, 			KEY_Up)\
		\
	KEYCODE_TO_ENUM(LeftControl, 	KEY_LeftControl)\
	KEYCODE_TO_ENUM(LeftShift, 		KEY_LeftShift)\
	KEYCODE_TO_ENUM(LeftAlt, 		KEY_LeftAlt)\
	KEYCODE_TO_ENUM(LeftOS, 		KEY_LeftOS)\
	KEYCODE_TO_ENUM(RightControl, 	KEY_RightControl)\
	KEYCODE_TO_ENUM(RightShift, 	KEY_RightShift)\
	KEYCODE_TO_ENUM(RightAlt, 		KEY_RightAlt)\
	KEYCODE_TO_ENUM(RightOS, 		KEY_RightOS)

#define MOUSE_DEFINITIONS \
	MOUSE_TO_ENUM(Left, 			MOUSE_Left)\
	MOUSE_TO_ENUM(Middle, 			MOUSE_Middle)\
	MOUSE_TO_ENUM(Right, 			MOUSE_Right)

#define BUTTON_DEFINITIONS \
	BUTTON_TO_ENUM(A, 				BTN_A)\
	BUTTON_TO_ENUM(B, 				BTN_B)\
	BUTTON_TO_ENUM(X, 				BTN_X)\
	BUTTON_TO_ENUM(Y, 				BTN_Y)\
	BUTTON_TO_ENUM(Back, 			BTN_Back)\
	BUTTON_TO_ENUM(Select, 			BTN_Select)\
	BUTTON_TO_ENUM(Start, 			BTN_Start)\
	BUTTON_TO_ENUM(LeftStick, 		BTN_LeftStick)\
	BUTTON_TO_ENUM(RightStick, 		BTN_RightStick)\
	BUTTON_TO_ENUM(LeftShoulder, 	BTN_LeftShoulder)\
	BUTTON_TO_ENUM(RightShoulder, 	BTN_RightShoulder)\
	BUTTON_TO_ENUM(Up, 				BTN_Up)\
	BUTTON_TO_ENUM(Down, 			BTN_Down)\
	BUTTON_TO_ENUM(Left, 			BTN_Left)\
	BUTTON_TO_ENUM(Right, 			BTN_Right)

#define AXIS_DEFINITIONS \
	AXIS_TO_ENUM(LeftX, 			AXIS_LEFT_X)\
	AXIS_TO_ENUM(LeftY, 			AXIS_LEFT_Y)\
	AXIS_TO_ENUM(RightX, 			AXIS_RIGHT_X)\
	AXIS_TO_ENUM(RightY, 			AXIS_RIGHT_Y)\
		\
	AXIS_TO_ENUM(LeftTrigger, 		LEFT_TRIGGER)\
	AXIS_TO_ENUM(RightTrigger, 		RIGHT_TRIGGER)

namespace blocs
{
	enum class Key
	{
		#define KEYCODE_TO_ENUM(name, value) name = value,
		KEY_DEFINITIONS
		#undef KEYCODE_TO_ENUM
	};

	enum class MouseButton
	{
		#define MOUSE_TO_ENUM(name, value) name = value,
		MOUSE_DEFINITIONS
		#undef MOUSE_TO_ENUM
	};

	enum class Button
	{
		#define BUTTON_TO_ENUM(name, value) name = value,
		BUTTON_DEFINITIONS
		#undef BUTTON_TO_ENUM
	};

	enum class Axis
	{
		#define AXIS_TO_ENUM(name, value) name = value,
		AXIS_DEFINITIONS
		#undef AXIS_TO_ENUM
	};
	
	namespace input
	{
		inline cstr key_name(u8 key)
		{
			switch (key)
			{
				#define KEYCODE_TO_ENUM(name, value) case value: return #name;
				KEY_DEFINITIONS
				#undef KEYCODE_TO_ENUM
				default: return "None";
			}
		}

		inline cstr key_name(Key key)
		{
			u8 key_code = (u8)key;
			return key_name(key_code);
		}

		inline cstr mouse_name(u8 button)
		{
			switch (button)
			{
				#define MOUSE_TO_ENUM(name, value) case value: return #name;
				MOUSE_DEFINITIONS
				#undef MOUSE_TO_ENUM
				default: return "None";
			}
		}

		inline cstr mouse_name(MouseButton button)
		{
			u8 mouse_button = (1 << (((u8)button) - 1));
			return mouse_name(mouse_button);
		}

		inline cstr btn_name(u8 btn)
		{
			switch (btn)
			{
				#define BUTTON_TO_ENUM(name, value) case value: return #name;
				BUTTON_DEFINITIONS
				#undef BUTTON_TO_ENUM
				default: return "None";
			}
		}

		inline cstr btn_name(Button btn)
		{
			u8 btn_code = (u8)btn;
			return btn_name(btn_code);
		}

		inline cstr axis_name(u8 axis)
		{
			switch (axis)
			{
				#define AXIS_TO_ENUM(name, value) case value: return #name;
				AXIS_DEFINITIONS
				#undef AXIS_TO_ENUM
				default: return "None";
			}
		}

		inline cstr axis_name(Axis axis)
		{
			u8 axis_code = (u8)axis;
			return axis_name(axis_code);
		}

#ifndef __EMSCRIPTEN__
		using GamepadPtr = SDL_GameController*;

		/**
		 * @brief Controller metadata, connection status, and the current
		 * and previous button/axis states.
		 */
		struct ControllerState
		{
			str  name;

			u16  vendor;
			u16  product;
			u16  version;

			 u8  prev_button_state[MAX_NUM_BUTTONS];
			 u8  curr_button_state[MAX_NUM_BUTTONS]; 

			f32  prev_axis_state[MAX_NUM_AXIS];
			f32  curr_axis_state[MAX_NUM_AXIS];

			i32  num_buttons;
			i32	 num_axis;

			bool connected = false;
		};
#endif

		/**
		 * @brief Current and previous state of the keyboard.
		 */
		struct KeyboardState
		{
			u8 prev_keyboard_state[NUM_SCANCODES];
			u8 curr_keyboard_state[NUM_SCANCODES];
		};

		/**
		 * @brief Current and previous states of the global mouse.
		 */
		struct MouseState
		{
			u32 prev_mouse_state;
			u32 curr_mouse_state;

			i32 prev_mouse_x;
			i32 prev_mouse_y;
			i32 curr_mouse_x;
			i32 curr_mouse_y;

			i32 prev_mouse_wheel_x;
			i32 prev_mouse_wheel_y;
			i32 curr_mouse_wheel_x;
			i32 curr_mouse_wheel_y;
		};
		
		namespace bind
		{
			struct ButtonBind
			{
				struct ControllerTriggerBind
				{
					i32 controller_index;
					Axis axis;
					f32 threshold;
					bool pos;
				};

				struct ControllerButtonBind
				{
					i32 controller_index;
					Button button;
				};

				std::vector<Key> keys;
				std::vector<MouseButton> mouse;
				std::vector<ControllerButtonBind> buttons;
				std::vector<ControllerTriggerBind> triggers;

				ButtonBind& add(Key key)
				{
					keys.push_back(key);
					return *this;
				}

				ButtonBind& add(MouseButton btn)
				{
					mouse.push_back(btn);
					return *this;
				}

				ButtonBind& add(ControllerButtonBind btn)
				{
					buttons.push_back(btn);
					return *this;
				}

				ButtonBind& add(i32 index, Button btn)
				{
					buttons.push_back({ index, btn });
					return *this;
				}

				ButtonBind& add(i32 index, Axis axis, f32 threshold = 0.01f, bool pos = true)
				{
					triggers.push_back({ index, axis, threshold, pos });
					return *this;
				}

				str print_binds() const
				{
					ss stream;
					for (i32 i = 0; i < keys.size(); i++)
						stream << "Key" + str(input::key_name((u8)keys[i])) + ", ";
					for (i32 i = 0; i < mouse.size(); i++)
						stream << "Mouse" + str(input::mouse_name((u8)mouse[i])) + ", ";
					for (i32 i = 0; i < buttons.size(); i++)
						stream << "Button" + str(input::btn_name((u8)buttons[i].button)) + ", ";
					for (i32 i = 0; i < triggers.size(); i++)
						stream << str(input::axis_name((u8)triggers[i].axis)) + ", ";
					return stream.str();
				}
			};

			struct AxisBind
			{
				ButtonBind pos;
				ButtonBind neg;

				template<typename T>
				AxisBind& add(T bind)
				{
					pos.add(bind);
					return *this;
				}

				template<typename T0, typename T1>
				AxisBind& add(T0 p, T1 n)
				{
					pos.add(p);
					neg.add(n);
					return *this;
				}

				AxisBind& add(i32 index, Axis p, Axis n, f32 threshold = 0.01f)
				{
					pos.add(index, p, threshold, true);
					neg.add(index, n, threshold, false);
					return *this;
				}

				AxisBind& add(i32 index, Axis axis, f32 threshold = 0.01f)
				{
					pos.add(index, axis, threshold, true);
					neg.add(index, axis, threshold, false);
					return *this;
				}
			};

			struct StickBind
			{
				AxisBind x;
				AxisBind y;

				template<typename T0, typename T1, typename T2, typename T3>
				StickBind& add(T0 left, T1 right, T2 up, T3 down)
				{
					x.pos.add(right);
					x.neg.add(left);
					y.pos.add(down);
					y.neg.add(up);
					return *this;
				}

				StickBind& add(i32 index, Axis hor, Axis vert, f32 threshold = 0.01f)
				{
					x.pos.add(index, hor, threshold, true);
					x.neg.add(index, hor, threshold, false);
					y.pos.add(index, vert, threshold, true);
					y.neg.add(index, vert, threshold, false);
					return *this;
				}
			};
		}
	}

	/**
	 * @brief Manages input state for Keyboard, Mouse, and Gamepads.
	 */
	struct Input
	{
#ifndef __EMSCRIPTEN__
		input::GamepadPtr 		gamepads   [MAX_NUM_CONTROLLERS];
		input::ControllerState 	controllers[MAX_NUM_CONTROLLERS];
#endif

		input::KeyboardState 	keyboard;
		input::MouseState 		mouse;
		
		/**
		 * @brief Cache the current state as previous state and reset (runs after
		 * events have processed).
		 */
		void update()
		{
			memcpy(keyboard.prev_keyboard_state, keyboard.curr_keyboard_state, sizeof(u8) * NUM_SCANCODES);
			memcpy(keyboard.curr_keyboard_state, SDL_GetKeyboardState(NULL),   sizeof(u8) * NUM_SCANCODES);

			mouse.prev_mouse_x 		 = mouse.curr_mouse_x;
			mouse.prev_mouse_y 		 = mouse.curr_mouse_y;
			mouse.prev_mouse_state 	 = mouse.curr_mouse_state;
			mouse.curr_mouse_state   = SDL_GetMouseState(&mouse.curr_mouse_x, &mouse.curr_mouse_y);

#ifndef __EMSCRIPTEN__
			for (i8 i = 0; i < MAX_NUM_CONTROLLERS; i++)
			{
				input::ControllerState& controller = controllers[i];
				if (controller.connected)
				{
					memcpy(controller.prev_button_state, controller.curr_button_state, sizeof(u8) * MAX_NUM_BUTTONS);
					memcpy(controller.prev_axis_state, 	 controller.curr_axis_state,   sizeof(f32) * MAX_NUM_AXIS);
					
					for (i32 j = 0; j < controller.num_buttons; j++)
						controller.curr_button_state[j] = SDL_GameControllerGetButton(
							gamepads[i],
							(SDL_GameControllerButton)j
						);

					for (i32 j = 0; j < controller.num_axis; j++)
					{
						f32 value = SDL_GameControllerGetAxis(
							gamepads[i],
							(SDL_GameControllerAxis)j
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

		void events()
		{
			mouse.curr_mouse_wheel_x = 0;
			mouse.curr_mouse_wheel_y = 0;
			mouse.prev_mouse_wheel_x = mouse.curr_mouse_wheel_x;
			mouse.prev_mouse_wheel_y = mouse.curr_mouse_wheel_y;
		}

		/**
		 * @return Whether the key was pressed once during this frame.
		 */
		constexpr bool key_pressed(u8 key) const
		{
			return keyboard.curr_keyboard_state[key] && !keyboard.prev_keyboard_state[key];
		}

		constexpr bool key_pressed(Key key) const
		{
			u8 key_code = (u8)key;
			return key_pressed(key_code);
		}

		/**
		 * @return Whether the key was held down during this frame.
		 */
		constexpr bool key_down(u8 key) const
		{
			return keyboard.curr_keyboard_state[key];
		}

		constexpr bool key_down(Key key) const
		{
			u8 key_code = (u8)key;
			return key_down(key_code);
		}

		/**
		 * @return Whether the key was released during this frame.
		 */
		constexpr bool key_up(u8 key) const
		{
			return !keyboard.curr_keyboard_state[key] && keyboard.prev_keyboard_state[key];
		}

		constexpr bool key_up(Key key) const
		{
			u8 key_code = (u8)key;
			return key_up(key_code);
		}

		/**
		 * @return Whether eother `shift` key was held down during this frame.
		 */
		constexpr bool shift() const
		{
			return keyboard.curr_keyboard_state[KEY_LeftShift] || keyboard.curr_keyboard_state[KEY_RightShift];
		}
		
		/**
		 * @return Whether either `control` key was held down during this frame.
		 */
		constexpr bool ctrl() const
		{
			return keyboard.curr_keyboard_state[KEY_LeftControl] || keyboard.curr_keyboard_state[KEY_RightControl];
		}

		/**
		 * @return Whether either `alt` key was held down during this frame.
		 */
		constexpr bool alt() const
		{
			return keyboard.curr_keyboard_state[KEY_LeftAlt] || keyboard.curr_keyboard_state[KEY_RightAlt];
		}

		/**
		 * @return Whether the mouse button was clicked once during this frame.
		 */
		constexpr bool mouse_click(const u32 button) const
		{
			return ((MOUSE(button) & mouse.curr_mouse_state) != 0) && ((MOUSE(button) & mouse.prev_mouse_state) == 0);
		}

		constexpr bool mouse_click(const MouseButton button) const
		{
			u8 mouse_button = (1 << (((u8)button) - 1));
			return mouse_click(mouse_button);
		}

		/**
		 * @return Whether the mouse button was held down during this frame.
		 */
		constexpr bool mouse_down(const u32 button) const
		{
			return (MOUSE(button) & mouse.curr_mouse_state) != 0;
		}

		constexpr bool mouse_down(const MouseButton button) const
		{
			u8 mouse_button = (1 << (((u8)button) - 1));
			return mouse_down(mouse_button);
		}

		/**
		 * @return Whether the mouse button was released during this frame.
		 */
		constexpr bool mouse_up(const u32 button) const
		{
			return ((MOUSE(button) & mouse.curr_mouse_state) == 0) && ((MOUSE(button) & mouse.prev_mouse_state) != 0);
		}

		constexpr bool mouse_up(const MouseButton button) const
		{
			u8 mouse_button = (1 << (((u8)button) - 1));
			return mouse_up(mouse_button);
		}

		/**
		 * @return Mouse position.
		 */
		constexpr point mouse_pos() const
		{
			return point(mouse.curr_mouse_x, mouse.curr_mouse_y);
		}

		/**
		 * @return Change in mouse position since last frame.
		 */
		constexpr point get_mouse_delta() const
		{
			return point(mouse.curr_mouse_x - mouse.prev_mouse_x, mouse.curr_mouse_y - mouse.prev_mouse_y);
		}

		/**
		 * @return Change in horizontal mouse wheel scroll since last frame.
		 */
		constexpr i32 get_mouse_wheel_delta_x() const
		{
			return mouse.curr_mouse_wheel_x - mouse.prev_mouse_wheel_x;
		}

		/**
		 * @return Change in vertical mouse wheel scroll since last frame.
		 */
		constexpr i32 get_mouse_wheel_delta_y() const
		{
			return mouse.curr_mouse_wheel_y - mouse.prev_mouse_wheel_y;
		}

#ifndef __EMSCRIPTEN__
		/**
		 * @brief Connect a new controller.
		 * 
		 * @param name 	  	  Name of controller.
		 * @param vendor  	  Vendor ID.
		 * @param product 	  Product ID.
		 * @param version 	  Version ID.
		 * @param num_buttons Number of buttons.
		 * @param num_axis 	  Number of axes.
		 */
		void connect_controller(i32 index, str name, u16 vendor, u16 product, u16 version, i32 num_buttons, i32 num_axis)
		{  
			controllers[index].name 	 	= name;
			controllers[index].vendor 	 	= vendor;
			controllers[index].product   	= product;
			controllers[index].version   	= version;
			controllers[index].num_buttons  = num_buttons;
			controllers[index].num_axis		= num_axis;
			controllers[index].connected 	= true;

			LOG_DEBUG("CONNECTED: " << name << " (vendor:" << i32(vendor) << ", product:" << i32(product) << ", version:" << i32(version) << ")");
			LOG_DEBUG("num buttons: " << num_buttons << ", num axis: " << num_axis);
		}

		/** @brief Disconnects a controller. */
		void disconnect_controller(i32 index)
		{
			auto ctrl = controllers[index];
			LOG_DEBUG("DISCONNECTED: " << ctrl.name << " (" << i32(ctrl.vendor) << ", " << i32(ctrl.product) << ", " << i32(ctrl.version) << ")");
			
			controllers[index] = {};
		}

		/**
		 * @brief Get index of Controller/Gamepad by instance id.
		 * 
		 * @param instance_id Gamepad instance id.
		 * 
		 * @return Array index for Controller/Gamepad.
		 */
		i32 get_controller_index_by_instance_id(i32 instance_id) const
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

		/**
		 * @return Whether the button on controller #n was pressed once during this frame.
		 */
		bool btn_pressed(i32 controller_index, u8 btn) const
		{
			if (controller_index >= 0 && controllers[controller_index].connected)
			{
				auto ctrl = controllers[controller_index];
				return ctrl.curr_button_state[btn] && !ctrl.prev_button_state[btn];
			}
			else return false;
		}

		bool btn_pressed(i32 controller_index, Button btn) const
		{
			u8 btn_code = (u8)btn;
			return btn_pressed(controller_index, btn_code);
		}

		/**
		 * @return Whether the button on controller #n was held down during this frame.
		 */
		bool btn_down(i32 controller_index, u8 btn) const
		{
			if (controller_index >= 0 && controllers[controller_index].connected)
			{
				auto ctrl = controllers[controller_index];
				return ctrl.curr_button_state[btn];
			}
			else return false;
		}

		bool btn_down(i32 controller_index, Button btn) const
		{
			u8 btn_code = (u8)btn;
			return btn_down(controller_index, btn_code);
		}

		/**
		 * @return Whether the button on controller #n was released during this frame.
		 */
		bool btn_up(i32 controller_index, u8 btn) const
		{
			if (controller_index >= 0 && controllers[controller_index].connected)
			{
				auto ctrl = controllers[controller_index];
				return !ctrl.curr_button_state[btn] && ctrl.prev_button_state[btn];
			}
			else return false;
		}

		bool btn_up(i32 controller_index, Button btn) const
		{
			u8 btn_code = (u8)btn;
			return btn_up(controller_index, btn_code);
		}

		f32 get_axis(i32 controller_index, u8 axis) const
		{
			if (controller_index >= 0 && controllers[controller_index].connected)
			{
				f32 value = SDL_GameControllerGetAxis(
					gamepads[controller_index],
					(SDL_GameControllerAxis)axis
				);

				if (value >= 0)
					value /= AXIS_RANGE;
				else
					value /= AXIS_RANGE + 1.f;

				return value;
			}
			else return 0;
		}

		f32 get_axis(i32 controller_index, Axis axis) const
		{
			u8 axis_code = (u8)axis;
			return get_axis(controller_index, axis_code);
		}

		bool axis_pressed(i32 controller_index, u8 axis, f32 threshold) const
		{
			if (controller_index >= 0 && controllers[controller_index].connected)
			{
				auto ctrl = controllers[controller_index];
				return std::abs(ctrl.curr_axis_state[axis]) >= threshold && std::abs(ctrl.prev_axis_state[axis]) < threshold;
			}
			else return false;
		}

		bool axis_pressed(i32 controller_index, Axis axis, f32 threshold = 0.01f) const
		{
			u8 axis_code = (u8)axis;
			return axis_pressed(controller_index, axis_code, threshold);
		}

		bool axis_down(i32 controller_index, u8 axis, f32 threshold = 0.01f) const
		{
			if (controller_index >= 0 && controllers[controller_index].connected)
			{
				auto ctrl = controllers[controller_index];
				return std::abs(ctrl.curr_axis_state[axis]) >= threshold;
			}
			else return false;
		}

		bool axis_down(i32 controller_index, Axis axis, f32 threshold = 0.01f) const
		{
			u8 axis_code = (u8)axis;
			return axis_down(controller_index, axis_code, threshold);
		}

		bool axis_up(i32 controller_index, u8 axis, f32 threshold = 0.01f) const
		{
			if (controller_index >= 0 && controllers[controller_index].connected)
			{
				auto ctrl = controllers[controller_index];
				return std::abs(ctrl.curr_axis_state[axis]) < threshold && std::abs(ctrl.prev_axis_state[axis]) >= threshold;
			}
			else return false;
		}

		bool axis_up(i32 controller_index, Axis axis, f32 threshold = 0.01f) const
		{
			u8 axis_code = (u8)axis;
			return axis_up(controller_index, axis_code, threshold);
		}

		/**
		 * @param controller_index Controller # to read joystick axis input value.
		 * @param alt 			   Whether to use the right joystick (uses left by default).
		 * 
		 * @return Horizontal axis of controller joystick. 
		 */
		f32 get_horizontal_axis(i32 controller_index, bool alt = false) const
		{
			return get_axis(controller_index, alt ? AXIS_RIGHT_X : AXIS_LEFT_X);
		}

		/**
		 * @param controller_index Controller # to read joystick axis input value.
		 * @param alt 			   Whether to use the right joystick (uses left by default).
		 * 
		 * @return Vertical axis of controller joystick. 
		 */
		f32 get_vertical_axis(i32 controller_index, bool alt = false) const
		{
			return get_axis(controller_index, alt ? AXIS_RIGHT_Y : AXIS_LEFT_Y);
		}

		/**
		 * @param controller_index Controller # to read trigger input value.
		 * 
		 * @return Value of left trigger.
		 */
		f32 get_left_trigger(i32 controller_index) const
		{
			return get_axis(controller_index, LEFT_TRIGGER);
		}

		/**
		 * @param controller_index Controller # to read trigger input value.
		 * 
		 * @return Value of right trigger.
		 */
		f32 get_right_trigger(i32 controller_index) const
		{
			return get_axis(controller_index, RIGHT_TRIGGER);
		}

		bool pressed(const input::bind::ButtonBind button) const
		{
			for (auto key : button.keys)
				if (key_pressed(key))
					return true;

			for (auto mouse : button.mouse)
				if (mouse_click(mouse))
					return true;

			for (auto btn : button.buttons)
				if (btn_pressed(btn.controller_index, btn.button))
					return true;

			for (auto trigger : button.triggers)
				if (axis_pressed(trigger.controller_index, trigger.axis, trigger.threshold))
					return true;

			return false;
		}
		
		bool down(const input::bind::ButtonBind button) const
		{
			for (auto key : button.keys)
				if (key_down(key))
					return true;

			for (auto mouse : button.mouse)
				if (mouse_down(mouse))
					return true;

			for (auto btn : button.buttons)
				if (btn_down(btn.controller_index, btn.button))
					return true;

			for (auto trigger : button.triggers)
			{
				f32 raw = get_axis(trigger.controller_index, trigger.axis);
				if ((raw > 0 && trigger.pos) || (raw < 0 && !trigger.pos))
				{
					if (axis_down(trigger.controller_index, trigger.axis, trigger.threshold))
						return true;
				}
			}

			return false;
		}

		bool up(const input::bind::ButtonBind button) const
		{
			for (auto key : button.keys)
				if (key_up(key))
					return true;

			for (auto mouse : button.mouse)
				if (mouse_up(mouse))
					return true;

			for (auto btn : button.buttons)
				if (btn_up(btn.controller_index, btn.button))
					return true;

			for (auto trigger : button.triggers)
				if (axis_up(trigger.controller_index, trigger.axis, trigger.threshold))
					return true;

			return false;
		}

		f32 val(const input::bind::ButtonBind button) const
		{
			for (auto key : button.keys)
				if (key_down(key))
					return 1.0f;

			for (auto mouse : button.mouse)
				if (mouse_down(mouse))
					return 1.0f;

			for (auto btn : button.buttons)
				if (btn_down(btn.controller_index, btn.button))
					return 1.0f;

			f32 max = 0.0f;
			for (auto trigger : button.triggers)
			{
				f32 raw = get_axis(trigger.controller_index, trigger.axis);
				if ((raw > 0 && trigger.pos) || (raw < 0 && !trigger.pos))
				{
					if (axis_down(trigger.controller_index, trigger.axis, trigger.threshold))
					{
						f32 mapped = calc::clamp_map(std::abs(raw), trigger.threshold, 1.0f, 0.0f, 1.0f); 
						if (mapped > max)
							max = mapped;
					}
				}
			}
			return max;
		}

		bool pressed(const input::bind::AxisBind bind) const
		{
			return pressed(bind.pos) || pressed(bind.neg);
		}
		
		bool down(const input::bind::AxisBind bind) const
		{
			return down(bind.pos) || down(bind.neg);
		}

		bool up(const input::bind::AxisBind bind) const
		{
			return up(bind.pos) || up(bind.neg);
		}

		f32 val(const input::bind::AxisBind bind) const
		{
			f32 pos = val(bind.pos);
			f32 neg = val(bind.neg);

			if (neg <= 0 && pos <= 0)
				return 0;

			if (neg > 0 && pos <= 0)
				return -neg;

			if (pos > 0 && neg <= 0)
				return pos;

			return pos - neg;
		}

		i32 sgn(const input::bind::AxisBind bind) const
		{
			return calc::sgn(val(bind));
		}

		vec2f val(const input::bind::StickBind bind) const
		{
			return { val(bind.x), val(bind.y) };
		}

		point sgn(const input::bind::StickBind bind) const
		{
			return { sgn(bind.x), sgn(bind.y) };
		}
#endif
	};
}

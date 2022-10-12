/*  blocs_fmod

    To add to your project, include this file in *one* C++ file
    to create the implementation:

    #define BLOCS_FMOD
    #include "blocs_fmod.h"
*/

#pragma once
#ifdef BLOCS_FMOD

#include "fmod.hpp"
#include "fmod_errors.h"
#include "fmod_studio.hpp"
#include "fmod_studio_common.h"

#include <blocs.h>

#define FMOD_CALL(x)                        \
    do                                      \
    {                                       \
        fmod_check_errors(x, #x, __LINE__); \
    } while (0)

#define LOG_FMOD_ERR(msg, fn, line)                                        \
    do                                                                     \
    {                                                                      \
        std::cerr << RED("(FMOD) [" << fn << ", " << line << "]: " << msg) \
                  << "\n";                                                 \
    } while (0)

namespace blocs
{
    inline void fmod_check_errors(FMOD_RESULT result, cstr fn, u32 line)
    {
        if (result != FMOD_OK)
        {
            LOG_FMOD_ERR(FMOD_ErrorString(result), fn, line);
        }
    }

    namespace fmod
    {
        inline FMOD::Studio::System* create()
        {
            FMOD::Studio::System* studio;
            FMOD_CALL(FMOD::Studio::System::create(&studio));
            FMOD_CALL(studio->initialize(
                32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr
            ));

            return studio;
        }

        inline void unload(FMOD::Studio::System* studio)
        {
            FMOD_CALL(studio->unloadAll());
        }

        inline void shutdown(FMOD::Studio::System* studio)
        {
            FMOD_CALL(studio->unloadAll());
            FMOD_CALL(studio->release());
        }

        inline void update(FMOD::Studio::System* studio)
        {
            FMOD_CALL(studio->update());
        }

        inline FMOD::Studio::Bank* load_bank(
            FMOD::Studio::System* studio, const str& path,
            FMOD_STUDIO_LOAD_BANK_FLAGS flags = FMOD_STUDIO_LOAD_BANK_NORMAL
        )
        {
            FMOD::Studio::Bank* bank = nullptr;
            FMOD_CALL(studio->loadBankFile(path.c_str(), flags, &bank));
            return bank;
        }

        inline FMOD::Studio::EventInstance* load_event(
            FMOD::Studio::System* studio, const str& guid
        )
        {
            FMOD::Studio::EventDescription* event_description = nullptr;
            FMOD_CALL(studio->getEvent(guid.c_str(), &event_description));
            FMOD::Studio::EventInstance* event = nullptr;
            FMOD_CALL(event_description->createInstance(&event));
            return event;
        }

        inline void play_event(
            FMOD::Studio::EventInstance* event, f32 vol = 1.0f,
            const vec3f& pos = vec3f::zero
        )
        {
            FMOD_3D_ATTRIBUTES attr;
            FMOD_CALL(event->get3DAttributes(&attr));

            FMOD_VECTOR vec;
            vec.x         = pos.x;
            vec.y         = pos.y;
            vec.z         = pos.z;
            attr.position = vec;

            FMOD_CALL(event->set3DAttributes(&attr));
            FMOD_CALL(event->setVolume(vol));
            FMOD_CALL(event->start());
        }

        inline void pause_event(FMOD::Studio::EventInstance* event)
        {
            FMOD_CALL(event->setPaused(true));
        }

        inline void unpause_event(FMOD::Studio::EventInstance* event)
        {
            FMOD_CALL(event->setPaused(false));
        }

        inline void stop_event(
            FMOD::Studio::EventInstance* event, bool fade = true
        )
        {
            FMOD_STUDIO_STOP_MODE mode;
            mode = fade ? FMOD_STUDIO_STOP_ALLOWFADEOUT
                        : FMOD_STUDIO_STOP_IMMEDIATE;
            FMOD_CALL(event->stop(mode));
        }

        inline bool event_is_playing(FMOD::Studio::EventInstance* event)
        {
            FMOD_STUDIO_PLAYBACK_STATE state;
            FMOD_CALL(event->getPlaybackState(&state));
            return state == FMOD_STUDIO_PLAYBACK_PLAYING;
        }
    }
}
#endif

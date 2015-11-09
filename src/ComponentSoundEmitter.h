#ifndef SOUND_EMITTER_H
#define SOUND_EMITTER_H

#include "Component.h"
#include "Sound.h"
#include "Audio.h"
#include "Serializable.h"
#include "Serialize.h"
#include <unordered_set>
#include <string>

namespace ForLeaseEngine {
    namespace Components {
        class SoundEmitter : public Component, public Serializable {
            public:
                static const ComponentType Type = ComponentType::SoundEmitter;

                SoundEmitter(Entity& owner);

                void Update();

                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

                // Play controls
                void Play();
                void Play(std::string const& filename);
                void Stop();
                //void Pause();
                //void Resume();
                //void FadeOut(float time);
                //void FadeIn(float time);
                //void FadeIn(std::string const& filename, float time);
                //void FadeTo(std::string const& filename, float time);

                // Attribute controls
                //void SetVolume(float volume);
                //void SetPitch(float pitch);
                //void ShiftVolume(float volume, float time);
                //void ShiftPitch(float pitch, float time);
            private:
                FMOD_CHANNEL* Channel;
                std::string CurrentSound;
                float Pitch;
                float Volume;

                //Something to affect interpolation type
                    // Linear
                    // Logarithmic
                    // Exponential
                    // Trigonometric

                //float VolumeTimer;
                //float PitchTimer;
                //float FadeTime;
                //float ShiftTimer;

                //bool FadingOut;
                //bool FadingIn;
                //bool FadeTo;
                //bool PitchShift
                //bool VolumeShift
        };
    }
}

#endif // SOUND_EMITTER_H

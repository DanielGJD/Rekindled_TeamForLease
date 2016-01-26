// /*!
    // \file   ComponentSoundEmitter.h
    // \author Christopher Hudson

    // \brief
        // Defines a component for playing a sound

    // \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
// */

// // Note:
// //   Due to the way fmod works, using any of the control functions on a sound that is
// //   longer playing will cause unintended effects!  Will rework at a later time.
// #ifndef SOUND_EMITTER_H
// #define SOUND_EMITTER_H

// #include "Component.h"
// #include "Sound.h"
// #include "Audio.h"
// #include "Serializable.h"
// #include "Serialize.h"
// #include <unordered_set>
// #include <string>

// namespace ForLeaseEngine {
    // namespace Components {
        // class SoundEmitter : public Component {
            // public:
                // static const ComponentType Type = ComponentType::SoundEmitter;
                // virtual ComponentType GetType() { return Type; }
                // float Pitch;
                // float Volume;
                // bool Looping;
                // int LoopCount;

                // SoundEmitter(Entity& owner);
                // ~SoundEmitter();

                // void Update();

                // void Serialize(Serializer& root);
                // void Deserialize(Serializer& root);

                // // Play controls
                // void Play();
                // void Play(std::string const& filename);
                // void Stop();
                // //void Pause();
                // //void Resume();
                // //void FadeOut(float time);
                // //void FadeIn(float time);
                // //void FadeIn(std::string const& filename, float time);
                // //void FadeTo(std::string const& filename, float time);

                // // Attribute controls
                // void SetVolume(float volume);
                // void SetPitch(float pitch);
                // void SetLooping(bool looping);
                // void SetLoopCount(int loopCount);
                // //void ShiftVolume(float volume, float time);
                // //void ShiftPitch(float pitch, float time);
            // private:
                // FMOD_CHANNEL* Channel;
                // std::string CurrentSound;

                // //Something to affect interpolation type
                    // // Linear
                    // // Logarithmic
                    // // Exponential
                    // // Trigonometric

                // //float VolumeTimer;
                // //float PitchTimer;
                // //float FadeTime;
                // //float ShiftTimer;

                // //bool FadingOut;
                // //bool FadingIn;
                // //bool FadeTo;
                // //bool PitchShift
                // //bool VolumeShift
        // };
    // }
// }

// #endif // SOUND_EMITTER_H

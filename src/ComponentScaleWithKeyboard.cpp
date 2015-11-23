#include "ComponentScaleWithKeyboard.h"
#include "Keys.h"
#include "Engine.h"
#include "KeyboardEvent.h"
#include "ComponentTransform.h"
#include "Entity.h"

namespace ForLeaseEngine {
    namespace Components {
        ScaleWithKeyboard::ScaleWithKeyboard(Entity& owner)
                                            : Component(owner, ComponentType::Transform), Active(false), ScaleSpeed(1),
                                              ScaleXUpKey(Keys::D), ScaleXDownKey(Keys::A),
                                              ScaleYUpKey(Keys::W), ScaleYDownKey(Keys::S),
                                              ScaleXUp(false), ScaleXDown(false),
                                              ScaleYUp(false), ScaleYDown(false) {}

        ScaleWithKeyboard::~ScaleWithKeyboard() {
        }

        ScaleWithKeyboard* ScaleWithKeyboard::Create(Entity& owner) {
            ScaleWithKeyboard* component = new ScaleWithKeyboard(owner);
            component->Initialize();
            return component;
        }

        ComponentType ScaleWithKeyboard::GetType() {
            return Type;
        }

        void ScaleWithKeyboard::Update() {
            if(Active) {
                Components::Transform* trans = Parent.GetComponent<Components::Transform>();
                float dt = ForLease->FrameRateController().GetUnscaledDt();
                if(ScaleXUp) {
                    trans->ScaleX += dt * ScaleSpeed;
                }
                if(ScaleXDown) {
                    trans->ScaleX -= dt * ScaleSpeed;
                }
                if(ScaleYUp) {
                    trans->ScaleY += dt * ScaleSpeed;
                }
                if(ScaleYDown) {
                    trans->ScaleY -= dt * ScaleSpeed;
                }
            }
        }

        void ScaleWithKeyboard::Serialize(Serializer& root) {
        }

        void ScaleWithKeyboard::Deserialize(Serializer& root) {
        }

        void ScaleWithKeyboard::Initialize() {
            ForLease->Dispatcher.Attach(NULL, this, "KeyDown", &ScaleWithKeyboard::OnKeyDown);
            ForLease->Dispatcher.Attach(NULL, this, "KeyUp", &ScaleWithKeyboard::OnKeyUp);

        }

        void ScaleWithKeyboard::OnKeyDown(Event const* e) {
            KeyboardEvent const* key_e = static_cast<KeyboardEvent const*>(e);
            if(key_e->Key == ScaleXUpKey)
                ScaleXUp = true;
            if(key_e->Key == ScaleXDownKey)
                ScaleXDown = true;
            if(key_e->Key == ScaleYUpKey)
                ScaleYUp = true;
            if(key_e->Key == ScaleYDownKey)
                ScaleYDown = true;
        }

        void ScaleWithKeyboard::OnKeyUp(Event const* e) {
            KeyboardEvent const* key_e = static_cast<KeyboardEvent const*>(e);
            if(key_e->Key == ScaleXUpKey)
                ScaleXUp = false;
            if(key_e->Key == ScaleXDownKey)
                ScaleXDown = false;
            if(key_e->Key == ScaleYUpKey)
                ScaleYUp = false;
            if(key_e->Key == ScaleYDownKey)
                ScaleYDown = false;
        }
    }
}
